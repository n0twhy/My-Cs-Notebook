| 你想看什么 | 命令 | 关键输出 |
|------------|------|----------|
| 所有进程 | `ps -ef` 或 `ps aux` | PID、PPID、CMD |
| 进程树 | `pstree -p` | 父子关系一目了然 |
| 某进程的所有线程 | `ps -eLf \| grep <pid>` 或 `top -H -p <pid>` | LWP 是线程 ID |
| 某进程的虚拟内存布局 | `cat /proc/<pid>/maps` | 各段起止地址、权限 |
| 某进程的内存详情（带大小） | `cat /proc/<pid>/smaps` 或 `pmap -X <pid>` | 每段的 RSS、PSS、Swap |
| 某进程总内存占用 | `cat /proc/<pid>/status \| grep -E 'Vm\|Rss'` | VmSize、VmRSS、VmPeak |
| 某进程的栈 | `cat /proc/<pid>/maps \| grep stack` | 主线程栈位置 |
| 某线程的栈 | `cat /proc/<pid>/task/<tid>/maps \| grep stack` | 该线程独立栈 |
| 系统总内存 | `free -h` | 已用/可用/cache/swap |
| 实时进程内存排名 | `top` 后按 `M` | RES 列是物理内存 |
| 某进程打开的 fd | `ls -l /proc/<pid>/fd/` 或 `lsof -p <pid>` | 文件描述符 |
| CPU 占用排名 | `top` / `htop` | 默认按 CPU |
| 看系统调用 | `strace -p <pid>` | 实时所有 syscall |
| 看哪个进程在用某文件 | `lsof <file>` | PID 列 |