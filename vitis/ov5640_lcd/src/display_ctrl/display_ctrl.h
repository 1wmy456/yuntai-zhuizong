#ifndef DISPLAY_CTRL_H_
#define DISPLAY_CTRL_H_

#include "xil_types.h"
#include "xvtc.h"
#include "lcd_modes.h"
#include "xgpio.h"

#define BIT_DISPLAY_RED 16
#define BIT_DISPLAY_GREEN 8
#define BIT_DISPLAY_BLUE 0

#define DISPLAY_NUM_FRAMES 1

typedef enum {
	DISPLAY_STOPPED = 0,
	DISPLAY_RUNNING = 1
} DisplayState;

typedef struct {
		XVtc vtc;
		VideoMode vMode;
		double pxlFreq;
		DisplayState state;
} DisplayCtrl;

int DisplayStop(DisplayCtrl *dispPtr);
int DisplayStart(DisplayCtrl *dispPtr);
int DisplayInitialize(DisplayCtrl *dispPtr,u16 vtcId);
int DisplaySetMode(DisplayCtrl *dispPtr, const VideoMode *newMode);
int DisplayChangeFrame(DisplayCtrl *dispPtr, u32 frameIndex);

u16 lcd_id_read(XGpio * axi_gpio_inst, unsigned chanel);

#endif

