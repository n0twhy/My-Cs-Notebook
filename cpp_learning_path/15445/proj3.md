内置类型自带，自定义struct必须手动提供两者，否则编译失败

## elevator pitch
实现了一个基于volcano模型的查询执行引擎，包括seqscan,insert,delete等
还有一些优化器规则

## volcano model
每个executor暴露init + next接口，上层算子拉去下层的数据，形成自顶向下的pipeline

优点：实现简单，算子之间解耦
缺点：每次next只返回一行tuple，函数调用的开销很大








