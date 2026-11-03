param([ValidateSet('speed150','speed100','lost')][string]$Profile='speed150',[switch]$CheckOnly,
 [string]$Xsct='C:/Xilinxx/Vitis/2020.2/bin/xsct.bat')
$ErrorActionPreference='Stop'
$root=Split-Path $PSScriptRoot -Parent
& python "$PSScriptRoot/verify_release.py" --profile $Profile
if($LASTEXITCODE){throw 'Release verification failed'}
if($CheckOnly){Write-Output 'DOWNLOAD_CHECK PASS; no hardware operation';return}
$release=Join-Path $root "release/$Profile"
& $Xsct "$PSScriptRoot/program_blue_tracker_board.tcl" "$release/system_wrapper.bit" "$release/ps7_init.tcl" "$release/ov5640_lcd.elf"
if($LASTEXITCODE){throw 'JTAG download failed'}
Write-Output 'DOWNLOAD PASS; manual mode, PWM OFF. Runtime settings must be restored separately.'
