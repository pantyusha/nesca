#include <Utils.h>

char *getSystemProxy() {

}

int Utils::getProxyPort() {
}

char * Utils::getProxy() {
    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#else
    getSystemProxy();
#endif
}
