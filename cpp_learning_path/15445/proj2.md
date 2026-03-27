[二分查找](../../project/database2022fall/src/storage/page/b_plus_tree_internal_page.cpp)
lower_bound(first, last, value)
找一个位置it，使得[first, it)严格在value左侧，也就是第一个满足*it >= value的地方
cmp(it. value) 语义上就是it是否在value前面。是就是true。lower返回第一个cmp为false的，也就是it < value 为false(which means it >= value)
upper_bound(first, last, value)
找一个位置it，使得*it > value，cmp是返回第一个cmp为true的。cmp(value, it)，第一个 value < it为true的,也就是第一个严格大于value的it

[友元](../../project/database2022fall/src/storage/page/b_plus_tree_leaf_page.cpp)
cpp相同类的不同对象为友元，可以互相调用其私有成员

注意哈  leaf的array尾巴就是size - 1，而internal的尾巴就是size

[内存泄漏](../../project/database2022fall/src/storage/index/b_plus_tree.cpp#L101)
vector<int> dummy
需要传指针时传入&dummy，可以有效防止内存泄漏的问题

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

offset 0~3 ReacordCount
offset 4~35 :
4~7: 'f' 'o' 'o' '_'
8~9 'p' 'k'
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

GetSize()瞄准了8~11的内存地址去取的size_ 
然而这里取得是header_page的内存空间，所以读到的是8~11的"pk"转换为int ('p':0x70, 'k':0x6B)，连续内存低位在前于是翻译过来就是0x6B70也就是27504

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



