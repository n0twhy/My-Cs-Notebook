[std::list](../../project/database2022fall/src/include/container/hash/extendible_hash_table.h#L166)
std::list<T> lst  默认是一个双链表容器
lst.empty()
lst.size()
lst.front()
lst.back()
lst.begin()
lst.end()
push_back push_front pop_back  pop_front  insert(it, x)//在迭代器it前插入x  erase(it) 删除迭代器指向的元素，返回下一个迭代器
如果要找某个值 用迭代器循环或者find
[](../../project/database2022fall/src/include/container/hash/extendible_hash_table.h#232)
这里就是经典用法，如果删除了it，又使用++it，会访问非法内存


[位运算符](../../project/database2022fall/src/container/hash/extendible_hash_table.cpp#L30)
1 << global_depth_是将"1"左移"global_depth"位

[扩容机制](../../project/database2022fall/src/include/container/hash/extendible_hash_table.h)
可以直接pushback  因为如果加1位，比如2->3，如果是01，那么同理的会有（101，001）所以101的也会指向这个桶，那么就简单了，直接push_back,非常优雅

成员变量在初始化列表中的顺序与他们在类中的声明的顺序应该一致

[friend](../../project/database2022fall/src/include/storage/page/page.h#L30)
关键字用于可以访问当前类的私有成员,或者protected

[new Page[pool_size]](../../project/database2022fall/src/buffer/buffer_pool_manager_instance.cpp#L24)
这里相当于vector<Page> pages_(pool_size)

[内存泄漏](../../project/database2022fall/src/buffer/buffer_pool_manager_instance.cpp#L62)
这里的指针指向已有的内存，可以不delete，等析构统一释放这片内存，不会发生内存泄漏，指针本身是存在全局变量栈

[this](../../project/database2022fall/src/storage/index/index_iterator.cpp)
this是当前这个对象的指针，返回当前对象的话，return *this

[迭代器]
发现了一个新用法，如果要删除vector中间某个元素,vector.erase(vector.begin() + i)，后面的元素会自动前移，返回下一个元素的迭代器。
因为后面的内存地址改变，erase前的所有迭代器失效
如果是map，底层是红黑树，内存独立，erase一个不影响其他的。

[运算符重载](../../project/database2022fall/src/storage/index/index_iterator.cpp)
运算符重载：operator*、operator++、operator== 是特殊命名的成员函数，编译器自动将 *it、++it、a == b 翻译为对应函数调用
Past-the-end 语义：End() 不指向最后一个元素，而是"越过末尾"的哨兵，和 STL 一致。默认构造（leaf_=nullptr）即为 end

[text](../../algorithm/huawei/模拟/海量日志抑制.cpp)
log.find是返回size_t类型的下标

[优先队列](../../algorithm/huawei/DFS/地铁耗时最短的线路.cpp)
priority_queue<pair<int, char>, vector<pair<int, char>>, greater<pair<int, char>>>
第一个传参就是元素类型
第二传参是优先队列的底层容器，一般是vector
第三个传参就是比较器，有greater（小顶堆）和less（大顶堆），也可以自己设置struct cmp比较器

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
vector dummy
需要传指针时传入&dummy，可以有效防止内存泄漏的问题

[后置int](../../project/database2022fall/src/storage/table/table_iterator.cpp)
operator++(int)表示后置: itr++
operator++()表示前置: ++itr

RID:  page_id  |   slot_num

[iter](../../project/database2022fall/src/include/execution/executors/seq_scan_executor.h)
可以看到iterator构造需要四个函数，这里只声明类型编译过不了

[hash重载](../../project/database2022fall/src/include/execution/plans/aggregation_plan.h#L126)
### 自定义类型作为unordered_map的key
`std::unordered_map<k, v>`要求k同时满足
1.`std::hash<k>`的特化
2.`operator==`

[text](../algorithm/huawei/DP/D路通信.cpp)
dp[i][j] = (i > 0 && j > 0) ? dp[i - 1][j - 1] + 1 : 1;
三元表达式的两个分支都会被计算，越界

有意思的小技巧：
### vector去重
unique以后，在物理上begin到end还在，长度没变，然后他会返回去重后的序列的end()
注意unique需要数组有序，他只会删除相邻的重复数，把删除掉的放在末尾
所以标准的去重是:
v.erase(unique(v.begin(), v.end()), v.end());
### vector删除
v.erase(remove(v.begin(), v.end(), key), v.end())