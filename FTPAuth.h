#ifndef FTPAUTH_H
#define FTPAUTH_H

#include <Connector.h>
#include <BruteUtils.h>
#include <Utils.h>
#include <externData.h>
#include <mainResources.h>

class FTPA {
    private:
    static bool checkOutput(const string *buffer);
    static lopaStr _FTPBrute(const char *ip, const int port, const PathStr *ps);

    public:
    static lopaStr _FTPLobby(const char *ip, const int port, const PathStr *ps);
};

#endif // FTPAUTH_H
