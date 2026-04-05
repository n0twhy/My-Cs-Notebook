

bug1: 27504

page_id_t page_id;
auto header_page = bpm->NewPage(&page_id);
ASSERT_EQ(page_id, HEADER_PAGE_ID);
测试文件里成功申请page_id = 0，然后随后在insert调用时，里面create的叶子也是page_id = 0，冲突
在createleaf里，有一个更新root结点的接口
这个接口是在在记录了所有b+树的header_page中，更新该b+树的根节点
┌──────────────────────────────────────────────────────────────────────────────┐  
│ 0 .. 3        RecordCount : int32_t                                          │
│              (GetRecordCount()/SetRecordCount 直接读写这 4 字节)               │
├──────────────────────────────────────────────────────────────────────────────┤
│ 4 .. 39       Entry #0  (固定 36 bytes)                                      │
│   4  .. 35      name[32]  (C-string，最多 31 字符 + '\0'；不足 32 留空/旧值)    │
│   36 .. 39      root_id : page_id_t (4 bytes, int32)                         │
├──────────────────────────────────────────────────────────────────────────────┤
│ 40 .. 75      Entry #1  (固定 36 bytes)                                      │
│   40 .. 71      name[32]                                                     │
│   72 .. 75      root_id (4 bytes)                                            │
├──────────────────────────────────────────────────────────────────────────────┤
│   76 .. 111     Entry #2 (固定 36 bytes)                                     │
│   76 .. 107     name[32]                                                     │
│   108.. 111     root_id (4 bytes)                                            │
├──────────────────────────────────────────────────────────────────────────────┤
│ ...                                                                          │
├──────────────────────────────────────────────────────────────────────────────┤
│ 4 + 36*i .. 4 + 36*i + 35   Entry #i                                         │
│   offset = 4 + i*36                                                          │
│   [offset + 0  .. offset + 31]  name[32]                                     │
│   [offset + 32 .. offset + 35]  root_id (page_id_t)                          │
└──────────────────────────────────────────────────────────────────────────────┘

offset 0~~3 ReacordCount
offset 4~~35 :
4~~7: 'f' 'o' 'o' '_'
8~~9 'p' 'k'
10~35 '\0' 补齐name[32]固定长度 

┌──────────────────────────────────────────────────────────────────────────────┐
│ 0  .. 3    page_type_      : IndexPageType (int32)  // LEAF_PAGE             │
│ 4  .. 7    lsn_            : lsn_t (int32)                                   │
│ 8  .. 11   size_           : int32   // 当前有多少条 key-value                │
│ 12 .. 15   max_size_       : int32                                           │
│ 16 .. 19   parent_page_id_ : page_id_t (int32)                               │
│ 20 .. 23   page_id_        : page_id_t (int32)                               │
├──────────────────────────────────────────────────────────────────────────────┤
│ 24 .. 27   next_page_id_   : page_id_t (int32)                               │
├──────────────────────────────────────────────────────────────────────────────┤
│ 28 .. ...  array_[] : MappingType = pair<KeyType, ValueType>                 │ <- slot 0
│            array_[0], array_[1], ... 连续存放（按 key 有序）                  │  <- slot 1
└──────────────────────────────────────────────────────────────────────────────┘    ......

GetSize()瞄准了8~~11的内存地址去取的size_ 
然而这里取得是header_page的内存空间，所以读到的是8~~11的"pk"转换为int ('p':0x70, 'k':0x6B)，连续内存低位在前于是翻译过来就是0x6B70也就是27504

这是27504的源头，然而，在创建leaf刚刚得到分配的page_id时，的确是1，他是在之后变成的0
很可能是header_page拿到的page_id为0的的page里有以前的脏数据，然而拿到的时候，他并不会进行初始化，而是直接读record_num = ....这里可能是之前的旧数据，非常非常大，所以memcpy崩了

但是实际上打上日志，并不是这方面的原因。是我犯蠢了，基类里有些函数没有实现。

bug2:
LookupChildPageID里的upper_bound边界不对，右边界应该在最后一个元素的右边（空）

整体的fetch，unpin逻辑：
  不是谁fetch谁unpin，而是谁拿到page，谁unpin

