#include "display_ctrl.h"
#include "xdebug.h"
#include "xil_io.h"

int DisplayStop(DisplayCtrl *dispPtr)
{

	if (dispPtr->state == DISPLAY_STOPPED)
	{
		return XST_SUCCESS;
	}

	XVtc_DisableGenerator(&dispPtr->vtc);

	dispPtr->state = DISPLAY_STOPPED;

	return XST_SUCCESS;
}

int DisplayStart(DisplayCtrl *dispPtr)
{
	XVtc_Timing vtcTiming;
	XVtc_SourceSelect SourceSelect;

	xdbg_printf(XDBG_DEBUG_GENERAL, "display start entered\n\r");

	if (dispPtr->state == DISPLAY_RUNNING)
	{
		return XST_SUCCESS;
	}

	vtcTiming.HActiveVideo = dispPtr->vMode.width;
	vtcTiming.HFrontPorch = dispPtr->vMode.hps - dispPtr->vMode.width;
	vtcTiming.HSyncWidth = dispPtr->vMode.hpe - dispPtr->vMode.hps;
	vtcTiming.HBackPorch = dispPtr->vMode.hmax - dispPtr->vMode.hpe + 1;
	vtcTiming.HSyncPolarity = dispPtr->vMode.hpol;
	vtcTiming.VActiveVideo = dispPtr->vMode.height;
	vtcTiming.V0FrontPorch = dispPtr->vMode.vps - dispPtr->vMode.height;
	vtcTiming.V0SyncWidth = dispPtr->vMode.vpe - dispPtr->vMode.vps;
	vtcTiming.V0BackPorch = dispPtr->vMode.vmax - dispPtr->vMode.vpe + 1;;
	vtcTiming.V1FrontPorch = dispPtr->vMode.vps - dispPtr->vMode.height;
	vtcTiming.V1SyncWidth = dispPtr->vMode.vpe - dispPtr->vMode.vps;
	vtcTiming.V1BackPorch = dispPtr->vMode.vmax - dispPtr->vMode.vpe + 1;;
	vtcTiming.VSyncPolarity = dispPtr->vMode.vpol;
	vtcTiming.Interlaced = 0;

	memset((void *)&SourceSelect, 0, sizeof(SourceSelect));
	SourceSelect.VBlankPolSrc = 1;
	SourceSelect.VSyncPolSrc = 1;
	SourceSelect.HBlankPolSrc = 1;
	SourceSelect.HSyncPolSrc = 1;
	SourceSelect.ActiveVideoPolSrc = 1;
	SourceSelect.ActiveChromaPolSrc= 1;
	SourceSelect.VChromaSrc = 1;
	SourceSelect.VActiveSrc = 1;
	SourceSelect.VBackPorchSrc = 1;
	SourceSelect.VSyncSrc = 1;
	SourceSelect.VFrontPorchSrc = 1;
	SourceSelect.VTotalSrc = 1;
	SourceSelect.HActiveSrc = 1;
	SourceSelect.HBackPorchSrc = 1;
	SourceSelect.HSyncSrc = 1;
	SourceSelect.HFrontPorchSrc = 1;
	SourceSelect.HTotalSrc = 1;

	XVtc_SelfTest(&(dispPtr->vtc));

	XVtc_RegUpdateEnable(&(dispPtr->vtc));
	XVtc_SetGeneratorTiming(&(dispPtr->vtc), &vtcTiming);
	XVtc_SetSource(&(dispPtr->vtc), &SourceSelect);

	XVtc_EnableGenerator(&dispPtr->vtc);

	dispPtr->state = DISPLAY_RUNNING;

	return XST_SUCCESS;
}

  int DisplayInitialize(DisplayCtrl *dispPtr, u16 vtcId)
{
	int Status;
	XVtc_Config *vtcConfig;

	dispPtr->state = DISPLAY_STOPPED;
	dispPtr->vMode = VMODE_800x480;

	vtcConfig = XVtc_LookupConfig(vtcId);

	if (NULL == vtcConfig) {
		return (XST_FAILURE);
	}
	Status = XVtc_CfgInitialize(&(dispPtr->vtc), vtcConfig, vtcConfig->BaseAddress);

	if (Status != (XST_SUCCESS)) {
		return (XST_FAILURE);
	}

	return XST_SUCCESS;
}

int DisplaySetMode(DisplayCtrl *dispPtr, const VideoMode *newMode)
{
	int Status;

	if (dispPtr->state == DISPLAY_RUNNING)
	{
		Status = DisplayStop(dispPtr);
		if (Status != XST_SUCCESS)
		{
			xdbg_printf(XDBG_DEBUG_GENERAL, "Cannot change mode, unable to stop display %d\r\n", Status);
			return XST_FAILURE;
		}
	}

	dispPtr->vMode = *newMode;

	return XST_SUCCESS;
}

u16 lcd_id_read(XGpio * axi_gpio_inst,unsigned chanel)
{
	u32 idx=0;
	idx = XGpio_DiscreteRead(axi_gpio_inst,chanel);
	switch(idx)
	{
		case 0:return 0x4342;
		case 1:return 0x7084;
		case 2:return 0x7016;
		case 4:return 0x4384;
		case 5:return 0x1018;
		default:return 0;
	}
}
