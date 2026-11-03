param([string]$Application='app_speed150',[string]$Project='',[string]$Workspace='',
 [string]$Xsct='C:/Xilinxx/Vitis/2020.2/bin/xsct.bat',[switch]$CheckOnly)
$ErrorActionPreference='Stop'
if($Project -or $Workspace){
 if(-not $Project -or -not $Workspace){throw 'Provide both Project and Workspace'}
 if($CheckOnly){throw 'Use the generic download script for custom build verification'}
 & "$PSScriptRoot/download_pan_tilt.ps1" -Project $Project -Workspace $Workspace -Application $Application -Xsct $Xsct
}else{
 $profiles=@{app_speed150='speed150';app_neutral_shape='speed100'}
 if(-not $profiles.ContainsKey($Application)){throw 'Unknown release application; provide Project and Workspace for a custom build'}
 & "$PSScriptRoot/download.ps1" -Profile $profiles[$Application] -Xsct $Xsct -CheckOnly:$CheckOnly
}
if($LASTEXITCODE){throw 'Touch download failed'}
