param([string]$Workspace='build/runtime_platform',
    [switch]$WideExperiment,[switch]$EnableAuto,
    [string]$Xsct='C:/Xilinxx/Vitis/2020.2/bin/xsct.bat')
$ErrorActionPreference='Stop'
$root=Split-Path $PSScriptRoot -Parent
& "$PSScriptRoot/restore_touch_tracking.ps1" -Workspace $Workspace -Faster -WideExperiment:$WideExperiment -Xsct $Xsct
if($LASTEXITCODE){throw 'Base profile restore failed'}
$header=Join-Path $root "$Workspace/board_platform/ps7_cortexa9_0/standalone_domain/bsp/ps7_cortexa9_0/include/xparameters.h"
$match=[regex]::Match((Get-Content -Raw -LiteralPath $header),'(?m)^#define XPAR_PAN_TILT_CTRL_0_S_AXI_BASEADDR (0x[0-9a-fA-F]+)')
if(!$match.Success){throw 'Peripheral address macro missing'}
$commands=@('rate yaw 300','rate pitch 300')
if($EnableAuto){$commands+=@('enable both','verify yaw','verify pitch','auto')}
$commands+=@('motion','status')
$file=Join-Path $root 'build/restore_speed300_commands.txt'
[IO.File]::WriteAllLines($file,$commands,[Text.Encoding]::ASCII)
& $Xsct "$PSScriptRoot/pan_tilt_jtag_batch.tcl" $match.Groups[1].Value $file
if($LASTEXITCODE){throw '300 profile rejected; confirm the matching 300 ELF is loaded'}
