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