//#include <ogcsys.h>
//#include <gccore.h>

//#include <wiiuse/wpad.h>

#include "fakewiiheader.h"

void *xfb;
GXRModeObj *rmode;

#define NULL 0

void *Display(void*) {
	VIDEO_Init();
	PAD_Init();
	WPAD_Init();
	
	rmode = VIDEO_GetPreferredMode(NULL);
	xfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
	//console_init(xfb,20,20,rmode->fbWidth,rmode->xfbHeight,rmode->fbWidth*VI_DISPLAY_PIX_SZ);
	VIDEO_Configure(rmode);
	VIDEO_SetNextFramebuffer(xfb);
	VIDEO_SetBlack(FALSE);
	VIDEO_Flush();

	//TODO: add input and display proccessing loop

	return (void*)0;
}