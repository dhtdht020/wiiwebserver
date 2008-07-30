#pragma once
#ifndef REQUEST_H
#define REQUEST_H

#include <map>
#include <string>

using std::map;
using std::string;

//class Request;

class Client;
#include "Resource.h"

class Resource;

class Request {
public:	
	Request(Client *c);
	void doRequest(void);
	~Request();
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
	string replyReason;
	map<string,string> replyHeaders;
	string replyBody;

private://methods that we DONT want but are forced to have,
	//make them unimplemented and private to prevent them from ever being used
	Request(void);
	Request(const Request &);
	Request &operator=(const Request &);

	void readRequest(void);
	void readRequestHeaders(void);

	void sendReply(void);
	void sendReplyHeaders(void);


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

#endif
