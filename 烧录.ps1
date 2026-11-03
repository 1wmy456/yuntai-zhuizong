param([ValidateSet('speed150','speed100','lost')][string]$Profile='speed150',[switch]$CheckOnly,
 [string]$Xsct='C:/Xilinxx/Vitis/2020.2/bin/xsct.bat')
$ErrorActionPreference='Stop'
& "$PSScriptRoot/scripts/download.ps1" -Profile $Profile -Xsct $Xsct -CheckOnly:$CheckOnly
if($LASTEXITCODE){throw 'Download failed'}
