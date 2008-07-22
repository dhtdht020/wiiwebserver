extern "C" {
#include <ogcsys.h>
#include <gccore.h>

#include <wiiuse/wpad.h>
}

#include "globals.h"

static void *xfb;
static GXRModeObj *rmode;

void *Display(void*) {
	VIDEO_Init();
	PAD_Init();
	WPAD_Init();
	
	rmode = VIDEO_GetPreferredMode(NULL);
	xfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
	//console_init(xfb,20,20,rmode->fbWidth,rmode->xfbHeight,rmode->fbWidth*VI_DISPLAY_PIX_SZ);
	VIDEO_Configure(rmode);
	VIDEO_SetNextFramebuffer(xfb);
	VIDEO_SetBlack(TRUE);
	VIDEO_Flush();

	//TODO: add input and display proccessing loop

	return (void*)0;
}

void startDisplayThread() {
	LWP_CreateThread(&displayThread,Display,NULL,NULL,NULL,1);
}
