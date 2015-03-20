#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <externData.h>
#include <Utils.h>
#include <BruteUtils.h>
#include "STh.h"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#include <iphlpapi.h>
#include <icmpapi.h>
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib,"curllib.lib")
#endif
#include <mainResources.h>

class Connector {

public:
    static int nConnect(const char* ip, const int port, std::string *buffer,
                        const char *postData = NULL,
                        const std::vector<std::string> *customHeaders = NULL,
                        const string *lpString = NULL);
    static int _ConnectToPort(string ip, int port, char *hl);
    static int _SSHLobby(std::string ip, int port, std::string *buffer);
};
#endif // CONNECTOR_H
