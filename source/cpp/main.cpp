extern "C" {
#include <fat.h>
#include <ogcsys.h>
#include <gccore.h>
}

#include "configuration.h"
#include "display.h"
#include "listeningSocket.h"
#include "globals.h"
#include "debug.h"

void waitForAllThreadsToEnd();

void loadInternalFiles();

void initConsole();

int main() {
	fatInitDefault();
	PAD_Init();//used by the crash screen

	loadConfiguration();

#ifndef USE_CONSOLE
	startDisplayThread();
#else
	initConsole();
#endif
	CONLOG("console ready for use");

#ifndef GUI_ONLY_TEST
	CONLOG("init fat");
	loadInternalFiles();

	try {
		CONLOG("init listen");
		setupListeningSocket();
		listenstatus=OK;

		CONLOG("launch listen");
		startListeningThread();
	} catch (const listenFailure &) {
		listenstatus=ERROR;
		CONLOG("listen init fail");
	}


#endif
	
	waitForAllThreadsToEnd();

	return 0;

}
