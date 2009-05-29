#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ogcsys.h>
#include <gccore.h>
#include <stdarg.h>
#include <unistd.h>
#include <network.h>
#include <fat.h>
#include <malloc.h>
#include <time.h>
#include <wiiuse/wpad.h>
#include <ogc/pad.h>
#include <mxml.h>
#include <ogc/mutex.h>
#include <fcntl.h>
#include <sys/dir.h>

#include "wiiwebserver.h"
#include "inbuilt.h"
#include "libhttp.h"

#include "../../../GRRLIB/GRRLIB/GRRLIB.h"

static u32 aMutex = 0;
lwp_t serverT;
#define THREADSTACK 8192
u8 threadonestack[THREADSTACK];
lwpq_t ThreadOneQ;

u32 ip;

const char servver[4]="1.00";

bool csocket(void);
int write_data(int s, char *buf, int n);
int socket_printf(const s32 socket, const char *format, ...);
void inits();
void senderror(client_t *client, int errorcode);
//s32 set_blocking(s32 s, bool blocking);

int server_timezone = 0;

void headersend(client_t *client)
{
	// Date/time
	long the_time = time(0) - (server_timezone * 3600);
	
	char* date_time[5];
	char* tok;
	int x = 0;
	
	tok = strtok (ctime (&the_time)," ");
	while (tok != NULL) {
		date_time[x] = tok;
		tok = strtok (NULL, " ");
		x++;
	}
	
	char year[6] = "";
	strncpy (year,date_time[4], 4);
	year[5]='\0';
	
	printf("Sending : %s %s %s\r\n",client->response.httpver,client->response.statuscode,client->response.status);
	printf("Sending : Date: %s, %s %s %s %s GMT\r\n", date_time[0], date_time[1], date_time[2], year, date_time[3]);
	printf("Sending : Content-Type: %s\r\n",client->response.mimetype);
	printf("Sending : Content-Length: %d\r\n",client->response.contlength);
	printf("Sending : Server: WiiWebServer/%s\r\n", servver);
	printf("Sending : Host: %s\r\n",inet_ntoa(*(struct in_addr *)&ip));
	
	socket_printf(client->socket, "%s %s %s\r\n",client->response.httpver,client->response.statuscode,client->response.status);
	socket_printf(client->socket, "Date: %s, %s %s %s %s GMT\r\n", date_time[0], date_time[1], date_time[2], year, date_time[3]);
	socket_printf(client->socket, "Content-Type: %s\r\n",client->response.mimetype);
	socket_printf(client->socket, "Content-Length: %d\r\n",client->response.contlength);
	socket_printf(client->socket, "Server: WiiWebServer/%s\r\n", servver);
	socket_printf(client->socket, "Host: %s\r\n",inet_ntoa(*(struct in_addr *)&ip));
	socket_printf(client->socket, "\r\n");
}

void linesicon(client_t *client)
{
	client->response.contlength=wiiwebserver_lines_png_size;
	printf("Sending admin page\n");
	client->response.statuscode = "200";
	client->response.status = "OK";
	strcpy(client->response.mimetype,"image/png");
	headersend(client);
	net_write(client->socket,wiiwebserver_lines_png,client->response.contlength);
	printf("Sent\n");
	usleep(200000);
	VIDEO_WaitVSync();
	return;
}

void admin(client_t *client)
{
	client->response.contlength=master_html_size;
	printf("Sending admin page\n");
	client->response.statuscode = "200";
	client->response.status = "OK";
	strcpy(client->response.mimetype,"text/html");
	headersend(client);
	net_write(client->socket,master_html,client->response.contlength);
	printf("Sent\n");
	usleep(200000);
	VIDEO_WaitVSync();
	return;
}

