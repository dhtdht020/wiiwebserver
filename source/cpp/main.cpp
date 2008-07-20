//#include <fat.h>
#include "fakewiiheader.h"

#include "configuration.h"
#include "display.h"
#include "listeningSocket.h"

void waitForAllThreadsToEnd();

int main() {
	fatInitDefault();

	loadConfiguration();

	startDisplayThread();

	setupListeningSocket();

	startListeningThread();
	
	waitForAllThreadsToEnd();

	return 0;

}