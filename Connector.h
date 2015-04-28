#ifndef CONNECTOR_H
#define CONNECTOR_H

#include "STh.h"
#include "BruteUtils.h"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#include <iphlpapi.h>
#include <icmpapi.h>
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib,"curllib.lib")
#endif
class Connector {

public:
    int nConnect(const char* ip, const int port, std::string *buffer,
                        const char *postData = NULL,
                        const std::vector<std::string> *customHeaders = NULL,
						const std::string *lpString = NULL,
						bool digestMode = false);
	//static int nConnect2(const char* ip, const int port, std::string *buffer,
	//	const char *postData = NULL,
	//	const std::vector<std::string> *customHeaders = NULL,
	//	const std::string *lpString = NULL,
	//	bool digestMode = false);
    int connectToPort(char *ip, int port);
};
#endif // CONNECTOR_H
