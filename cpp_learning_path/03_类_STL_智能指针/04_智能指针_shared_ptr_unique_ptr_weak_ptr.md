## 智能指针：shared_ptr / unique_ptr / weak_ptr

可以在这里整理和实现这些问题：

- 介绍一下 **智能指针**？解决了什么问题？
- 在哪些场景下最应该使用智能指针？
- `shared_ptr` 的作用是什么？引用计数的大致实现原理？
- `weak_ptr` 的作用是什么？如何解决 `shared_ptr` 循环引用问题？
- 为什么 `shared_ptr` 不能直接管理数组？（及对应解决方式）
- 什么是 `unique_ptr`？怎么把一个 `unique_ptr` 转移给另一个 `unique_ptr` 对象？
- 在容器中使用 `unique_ptr` 可能会遇到什么问题？如何解决？

建议：分别用裸指针和智能指针实现对象所有权管理，对比内存泄漏风险；尤其练习 `shared_ptr` 循环引用与 `weak_ptr` 解除循环。

1.unique_ptr是单独持有的指针  不可共享，需要move