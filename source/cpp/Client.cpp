#include "Client.h"
#include "request.h"

extern "C" {
#include <network.h>
}

Client::Client(SOCKET s) : numRequests(0), lastRequest(0), socket(s) {
};

Client::~Client(void) {
};

void Client::startThread() {
	LWP_CreateThread(&thread,objectRun<Client>,(void *)this,NULL,NULL,1);
};

int Client::run() {
	for(;;) {
		fd_set readfds; FD_SET(socket, &readfds);
		int result = net_select(socket,&readfds,0,0,0);
		if(result <0) {
			break;
		} else {
			Request r(this);
			r.doRequest();
		}
	}
	return 0;
}

void Client::close() {
	net_close(socket);
}
