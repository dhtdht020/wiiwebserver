extern "C" {
#include <fat.h>
#include <ogcsys.h>
#include <gccore.h>
}

#include "configuration.h"
#include "display.h"
#include "listeningSocket.h"

void waitForAllThreadsToEnd();

void loadInternalFiles();

int main() {
	fatInitDefault();
	PAD_Init();//used by the crash screen

	loadConfiguration();

	startDisplayThread();

#ifndef GUI_ONLY_TEST
	loadInternalFiles();

	setupListeningSocket();

	startListeningThread();
#endif
	
	waitForAllThreadsToEnd();

	return 0;

}
