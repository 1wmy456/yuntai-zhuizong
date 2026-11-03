param([ValidateSet('speed150','speed100')][string]$Profile='speed150',[switch]$WideExperiment,[switch]$EnableAuto,
 [string]$Xsct='C:/Xilinxx/Vitis/2020.2/bin/xsct.bat')
$ErrorActionPreference='Stop'
$root=Split-Path $PSScriptRoot -Parent
& python "$PSScriptRoot/verify_release.py" --profile $Profile
if($LASTEXITCODE){throw 'Release verification failed'}
$bsp=Join-Path $root build/release_bsp
if(-not(Test-Path "$bsp/include/xparameters.h")){
 New-Item -ItemType Directory -Force $bsp | Out-Null
 Expand-Archive -LiteralPath "$root/platform/standalone_bsp.zip" -DestinationPath $bsp
}
$params=@{Workspace='build/runtime_platform';Xsct=$Xsct}
$hw=Join-Path $root build/runtime_platform/board_platform/ps7_cortexa9_0/standalone_domain/bsp/ps7_cortexa9_0/include
New-Item -ItemType Directory -Force $hw | Out-Null
Copy-Item -LiteralPath "$bsp/include/xparameters.h" -Destination "$hw/xparameters.h" -Force
if($Profile -eq 'speed150'){$params.Faster=$true}else{$params.Responsive=$true}
if($WideExperiment){$params.WideExperiment=$true}
if($EnableAuto){$params.EnableAuto=$true}
& "$PSScriptRoot/restore_touch_tracking.ps1" @params
if($LASTEXITCODE){throw 'Runtime settings failed'}
