#include "STh.h"
#include "mainResources.h"
#include "externData.h"
#include "externFunctions.h"

bool __savingBackUpFile = false;

int gThreadDelay = 10;
int gC = 0;
int gTimeOut = 3;
int PieAnomC1 = 0, PieBA = 0, PieSusp = 0, PieLowl = 0, PieWF = 0, PieSSH = 0;
int AnomC1 = 0, Filt = 0, Overl = 0, Lowl = 0, Alive = 0, saved = 0, Susp = 0, WF = 0, offlines = 0, ssh = 0;
int GlobalNegativeSize = 0;
int found = 0, indexIP = 0;
int gMode;
int MaxPass = 0, MaxLogin = 0, MaxTags = 0, MaxWFLogin = 0, MaxWFPass = 0, MaxSSHPass = 0;
int ipsstart[4], ipsend[4],
overallPorts,
octet[4];
int baCount = 0;
int gPingTimeout = 1;
int gMaxBrutingThreads = 50;
unsigned int Activity = 0;

double ips = 0;
char **GlobalNegatives = 0;
char **loginLst, **passLst;
char **wfLoginLst, **wfPassLst;
char **sshlpLst;
char saveEndIP[128] = { 0 };
char gTLD[128] = { 0 };
char gPorts[65536] = { 0 };

char currentIP[MAX_ADDR_LEN] = { 0 };
char finalIP[32] = { 0 };

bool ErrLogFirstTime = true;
bool gPingNScan = false;
volatile int gThreads;
volatile int cons = 0;
volatile int BrutingThrds = 0;
volatile int threads = 20;

unsigned char tl(unsigned char d)
{
	if (d >= 192 && d <= 223)
	{
		return (unsigned char)(d + 32);
	}
	else
	{
		return tolower(d);
	};
}

std::string toLowerStr(const char *str)
{
	if (str != NULL) {
		int tsz = strlen(str);
		char *strr = new char[tsz + 1];
		ZeroMemory(strr, tsz);

		for (int i = 0; i < tsz; i++)
		{
			strr[i] = tl(str[i]);
		};

		memset(strr + tsz, '\0', 1);

		std::string tstr = std::string(strr);
		delete[]strr;
		return tstr;
	}
	else return "";
}

QString GetNSErrorDefinition(const char *str, const char *elem)
{
	const char *temp = strstr(str, elem);

	if (temp != NULL)
	{
		char definition[128] = { 0 };
		const char *firstComma = strstr(temp + strlen(elem) + 1, "\"");
		const char *lastComma = strstr(firstComma + 1, "\"");

		int sz = lastComma - firstComma - 1;

		strncpy(definition, firstComma + 1, (sz < 128 ? sz : 128));

		return QString(definition);
	}
	else return QString("No definition found!");
}

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
unsigned char* ASCIItoUNICODE(unsigned char ch)
{
	unsigned char Val[2];
	if ((ch < 192) && (ch != 168) && (ch != 184))  { Val[0] = 0; Val[1] = ch;    return Val; }
	if (ch == 168) { Val[0] = 208;   Val[1] = 129;   return Val; }
	if (ch == 184) { Val[0] = 209;   Val[1] = 145;   return Val; }
	if (ch < 240)  { Val[0] = 208;   Val[1] = ch - 48; return Val; }
	if (ch < 249)  { Val[0] = 209;   Val[1] = ch - 112;        return Val; }
}
#endif

std::string xcode(LPCSTR src, UINT srcCodePage, UINT dstCodePage) {
	std::string res;
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	int wsize = MultiByteToWideChar(srcCodePage, 0, src, -1, 0, 0);
	LPWSTR wbuf = (LPWSTR)new char[wsize * sizeof(WCHAR)];
	MultiByteToWideChar(srcCodePage, 0, src, -1, wbuf, wsize);
	int size = WideCharToMultiByte(dstCodePage, 0, wbuf, -1, 0, 0, 0, 0);
	char * buf = (char *)new char[size];
	WideCharToMultiByte(dstCodePage, 0, wbuf, -1, buf, size, 0, 0);
	delete wbuf;

	res.append(buf);
	delete buf;
#else
	unsigned int size = 0;
	while (src[size++]!=0);
	char * buf = (char *)new char[size];
	unsigned char uni[16] = {0};

	size=0;
	while (src[size]!=0)
	{

	};
	delete buf;
#endif
	return res;
}
