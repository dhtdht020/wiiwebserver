#include "listeningSocket.h"

#include "globals.h"

extern "C" {
#include <network.h>
#include <errno.h>
}

#include "Client.h"

typedef s32 SOCKET;

SOCKET listenSocket;

void setupListeningSocket(void) {


	s32 s32Success;

	do {
		s32Success=net_init();
	} while (s32Success==-EAGAIN);

	if(s32Success<0) throw listenFailure();

	listenSocket=net_socket(AF_INET, SOCK_STREAM, 0);

	if(listenSocket == INVALID_SOCKET) throw listenFailure();

	struct sockaddr_in sa;

	memset(&sa, 0, sizeof(struct sockaddr_in)); /* clear our address */
	sa.sin_family= AF_INET;              
	sa.sin_port= htons(80);         /* this is our port number */
	sa.sin_addr.s_addr = INADDR_ANY;/* this is our host address */

	s32Success=net_bind(listenSocket,(struct sockaddr*)&sa, sizeof(sa));

	if(s32Success<0) throw listenFailure();

	s32Success=net_listen(listenSocket,1);

	if(s32Success<0) throw listenFailure();

	return;
}

void *Listen(void*) {
	while(listenForConnections) {
		struct sockaddr_in sa;
		size_t saLenght = sizeof(sa);


		fd_set listenset;
		FD_ZERO(&listenset);
		FD_SET(listenSocket,&listenset);
		int numIncomming = net_select(1,&listenset,0,0,0);

		if(numIncomming>0) {

			SOCKET clientSocket;
			clientSocket=net_accept(listenSocket,(sockaddr*)&sa,&saLenght);

			if(clientSocket!=INVALID_SOCKET) {
				Client *c=new Client(clientSocket);
				c->startThread();
				clients.push_back(c);
			}
		}
	}
	return (void*)0;
}

void startListeningThread(void) {
	LWP_CreateThread(&listenThread,Listen,NULL,NULL,NULL,1);
}