void favicon(client_t *client)
{
	client->response.contlength=favicon_ico_size;
	printf("Sending favicon\n");
	client->response.statuscode = "200";
	client->response.status = "OK";
	strcpy(client->response.mimetype,"icon/x-icon");
	headersend(client);
	net_write(client->socket,favicon_ico,client->response.contlength);
	printf("Sent\n");
	usleep(200000);
	VIDEO_WaitVSync();
	return;
}

#define MAX_NET_BUFFER_SIZE 32768
#define MIN_NET_BUFFER_SIZE 4096
#define FREAD_BUFFER_SIZE 32768
#define NET_BUFFER_SIZE 2048

typedef s32 (*transferrer_type)(s32 s, void *mem, s32 len);

inline static s32 transfer_exact(s32 s, char *buf, s32 length, transferrer_type transferrer) 
{
    s32 bytes_transferred = 0;
    s32 remaining = length;
    while (remaining) {
        if ((bytes_transferred = transferrer(s, buf, remaining > NET_BUFFER_SIZE ? NET_BUFFER_SIZE : remaining)) > 0) 
		{
            remaining -= bytes_transferred;
            buf += bytes_transferred;
        } 
		else if (bytes_transferred < 0) 
		{
            return bytes_transferred;
        } 
		else 
		{
            return -ENODATA;
        }
    }
    return 0;
}

s32 send_exact(s32 s, char *buf, s32 length) {
    return transfer_exact(s, buf, length, (transferrer_type)net_write);
}

void sendthingy(client_t *client,int device)
{
	//while (LWP_MutexLock (aMutex) != 0);
	char* path = "";
	if(device == 1)
	{
		sprintf(path, "sd://data/web%s",client->request.path);
	}
	else
	{
		if(device == 2)
		{
			sprintf(path, "usb://data/web%s",client->request.path);
		}
		else
		{
			sprintf(path, "sd://data/web%s",client->request.path);
		}
	}
	
	/*DIR_ITER* dp = diropen (path);
	if((dp==NULL))
	{
		senderror(client,500);
		dirclose (dp);
		net_close(client->socket);
		return;
	}
	else
	{
		strcat(path,"/index.html");
	}
	dirclose (dp);
	*/
	
	client->response.status = "OK";
	client->response.statuscode = "200";
	
	printf("Opening : %s\r\n",path);
	
	FILE* pFile;
	long lSize = 0;
	char * buffer = "";
	size_t result;
	pFile = fopen (path, "r");
	if (pFile!=NULL) 
	{   
		fseek (pFile , 0 , SEEK_END);
		lSize = ftell (pFile);
		rewind (pFile);
		buffer = (char*) malloc (sizeof(char)*lSize);
		if (buffer == NULL) {senderror(client,500); fclose(pFile); net_close(client->socket); return;}
		result = fread (buffer,1,lSize,pFile);
		if (result != lSize) {senderror(client,500); fclose(pFile); net_close(client->socket); return;}
		fclose (pFile); 
	}
	else
	{
		senderror(client,404);
	}
	client->response.contlength=lSize;
	
	headersend(client);
	
	if((strcmp(client->request.method,"HEAD")==0))
	{
	}
	else
	{
		send_exact(client->socket,buffer,client->response.contlength);
		//net_write(client->socket,buffer,client->response.contlength);
	}
	
	sleep(2);
	
	printf("Sent...\r\n");
	
	net_close(client->socket);
	
	//LWP_MutexUnlock (aMutex);
	
	return;
}

