#pragma once
#ifndef RESOURCE_H
#define RESOURCE_H

//class Resource;

#include "request.h"

class Request;

class Resource {
public:
	Resource(void);
	virtual ~Resource(void);
	static Resource *load(string name);

	virtual void setAllowHeader(Request &r)=0;
	virtual void loadBuffer(string &outBuffer)=0;
	virtual void loadBuffer(string &outBuffer,unsigned int minPos,unsigned int maxPos)=0;

	string mimeType;

private:
	Resource(const Resource &);
	Resource &operator=(const Resource &);
};

class NonExistantResource {};
class InvalidRequestMethod {};
class ForbiddenResource {
public:
	virtual ~ForbiddenResource();
};
class AuthorizationRequired : public ForbiddenResource {};

#endif
