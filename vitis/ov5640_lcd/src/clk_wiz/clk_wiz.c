#include "xclk_wiz.h"
#include "clk_wiz.h"
#include "xparameters.h"

#define CLK_WIZ_IN_FREQ 100

XClk_Wiz clk_wiz_inst;

void clk_wiz_cfg(u32 clk_device_id,double freq){
	double div_factor = 0;
	u32 div_factor_int = 0,dviv_factor_frac=0;
	u32 clk_divide = 0;
	u32 status = 0;

	XClk_Wiz_Config *clk_cfg_ptr;
	clk_cfg_ptr = XClk_Wiz_LookupConfig(clk_device_id);
	XClk_Wiz_CfgInitialize(&clk_wiz_inst,clk_cfg_ptr,clk_cfg_ptr->BaseAddr);

	if(freq <= 0)
		return;

	XClk_Wiz_WriteReg(clk_cfg_ptr->BaseAddr,CLK_CFG0_OFFSET,0x00000a01);

	div_factor = CLK_WIZ_IN_FREQ * 10 / freq;
	div_factor_int = (u32)div_factor;
	dviv_factor_frac = (u32)((div_factor - div_factor_int) * 1000);
	clk_divide = div_factor_int | (dviv_factor_frac<<8);

	XClk_Wiz_WriteReg(clk_cfg_ptr->BaseAddr,CLK_CFG2_OFFSET,clk_divide);

	XClk_Wiz_WriteReg(clk_cfg_ptr->BaseAddr,CLK_CFG23_OFFSET,0x00000003);

	while(1){
		status = XClk_Wiz_ReadReg(clk_cfg_ptr->BaseAddr,CLK_SR_OFFSET);
		if(status&0x00000001)
			return ;
	}
}
