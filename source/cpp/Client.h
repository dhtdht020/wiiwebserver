#pragma once

#include <ctime>

extern "C" {
#include <ogcsys.h>
#include <gccore.h>
}

#include "threadwraper.h"

typedef int SOCKET;

class Client {
	//warning, not closed unless there is a timeout
	//or we are required to because the client wanted us to
	SOCKET socket;

	//misc statistic variables
	unsigned int numRequests;
	time_t lastRequest;

public:
	Client(SOCKET);
	~Client(void);

	void startThread(void);

	void close(void);

private:
	lwp_t thread;

	int run();

	friend void *objectRun<Client>(void *);
};
