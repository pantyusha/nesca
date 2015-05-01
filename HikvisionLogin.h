#ifndef HIKVISIONLOGIN_H
#define HIKVISIONLOGIN_H

#include "STh.h"
#include "mainResources.h"

class HikVis {
public: static bool isInitialized;

private: lopaStr hikLogin(const char * sDVRIP, int wDVRPort);

public:
	void hikInit();
	void hikCleanup();
	lopaStr HVLobby(const char *ip, const int port);
};

#endif // UTILS_H