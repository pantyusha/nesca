#ifndef BASICAUTH_H
#define BASICAUTH_H

#include <BruteUtils.h>
#include <mainResources.h>

class BA : BruteUtils{
private:
    lopaStr _BABrute(char *cookie,
                     char *ip,
                     int port,
                     char *pathT,
                     char *method);

    lopaStr _BALobby(char *cookie,
                     char *ip,
                     int port,
                     char *path,
                     char *method);
};

#endif // BASICAUTH_H
