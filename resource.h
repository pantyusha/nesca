#pragma once
#include "base64.h"

#if defined(Q_OS_WIN32)
#pragma once 
#include "iostream"
#include <time.h>
#include <process.h>
#include <conio.h>
#include <openssl/ssl.h>
#endif
#if defined(Q_OS_LINUX)
#pragma once
#include <string>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>  
#include <arpa/inet.h>
#include <pthread.h>
#include <netdb.h>
#include <errno.h>
#include <fcntl.h>
#include <math.h>
#include <cstring>

#define ZeroMemory(Destination,Length) memset((Destination),0,(Length))
#define Sleep(secs) usleep((secs)*1000)
#define WSAGetLastError() errno
#define closesocket(sock) ::close((sock))

typedef unsigned int		UINT;
typedef const char *		LPCSTR;
typedef int					SOCKET;
typedef				hostent HOSTENT;
typedef struct		linger	LINGER;
typedef int					BOOL;
#define INVALID_SOCKET  (SOCKET)(~0)
#define SOCKET_ERROR            (-1)
#define SD_BOTH         0x02
#define FAR                 far
#endif

#define MAX_ADDR_LEN 128
#define TITLE_MAX_SIZE 512
#define COOKIE_MAX_SIZE 1024

struct PathStr{
	char codepage[32];
	char headr[TITLE_MAX_SIZE];
	char path[1024];
	int flag;
	int port;
	char ip[MAX_ADDR_LEN];
	char cookie[COOKIE_MAX_SIZE];
};

struct lopaStr{
	char login[128];
	char pass[32];
	char other[128];
};

struct conSTR{
	char *lowerBuff;
	int size;
};

class Lexems
	{
	public:
		int iterationCount, flag;

		Lexems()
		{
			iterationCount = 0;
			flag = 0;
		};
		~Lexems()
		{
			iterationCount = 0;
		};
		
		int _header(char *ip, int port, char str[],  Lexems *l, PathStr *ps, std::vector<std::string> *lst, char *rBuff);
		int _filler(int p, char* buffcpy, char* ipi, int recd, Lexems *lx, char *hl);
		int globalSearchNeg(const char *buffcpy, char *ip, int port);
	};


class Connector
	{
	public:
		int _Updater();
		
		lopaStr _FTPLobby(char *ip, int port, PathStr *ps);
		lopaStr _BALobby(char *cookie, char *ip, int port, char *path, char *method, char *data);
		lopaStr _WFLobby(char *cookie, char *ip, int port, char *methodVal, char *actionVal, char *userVal, char *passVal, char *formVal);
		lopaStr _IPCameraBLobby(char *ip, int port, char *SPEC);
		
		int _EstablishConnection(char *ip, int port, char *request, conSTR *cstr, int force = 0);
		int _EstablishSSLConnection(char *ip, int port, char *request, conSTR *cstr);
		//void _StartRangeFapping(int ipsstart[], int ipsend[], int &cons, char *argv2, ST *st);
		void _Connect(void *s);
		int _ConnectToPort(char *ip, const char *port, char *hl);
	};

