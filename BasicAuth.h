#ifndef BASICAUTH_H
#define BASICAUTH_H

#include "Connector.h"
#include "BruteUtils.h"
#include "Utils.h"
#include "externData.h"
#include "mainResources.h"

class BA {
private:
	static bool checkOutput(const string *buffer, const char *ip, const int port);
    static lopaStr BABrute(const char *ip, const int port);

public:
    static lopaStr BALobby(const char *ip, const int port);
};

#endif // BASICAUTH_H
