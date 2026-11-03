#ifndef VDMA_API_H_
#define VDMA_API_H_

#include "xaxivdma.h"
#include "xparameters.h"
#include "xil_exception.h"

typedef enum
{
	ONLY_READ=1,
	ONLY_WRITE,
	BOTH
}vdma_run_mode;

int run_vdma_frame_buffer(XAxiVdma* InstancePtr, int DeviceId, int hsize,
		int vsize, int buf_base_addr, int number_frame_count,
		int enable_frm_cnt_intr,vdma_run_mode mode);

#endif