static void* reqhandle(void *arg)
{	
	while (LWP_MutexLock (aMutex) != 0);
	
	char recieved[256];
	client_t *client = arg;
	net_recv(client->socket, recieved, sizeof(recieved), 0);
	
	if(parserequest(recieved,true,client)==0)
	{
		free(client);
		printf("Something failed ... \r\n\r\n\r\n");
		LWP_MutexUnlock (aMutex);
		while(1);
		return NULL;
	}
	
	if(strcmp(client->request.httpver,"HTTP/1.1")==0)
	{
		client->ishttp11 = true;
	}
	else
	{
		client->ishttp11 = false;
	}
	
	if(strstr(client->request.path,"../") || strstr(client->request.path,"./"))
	{
		senderror(client,403);
		net_close(client->socket);
		free(client);
		LWP_MutexUnlock (aMutex);
		return NULL;
	}
	
	if(strcmp(client->request.path,"/favicon.ico")==0)
	{
		favicon(client);
		LWP_MutexUnlock (aMutex);
		net_close(client->socket);
		free(client);
		return NULL;
	}

	if(strcmp(client->request.path,"/admin")==0)
	{
		admin(client);
		LWP_MutexUnlock (aMutex);
		net_close(client->socket);
		free(client);
		return NULL;
	}
	
	if(strcmp(client->request.path,"/wiiwebserver_lines.png")==0)
	{
		linesicon(client);
		LWP_MutexUnlock (aMutex);
		net_close(client->socket);
		free(client);
		return NULL;
	}
	
	if ((client->request.path[strlen(client->request.path)-1] == '/'))
	{
		printf("Last char is a slash\r\n");
		strcat(client->request.path, "index.html");
	}
	
	if((client->request.path[1] == 'u') && (client->request.path[2] == 's') && (client->request.path[3] == 'b'))
	{
		sendthingy(client,2);
	}
	else
	{
		sendthingy(client,1);
	}
	free(client);
	LWP_MutexUnlock (aMutex);
	printf("Connection closed...\r\n");
	return NULL;
}

static void* waitforhome()
{
	while(1) 
	{
		WPAD_ScanPads();
		u32 pressed = WPAD_ButtonsDown(0);
		if ( (pressed & WPAD_CLASSIC_BUTTON_HOME) || (pressed & WPAD_BUTTON_HOME) || (pressed & WPAD_GUITAR_HERO_3_BUTTON_YELLOW) || (pressed & WPAD_NUNCHUK_BUTTON_C) || (pressed & WPAD_BUTTON_A) )
		{
			while (LWP_MutexDestroy(aMutex) != 0);
			VIDEO_WaitVSync();
			exit(0);
		}
		VIDEO_WaitVSync();
	}
}

int UnBase64(unsigned char *dest, const unsigned char *src, int srclen);

int main(int argc, char **argv) 
{
	inits();
		
	printf("\r\n");
	printf("Wii Web Server v%s + LibHTTP v%s\r\n", servver, libhttpver);
	printf("\r\n");
	printf("Starting ...\r\n");
	printf("\r\n");
	
	
	/*unsigned char* b64in = (unsigned char*)"bHVrZSBicmlkZ2Vz";
	unsigned char* b64out;
	UnBase64(b64out,b64in,16);
	printf("%s\r\n\r\n",b64out);
	*/
	
	LWP_MutexInit (&aMutex, false);
	
	while(!csocket())
	{}
	
	ip = net_gethostip();
    printf("IP : %s\r\n\r\n", inet_ntoa(*(struct in_addr *)&ip));
		
	int client_s;
	u32 len = sizeof(sa);
	
	LWP_CreateThread (&serverT, waitforhome, NULL, NULL, 0, 80);
	
	printf("Ready ...\r\n\r\n");
	
	while(1)
	{	
		client_s=net_accept(netSocket, (struct sockaddr*)&sa, &len);
		client_t *client = malloc(sizeof(client_t));
		strcpy(client->request.method,"          ");
		strcpy(client->request.path,"                                                                 ");
		strcpy(client->request.httpver,"        ");
		strcpy(client->request.path,"        ");
		strcpy(client->request.path,"                                                                 ");
		strcpy(client->response.mimetype,"                                ");
		client->response.httpver = "       ";
		client->response.statuscode = "                               ";
		printf("Connection\r\n");
		client->socket = client_s;
		LWP_CreateThread (&serverT, reqhandle, client, NULL, 0, 80);
	}

    exit(0);
}
