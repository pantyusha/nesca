#pragma once
#include "base64.h"
#include <libssh/libssh.h>
#include <curl/curl.h>
#include <vector>
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#include "iostream"
#include <time.h>
#include <process.h>
#include <conio.h>
#include <direct.h>

#define GetCurrentDir _getcwd
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
#include <unistd.h>

#define GetCurrentDir getcwd
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

#define TITLE_MAX_SIZE 512
#define COOKIE_MAX_SIZE 1024
#define  TYPE1  "camera"
#define  TYPE2  "other"
#define  TYPE3  "auth"
#define  TYPE4  "ftp"
#define  TYPE5  "ssh"

#define PWD_LIST_FOLDER "./pwd_lists/"
#define LOGIN_FN PWD_LIST_FOLDER"login.txt"
#define PASS_FN PWD_LIST_FOLDER"pass.txt"
#define FTP_LOGIN_FN PWD_LIST_FOLDER"ftplogin.txt"
#define FTP_PASS_FN PWD_LIST_FOLDER"ftppass.txt"
#define WF_LOGIN_FN PWD_LIST_FOLDER"wflogin.txt"
#define WF_PASS_FN PWD_LIST_FOLDER"wfpass.txt"
#define SSH_PASS_FN PWD_LIST_FOLDER"sshpass.txt"
#define NEGATIVE_FN PWD_LIST_FOLDER"negatives.txt"

#define HTTP_FILE_STYLE "<style> #recvSpan{display: inline-block;width: 150px;} \
	#hostSpan{display: inline-block;width: 200px;} \
	body { background-color: #141414; font-family: monospace; font-size:95%;} \
	#ipd{background:black;width:100%;white-space:nowrap;overflow-x:none;display:inline-block;} \
	#ipd:hover{color: #909090;background:#202020;} \
	#tit{text-align:center;border:1px solid #5d5d5d;} \
	a{color: gray;text-decoration: underline;} \
	a:focus{ outline-style: dashed;outline-width:1px; outline-color: red;}</style>"

#define HTTP_FILE_HEADER "<div id=\"tit\"><a href=\"" TYPE1 ".html\">." TYPE1 "</a> \
	<a href=\"" TYPE2 ".html\">." TYPE2 "</a> \
	<a href=\"" TYPE3 ".html\">." TYPE3 "</a> \
	<a href=\"" TYPE4 ".html\">." TYPE4 "</a> \
	<a href=\"" TYPE5 ".html\">." TYPE5 "</a> \
	</div><br><br>"

#ifndef MAX_ADDR_LEN
#define MAX_ADDR_LEN 128
#endif

struct NET_DVR_DEVICEINFO_V30
{
	unsigned char sSerialNumber; //序列号
	unsigned char byAlarmInPortNum; //报警输入个数
	unsigned char byAlarmOutPortNum; //报警输出个数
	unsigned char byDiskNum; //硬盘个数
	unsigned char byDVRType; //设备类型, 1:DVR 2:ATM DVR 3:DVS ......
	unsigned char byChanNum; //模拟通道个数
	unsigned char byStartChan; //起始通道号,例如DVS-1,DVR - 1
	unsigned char byAudioChanNum; //语音通道数
	unsigned char byIPChanNum; //最大数字通道个数
	unsigned char byZeroChanNum; //零通道编码个数 //2010-01-16
	unsigned char byMainProto; //主码流传输协议类型 0-private, 1-rtsp
	unsigned char bySubProto; //子码流传输协议类型0-private, 1-rtsp
	unsigned char bySupport; //能力，位与结果为0表示不支持，1表示支持，
	unsigned char bySupport1; // 能力集扩充，位与结果为0表示不支持，1表示支持
	unsigned char byRes1;
	int wDevType; //设备型号

	unsigned char byRes2; //保留
};

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

	/*int getHeader(char *ip,
        int port,
        const char str[],
        Lexems *l,
        PathStr *ps,
        std::vector<std::string> *lst, int size);*/

	int filler(char* ip, 
		int port,
		std::string *buffcpy,
        int size,
        Lexems *lx);
};

//Hikvision SDK extern functions
//typedef int(__stdcall *f_func)();
typedef void(__stdcall *NET_DVR_Init)();
typedef void(__stdcall *NET_DVR_Cleanup)();
typedef int(__stdcall *NET_DVR_Login_V30)(const char * sDVRIP,
	int wDVRPort,
	const char * sUserName,
	const char * sPassword,
	NET_DVR_DEVICEINFO_V30 *lpDeviceInfo);

extern NET_DVR_Init hik_init_ptr;
extern NET_DVR_Cleanup hik_cleanup_ptr;
extern NET_DVR_Login_V30 hik_login_ptr;


