#pragma once
#include "resource.h"
#include <map>
using std::map;

class InternalFile : public Resource {
public:
	virtual ~InternalFile(void);
	static InternalFile * load(string const &name) ;

	InternalFile(const InternalFile &);

	void setAllowHeader(Request&);
	void loadBuffer(string &outBuffer);
	void loadBuffer(string &outBuffer,unsigned int minPos,unsigned int maxPos);

	static InternalFile epicFail;
private:
	InternalFile(const string &content,const string &mimeType);
	static map<string,InternalFile> files;
	//string name;
	string contents;
	friend void loadInternalFiles();
};
