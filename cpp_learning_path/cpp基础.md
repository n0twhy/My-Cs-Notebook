[三个数比较](../../algorithm/leetcode_hot100/DP/88_乘积最大子数组.cpp)
用min({a, b, c})

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