extern "C" {

#include <ogcsys.h>
#include <gccore.h>

}

#include "globals.h"


void waitForAllThreadsToEnd(void) {
	LWP_JoinThread(displayThread,NULL);
#ifndef GUI_ONLY_TEST
	LWP_JoinThread(listenThread,NULL);
#endif
}
