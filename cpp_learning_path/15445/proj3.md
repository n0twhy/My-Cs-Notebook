内置类型自带，自定义struct必须手动提供两者，否则编译失败

## elevator pitch
实现了一个基于volcano模型的查询执行引擎，包括seqscan,insert,delete等
还有一些优化器规则

## volcano model
每个executor暴露init + next接口，上层算子拉去下层的数据，形成自顶向下的pipeline

优点：实现简单，算子之间解耦
缺点：每次next只返回一行tuple，函数调用的开销很大

[浅拷贝](../../project/database2022fall/src/execution/index_scan_executor.cpp)
iter_ = b_plus_tree->GetBeginIterator();  
这里返回的是临时迭代器A，A手里拿着page=11

浅拷贝也就是iter_也变成page = 11
但A还是指向page11
临时对象A的生命周期结束，析构unpin

iter_此时手里的page就是pin数为0
iter_再析构，就会在调一次unpin
所以duble unpin

Solution:operator=换成move语义，如果move，原临时对象A就会被置空,析构也没用





