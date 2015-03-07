#include <Utils.h>

char *getSystemProxy() {
	return "";
}

int Utils::getProxyPort() {
	return 0;
}

char * Utils::getProxy() {
    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#else
    getSystemProxy();
#endif
	return "";
}
