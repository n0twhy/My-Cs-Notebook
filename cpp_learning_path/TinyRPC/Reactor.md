# loop()一圈

while (!m_stop_flag) {
A.  if (first_coroutine) Resume(first_coroutine) //上一轮登记的
B.  while (CoroutineTaskQueue::pop() == ptr) Resume; //只有SubReactor做
C.  for task in m_pending_tasks: task() //本地普通任务
D.  epoll_wait(...);
E.  遍历返回事件：
      第一个绑协程的->登记first_coroutine(不resume)
      第2...N个，push进全局task队列  
}

epoll_wait拿到事件，本轮先分发，第一个归自己，剩下的扔全局。下一轮再开始执行

Reactor有五种状态
状态 | 阶段 | CPU占用
| --- | --- | --- |
跑子协程业务代码 | A/B中`Resume` | 忙(用户态CPU)
跑`pending_task的lambda` | C | 忙(用户态CPU)
在E分发事件 | E | 忙(非常短)
`epoll_wait` block | D | 不忙




