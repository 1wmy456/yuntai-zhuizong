param([string]$Gcc='D:/msys64/ucrt64/bin/gcc.exe')
$ErrorActionPreference='Stop'
$root=Split-Path $PSScriptRoot -Parent
$env:PATH=(Split-Path $Gcc -Parent)+';'+$env:PATH
Push-Location $root
try {
    New-Item -ItemType Directory -Force build/cpu_opt_tests | Out-Null
    $src='vitis/ov5640_lcd/src'
    $cases=@(
        @{Name='runtime';Files=@('sim/touch/test_appearance_runtime.c',"$src/touch/appearance_tracker.c","$src/touch/box_shape_filter.c","$src/pan_tilt/cap_components.c")},
        @{Name='appearance';Files=@('sim/touch/test_appearance.c',"$src/touch/appearance_tracker.c","$src/pan_tilt/cap_components.c")},
        @{Name='trajectory';Files=@('sim/touch/test_position_trajectory.c',"$src/pan_tilt/position_trajectory.c","$src/pan_tilt/tracking_control.c")},
        @{Name='box';Files=@('sim/touch/test_box_shape.c',"$src/touch/box_shape_filter.c")}
    )
    foreach($case in $cases){
        $exe="build/cpu_opt_tests/$($case.Name).exe"
        & $Gcc -std=c11 -O2 -Wall -Wextra -Werror -I sim/touch/host_include -I sim/pan_tilt/host_include @($case.Files) -lm -o $exe
        if($LASTEXITCODE){throw "Compile failed: $($case.Name)"}
        $result=& "./$exe"
        $result | Set-Content "build/cpu_opt_tests/$($case.Name).txt" -Encoding UTF8
        $result | Out-Host
        if($LASTEXITCODE -or !($result -match '^PASS ')){throw "Test failed: $($case.Name)"}
    }
} finally {Pop-Location}
