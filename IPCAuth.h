#ifndef IPCAUTH_H
#define IPCAUTH_H

#include "Connector.h"
#include "mainResources.h"

class IPC {
private:
    bool doPost;
    char postData[1024];
private:
    lopaStr IPCBrute(const char *ip, int port, char *SPEC, const std::string *cookie);

public:
    IPC() {
        doPost = false;
        //ZeroMemory(postData, 1024);
		postData[0] = 0;
    }

    lopaStr IPCLobby(const char *ip, int port, char *SPEC, const std::string *cookie);
};

#endif // IPCAUTH_H
