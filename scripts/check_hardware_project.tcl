set root [file normalize [file join [file dirname [info script]] ..]]
open_project [file join $root hardware ov5640_lcd.xpr]
open_bd_design [get_files */system.bd]
validate_bd_design
foreach cell {axis_blue_tracker_0 axis_raw_capture_0 axis_touch_cross_0 pan_tilt_ctrl_0 touch_gpio raw_capture_dma} {
 if {[llength [get_bd_cells -quiet $cell]]!=1} {error "Missing cell: $cell"}
}
foreach name {servo_yaw_pwm servo_pitch_pwm touch_io} {
 if {![llength [get_bd_ports -quiet $name]]&&![llength [get_bd_intf_ports -quiet $name]]} {error "Missing port: $name"}
}
puts "PORTABLE_HARDWARE: PASS"
close_project
