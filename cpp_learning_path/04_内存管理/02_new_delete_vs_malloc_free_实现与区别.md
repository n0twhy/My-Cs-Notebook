## new/delete 与 malloc/free 的区别与实现

可以在这里整理和实现这些问题：

- C++ 中 `new` 是在内存上哪一块分配的内存？`delete` 做了什么？
- `new` 失败了会怎么样？和 `malloc` 返回值为 `NULL` 的对比？
- C++ 中 `malloc` 和 `new` 的区别是什么？在构造/析构、类型安全、对齐等方面的差异？
- `new` 和 `malloc` 如何判断是否申请到内存？是否可以自定义 `operator new`？
- `delete` 的实现原理？`delete` 与 `delete[]` 的区别？

建议：写例子混用 `malloc/free` 与 `new/delete`（包括错误用法），观察 UB 和崩溃；再用自定义 `operator new/delete` 打日志。

new和malloc是内存上堆那一块的内存，从下往上，操作系统有一个记录空闲内存地址空间的链表，当收到申请时，会遍历找一个大于所需空间的内存空间来分配

new失败会抛出异常std::bad_alloc

delete是执行该对象的析构函数，然后释放内存空间
delete[]