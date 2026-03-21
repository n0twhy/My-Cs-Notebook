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