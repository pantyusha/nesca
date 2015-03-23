#ifndef SSHAUTH_H
#define SSHAUTH_H

#include "Utils.h"
#include "Connector.h"
#include "externData.h"
#include "mainResources.h"

class SSHAuth {
public:
    static int SSHLobby(const char *ip,
                            const int port,
                            std::string *buffer);
};
#endif // SSHAUTH_H
