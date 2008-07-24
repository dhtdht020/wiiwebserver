#pragma once
#include "resource.h"
#include <map>
using std::map;

class InternalFile : public Resource {
public:
	InternalFile(const std::string &name);
	virtual ~InternalFile(void);

	void setAllowHeader(Request&);
	void loadBuffer(string &outBuffer);
	void loadBuffer(string &outBuffer,unsigned int minPos,unsigned int maxPos);
private:
	static map<string,string> files;
	string name;
	string contents;
};
