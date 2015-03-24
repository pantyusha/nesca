#ifndef FTPAUTH_H
#define FTPAUTH_H

#include "Utils.h"
#include "Connector.h"
#include "externData.h"
#include "mainResources.h"

class FTPA {
    private:
    static bool checkOutput(const string *buffer);
    static lopaStr FTPBrute(const char *ip, const int port, PathStr *ps);

    public:
    static lopaStr FTPLobby(const char *ip, const int port, PathStr *ps);
};

#endif // FTPAUTH_H
