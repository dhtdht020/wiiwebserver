/*

Copyright (C) 2008 Joseph Jordan <joe.ftpii@psychlaw.com.au>

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from
the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1.The origin of this software must not be misrepresented; you must not
claim that you wrote the original software. If you use this software in a
product, an acknowledgment in the product documentation would be
appreciated but is not required.

2.Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original software.

3.This notice may not be removed or altered from any source distribution.

*/
#include <errno.h>
#include <fat.h>
#include <math.h>
#include <network.h>
#include <ogc/lwp_watchdog.h>
#include <ogcsys.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/dir.h>
#include <unistd.h>
#include <wiiuse/wpad.h>

#include "common.h"

#define NET_BUFFER_SIZE 1024
#define FREAD_BUFFER_SIZE 1024

const char *CRLF = "\r\n";
const u32 CRLF_LENGTH = 2;

typedef s32 (*transferrer_type)(s32 s, void *mem, s32 len);
inline static s32 transfer_exact(s32 s, char *buf, s32 length, transferrer_type transferrer) {
    s32 bytes_transferred = 0;
    s32 remaining = length;
    while (remaining) {
        if ((bytes_transferred = transferrer(s, buf, remaining > NET_BUFFER_SIZE ? NET_BUFFER_SIZE : remaining)) > 0) {
            remaining -= bytes_transferred;
            buf += bytes_transferred;
        } else if (bytes_transferred < 0) {
            return bytes_transferred;
        } else {
            return -ENODATA;
        }
    }
    return 0;
}

inline s32 write_exact(s32 s, char *buf, s32 length) {
    return transfer_exact(s, buf, length, (transferrer_type)net_write);
}
