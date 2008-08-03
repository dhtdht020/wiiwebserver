extern "C" {
#include <ogcsys.h>
#include <gccore.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
}

#include "buildconfig.h"

#ifdef USE_CONSOLE
#define CONLOG(x) printf("%s",x)
#else
#define CONLOG(x)
#endif
