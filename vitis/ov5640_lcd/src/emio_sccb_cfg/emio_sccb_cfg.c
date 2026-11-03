#include"emio_sccb_cfg.h"

#define  GPIOPS_ID  XPAR_XGPIOPS_0_DEVICE_ID

static  XGpioPs  gpiops_inst;

void emio_init(void)
{

	XGpioPs_Config *gpiops_cfg_ptr;

	gpiops_cfg_ptr = XGpioPs_LookupConfig(GPIOPS_ID);

	XGpioPs_CfgInitialize(&gpiops_inst, gpiops_cfg_ptr, gpiops_cfg_ptr->BaseAddr);

	XGpioPs_SetDirectionPin(&gpiops_inst, EMIO_SCL_NUM, 1);
	XGpioPs_SetDirectionPin(&gpiops_inst, EMIO_SDA_NUM, 1);

	XGpioPs_SetOutputEnablePin(&gpiops_inst, EMIO_SCL_NUM, 1);
	XGpioPs_SetOutputEnablePin(&gpiops_inst, EMIO_SDA_NUM, 1);

	XGpioPs_WritePin(&gpiops_inst, EMIO_SCL_NUM, 1);
	XGpioPs_WritePin(&gpiops_inst, EMIO_SDA_NUM, 1);
}

void sccb_start(void)
{
	XGpioPs_WritePin(&gpiops_inst, EMIO_SCL_NUM, 1);
	XGpioPs_WritePin(&gpiops_inst, EMIO_SDA_NUM, 1);

	usleep(4);

 	XGpioPs_WritePin(&gpiops_inst, EMIO_SDA_NUM, 0);

 	usleep(4);
}

void sccb_stop(void)
{
	XGpioPs_WritePin(&gpiops_inst,EMIO_SDA_NUM, 0);

 	usleep(4);

	XGpioPs_WritePin(&gpiops_inst, EMIO_SCL_NUM, 1);

	usleep(4);

	XGpioPs_WritePin(&gpiops_inst, EMIO_SDA_NUM, 1);

	usleep(4);
}

void sccb_send_byte(u8 txd)
{
    u8 t;

    XGpioPs_WritePin(&gpiops_inst,EMIO_SCL_NUM,0);

    for(t=0; t<8; t++)
    {
    	usleep(2);

        XGpioPs_WritePin(&gpiops_inst, EMIO_SDA_NUM, (txd&0x80)>>7);

        txd<<=1;

        usleep(2);

        XGpioPs_WritePin(&gpiops_inst, EMIO_SCL_NUM, 1);

        usleep(4);

        XGpioPs_WritePin(&gpiops_inst, EMIO_SCL_NUM, 0);
    }
}

u8  sccb_rece_byte(void)
{
	unsigned char i=0 , rxd=0;

	XGpioPs_SetDirectionPin(&gpiops_inst, EMIO_SDA_NUM, 0);
	XGpioPs_SetOutputEnablePin(&gpiops_inst, EMIO_SDA_NUM, 0);

	for(i=0;i<8;i++ )
	{
		usleep(4);
        XGpioPs_WritePin(&gpiops_inst, EMIO_SCL_NUM, 1);
        usleep(2);

        rxd <<= 1;
        if( XGpioPs_ReadPin(&gpiops_inst, EMIO_SDA_NUM) ) {
        	rxd = rxd | 0x01;
        }
		usleep(2);

		XGpioPs_WritePin(&gpiops_inst, EMIO_SCL_NUM, 0);
    }
    return rxd;
}

void sccb_ack(void)
{
	usleep(2);

	XGpioPs_SetDirectionPin(&gpiops_inst, EMIO_SDA_NUM, 0);
	XGpioPs_SetOutputEnablePin(&gpiops_inst, EMIO_SDA_NUM, 0);
    usleep(2);

	XGpioPs_WritePin(&gpiops_inst, EMIO_SCL_NUM, 1);

	usleep(4);

	XGpioPs_WritePin(&gpiops_inst, EMIO_SCL_NUM, 0);

	XGpioPs_SetDirectionPin(&gpiops_inst, EMIO_SDA_NUM, 1);
	XGpioPs_SetOutputEnablePin(&gpiops_inst, EMIO_SDA_NUM, 1);
}

void sccb_host_no_ack(void)
{
	usleep(2);

	XGpioPs_SetDirectionPin(&gpiops_inst, EMIO_SDA_NUM, 1);
	XGpioPs_SetOutputEnablePin(&gpiops_inst, EMIO_SDA_NUM, 1);

	XGpioPs_WritePin(&gpiops_inst, EMIO_SDA_NUM, 1);
	usleep(2);

	XGpioPs_WritePin(&gpiops_inst, EMIO_SCL_NUM, 1);

	usleep(4);

	XGpioPs_WritePin(&gpiops_inst, EMIO_SCL_NUM, 0);
}

