/**
Wii USB Keyboard Library

sorce code copyright to Luke Bridges
except for any code that is credited to other
developers, this goes for all wii web server 
source files

This was made using information from wiibrew.org

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from
the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to  redistribute it freely, subject 
to the following restrictions:

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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gccore.h>
#include <ogc/ipc.h>
#include <wiiuse/wpad.h>

#include "kbd.h"

int libkbdver = 1.0;

struct kbdmessage
{
	int type[4];
	int unknown[4];
	int modifiers[1];
	int unknown2[1];
	int pressed[6];
};

struct messagetype
{
	int option;
	char* typechar;
}
message_types[] = 
{
	{0,"Keyboard connected"},
	{1,"Keyboard disconnected"},
	{2,"Event"},
	{999,NULL}
};

struct messagemodifiers 
{
	int option;
	char* typechar;
}
message_mods[] =
{
	{0,"No modifier"},
	{1,"Left control"},
	{2,"Left shift"},
	{4,"Left alt"},
	{8,"Left windows key"},
	{10,"Right control"},
	{20,"Right shift"},
	{40,"Right alt"},
	{80,"Right windows key"},
	{999,"No modifier"}
};

struct messagepressed
{
	int option;
	char* typechar;
}
message_keys[] =
{
	{4,"a"},
	{5,"b"},
	{6,"c"},
	{7,"d"},
	{8,"e"},
	{9,"f"},
	{10,"g"},
	{11,"h"},
	{12,"i"},
	{13,"j"},
	{14,"k"},
	{15,"l"},
	{16,"m"},
	{17,"n"},
	{18,"o"},
	{19,"p"},
	{20,"q"},
	{21,"r"},
	{22,"s"},
	{23,"t"},
	{24,"u"},
	{25,"v"},
	{26,"w"},
	{27,"x"},
	{28,"y"},
	{29,"z"},
	{30,"1"},
	{31,"2"},
	{32,"3"},
	{33,"4"},
	{34,"5"},
	{35,"6"},
	{36,"7"},
	{37,"8"},
	{38,"9"},
	{39,"0"},
	{40,"enter"},
	{41,"escape"},
	{42,"backspace"},
	{43,"tab"},
	{44," "},
	{45,"-"},
	{46,"="},
	{47,"["},
	{48,"]"},
	{51,";"},
	{52,"'"},
	{53,"#"},
	{54,","},
	{55,"."},
	{56,"/"},
	{57,"capslock"},
	{58,"F1"},
	{59,"F2"},
	{60,"F3"},
	{61,"F4"},
	{62,"F5"},
	{63,"F6"},
	{64,"F7"},
	{65,"F8"},
	{66,"F9"},
	{67,"F10"},
	{68,"F11"},
	{69,"F12"},
	{70,"printscreen"},
	{71,"scrollolck"},
	{72,"pause"},
	{73,"insert"},
	{74,"home"},
	{75,"pageup"},
	{76,"delete"},
	{77,"end"},
	{78,"pagedown"},
	{79,"right"},
	{80,"left"},
	{81,"down"},
	{82,"up"},
	{83,"numlock"},
	{84,"num/"},
	{85,"num*"},
	{86,"num-"},
	{87,"num+"},
	{88,"numenter"},
	{89,"num1"},
	{90,"num2"},
	{91,"num3"},
	{92,"num4"},
	{93,"num5"},
	{94,"num6"},
	{95,"num7"},
	{96,"num8"},
	{97,"num9"},
	{98,"num0"},
	{99,"num."},
	{100,"|"},
	{999,NULL}
};

struct messagetype msgtype;
struct messagemodifiers msgmods;
struct messagepressed msgkeys;
struct kbdmessage kbdm;

char* kbdhandle(int fd, char message[15], bool output)
{
	char* ret="";
	kbdm.type[0]=message[0];
	kbdm.type[1]=message[1];
	kbdm.type[2]=message[2];
	kbdm.type[3]=message[3];
	kbdm.unknown[0]=message[4];
	kbdm.unknown[1]=message[5];
	kbdm.unknown[2]=message[6];
	kbdm.unknown[3]=message[7];
	kbdm.modifiers[0]=message[8];
	kbdm.unknown2[0]=message[9];
	kbdm.pressed[0]=message[10];
	kbdm.pressed[1]=message[11];
	kbdm.pressed[2]=message[12];
	kbdm.pressed[3]=message[13];
	kbdm.pressed[4]=message[14];
	kbdm.pressed[5]=message[15];
	if(output==true)
	{
		printf("\n");
		printf("Message breakdown = \n");
		printf("  Type      = %x %x %x %x\n",kbdm.type[0],kbdm.type[1],kbdm.type[2],kbdm.type[3]);
		printf("  Unknown   = %x %x %x %x\n",kbdm.unknown[0],kbdm.unknown[1],kbdm.unknown[2],kbdm.unknown[3]);
		printf("  Modifiers = %x\n",kbdm.modifiers[0]);
		printf("  Unknown2  = %x\n",kbdm.unknown2[0]);
		printf("  Pressed   = %x %x %x %x %x %x\n",kbdm.pressed[0],kbdm.pressed[1],kbdm.pressed[2],kbdm.pressed[3],kbdm.pressed[4],kbdm.pressed[5]);
		printf("\n");
		printf("  Type      = ");
		int i;
		for (i = 0; message_types[i].option != 999; i++) 
		{
			if (kbdm.type[3]==message_types[i].option)
			{
				printf("%s",message_types[i].typechar);
			}
		}
		printf("\n");
		printf("  Modifiers = ");
		int j;
		for (j = 0; message_mods[j].option != 999; j++) 
		{
			if (kbdm.modifiers[0]==message_mods[j].option)
			{
				printf("%s",message_mods[j].typechar);
			}
		}
		printf("\n");
		printf("  Pressed   = ");
	}
	int f;
	for (f = 0; message_keys[f].option != 999; f++) 
	{
		if (kbdm.pressed[0]==message_keys[f].option)
		{
			if(output==true)
			{
				printf("%s",message_keys[f].typechar);
			}
			ret=message_keys[f].typechar;
		}
	}
	if(output==true)
	{
		printf("\n\n");
	}
	return ret;
}

char* kbdscan(int kbdfd)
{
	char* kbdmo="";
	IOS_Ioctl(kbdfd,0x00," ",1,kbdmo,16);
	return kbdmo;
}

void kbddeinit(int kbdfd)
{
	IOS_Close(kbdfd);
}

int kbdinit()
{
	return IOS_Open("/dev/usb/kbd",1);
}

int kbdver()
{
	return libkbdver;
}

