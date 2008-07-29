#include "debug.h"

void *xfb = NULL;
GXRModeObj *rmode = NULL;

void initConsole() {
	switch(VIDEO_GetCurrentTvMode()) {
		case VI_NTSC:
			rmode = &TVNtsc480IntDf;
		break;

		case VI_PAL:
			rmode = &TVPal528IntDf;
		break;

		case VI_MPAL:
			rmode = &TVMpal480IntDf;
		break;

		default:
			rmode = &TVNtsc480IntDf;
		break;
	}
	
	rmode = VIDEO_GetPreferredMode(NULL);
	xfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
	console_init(xfb,20,20,rmode->fbWidth,rmode->xfbHeight,rmode->fbWidth*VI_DISPLAY_PIX_SZ);
	VIDEO_Configure(rmode);
	VIDEO_SetNextFramebuffer(xfb);
	VIDEO_SetBlack(FALSE);
	VIDEO_Flush();
	VIDEO_WaitVSync();
	if(rmode->viTVMode&VI_NON_INTERLACE) VIDEO_WaitVSync();
};
