extern "C" {

#include <ogcsys.h>
#include <gccore.h>

}

#include "globals.h"


void waitForAllThreadsToEnd(void) {
	LWP_JoinThread(displayThread,NULL);
	LWP_JoinThread(listenThread,NULL);
}
