#ifndef BASICAUTH_H
#define BASICAUTH_H

#include <Connector.h>
#include <BruteUtils.h>
#include <Utils.h>
#include <externData.h>
#include <mainResources.h>

class BA : BruteUtils{
private:
    static bool checkOutput(const string *buffer);
    static lopaStr _BABrute(const char *ip, const int port);

public:
    static lopaStr _BALobby(const char *ip, const int port);
};

#endif // BASICAUTH_H
