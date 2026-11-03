# Definitional proc to organize widgets for parameters.
proc init_gui { IPINST } {
  ipgui::add_param $IPINST -name "Component_Name"
  #Adding Page
  set Page_0 [ipgui::add_page $IPINST -name "Page 0"]
  ipgui::add_param $IPINST -name "CLOCK_HZ" -parent ${Page_0}
  ipgui::add_param $IPINST -name "COMPONENT_MODE" -parent ${Page_0}
  ipgui::add_param $IPINST -name "C_S_AXI_ADDR_WIDTH" -parent ${Page_0}
  ipgui::add_param $IPINST -name "C_S_AXI_BASEADDR" -parent ${Page_0}
  ipgui::add_param $IPINST -name "C_S_AXI_HIGHADDR" -parent ${Page_0}
  ipgui::add_param $IPINST -name "IMAGE_HEIGHT" -parent ${Page_0}
  ipgui::add_param $IPINST -name "IMAGE_WIDTH" -parent ${Page_0}
  ipgui::add_param $IPINST -name "MASK_BLOCK" -parent ${Page_0}
  ipgui::add_param $IPINST -name "TOUCH_MODE" -parent ${Page_0}


}

proc update_PARAM_VALUE.CLOCK_HZ { PARAM_VALUE.CLOCK_HZ } {
	# Procedure called to update CLOCK_HZ when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.CLOCK_HZ { PARAM_VALUE.CLOCK_HZ } {
	# Procedure called to validate CLOCK_HZ
	return true
}

proc update_PARAM_VALUE.COMPONENT_MODE { PARAM_VALUE.COMPONENT_MODE } {
	# Procedure called to update COMPONENT_MODE when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.COMPONENT_MODE { PARAM_VALUE.COMPONENT_MODE } {
	# Procedure called to validate COMPONENT_MODE
	return true
}

proc update_PARAM_VALUE.C_S_AXI_ADDR_WIDTH { PARAM_VALUE.C_S_AXI_ADDR_WIDTH } {
	# Procedure called to update C_S_AXI_ADDR_WIDTH when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_S_AXI_ADDR_WIDTH { PARAM_VALUE.C_S_AXI_ADDR_WIDTH } {
	# Procedure called to validate C_S_AXI_ADDR_WIDTH
	return true
}

proc update_PARAM_VALUE.C_S_AXI_BASEADDR { PARAM_VALUE.C_S_AXI_BASEADDR } {
	# Procedure called to update C_S_AXI_BASEADDR when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_S_AXI_BASEADDR { PARAM_VALUE.C_S_AXI_BASEADDR } {
	# Procedure called to validate C_S_AXI_BASEADDR
	return true
}

proc update_PARAM_VALUE.C_S_AXI_HIGHADDR { PARAM_VALUE.C_S_AXI_HIGHADDR } {
	# Procedure called to update C_S_AXI_HIGHADDR when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_S_AXI_HIGHADDR { PARAM_VALUE.C_S_AXI_HIGHADDR } {
	# Procedure called to validate C_S_AXI_HIGHADDR
	return true
}

proc update_PARAM_VALUE.IMAGE_HEIGHT { PARAM_VALUE.IMAGE_HEIGHT } {
	# Procedure called to update IMAGE_HEIGHT when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.IMAGE_HEIGHT { PARAM_VALUE.IMAGE_HEIGHT } {
	# Procedure called to validate IMAGE_HEIGHT
	return true
}

proc update_PARAM_VALUE.IMAGE_WIDTH { PARAM_VALUE.IMAGE_WIDTH } {
	# Procedure called to update IMAGE_WIDTH when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.IMAGE_WIDTH { PARAM_VALUE.IMAGE_WIDTH } {
	# Procedure called to validate IMAGE_WIDTH
	return true
}

proc update_PARAM_VALUE.MASK_BLOCK { PARAM_VALUE.MASK_BLOCK } {
	# Procedure called to update MASK_BLOCK when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.MASK_BLOCK { PARAM_VALUE.MASK_BLOCK } {
	# Procedure called to validate MASK_BLOCK
	return true
}

proc update_PARAM_VALUE.TOUCH_MODE { PARAM_VALUE.TOUCH_MODE } {
	# Procedure called to update TOUCH_MODE when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.TOUCH_MODE { PARAM_VALUE.TOUCH_MODE } {
	# Procedure called to validate TOUCH_MODE
	return true
}


proc update_MODELPARAM_VALUE.CLOCK_HZ { MODELPARAM_VALUE.CLOCK_HZ PARAM_VALUE.CLOCK_HZ } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.CLOCK_HZ}] ${MODELPARAM_VALUE.CLOCK_HZ}
}

proc update_MODELPARAM_VALUE.IMAGE_WIDTH { MODELPARAM_VALUE.IMAGE_WIDTH PARAM_VALUE.IMAGE_WIDTH } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.IMAGE_WIDTH}] ${MODELPARAM_VALUE.IMAGE_WIDTH}
}

proc update_MODELPARAM_VALUE.IMAGE_HEIGHT { MODELPARAM_VALUE.IMAGE_HEIGHT PARAM_VALUE.IMAGE_HEIGHT } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.IMAGE_HEIGHT}] ${MODELPARAM_VALUE.IMAGE_HEIGHT}
}

proc update_MODELPARAM_VALUE.C_S_AXI_ADDR_WIDTH { MODELPARAM_VALUE.C_S_AXI_ADDR_WIDTH PARAM_VALUE.C_S_AXI_ADDR_WIDTH } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.C_S_AXI_ADDR_WIDTH}] ${MODELPARAM_VALUE.C_S_AXI_ADDR_WIDTH}
}

proc update_MODELPARAM_VALUE.COMPONENT_MODE { MODELPARAM_VALUE.COMPONENT_MODE PARAM_VALUE.COMPONENT_MODE } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.COMPONENT_MODE}] ${MODELPARAM_VALUE.COMPONENT_MODE}
}

proc update_MODELPARAM_VALUE.MASK_BLOCK { MODELPARAM_VALUE.MASK_BLOCK PARAM_VALUE.MASK_BLOCK } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.MASK_BLOCK}] ${MODELPARAM_VALUE.MASK_BLOCK}
}

proc update_MODELPARAM_VALUE.TOUCH_MODE { MODELPARAM_VALUE.TOUCH_MODE PARAM_VALUE.TOUCH_MODE } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.TOUCH_MODE}] ${MODELPARAM_VALUE.TOUCH_MODE}
}

proc update_MODELPARAM_VALUE.C_S_AXI_BASEADDR { MODELPARAM_VALUE.C_S_AXI_BASEADDR PARAM_VALUE.C_S_AXI_BASEADDR } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.C_S_AXI_BASEADDR}] ${MODELPARAM_VALUE.C_S_AXI_BASEADDR}
}

proc update_MODELPARAM_VALUE.C_S_AXI_HIGHADDR { MODELPARAM_VALUE.C_S_AXI_HIGHADDR PARAM_VALUE.C_S_AXI_HIGHADDR } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.C_S_AXI_HIGHADDR}] ${MODELPARAM_VALUE.C_S_AXI_HIGHADDR}
}

