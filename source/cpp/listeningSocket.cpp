#include "listeningSocket.h"

#include "globals.h"

extern "C" {
#include <network.h>
#include <errno.h>
#include <fcntl.h>
}

#include "Client.h"
#include "debug.h"

typedef s32 SOCKET;

SOCKET listenSocket;

void setupListeningSocket(void) {


	s32 s32Success;

	CONLOG("init network");

	do {
		CONLOG("try");
		s32Success=net_init();
	} while (s32Success==-EAGAIN);

	if(s32Success<0) throw listenFailure();

	CONLOG("create socket");

	listenSocket=net_socket(AF_INET, SOCK_STREAM, 0);

	if(listenSocket == INVALID_SOCKET) throw listenFailure();

	struct sockaddr_in sa;

	CONLOG("init sockaddr");

	memset(&sa, 0, sizeof(struct sockaddr_in)); /* clear our address */
	sa.sin_family= AF_INET;              
	sa.sin_port= htons(80);         /* this is our port number */
	sa.sin_addr.s_addr = INADDR_ANY;/* this is our host address */

	CONLOG("bind");

	s32Success=net_bind(listenSocket,(struct sockaddr*)&sa, sizeof(sa));

	s32 t =net_fcntl(listenSocket,F_GETFL, 0);
	t |= O_NONBLOCK;
	net_fcntl(listenSocket, F_SETFL, t);

	if(s32Success<0) throw listenFailure();

	CONLOG("listen");

	s32Success=net_listen(listenSocket,1);

	if(s32Success<0) throw listenFailure();

	return;
}

void *Listen(void*) {
	CONLOG("Listen thread");
	while(listenForConnections) {
		CONLOG("wait con");
		struct sockaddr_in sa;
		size_t saLenght = sizeof(sa);


		CONLOG("con accept");

		SOCKET clientSocket;
		clientSocket=net_accept(listenSocket,(sockaddr*)&sa,&saLenght);

		if(clientSocket!=INVALID_SOCKET) {
			CONLOG("con accepted");
			Client *c=new Client(clientSocket);
			c->startThread();
			clients.push_back(c);
		}
	}
	return (void*)0;
}

void startListeningThread(void) {
	LWP_CreateThread(&listenThread,Listen,NULL,NULL,NULL,1);
}