bug3:
operator* 返回leaf == nullptr
可能是因为begin(key)是一个临时对象，赋值以后自动析构调用unpin了，所以此时的leaf是一堆垃圾数据，但是这样也不应该返回operator* 是nullptr

打了日志，发现test循环本来只应该跑五次，结果第六次条件iterator != tree.end()是true，进入了本不应该存在的第六次循环
operator!=的实现有问题
第 5 次 ++ 后： iterator leaf_=nullptr, index_=上一个叶子的末尾值，End 是 leaf_=nullptr, index_=0。index_ 不同，所以 != 返回 true。
operator!= 的实现应该加一层，如果两个的leaf都是nullptr，直接返回false
成功

std::vector dummy;
  bool is_dup = GetValue(key, &dummy);  
  if (is_dup) return false;
Insert里调用getvalue是找死，他们都会调用root_latch，分分钟死锁

ReleaseAnccestor函数， 里面全部是unpin（false），然后split很可能会递归上去，修改父亲，这完全不对。我需要set当前的page是脏页

优化一下io，遇到父节点有改动，就直接处理脏页

gdb调试并发死锁的流程

gdb --args ./test/b_plus_tree_concurrent_test --gtest_also_run_disabled_tests --gtest_filter=*InsertTest1
run
set pagination off //关掉翻页
info threads

thread apply all bt //查看所有线程的调用栈

thread 2
bt

thread 2
frame 8
info locals

并发bug1：
发现bug：insert发现是空树调用crateleaf后，解锁返回nullptr，但是transaction里还有一个null哨兵，这个pageset队列不会自动清空，一直留着的，会造成重复解锁，可能搞坏

已解决。

并发bug2：
thread6与thread7冲突，
#14 0x00005d1dc133a40c in bustub::BPlusTree<bustub::GenericKey<8ul>, bustub::RID, bustub::GenericComparator<8ul> >::Remove (this=0x7ffde400d350, key=..., transaction=0x512000020440) at /bustub/src/storage/index/b_plus_tree.cpp:212 //thread6的状态
#8  0x00005d1dc133a32e in bustub::BPlusTree<bustub::GenericKey<8ul>, bustub::RID, bustub::GenericComparator<8ul> >::Remove (this=0x7ffde400d350, key=..., transaction=0x512000010540) at /bustub/src/storage/index/b_plus_tree.cpp:203 //thread7的状态

gdb发现，是在这里发生的死锁，此时两线程都处于remove，而且thread6已经拿到那个全局唯一的root_latch_了，在想拿root_page的锁，但是卡住了，而thread7在想拿root_latch_。既然发生了死锁，说明这个root_page的锁在thread7手上。

分析thread各自的frame，thread7变量还是乱码，说明还没进入remove。
现在发现是remove坏了，回去重新检查

AdjustRoot:
page_id = 1 (root)
IsLeafPage = true
GetSize = 4

说明命中adjustRoot以后删除没有走进任何语义，直接return。root_page的锁泄露。

已解决。

并发bug3:
contention_test里，野指针
#1 0x604c356037d1 in bustub::BPlusTreeLeafPage<bustub::GenericKey<8ul>, bustub::RID, bustub::GenericComparator<8ul> >::InserttoLeaf(bustub::GenericKey<8ul> const&, bustub::RID const&, bustub::GenericComparator<8ul> const&) /bustub/src/storage/page/b_plus_tree_leaf_page.cpp:111

e_leaf_page.cpp:88
88          right->array_[j] = std::move(array_[i]);
(gdb) info locals
i = 16670，j = 2918，size = 27505，mid = 13752，moved = 13753
这里也有问题，页的元数据被写烂，而不是split边界有问题。

getpageid = 0,这个是页头，有人把页头传过来了。

脏页，分析一下，可能是double unpin了 bpm evict了写来了新东西，别的线程如果在原frame有东西，那么这时候getsize()读的就是垃圾

断言double unpin
#9  0x0000605758ed309e in bustub::BPlusTree<bustub::GenericKey<8ul>, bustub::RID, bustub::GenericComparator<8ul> >::SplitLeaf (this=0x7ffc63497850, page=0x7d0270d4c848, transaction=0x51200000ff40)
    at /bustub/src/storage/index/b_plus_tree.cpp:407
