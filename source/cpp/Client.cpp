#include "Client.h"
#include "request.h"
#include "buildconfig.h"

extern "C" {
#include <network.h>
}

Client::Client(SOCKET s) : numRequests(0), lastRequest(0), socket(s) {
};

Client::~Client(void) {
};

void Client::startThread() {
#ifndef NO_CLIENT_THREADS
	LWP_CreateThread(&thread,objectRun<Client>,(void *)this,NULL,NULL,1);
#else
	this->run();
#endif
};

int Client::run() {
	for(;;) {
	
		try {
			Request r(this);
			r.doRequest();
		} catch (...) {
			break;
		}
	
	}
	return 0;
}

void Client::close() {
	net_close(socket);
}
