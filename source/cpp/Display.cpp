extern "C" {
#include <ogcsys.h>
#include <gccore.h>

#include <wiiuse/wpad.h>
}

#include "globals.h"

#include <wiisprite.h>

using namespace wsp;

void *Display(void*) {

	PAD_Init();
	WPAD_Init();

	GameWindow gwd;

	gwd.InitVideo();
	
	//TODO: add input and display proccessing loop

	gwd.StopVideo();

	return (void*)0;
}

void startDisplayThread() {
	LWP_CreateThread(&displayThread,Display,NULL,NULL,NULL,1);
}
