#define DEBUG_MODE		0

#include "xaxivdma.h"
#include "xparameters.h"
#include "xil_exception.h"
#include "vdma_api.h"

typedef struct vdma_handle
{

	unsigned int device_id;

	unsigned int init_done;

	XAxiVdma* InstancePtr;

	XAxiVdma_DmaSetup ReadCfg;
	XAxiVdma_DmaSetup WriteCfg;

	unsigned int hsize;

	unsigned int vsize;

	unsigned int buffer_address;

	unsigned int enable_frm_cnt_intr;

	unsigned int number_of_frame_count;
}vdma_handle;

vdma_handle vdma_context[XPAR_XAXIVDMA_NUM_INSTANCES];
static unsigned int context_init=0;

static int ReadSetup(vdma_handle *vdma_context);
static int WriteSetup(vdma_handle *vdma_context);
static int StartTransfer(XAxiVdma *InstancePtr,vdma_run_mode mode);

int run_vdma_frame_buffer(XAxiVdma* InstancePtr, int DeviceId, int hsize,
		int vsize, int buf_base_addr, int number_frame_count,
		int enable_frm_cnt_intr,vdma_run_mode mode)
{
	int Status,i;
	XAxiVdma_Config *Config;
	XAxiVdma_FrameCounter FrameCfgPtr;

	if(context_init==0) {
		for(i=0; i < XPAR_XAXIVDMA_NUM_INSTANCES; i++) {
			vdma_context[i].InstancePtr = NULL;
			vdma_context[i].device_id = -1;
			vdma_context[i].hsize = 0;
			vdma_context[i].vsize = 0;
			vdma_context[i].init_done = 0;
			vdma_context[i].buffer_address = 0;
			vdma_context[i].enable_frm_cnt_intr = 0;
			vdma_context[i].number_of_frame_count = 0;

		}
		context_init = 1;
	}

	Config = XAxiVdma_LookupConfig(DeviceId);
	if (!Config) {
		xil_printf("No video DMA found for ID %d\r\n",DeviceId );
		return XST_FAILURE;
	}

	if(vdma_context[DeviceId].init_done ==0) {
		vdma_context[DeviceId].InstancePtr = InstancePtr;

		Status = XAxiVdma_CfgInitialize(vdma_context[DeviceId].InstancePtr,
						Config, Config->BaseAddress);
		if (Status != XST_SUCCESS) {
			xil_printf("Configuration Initialization failed %d\r\n",
					Status);
			return XST_FAILURE;
		}

		vdma_context[DeviceId].init_done = 1;
	}

	vdma_context[DeviceId].device_id = DeviceId;
	vdma_context[DeviceId].vsize = vsize;

	vdma_context[DeviceId].buffer_address = buf_base_addr;
	vdma_context[DeviceId].enable_frm_cnt_intr = enable_frm_cnt_intr;
	vdma_context[DeviceId].number_of_frame_count = number_frame_count;
	vdma_context[DeviceId].hsize = hsize * (Config->Mm2SStreamWidth>>3);

	if ((mode == BOTH) || (mode == ONLY_WRITE)) {
		Status = WriteSetup(&vdma_context[DeviceId]);
		if (Status != XST_SUCCESS) {
			xil_printf("Write channel setup failed %d\r\n", Status);
			if (Status == XST_VDMA_MISMATCH_ERROR)
				xil_printf("DMA Mismatch Error\r\n");
			return XST_FAILURE;
		}
	}

	if ((mode == BOTH) || (mode == ONLY_READ)) {
		Status = ReadSetup(&vdma_context[DeviceId]);
		if (Status != XST_SUCCESS) {
			xil_printf("Read channel setup failed %d\r\n", Status);
			if (Status == XST_VDMA_MISMATCH_ERROR)
				xil_printf("DMA Mismatch Error\r\n");
			return XST_FAILURE;
		}
	}

	if(vdma_context[DeviceId].enable_frm_cnt_intr) {
		FrameCfgPtr.ReadDelayTimerCount = 1;
		FrameCfgPtr.ReadFrameCount = number_frame_count;
		FrameCfgPtr.WriteDelayTimerCount = 1;
		FrameCfgPtr.WriteFrameCount = number_frame_count;

		XAxiVdma_SetFrameCounter(vdma_context[DeviceId].InstancePtr,&FrameCfgPtr);

		XAxiVdma_IntrEnable(vdma_context[DeviceId].InstancePtr,
				XAXIVDMA_IXR_ERROR_MASK |
				XAXIVDMA_IXR_FRMCNT_MASK,XAXIVDMA_WRITE);
		XAxiVdma_IntrEnable(vdma_context[DeviceId].InstancePtr,
				XAXIVDMA_IXR_ERROR_MASK |
				XAXIVDMA_IXR_FRMCNT_MASK,XAXIVDMA_READ);
	} else	{

		XAxiVdma_IntrEnable(vdma_context[DeviceId].InstancePtr,
					XAXIVDMA_IXR_ERROR_MASK,XAXIVDMA_WRITE);
		XAxiVdma_IntrEnable(vdma_context[DeviceId].InstancePtr,
					XAXIVDMA_IXR_ERROR_MASK ,XAXIVDMA_READ);
	}

	Status = StartTransfer(vdma_context[DeviceId].InstancePtr,mode);
	if (Status != XST_SUCCESS) {
		if(Status == XST_VDMA_MISMATCH_ERROR)
			xil_printf("DMA Mismatch Error\r\n");
		return XST_FAILURE;
	}
#if DEBUG_MODE
	xil_printf("Code is in Debug mode, Make sure that buffer addresses are at valid memory \r\n");
	xil_printf("In triple mode, there has to be six consecutive buffers for Debug mode \r\n");
	{
		u32 pixels,j,Addr = vdma_context[DeviceId].buffer_address;
		u8 *dst,*src;
		u32 total_pixel = vdma_context[DeviceId].WriteCfg.Stride * vdma_context[DeviceId].vsize;
		src = (unsigned char *)Addr;
		dst = (unsigned char *)Addr + (total_pixel * vdma_context->InstancePtr->MaxNumFrames);

		for(j=0;j<vdma_context->InstancePtr->MaxNumFrames;j++) {
			for(pixels=0;pixels<total_pixel;pixels++) {
				if(src[pixels] != dst[pixels]) {
					xil_printf("VDMA transfer failed: SRC=0x%x, DST=0x%x\r\n",
							src[pixels],dst[pixels]);
					exit(-1);
				}
			}
			src = src + total_pixel;
			dst = dst + total_pixel;
		}
	}
	xil_printf("VDMA transfer is happening and checked for 3 frames \r\n");
#endif

	return XST_SUCCESS;
}

