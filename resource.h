//#if defined(WIN32)
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
#define RECV_MAX_SIZE 350000
#define REQUEST_MAX_SIZE 4096
#define COOKIE_MAX_SIZE 1024
#define SD_BOTH 2
#define PORTSET "80,81,88,8080,8081,60001,60002,8008,8888,554,9000,441,4111,6667,3536,22,21"
#define IRC_CHAN "iskopasi_lab03"

using namespace std;

extern bool gGlobalTrackLocked;

extern SOCKET lSock;
extern char gVER[16];
extern QVector<int> vAlivLst;
extern QVector<int> vAnomLst;
extern QVector<int> vWFLst;
extern QVector<int> vSuspLst;
extern QVector<int> vLowlLst;
extern QVector<int> vBALst;
extern QVector<int> vSSHLst;
extern QVector<int> vOvrlLst;
extern QVector<QPointF> vect;
extern bool printDelimiter;
extern QJsonArray *jsonArr;
extern bool gPingNScan;
extern bool smBit_1;
extern bool smBit_2;
extern bool smBit_3;
extern bool smBit_4;
extern bool smBit_5;
extern bool smBit_6;
extern bool smBit_7;
extern bool smBit_8;
extern bool horLineFlag;
extern bool gDebugMode;
extern bool gNegDebugMode;
extern bool HTMLDebugMode;
extern bool utfIRCFlag;
extern QVector<int> actLst;
extern char inputStr[256];
extern bool proxyEnabledFlag;
extern int nickFlag;
extern int offlineFlag;
extern bool OnlineMsgSentFlag;
extern int globalPinger;
extern int gPingTimeout;
extern bool destroychPThFlag;
extern string toLowerStr(const char *str);
extern QList<int> lstOfLabels;
extern bool ME2ScanFlag, QoSScanFlag, VoiceScanFlag, PieStatFlag;
extern int AnomC1, Filt, Overl, Lowl, Alive, saved, Susp, WF, offlines, ssh;
extern int PieAnomC1, PieSusp, PieBA, PieLowl, PieWF, PieSSH;
extern int gThreadDelay;
extern bool connectedToIRC;
extern bool globalScanFlag;
extern float QoSStep;
extern int MaxDataVal;
extern int tMax;
extern bool widgetIsHidden;
extern bool MapWidgetOpened;
extern int gTimeOut;
extern char endIP2[128];
extern char metaIPDNS[256];
extern char metaRange[256];
extern char metaPercent[256];
extern char metaIPS[256];
extern char metaTargets[256];
extern char metaETA[256];
extern char metaOffline[256];
extern int GlobalWSAErr;
extern bool globalScanFlag;
extern bool trackerOK;
extern char trcPort[32];
extern char trcSrvPortLine[32];
extern char trcSrv[256];
extern char trcScr[256];
extern char trcProxy[128];
extern char trcPersKey[32];
extern char ircServer[32];
extern char ircPort[32];
extern char ircProxy[64];
extern char ircProxyPort[8];
extern char ircNick[32];
extern int stopGlobalLog;
extern int GlobalNegativeSize;
extern char* thrds, top_level_domain[128];
extern char startM[64], endM[64];

extern char **GlobalNegatives;
extern char **loginLst, **passLst, **wfLoginLst, **wfPassLst, **sshlpLst;
extern int MaxPass, MaxLogin, MaxWFLogin, MaxWFPass, MaxSSHPass;
extern double ips;
extern int ovrlIPs, ipCounter;
extern int mode;
extern unsigned long long gTargets, gTargetsOverall, targets, Activity;
extern volatile int BA;
extern volatile int cons;
extern volatile int BrutingThrds;
extern volatile int gThreads;
extern volatile int threads;
extern int found, fillerFlag, indexIP;
extern int gMaxSize;
extern char saveStartIP[128];
extern char saveEndIP[128];
extern int gMode;
extern char gRange[128];
extern char gFirstDom[128];
extern char gPorts[65536];
extern int gMaxBrutingThreads;
extern int OnLiner;
extern bool __savingBackUpFile;
extern unsigned int importFileSize;
extern int ipsstart[4], ipsend[4], 
	startNum, endNum, overallPorts, flCounter, octet[4];
//extern unsigned char ipsstartfl[8192][4], ipsendfl[8192][4], starterIP[8192][4];
extern unsigned char **ipsstartfl, **ipsendfl, **starterIP;

typedef struct ST{ 
    char argv[MAX_ADDR_LEN];
}sockstruct;
struct conSTR{
	char *lowerBuff;
	int size;
};
struct assClSt{
	const char *argv2;
};
struct PathStr{
	char codepage[32];
	char headr[TITLE_MAX_SIZE];
	char path[1024];
	int flag;
	int port;
	char ip[MAX_ADDR_LEN];
	char cookie[COOKIE_MAX_SIZE];
};
struct pl{
	int loginCounter;
	int passCounter;
};
struct lopaStr{
	char login[128];
	char pass[32];
	char other[128];
};
extern int recvS(int lSock, char *recvBuffT, int len, int mode);
extern int sendS(int lSock, char *msg, int len, int mode);
extern std::string xcode(LPCSTR src, UINT srcCodePage, UINT dstCodePage);
extern void UserNickInit(SOCKET lSock);
extern void GetNicks();
extern int startScan(char* argv);
extern volatile bool BConnLocked;
extern void BConInc();
extern void BConDec();
extern QString GetNSErrorDefinition(char *str, char *defin);
extern void _SaveBackupToFile();
extern char* __cdecl strstri(char *_Str, const char *_SubStr);
extern char* _getAttribute(char *str, char *attrib);
extern char *_findFirstOcc(char *str, char *delim);
extern int _SSHLobby(char *ip, int port, conSTR *CSTR);
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
		void _StartRangeFapping(int ipsstart[], int ipsend[], int &cons, char *argv2, ST *st);
		void _Connect(void *s);
		int _ConnectToPort(char *ip, const char *port, char *hl);
	};

