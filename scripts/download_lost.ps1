param([string]$Xsct='C:/Xilinxx/Vitis/2020.2/bin/xsct.bat',[switch]$CheckOnly)
$ErrorActionPreference='Stop'
& "$PSScriptRoot/download.ps1" -Profile lost -Xsct $Xsct -CheckOnly:$CheckOnly
if($LASTEXITCODE){throw 'Lost download failed'}
