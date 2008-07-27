#include "InternalFile.h"

InternalFile InternalFile::epicFail(
	"<h1>404 - Error page not found</h1><p>Who ever runs this server managed to screw up the config so badly that not even loading the error page worked.</p>",
	"text/html"
);

void loadInternalFiles() {
	InternalFile::files.insert(
		std::make_pair(
		"errors/404",
			InternalFile(
				"<h1>404 Not found</h1><p>The requested resource was not found. Either you are looking for something that doesn't exist, or your spelling sucks.</p>",
				"text/html"
			)
		)
	);
};
