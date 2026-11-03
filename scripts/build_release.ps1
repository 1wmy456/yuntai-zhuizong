param([string]$Output='build/app_release',
 [string]$Tools='C:/Xilinxx/Vitis/2020.2/gnu/aarch32/nt/gcc-arm-none-eabi/bin')
$ErrorActionPreference='Stop'
$root=Split-Path $PSScriptRoot -Parent
Push-Location $root
try{
 & python "$PSScriptRoot/verify_release.py"
 if($LASTEXITCODE){throw 'Bundled release verification failed'}
 $bsp=Join-Path $root build/release_bsp
 if(-not(Test-Path "$bsp/include/xparameters.h")){
  New-Item -ItemType Directory -Force $bsp | Out-Null
  Expand-Archive -LiteralPath "$root/platform/standalone_bsp.zip" -DestinationPath $bsp
 }
 & python "$PSScriptRoot/compile_blue_tracker_app.py" --bsp $bsp --out $Output --tools $Tools
 if($LASTEXITCODE){throw 'Application compilation failed'}
 Write-Output "BUILD_RELEASE PASS: $Output/ov5640_lcd.elf"
}finally{Pop-Location}
