#define RECV_MAX_SIZE 350000
#define REQUEST_MAX_SIZE 4096
#define SD_BOTH 2
#define PORTSET "80,81,88,8080,8081,60001,60002,8008,8888,554,9000,441,4111,6667,3536,22,21"
#define IRC_CHAN "iskopasi_lab03"

extern QJsonArray *jsonArr;

extern unsigned long long gTargetsOverall, Activity;
extern char top_level_domain[128];
extern volatile int BA, cons, BrutingThrds, gThreads;
extern char **loginLst, **passLst, **wfLoginLst, **wfPassLst, **sshlpLst, **GlobalNegatives;
extern bool trackerOK, __savingBackUpFile, globalScanFlag, MapWidgetOpened,
	widgetIsHidden, OnlineMsgSentFlag, utfIRCFlag, HTMLDebugMode, gNegDebugMode, 
	gDebugMode, horLineFlag, gPingNScan;
extern int found, indexIP, gMode, 
	MaxPass, MaxLogin, MaxWFLogin, MaxWFPass, MaxSSHPass,
	GlobalNegativeSize, isActive, gMaxBrutingThreads,
	gTimeOut, PieAnomC1, PieSusp, PieBA, PieLowl, PieWF, PieSSH, 
	gThreadDelay, AnomC1, Filt, Overl, Lowl, Alive, saved, 
	Susp, WF, offlines, ssh, globalPinger, gPingTimeout, nickFlag, offlineFlag;
extern char trcSrv[256], trcScr[256], trcProxy[128], trcPersKey[32], 
	ircServer[32], ircPort[32], ircProxy[64], ircProxyPort[8], ircNick[32],
	trcPort[32], trcSrvPortLine[32], saveStartIP[128], saveEndIP[128],
	gRange[128], gFirstDom[128], gPorts[65536], endIP2[128], 
	gVER[16];

struct pl{
	int loginCounter;
	int passCounter;
};
