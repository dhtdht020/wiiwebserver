#pragma once
#ifndef RESOURCE_H
#define RESOURCE_H

//class Resource;

#include "Request.h"

class Request;

class Resource {
public:
	Resource(void);
	virtual ~Resource(void);
	static Resource *load(string name);

	virtual void setAllowHeader(Request *r)=0;
	virtual void loadBuffer(string &buffer)=0;
	virtual void loadBuffer(string &buffer,unsigned int minPos,unsigned int maxPos)=0;
};

class NonExistantResource {};
class InvalidRequestMethod {};
class ForbidenResource {
public:
	virtual ~ForbidenResource();
};
class AuthorizationRequired : public ForbidenResource {};

#endif