#10 0x0000605758ece42f in bustub::BPlusTree<bustub::GenericKey<8ul>, bustub::RID, bustub::GenericComparator<8ul> >::Insert (this=0x7ffc63497850, key=..., value=..., transaction=0x51200000ff40)
    at /bustub/src/storage/index/b_plus_tree.cpp:185

```
铁证
```

inserttoparent里那个if当前是根节点的路径，多unpin了right，这个本来是留给调用方unpin的

依然未解决，问题甚至相同，说明还有bug.

问题2：
[off-by-one](../../project/database2022fall/src/storage/page/b_plus_tree_internal_page.cpp)
如果当前pos和getsize一样，直接不进入循环，原来那个key就扔掉了。
后面有increase(1)，最后这个就是垃圾值，所以乱读读错了。

问题3：
有循环引用
(gdb) p next_page_id
$1 = 93
(gdb) p curr_page->GetPageId()
[New Thread 0x7cd2dece0640 (LWP 8916)]
$2 = 93
(gdb) p root_page_id_
$3 = 94

next_page 和 curr_page是一个id ，internal里面存了一个value结果指向的是自己

value里并没有93，理论上来说不可能返回当前节点page_id的

找到原因：是上一层bufferpool fetch以后没有设置is_evictable，导致fetch以后仍能被扔掉。

done

现在 我们做一个总的架构复习

DiskManager -> BufferPoolManager -> Page -> B+TreePage

分为Internal Page 和Leaf Page
其中Internal的keyat[0]是空，绝对不要调用,Value是叶子的page_id
Leaf的value则是对应Tuple的RID，RID里面包含磁盘里装了该Tuple的物理Page的pageid和slot
只有leaf才装着真实的数据，每个leaf_node是一个单向链表，天生有序，适合range scan

## FindLeafPage

一个非常常用的接口
核心是，乐观锁
insert或remove的时候，先用读锁一步一步往下，和纯读的路径唯一的区别是，拿到叶子要加写锁，如果叶子安全，直接写，结束。如果不安全，那么就切换悲观锁，重来一次

## FindLeafPage 并发策略


| 操作         | 初始 latch                                 | 下降过程               | safe node 判定                                                     | transaction 记录          |
| ---------- | ---------------------------------------- | ------------------ | ---------------------------------------------------------------- | ----------------------- |
| Read       | root R-latch → child R-latch → 释放 parent | 逐层释放               | 不需要                                                              | 不需要                     |
| Write (乐观) | 同 Read，到 leaf 升级 W-latch                 | 同 Read             | leaf 未满/超半满 → safe，直接写                                           | root_latch 用 nullptr 哨兵 |
| Write (悲观) | root W-latch → 全程 W-latch                | child safe 则释放所有祖先 | insert: size < max-1(leaf，internal则是<max即可>); delete: size > min | nullptr → 祖先页（leaf 不入）  |


**关键不变量：**

1. **Latch ordering**: 永远先拿 child latch 再放 parent latch，反之死锁
2. **Transaction 不持有 leaf**: leaf 由 Insert/Remove 调用方管理 unpin，transaction 只管祖先路径。违反 → double unpin → 野指针

### GetValue


| 步骤  | 动作                            | 资源管理               |
| --- | ----------------------------- | ------------------ |
| 1   | FindLeafPage(Read)            | 返回时持有 leaf R-latch |
| 2   | leaf 内 lower_bound 查 key      | —                  |
| 3   | 找到则写入 result，未找到返回 false      | —                  |
| 4   | leaf R-unlatch + Unpin(false) | dirty = false，只读   |


### Insert


| 步骤  | 动作                                         | 资源管理                          |
| --- | ------------------------------------------ | ----------------------------- |
| 1   | 空树 → CreateLeafPage 建根，直接插入返回              | 新页 Unpin(true)，释放 root_latch  |
| 2   | FindLeafPage(Insert, 乐观)                   | 返回时持有 leaf W-latch            |
| 3   | 检查 duplicate key，有则释放 leaf 返回 false        | leaf W-unlatch + Unpin(false) |
| 4   | 插入 key-value 到 leaf                        | —                             |
| 5   | leaf size < max_size → safe，Unpin(true) 结束 | 释放 leaf W-latch               |
| 6   | leaf size == max_size → Split              | 新建 sibling，移动后半到 sibling      |
| 7   | InsertToParent(parent, split_key, sibling) | 递归：parent 满则继续 split，safe 则停止 |
| 8   | 释放 transaction 中所有祖先页                      | 逐个 W-unlatch + Unpin(dirty)   |


