#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "xil_types.h"
#include "xil_cache.h"
#include "xparameters.h"
#include "xgpio.h"
#include "xaxivdma.h"
#include "xaxivdma_i.h"
#include "display_ctrl/display_ctrl.h"
#include "vdma_api/vdma_api.h"
#include "clk_wiz/clk_wiz.h"
#include "emio_sccb_cfg/emio_sccb_cfg.h"
#include "ov5640/ov5640_init.h"
#include "pan_tilt/pan_tilt_app.h"

#define BYTES_PIXEL        3
#define FRAME_BUFFER_NUM   3
#define CLK_WIZ_ID         XPAR_CLK_WIZ_0_DEVICE_ID

#define VDMA_ID            XPAR_AXIVDMA_0_DEVICE_ID
#define DISP_VTC_ID        XPAR_VTC_0_DEVICE_ID

#define AXI_GPIO_0_ID      XPAR_AXI_GPIO_0_DEVICE_ID

#define AXI_GPIO_0_CHANEL  1

unsigned int const frame_buffer_addr = (XPAR_PS7_DDR_0_S_AXI_BASEADDR
										+ 0x1000000);
XAxiVdma     vdma;
DisplayCtrl  dispCtrl;
XGpio        axi_gpio_inst;
VideoMode    vd_mode;
unsigned int lcd_id;

int main(void)
{
	u32 status;
	u16 cmos_h_pixel;
	u16 cmos_v_pixel;
	u16 total_h_pixel;
	u16 total_v_pixel;

	XGpio_Initialize(&axi_gpio_inst,AXI_GPIO_0_ID);
	XGpio_SetDataDirection(&axi_gpio_inst,AXI_GPIO_0_CHANEL,0x07);
	lcd_id = lcd_id_read(&axi_gpio_inst,AXI_GPIO_0_CHANEL);
	XGpio_SetDataDirection(&axi_gpio_inst,AXI_GPIO_0_CHANEL,0x00);
	xil_printf("LCD ID: %x\r\n",lcd_id);

	switch(lcd_id){
		case 0x4342 :
			cmos_h_pixel = 480;
			cmos_v_pixel = 272;
			total_h_pixel = 1800;
			total_v_pixel = 1000;
			break;
		case 0x4384 :
			cmos_h_pixel = 800;
			cmos_v_pixel = 480;
			total_h_pixel = 1800;
			total_v_pixel = 1000;
			break;
		case 0x7084 :
			cmos_h_pixel = 800;
			cmos_v_pixel = 480;
			total_h_pixel = 1800;
			total_v_pixel = 1000;
			break;
		case 0x7016 :
			cmos_h_pixel = 1024;
			cmos_v_pixel = 600;
			total_h_pixel = 2200;
			total_v_pixel = 1000;
			break;
		case 0x1018 :
			cmos_h_pixel = 1280;
			cmos_v_pixel = 800;
			total_h_pixel = 2570;
			total_v_pixel = 980;
			break;
		default :
			cmos_h_pixel = 800;
			cmos_v_pixel = 480;
			total_h_pixel = 1800;
			total_v_pixel = 1000;
			break;
	}

	emio_init();
	status = ov5640_init( cmos_h_pixel,
						  cmos_v_pixel,
						 total_h_pixel,
						 total_v_pixel);
	if(status == 0)
		xil_printf("OV5640 detected successful!\r\n");
	else
		xil_printf("OV5640 detected failed!\r\n");

	switch(lcd_id){
		case 0x4342 : vd_mode = VMODE_480x272;  break;
		case 0x4384 : vd_mode = VMODE_800x480;  break;
		case 0x7084 : vd_mode = VMODE_800x480;  break;
		case 0x7016 : vd_mode = VMODE_1024x600; break;
		case 0x1018 : vd_mode = VMODE_1280x800; break;
		default : vd_mode = VMODE_800x480; break;
	}
	xil_printf("Display width  : %d\r\n", vd_mode.width);
	xil_printf("Display height : %d\r\n", vd_mode.height);
	xil_printf("Pixel clock kHz: %d\r\n", (int)(vd_mode.freq*1000.0));

	run_vdma_frame_buffer(&vdma, VDMA_ID, vd_mode.width, vd_mode.height,
							frame_buffer_addr,0,0,BOTH);

	clk_wiz_cfg(CLK_WIZ_ID,vd_mode.freq);

	DisplayInitialize(&dispCtrl, DISP_VTC_ID);

	DisplaySetMode(&dispCtrl, &vd_mode);
	DisplayStart(&dispCtrl);

    pan_tilt_run(vd_mode.width, vd_mode.height, status == 0);
    return 0;
}

