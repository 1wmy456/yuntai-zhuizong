# 云台追踪

基于 Zynq-7020、OV5640、800×480 触摸 LCD 和双 MG90S 的视觉云台。点击屏幕选择目标，系统在板上完成外观建模、定位、画框和双轴跟随。

## 功能

- GT1158 触摸读取、坐标映射和位置十字。
- 点击建立目标模型；蓝、红、黑瓶盖已进行现场测试。
- 三色同时入镜时点击切换目标；空白点击保留有效目标。
- 目标丢失清框并保持，不自动接管其他目标。
- 150μs/s 脉宽变化率、加减速约束与死区滞回。
- P/PD/PID 控制框架；触摸配置使用 Kp=2、Ki=Kd=0。

白色复杂背景和黄色目标测试未通过；算法依据外观和区域形状，不提供通用瓶盖语义识别。

## 目录

|目录|内容|
|---|---|
|`rtl/`|视频、原始帧、触摸叠加及 PWM RTL|
|`vitis/ov5640_lcd/src/`|实际参与编译的17个C文件及头文件|
|`ov5640_lcd.srcs/`、`ip_repo/`|基础BD、XDC和自定义视频IP|
|`constraints/`|PWM和触摸接口约束|
|`hardware/`|已集成触摸与双轴控制的可打开Vivado工程|
|`sim/`|RTL自检与主机C测试|
|`scripts/`|集成、编译、测试、下载及参数恢复|
|`release/`|可直接下载的 bit、XSA、PS初始化及ELF|
|`platform/`|匹配XSA的standalone BSP归档|
|`docs/`|接线、测试、构建和发布说明|

## 下载程序

Windows PowerShell，Vivado/Vitis 2020.2，开发板上电并连接JTAG。工具默认目录为 `C:/Xilinxx/`，可通过脚本参数调整。

```powershell
cd D:\2026.test\kaoshi
.\烧录.ps1
.\scripts\restore.ps1 -Profile speed150
```

启动为手动、PWM关闭。确认LCD画面正常，点击瓶盖出现绿框后，在115200/8N1串口以ASCII逐行发送，末尾加CR/LF：

```text
enable both
verify yaw
verify pitch
auto
```

`manual`保持位置，`disable`关闭PWM。普通恢复范围为yaw 1100～1650μs、pitch 1240～1800μs。需要沿用现场宽范围实验时执行：

```powershell
.\scripts\restore.ps1 -Profile speed150 -WideExperiment
```

500～2500μs是实验设置，不是完整机械行程标定。恢复脚本不自动回中，只有增加 `-EnableAuto` 才自动使能。JTAG下载不写Flash，重新上电需要重新下载和选择目标。

只检查文件、不操作板卡：

```powershell
.\烧录.ps1 -CheckOnly
```

## 重新编译

只编译裸机应用，使用随包BSP：

```powershell
.\scripts\build_release.ps1 -Output build/app_new
```

完整生成硬件及新BSP，输出目录必须不存在：

```powershell
.\scripts\build_touch_tracking.ps1 -Project build/hardware_new -Workspace build/software_new
```

Vivado界面可直接打开 `hardware/ov5640_lcd.xpr`。根目录XPR是集成脚本使用的视频基础工程。

新应用不会自动覆盖发布文件。完整构建产物可用通用入口下载：

```powershell
.\scripts\download_pan_tilt.ps1 -Project build/hardware_new -Workspace build/software_new -Application app_compiled
```

修改源码后仅运行 `烧录.ps1` 仍下载已归档的发布文件；要测试改动，应先编译并明确选择对应ELF。

## 测试与回退

```powershell
.\scripts\test_appearance.ps1
.\scripts\test_pid_control.ps1
.\scripts\test_touch.ps1
.\scripts\test_raw_capture.ps1
.\scripts\sim_component_tracker.ps1
```

100档及视觉PD版本：

```powershell
.\烧录.ps1 -Profile speed100
.\scripts\restore.ps1 -Profile speed100
.\烧录.ps1 -Profile lost
```

`lost`的运行参数见 `docs/RELEASE.md`，不要对该固件使用触摸参数恢复命令。

源工程基线为 `ov5640-lcd-analysis` 的 `touch-speed150-validated`，提交 `0d6e2f6`。本仓库为独立整理版，注释精简并补充必要中文说明；第三方声明集中在 [THIRD_PARTY_NOTICES.md](THIRD_PARTY_NOTICES.md)。发布清单记录SHA256，生成目录、个人照片及Word报告不随源码上传。
