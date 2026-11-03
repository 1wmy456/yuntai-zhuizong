# Vivado与Vitis使用

## Vivado

`hardware/ov5640_lcd.xpr`为包含触摸、原始帧和双轴控制的集成工程。根目录的`ov5640_lcd.xpr`为脚本集成所用的视频基础工程。

使用Vivado 2020.2打开集成工程，生成BD输出与wrapper后进行综合、实现及bit生成。若直接下载发布程序，不需要重新运行综合。

## Vitis

1. 用`release/speed300/system_wrapper.xsa`创建平台，处理器选择`ps7_cortexa9_0`，系统选择standalone。
2. 创建空C应用，导入`vitis/ov5640_lcd/src`的全部文件和子目录。
3. 使用随源码的`lscript.ld`及`Xilinx.spec`。算法目录的C文件必须全部参与编译；`appearance_tracker.c`、`appearance_runtime.c`、`cap_components.c`、`cap_runtime.c`使用O2，其余可使用O0。
4. 核对程序末地址低于`0x01100000`，使用同一平台的BSP及PS初始化文件。
5. 下载bit后运行硬件应用，确认处理器已执行PS初始化；启动默认manual、PWM关闭。

采用命令行`build_release.ps1`可直接复用随包BSP和相同编译设置，避免手工遗漏源文件。主机算法测试还需GCC，默认路径为`D:/msys64/ucrt64/bin/gcc.exe`，测试脚本提供`HostGcc`参数。