static int ReadSetup(vdma_handle *vdma_context)
{
	int Index;
	u32 Addr;
	int Status;

	vdma_context->ReadCfg.VertSizeInput = vdma_context->vsize;
	vdma_context->ReadCfg.HoriSizeInput = vdma_context->hsize;

	vdma_context->ReadCfg.Stride = vdma_context->hsize;
	vdma_context->ReadCfg.FrameDelay = 0;

	vdma_context->ReadCfg.EnableCircularBuf = 1;
	vdma_context->ReadCfg.EnableSync = 1;

	vdma_context->ReadCfg.PointNum = 0;
	vdma_context->ReadCfg.EnableFrameCounter = 0;

	vdma_context->ReadCfg.FixedFrameStoreAddr = 0;

	Status = XAxiVdma_DmaConfig(vdma_context->InstancePtr, XAXIVDMA_READ, &vdma_context->ReadCfg);
	if (Status != XST_SUCCESS) {
		xil_printf("Read channel config failed %d\r\n", Status);
		return XST_FAILURE;
	}

	Addr = vdma_context->buffer_address;

	for(Index = 0; Index < vdma_context->InstancePtr->MaxNumFrames; Index++) {
		vdma_context->ReadCfg.FrameStoreStartAddr[Index] = Addr;

#if DEBUG_MODE
		{
			u32 i;
			u8 *src;
			u32 total_pixel = vdma_context->ReadCfg.Stride * vdma_context->vsize;
			src = (unsigned char *)Addr;
			xil_printf("Read Buffer %d address: 0x%x \r\n",Index,Addr);
			for(i=0;i<total_pixel;i++)
			{
				src[i] = i & 0xFF;
			}
		}
#endif
		Addr +=  vdma_context->hsize * vdma_context->vsize;
	}

	Status = XAxiVdma_DmaSetBufferAddr(vdma_context->InstancePtr, XAXIVDMA_READ,
			vdma_context->ReadCfg.FrameStoreStartAddr);
	if (Status != XST_SUCCESS) {
		xil_printf(
			"Read channel set buffer address failed %d\r\n", Status);

		return XST_FAILURE;
	}

	return XST_SUCCESS;
}

