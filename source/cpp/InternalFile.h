#pragma once
#include "resource.h"
#include <map>
using std::map;

class InternalFile : public Resource {
public:
	InternalFile(const std::string &name);
	virtual ~InternalFile(void);
private:
	static map<string,string> files;
	string name;
};
