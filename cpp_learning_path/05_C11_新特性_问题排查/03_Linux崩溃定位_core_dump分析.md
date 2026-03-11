## Linux 程序崩溃定位与 core dump 分析

可以在这里整理和实现这些问题：

- Linux 程序崩溃一般怎么定位问题？常见崩溃类型（段错误、断言失败等）？
- C++ 代码异常 core dump 会生成一个 core 文件，里面大致包含什么信息？
- 如何打开/配置 core dump（ulimit、/proc/sys/kernel/core_pattern 等）？
- 如何用 `gdb`、`addr2line` 等工具分析 core dump，定位崩溃位置和调用栈？

建议：写几个会崩溃的小程序（空指针解引用、越界访问等），打开 core dump，实际用 gdb 加载并分析。