static int WriteSetup(vdma_handle *vdma_context)
{
	int Index;
	u32 Addr;
	int Status;

	vdma_context->WriteCfg.VertSizeInput = vdma_context->vsize;
	vdma_context->WriteCfg.HoriSizeInput = vdma_context->hsize;

	vdma_context->WriteCfg.Stride = vdma_context->hsize;
	vdma_context->WriteCfg.FrameDelay = 0;

	vdma_context->WriteCfg.EnableCircularBuf = 1;
	vdma_context->WriteCfg.EnableSync = 1;

	vdma_context->WriteCfg.PointNum = 0;
	vdma_context->WriteCfg.EnableFrameCounter = 0;

	vdma_context->WriteCfg.FixedFrameStoreAddr = 0;

	Status = XAxiVdma_DmaConfig(vdma_context->InstancePtr, XAXIVDMA_WRITE, &vdma_context->WriteCfg);
	if (Status != XST_SUCCESS) {
		xil_printf(
			"Write channel config failed %d\r\n", Status);

		return Status;
	}

	Addr = vdma_context->buffer_address;

#if DEBUG_MODE
	Addr = Addr + vdma_context->InstancePtr->MaxNumFrames * \
			(vdma_context->WriteCfg.Stride * vdma_context->vsize);
#endif

	for(Index = 0; Index < vdma_context->InstancePtr->MaxNumFrames; Index++) {
		vdma_context->WriteCfg.FrameStoreStartAddr[Index] = Addr;
#if DEBUG_MODE
		xil_printf("Write Buffer %d address: 0x%x \r\n",Index,Addr);
#endif

		Addr += (vdma_context->hsize * vdma_context->vsize);
	}

	Status = XAxiVdma_DmaSetBufferAddr(vdma_context->InstancePtr,
			XAXIVDMA_WRITE,
			vdma_context->WriteCfg.FrameStoreStartAddr);
	if (Status != XST_SUCCESS) {
		xil_printf("Write channel set buffer address failed %d\r\n",
				Status);
		return XST_FAILURE;
	}

#if DEBUG_MODE
	memset((void *)vdma_context->buffer_address, 0,
			vdma_context->ReadCfg.Stride * vdma_context->ReadCfg.VertSizeInput * vdma_context->InstancePtr->MaxNumFrames);
#endif
	return XST_SUCCESS;
}

static int StartTransfer(XAxiVdma *InstancePtr,vdma_run_mode mode)
{
	int Status;
	if ((mode == BOTH) || (mode == ONLY_WRITE)) {

		Status = XAxiVdma_DmaStart(InstancePtr, XAXIVDMA_WRITE);
		if (Status != XST_SUCCESS) {
			xil_printf("Start Write transfer failed %d\r\n", Status);
			return XST_FAILURE;
		}
	}

	if ((mode == BOTH) || (mode == ONLY_READ)) {
		Status = XAxiVdma_DmaStart(InstancePtr, XAXIVDMA_READ);
		if (Status != XST_SUCCESS) {
			xil_printf("Start read transfer failed %d\r\n", Status);
			return XST_FAILURE;
		}
	}
	return XST_SUCCESS;
}
