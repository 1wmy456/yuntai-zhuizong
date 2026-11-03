#ifndef CLK_WIZ_H_
#define CLK_WIZ_H_

#include "xil_types.h"

#define CLK_SR_OFFSET    0x04
#define CLK_CFG0_OFFSET  0x200
#define CLK_CFG2_OFFSET  0x208
#define CLK_CFG23_OFFSET 0x25C

void clk_wiz_cfg(u32 clk_base_addr,double freq);

#endif
