#pragma once
#ifndef RTSPAUTH_H
#define RTSPAUTH_H

#include "Utils.h"
#include "Connector.h"
#include "externData.h"
#include "mainResources.h"

class RTSP
{
private:
	static lopaStr RTSPBrute(const char *ip, const int port);
public:
	static lopaStr RTSPLobby(const char *ip, const int port);
};

#endif // RTSPAUTH_H

