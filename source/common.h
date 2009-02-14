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
#ifndef _COMMON_H_
#define _COMMON_H_

#include <ogcsys.h>

const char *CRLF;
const u32 CRLF_LENGTH;

s32 write_exact(s32 s, char *buf, s32 length);

#endif /* _COMMON_H_ */

/**
NON Joseph Jordan Code
Wii Web server

sorce code copyright to Luke Bridges
except for any code that is credited to other
developers, this goes for all wii web server 
source files

joedj - sections of ftpii are used and unchanged in common.c and common.h
        unwanted code has been removed from these files, the only code left
		  should be write_exact and relating functions
		  
teknecal - sections of wiihttpd are used, these include converting %20 to spaces
           and checking for ../ and ./
			  the "HEAD" handling routine is used as 'inspiration' and is not exactly
			  the same but credit is still given here

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

The best effort has been made to keep to the  copyright rights, if there
are any faults please contact me and i'll be glad to make any necessary corrections.
**/

int write_data(int s, char *buf, int n);
int socket_printf(const s32 socket, const char *format, ...);

/**
Other
**/
bool scheck(void);
void update();

s32 set_blocking(s32 s, bool blocking);
