#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <mainResources.h>
#include <externData.h>
#include <STh.h>

class Connector {

public:
    static int nConnect(const char *ip, const int port, std::string *buffer,
                               const char *postData = NULL,
                               const std::vector<std::string> *customHeaders = NULL);
    static int _ConnectToPort(char *ip, int port, char *hl);
    static int _SSHLobby(char *ip, int port, std::string *buffer);
};
#endif // CONNECTOR_H
