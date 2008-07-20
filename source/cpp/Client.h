#pragma once

#include <ctime>

typedef int SOCKET;

class Client {
	//warning, not closed unless there is a timeout
	//or we are required to because the client wanted us to
	SOCKET socket;

	//misc statistic variables
	unsigned int numRequests;
	time_t lastRequest;

public:
	Client(void);
	~Client(void);

	void close(void);
};