**Split 要点：**

- leaf split: `split_key = sibling.array_[0].first`（sibling 最小 key 上推）
- internal split: `split_key = 中间 key 上推后从 internal 中删除`（不保留在 internal）
- 根分裂 → 新建 root page，更新 header page 中的 root_page_id

### Remove


| 步骤  | 动作                                             | 资源管理                                                 |
| --- | ---------------------------------------------- | ---------------------------------------------------- |
| 1   | FindLeafPage(Delete, 乐观)                       | 返回时持有 leaf W-latch                                   |
| 2   | leaf 中找不到 key → 释放 leaf 返回 false               | leaf W-unlatch + Unpin(false)                        |
| 3   | 删除 key-value                                   | —                                                    |
| 4   | leaf size >= min_size → safe，Unpin(true) 结束    | 释放 leaf W-latch                                      |
| 5   | leaf underfull → 先尝试 Redistribute（向 sibling 借） | 需要 fetch sibling + 更新 parent key                     |
| 6   | Redistribute 不可行 → Merge（与 sibling 合并）         | 合并后 delete sibling page，递归向上删 parent key             |
| 7   | 递归到 root → AdjustRoot                          | root 只剩 1 个 child → child 变新 root；root 是空 leaf → 树清空 |
| 8   | 释放 transaction 中所有祖先页 + 删除 deleted page set    | W-unlatch + Unpin(dirty)；DeletePage 回收               |


**Merge vs Redistribute 判定：**

- sibling size > min_size → Redistribute（借一个过来，更新 parent 的 split key）
- sibling size == min_size → Merge（合并后 size ≤ max，向 parent 删 key，递归）
- 优先同一 parent 下的左兄弟，没有才用右兄弟

## 并发控制
## latch vs lock
lock是事务级别的，保护逻辑数据，最大的区别应该是支持deadlock detection

latch是内存级别的互斥，保护内存的数据结构，不存在deadlock detection，只能做到尽量避免

## 悲观锁与乐观锁
悲观锁：读和写要考虑transaction, 判断路径中的结点是否安全，如果安全则说明目前即使孩子分裂或合并了，也不会影响祖先的，所以可以释放了。
乐观锁：纯读的路径遍历，区别就是找到叶子结点时是上的写锁，判断叶子是否安全，安全就继续写，不安全就切换悲观锁模式，再走一遍。

## root_latch
根节点的page_id需要维护一个单独的root_latch,因为我们要拿到根节点的id，此时完全有可能因为其他线程的合并或者分裂而改变了根节点的id，如果不加锁，本线程拿到旧的root_id来从bpm取page，就会变得一团糟。

root_latch_ 逻辑上是 root page 的 parent latch，保护 root_page_id_ 的读写一致性。遍历时将其视为第一层 parent，遵循相同的 crabbing 释放规则。

## SafeNode定义
当目前节点，即使插入了一个key，也不会触发分裂。或者即使删除一个节点，也不会触发合并，就可以定义为安全

当然，对于根节点就是叶子的情况要另外看待


# 踩坑
## bug 1 27504
- 现象: node的getsize读出27504
- 分析：很明显page被读坏，分析得到私有成员变量就在这个page地址的8~11偏移处，而27504换为底层十六进制的表达是0x6B70,注意到p的ascii码是0x70,k的ascii码是0x6B
在header_page中，每棵树在这里都有对名字和对应root_id的记录，正好在测试文件中，这个树叫做foo_pk
分析内存位置：header_page的offset 0~3是记录了树的个数的int整数，四个字节，随后是name[32]固定占32字节，和rootid，每一块36个字节
观察8~11偏移处
4~7: 'f' 'o' 'o' '_' 
8~11: 'p' 'k' ...后面是为了内存对齐，padding的字节
破案，我们读到page0了。读数字时，低地址在前，高地址在后,所以pk会变成0x6B70

正常来说，不可能取到page0
bug非常可笑，是测试文件里用到了迭代器，而我有一些迭代器的接口没有实现，所以是这样子的

