`EPOLLIN`：事件可读
`EPOLLOUT`:事件可写

分别用来监听`recv_buf`, `send_buf`

socket() 申请一个fd
bind() 可以将fd绑定IP,PORT
listen() 内核给这个fd分配两个队列：SYN队列(也就是半连接队列，SYN报文就在这)
                                 accept队列(全连接队列，三次握手完成的就放这，等我的Reactor来accept)
client连接(三次握手，内核自己完成)
client: SYN  ------->  server:丢进SYN队列
client：     <-------  server:ACK + SYN 
client: ACK  ------->  server：从SYN队列拿出，放进accept队列

这一刻listenfd变得可读，epoll报EPOLLIN

accept可以拿到clientfd

每一个fd都带有一个socket的结构里面装着recv_queue和write_queue,epoll_wait监听可读和可写就是检测这两个队列

EINTR 被信号打断
EAGAIN 没有可读或可写的了 
ECONNRESET 网络重置了

SOCK_NONBLOCK 非阻塞模式，I/O操作不会挂起
SOCK_CLOEXEC exec系列系统调用时自动关闭该fd

TCP事件 | read返回 | errno | 语义
| --- | --- | --- | --- |
对端close | 0 | 不设置 | 收到FIN，关闭
RST | -1 | ECONNRESET | 收到RST，暴力中断

printf 'HELLO\n' | nc -q0 127.0.0.1 8080