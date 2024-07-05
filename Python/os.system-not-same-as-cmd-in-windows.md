Windows 下 os.system 执行结果和命令行不一致
os.system-not-same-as-cmd-in-windows

Created: 2024-06-19T15:52+08:00
Published: 2024-06-19T16:01+08:00

Categories: Python
Tags: Bug

长话短说，Windows 下，`os.system(cmd)` 的执行结果和直接在 cmd.exe 中敲 cmd 的结果是不一样的。
尤其是涉及到管道的使用，要极为小心。

`os.system(echo password | login)`: password 后面的空格也被一起送入 pipe 
`os.system(echo password| login)` 才是正确的

验证：

```py
import os
os.system("echo hello   n")
```