## bug 2 upper_bound边界问题
LookupChildPageID里的upper_bound边界不对，右边界应该在最后一个元素的右边（空），否则始终会漏掉最后一个元素

### Bug: iterator operator!= 语义错误
- **现象**: 迭代器遍历多跑一次，最后一次 `operator*` 返回 nullptr
- **定位**: 打日志发现循环 5 次变 6 次，`iter != end()` 在应该终止时返回 true
- **根因**: `operator!=` 只比较 `leaf_ != other.leaf_`，
  未处理双方 leaf_ 均为 nullptr 的情况（end iterator）
- **修复**: 双方 leaf_ 均为 nullptr 时直接返回 false
- **教训**: 迭代器的终止条件必须覆盖 sentinel 状态，写完 operator 先枚举边界组合


### Bug: 空树 insert 后 root_latch 重复解锁
- **现象**: unlock of an unlocked mutex
- **定位**: 给所有 unlock 语句打断点，定位到 root_latch 被解锁两次
- **根因**: 空树时 CreateLeaf 路径手动解锁了 root_latch 并 return，
  但 transaction 中的 nullptr 哨兵未清理，后续统一释放逻辑再解一次
- **修复**: CreateLeaf 路径 return 前清空 transaction 的 page set
- **教训**: 提前返回的特殊路径必须与通用 cleanup 路径互斥，
  否则资源被释放两次

### Bug: AdjustRoot 锁泄露 → 全局死锁
- **现象**: 死锁，gdb 发现 T6 持有 root_latch 等 root_page W-latch，
  T7 持有 root_page W-latch 等 root_latch
- **定位**: 分析 T7 调用栈，处于 Remove 入口 → 说明上一次 Remove
  退出时没释放 root_page 的锁
- **根因**: AdjustRoot 只处理了"根是空叶子"和"根只剩一个 child"两种 case，
  第三种情况（根删 key 后仍合法）直接 return，未释放 root_page W-latch
- **修复**: AdjustRoot 末尾补 default 路径的 unlatch
- **教训**: 持有 latch 的函数，每条 return 路径逐一审计释放逻辑


### Bug: InsertToParent 多 Unpin sibling → 高并发脏数据
- **现象**: 并发 split 时 leaf page 读到垃圾数据（size=27505）
- **定位**: gdb 查看出错 page 的 page_id=0（header page），
  frame 已被其他 page 占用
- **根因**: InsertToParent 中根分裂路径多 Unpin 了 sibling，
  pin_count 提前归零 → BPM 驱逐该 frame → 其他 page 写入
- **修复**: 根分裂路径删除对 sibling 的 Unpin，由调用方统一管理
- **教训**: 一个 page 的 Unpin 只能有一个 owner，
  分裂/合并路径中用注释标明"谁负责释放谁"

### Bug: 自锁 —— parent 与 child 是同一 page_id
- **现象**: 高并发下偶发死锁，线程持有 parent W-latch 后 fetch child，
  发现 child page_id == parent page_id，再拿锁 → 自锁。极低概率复现
- **定位**: 排除应用层逻辑（正常 B+ Tree 中 parent 与 child 不可能同 page_id）
  → 怀疑 BPM 层 frame 被错误复用 → 检查 FetchPage 流程
- **根因**: FetchPage 后未调用 SetEvictable(false)，
  replacer 仍认为该 frame 可驱逐 → child 的 frame 被 evict →
  其他线程触发 fetch 恰好将 parent page load 进同一 frame →
  T1 用旧指针读到 parent 的数据，解析出的 child page_id == parent page_id
- **修复**: FetchPage 后立即 SetEvictable(false)，
  与 UnpinPage 中 pin_count=0 时 SetEvictable(true) 配对
- **教训**: BPM 与 Replacer 是两套独立状态，
  靠调用方保持同步。漏一处 → 状态不一致 → 非确定性 bug，极难复现

## 工业界对比

### 崩溃恢复
InnoDB的b+tree修改流程，要先写redo log，崩溃后根据redo log来重放未刷盘的修改

### 页管理
bustub的这个只有一个bpm，用一把全局锁保护page table
InnoDB将bpm分为多个instance，page_id hash到不同的instance，减少锁的争用

