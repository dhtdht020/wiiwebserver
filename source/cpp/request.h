#pragma once
#include <map>
#include <string>

using std::map;
using std::string;

//class Request;

#include "Client.h"
#include "Resource.h"

class Resource;

class Request {
private:
	Client *client;

	struct {
		unsigned int major;
		unsigned int minor;
	} httpVersion;

	string resourceName;

	map<string,string> requestHeaders;
	string requestMethod;
	string requestBody;

	unsigned int replyNumber;
	map<string,string> replyHeaders;
	string replyBody;

	Request(Client *c);
/*
private://methods that we DONT want but are forced to have,
	//make them unimplemented and private to prevent them from ever being used
	Request();
	Request(const &Request);
	operator=(const &Request);*/

	void readRequest(void);
	void readRequestHeaders(void);

	void sendReply(void);
	void sendReplyHeaders(void);

	void doRequest(void);

	bool locateAndLoadResource(void);

	void loadErrorReply(unsigned int replyNumber);

	//now for the flow control members
	bool requestExists;
	bool requestAllowed;

	Resource *resource;

	class CacheControll {
	public:
		enum { PUBLIC, PRIVATE, NO_CACHE, NO_STORE } cacheMode;
		bool mustRevalidate;
		bool noTransform;

		CacheControll();
	} cacheControll;
};

class InvalidRequest {};
