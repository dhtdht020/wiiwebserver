extern "C" {

#include <ogcsys.h>
#include <gccore.h>

}

#include "globals.h"


void waitForAllThreadsToEnd(void) {
	if(LWP_THREAD_NULL!=displayThread) {
		LWP_JoinThread(displayThread,NULL);
	}
#ifndef GUI_ONLY_TEST
	if(LWP_THREAD_NULL!=listenThread) {
		LWP_JoinThread(listenThread,NULL);
	}
#endif
}
