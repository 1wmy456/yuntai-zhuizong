# 云台追踪

基于 Zynq-7020、OV5640、800×480 触摸 LCD 和双 MG90S 的视觉追踪系统。点击屏幕选择目标，完成外观建模、实时画框和双轴跟随。

本仓库默认发布 **300档优化版**，仅提供一套固件。旧版不在发布目录中保留，可通过 Git 标签 `v1.0-touch150` 获取。

## 功能与参数

- GT1158 触摸定位，点击切换目标，背景点击保护。
- 连通区域、形状与外观匹配；失目标清框并保持位置，不自动追其他目标。
- 两轴速度上限300μs/s，加速度800μs/s²，减速度1200μs/s²，死区15/22像素。
- 控制框架支持P/PD/PID；发布配置Kp=2，Ki=Kd=0。
- 800×480完整RGB帧经DMA到PS，每个4×4块取中央四点均值生成200×120图像。
- `appearance` 命令输出最近128次请求到识别的P50/P95耗时。

蓝、红、黑瓶盖已进行触摸选择测试；300档优化版已确认绿框稳定、跟随与停稳正常。复杂背景下的白色、低对比和相似目标仍有限制。本版没有PL图像裁剪、YOLO或神经网络推理。

## 下载与运行

Vivado/Vitis 2020.2，工具默认安装在 `C:/Xilinxx/`。开发板上电并连接JTAG：

```powershell
cd D:\2026.test\kaoshi
.\烧录.ps1
.\scripts\restore.ps1 -EnableAuto
```

下载后初始为手动、PWM关闭；第二条命令恢复300档配置并启用自动。随后点击瓶盖开始跟随。恢复脚本默认两轴500～2500μs宽范围实验限幅，此范围不代表完整机械行程已标定。使用已标定窄范围可加 `-WideExperiment:$false`。

只恢复参数、暂不使能舵机：

```powershell
.\scripts\restore.ps1
```

115200/8N1串口使用ASCII，命令末尾加换行：

```text
enable both
verify yaw
verify pitch
auto
appearance
motion
status
```

`manual`保持位置，`disable`关闭PWM。目标丢失后保持，重新点击可以选择目标。JTAG下载不写Flash，断电后需重新下载。

## 构建与测试

```powershell
.\烧录.ps1 -CheckOnly
.\scripts\test_speed300_cpu_opt.ps1
.\scripts\build_release.ps1 -Output build/app_new
```

完整重建硬件和BSP：

```powershell
.\scripts\build_touch_tracking.ps1 -Project build/hardware_new -Workspace build/software_new
```

Vivado可打开 `hardware/ov5640_lcd.xpr`。根目录XPR供集成脚本使用。实际C构建输入为 `vitis/ov5640_lcd/src`；`release/speed300` 是已归档固件。修改源码后要重新编译并选择新ELF，直接执行烧录入口仍下载归档版本。

## 项目文件

|目录|内容|
|---|---|
|`rtl/`、`hardware/`、`ip_repo/`|RTL、集成工程与IP|
|`constraints/`|触摸和PWM约束|
|`vitis/ov5640_lcd/src/`|裸机控制与跟踪源码|
|`sim/`、`scripts/`|测试、构建、下载与参数恢复|
|`release/speed300/`、`platform/`|配套BIT、XSA、ELF、PS初始化与BSP|
|`docs/evidence/speed300/`|实板日志、回归及程序段一致性证据|

实板统计：请求到识别P50约49.1ms，P95约49.2ms；不是曝光到舵机运动的端到端延迟，也没有同场景基线来计算加速比。完整帧仍传输1.536MB，四点缩图并未减少DMA数据量。

接线见 [WIRING.md](docs/WIRING.md)，固件证据见 [RELEASE.md](docs/RELEASE.md)。第三方声明见 [THIRD_PARTY_NOTICES.md](THIRD_PARTY_NOTICES.md)。个人照片、原始采集图像和Word报告不随源码上传。
