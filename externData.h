#ifndef EXTERNDATA_H
#define EXTERNDATA_H
#include <atomic>

#include <qjsonarray.h>
#define RECV_MAX_SIZE 350000
#define REQUEST_MAX_SIZE 4096
#define PORTSET "80,81,88,8080,8081,60001,60002,8008,8888,554,9000,3536,21"
#define CSSOCKET(Socket) shutdown(Socket, SD_BOTH); closesocket(Socket); Socket = -1;

#ifndef CP_UTF8
#define CP_UTF8 65001
#endif
#ifndef CP_ACP
#define CP_ACP 0
#endif

#ifndef MAX_ADDR_LEN
#define MAX_ADDR_LEN 128
#endif

extern QJsonArray *jsonArr;

extern unsigned long long gTargetsNumber;
extern long long unsigned int gTargets;
extern std::atomic<int> cons, BrutingThrds, gThreads;
extern char **loginLst, **passLst,
**wfLoginLst, **wfPassLst,
**ftpLoginLst, **ftpPassLst,
**sshlpLst;
extern bool trackerOK, globalScanFlag, MapWidgetOpened,
    widgetIsHidden, gNegDebugMode,
    gDebugMode, horLineFlag, gPingNScan, gShuffle,
	BALogSwitched;
extern int found, indexIP, gMode, 
	MaxPass, MaxLogin, 
	MaxWFLogin, MaxWFPass,
	MaxFTPLogin, MaxFTPPass,
	MaxSSHPass,
    gMaxBrutingThreads,
	gTimeOut, PieCamerasC1, PieOther, PieBA, PieSSH,
	camerasC1, filtered, Overl, Alive, saved,
    other,
	baCount,
    ssh, globalPinger, gPingTimeout,
	cIndex;
extern unsigned int Activity;
extern char trcSrv[256], trcScr[256], trcProxy[128], trcPersKey[64],
    trcPort[32], trcSrvPortLine[32],
    gTLD[128], gPorts[65536],
    gProxyIP[64], gProxyPort[8],
    currentIP[MAX_ADDR_LEN],
    finalIP[32];

extern char gVER[32];

#endif // EXTERNDATA
