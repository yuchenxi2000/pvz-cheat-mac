# pvz-cheat-mac

植物大战僵尸Mac版修改器

> 该项目已废弃

> This repo is no longer maintained.

> 该修改器没有UI，使用时在 main.cpp 里调函数然后编译运行吧（以 root 身份）

## 简介

该修改器是对 https://github.com/zhoury18/PvZHelper-Mac 的扩展，主要是植物强化。

因为是扩展，所以功能不完全，请配合 zhoury18/PvZHelper-Mac 使用。

* 作弊

在场地上添加子弹，曾哥全屏，窝瓜全屏，植物狂暴，大嘴花吃巨人，植物射玉米炮等植物强化，以及无视阳光种植，我是僵尸模式随意放置等。

* 补丁

爆炸坚果在非保龄球模式死亡时爆炸，我是僵尸模式正常放置植物。

## 实现

游戏版本 Plants vs. Zombies v1.0.40 (Mac)，反汇编工具 Hopper Disassembler。使用 zhoury18/PvZHelper-Mac 里的内存读写工具，使用 asmjit/asmjit 作为汇编器。

## 你可以做什么

因为我懒得写图形界面，平时要修改时直接Xcode里面运行，给不会编程的带来了一些麻烦。。

1. 下源码（pvz-code文件夹）在Xcode或者其他IDE里面运行。（必须以root身份运行）

2. pvz-disassemble 里是我通过反汇编找到的函数地址、内存地址表以及植物、僵尸ID表、关卡ID，还有一些补丁（汇编），可以作为反汇编写修改器的参考

3. pvz-code 里的代码注入框架和 zhoury18/PvZHelper-Mac 里的一致。你可以把这个整合到 zhoury18/PvZHelper-Mac 里
