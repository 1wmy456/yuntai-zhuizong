# 构建与验证

## 发布配置

|配置|用途|主要参数|
|---|---|---|
|speed150|触摸选择与快速跟随|Kp2，Ki/Kd0，速率150，加速800/减速1200|
|speed100|触摸三色切换对照|Kp1.5，Ki/Kd0，速率100，加速500/减速800|
|lost|固定视觉PD对照|蓝色目标、Kp0.7，Ki0，Kd0.03，速率75|

速率单位为μs/s，加减速度单位为μs/s²。均默认manual、PWM关闭。speed150和speed100匹配touch_capture_v3硬件；lost随附自身硬件及PS初始化。

lost运行时可在ASCII串口设置：

```text
manual
disable
dir yaw -1
dir pitch 1
limits yaw 1100 1650
limits pitch 1240 1800
smooth on
deadband 15 22
controller pd
kp yaw 0.7
kp pitch 0.7
ki yaw 0
ki pitch 0
kd yaw 0.03
kd pitch 0.03
rate yaw 75
rate pitch 75
accel yaw 400
accel pitch 400
decel yaw 600
decel pitch 600
```

确认方向、范围、目标及供电后，再enable both、verify yaw、verify pitch、auto。

## 注释与构建

项目C、头文件与RTL共79个文件进行注释清理，保留综合指令和必要中文说明。非注释代码经过比较，字符串内容及逻辑未改。第三方声明移至单独文档。

整理后的17个C文件在本目录重新编译，text 170464、data 2857、bss 1918480字节，_end=0x00300650，低于VDMA缓存起点0x01100000。`.text`、`.rodata`、`.data`及`.vectors`与已验收150档ELF逐字节一致。

发布硬件实现结果为WNS +0.571ns、WHS +0.014ns、TNS/THS 0；LUT 10291、FF 14923、BRAM 15.5、DSP 2；DRC无Error，31 Warning、6 Advisory。`release`中使用该已上板验收硬件。

独立目录另完成综合、布局布线及bit/XSA生成，WNS +0.319ns、WHS +0.062ns，TNS/THS为0，时钟约束未放宽。其报告位于`docs/evidence/rebuilt`。触摸及PWM引脚、地址、视频异步时钟跨域和原始帧来源检查均通过。该重新实现结果没有替换默认烧录硬件，也没有重新进行物理云台验收。

## 功能证据范围

源基线现场测试已确认触摸五点定位、蓝红黑点击选择与自动切换、空白点击保护、多目标中丢失所选目标不抢锁；150档移动、换向、停稳、失目标保持和重选恢复正常。独立整理版另进行软件重编译、程序段一致性、自动回归、BD集成及发布文件检查。整理过程不重新驱动现场舵机。

白色在低对比背景下会误匹配或丢失，黄色未稳定识别；没有任意颜色或通用语义识别能力。控制框架含PD/PID，但150档Ki/Kd均为零；非零Ki没有机械验收结论。

详细检查结果及发布校验值见 `docs/evidence/`、`release/manifest.json`。
