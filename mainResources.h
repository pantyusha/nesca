#pragma once
#include "base64.h"

#include <libssh/libssh.h>
#include <openssl/ssl.h>
#include <vector>
#include <curl/curl.h>
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#include "iostream"
#include <time.h>
#include <process.h>
#include <conio.h>
#else
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
#define Sleep(msecs) usleep((msecs)*1000)
#define WSAGetLastError() errno
#define closesocket(sock) ::close((sock))

typedef unsigned int        DWORD;
typedef void*               LPVOID;
typedef void*               HANDLE;
typedef unsigned int		UINT;
typedef const char *		LPCSTR;
typedef int					SOCKET;
typedef	hostent             HOSTENT;
typedef struct linger       LINGER;
typedef int					BOOL;

#define SD_BOTH             0x02
#ifndef FAR
#define FAR                 far
#endif

#ifndef INVALID_SOCKET
#define INVALID_SOCKET      (SOCKET)(~0)
#endif
#ifndef SOCKET_ERROR
#define SOCKET_ERROR        (-1)
#endif
#ifndef WSAEWOULDBLOCK
#define WSAEWOULDBLOCK         EWOULDBLOCK          //10035
#endif
#ifndef WSAEINPROGRESS
#define WSAEINPROGRESS         EINPROGRESS          //10036
#endif
#ifndef WSAENOTSOCK
#define WSAENOTSOCK            ENOTSOCK             //10038
#endif
#ifndef WSAEADDRNOTAVAIL
#define WSAEADDRNOTAVAIL       EADDRNOTAVAIL        //10049
#endif
#ifndef WSAECONNRESET
#define WSAECONNRESET          ECONNRESET           //10054
#endif
#ifndef WSAENOBUFS
#define WSAENOBUFS             ENOBUFS              //10055
#endif
#ifndef WSAETIMEDOUT
#define WSAETIMEDOUT           ETIMEDOUT            //10060
#endif
#ifndef WSAECONNREFUSED
#define WSAECONNREFUSED        ECONNREFUSED         //10061
#endif

#endif

#define MAX_ADDR_LEN 128
#define TITLE_MAX_SIZE 512
#define COOKIE_MAX_SIZE 1024
#define RESULT_DIR_NAME "./result_files-" __DATE__

typedef struct {
    char argv[MAX_ADDR_LEN];
} ST;

struct PathStr{
	char codepage[32];
	char headr[TITLE_MAX_SIZE];
	char path[1024];
	int flag;
	int port;
	char ip[MAX_ADDR_LEN];
	char cookie[COOKIE_MAX_SIZE];
	int directoryCount;
};

struct lopaStr{
    char login[128];
    char pass[32];
	char other[128];
};

struct conSTR{
	char *lowerBuff;
	int size;
	bool overflow = false;
};

class Lexems
	{
	public:
		int iterationCount, flag;

		Lexems()
		{
			iterationCount = 0;
			flag = 0;
        }

		~Lexems()
		{
			iterationCount = 0;
        }
		
        int _header(char *ip,
                    int port,
                    const char str[],
                    Lexems *l,
                    PathStr *ps,
                    std::vector<std::string> *lst);

        int _filler(int p,
                    const char *buffcpy,
                    char* ipi,
                    int recd,
                    Lexems *lx,
                    char *hl);
	};


class Connector_old
	{
	public:
		int _Updater();
		
//        lopaStr _FTPLobby(char *ip,
//                          int port,
//                          PathStr *ps);

//        lopaStr _BALobby(char *cookie,
//                         char *ip,
//                         int port,
//                         char *path,
//                         char *method,
//                         char *data);

        lopaStr _WFLobby(char *cookie,
                         char *ip,
                         int port,
                         char *methodVal,
                         char *actionVal,
                         char *userVal,
                         char *passVal,
                         char *formVal);

        lopaStr _IPCameraBLobby(char *ip,
                                int port,
                                char *SPEC);

        int _SSHLobby(char *ip,
                      int port,
                      std::string *buffer);
		
        void _Connect(void *s);

        int _ConnectToPort(char *ip,
                           int port,
                           char *hl);
	};


extern lopaStr _IPCameraBLobby(char *ip, int port, char *SPEC);
//extern lopaStr _BALobby(char *cookie, char *ip, int port, char *path, char *method);
//extern lopaStr _FTPLobby(char *ip, int port, PathStr *ps);
