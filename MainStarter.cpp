#include "STh.h"
#include "MainStarter.h"
#include "Connector.h"
#include "Utils.h"
#include <qjsonobject.h>
#include <qjsonvalue.h>
#include <qjsonarray.h>
#include <qjsondocument.h>

int gTimeOut = 3;
int gPingTimeout = 1;
int gMode;
int PieAnomC1 = 0, PieBA = 0, PieSusp = 0, PieLowl = 0, PieWF = 0, PieSSH = 0;
int AnomC1 = 0, filtered = 0, Overl = 0, Lowl = 0, Alive = 0, saved = 0, Susp = 0, WF = 0, ssh = 0;
int found = 0, indexIP = 0;
int MaxPass = 0, MaxLogin = 0, MaxTags = 0, MaxWFLogin = 0, MaxWFPass = 0, MaxSSHPass = 0;
int baCount = 0;
int gMaxBrutingThreads = 50;
unsigned int Activity = 0;
char **loginLst, **passLst;
char **wfLoginLst, **wfPassLst;
char **sshlpLst;
char gTLD[128] = { 0 };
char gPorts[65536] = { 0 };
char currentIP[MAX_ADDR_LEN] = { 0 };
char finalIP[32] = { 0 };
bool gPingNScan = false;
std::atomic<int> cons = 0, BrutingThrds = 0, gThreads;
std::vector<int> MainStarter::portVector;
int MainStarter::flCounter = 0;
bool MainStarter::savingBackUpFile = false;
QJsonArray *jsonArr = new QJsonArray();
bool horLineFlag = false;
bool gShuffle = true;
int ipCounter = 0;
long long unsigned gTargets = 0, gTargetsNumber = 1;
char currentMask[128]	= { 0 };
char metaRange[256]		= { 0 };
char metaPercent[256]	= { 0 };
char metaIPS[256]		= { 0 };
char metaTargets[256]	= { 0 };
char metaETA[256]		= { 0 };
char metaOffline[256]	= { 0 };


void MainStarter::unBlockButtons(){
	stt->doEmitionBlockButton(false);
}
int MainStarter::fileLoader(const char *fileName) {

	char curIP[256] = { 0 }, curIPCopy[256] = { 0 };
	unsigned int importFileSize = 0;

	FILE *fl = fopen(fileName, "r");
	if (fl != NULL)
	{
		while (fgets((char*)curIP, sizeof(curIP), fl) != NULL)
		{
			if (curIP[0] != '#' && curIP[0] != ' ' && curIP[0] != '\n' && curIP[0] != '\r' && strcmp(curIP, "") != 0 &&
				((curIP[0] == '/' && curIP[1] == '/') == false) && ((curIP[0] == '\t' && curIP[1] == '\t' && curIP[2] == '\t' && (curIP[3] == 13 || curIP[3] == 10 || curIP[3] == '#')) == false)
				&& (curIP[0] == '\t' && curIP[1] == '\t' && curIP[2] == '\t' && (curIP[3] == '/' && curIP[4] == '/')) == false
				) ++importFileSize;
			ZeroMemory(curIP, sizeof(curIP));
		};

		if (importFileSize == 0) {
			fclose(fl);
			return -1;
		}

		ipsstartfl = new unsigned int*[importFileSize + 1];
		ipsendfl = new unsigned int*[importFileSize + 1];
		ZeroMemory(ipsstartfl, sizeof(ipsstartfl));
		ZeroMemory(ipsendfl, sizeof(ipsendfl));

		for (int i = 0; i < importFileSize; ++i)
		{
			ipsstartfl[i] = new unsigned int[4];
			ipsendfl[i] = new unsigned int[4];

			ZeroMemory(ipsstartfl[i], sizeof(ipsstartfl[i]));
			ZeroMemory(ipsendfl[i], sizeof(ipsendfl[i]));
		};

		rewind(fl);

		while (fgets(curIP, 256, fl) != NULL)
		{
			if (curIP[0] != '#' && curIP[0] != ' ' && curIP[0] != '\n' && curIP[0] != '\r' && strcmp(curIP, "") != 0 &&
				((curIP[0] == '/' && curIP[1] == '/') == false) && ((curIP[0] == '\t' && curIP[1] == '\t' && curIP[2] == '\t' && (curIP[3] == 13 || curIP[3] == 10 || curIP[3] == '#')) == false)
				&& (curIP[0] == '\t' && curIP[1] == '\t' && curIP[2] == '\t' && (curIP[3] == '/' && curIP[4] == '/')) == false)
			{
				strcpy(curIPCopy, curIP);
				char *ptr1 = strstr(curIP, " ");
				if (ptr1 != NULL) curIP[(int)(ptr1 - curIP)] = '\0';
				ptr1 = strstr(curIP, "	");
				if (ptr1 != NULL) curIP[(int)(ptr1 - curIP) - 1] = '\0';
				ptr1 = strstr(curIP, "#");
				if (ptr1 != NULL) curIP[(int)(ptr1 - curIP) - 1] = '\0';
				if (strcmp(curIP, "") == 0 || strcmp(curIP, " ") == 0 || strcmp(curIP, "\r\n") == 0 || strcmp(curIP, "\n") == 0 || curIP[0] == ' ' || curIP[0] == '#')
				{
					ZeroMemory(curIPCopy, sizeof(curIPCopy));
					ZeroMemory(curIP, sizeof(curIP));
					continue;
				};

				if (strstr(curIP, "-") != NULL)
				{
					std::vector<std::string> tmpIPVec = Utils::splitToStrVector(curIP, '-');
					std::vector<int> tmpIPIntVec1 = Utils::splitToIntVector(tmpIPVec[0], '.');
					std::vector<int> tmpIPIntVec2 = Utils::splitToIntVector(tmpIPVec[1], '.');

					ipsstartfl[MainStarter::flCounter][0] = tmpIPIntVec1[0];
					ipsstartfl[MainStarter::flCounter][1] = tmpIPIntVec1[1];
					ipsstartfl[MainStarter::flCounter][2] = tmpIPIntVec1[2];
					ipsstartfl[MainStarter::flCounter][3] = tmpIPIntVec1[3];

					ipsendfl[MainStarter::flCounter][0] = tmpIPIntVec2[0];
					ipsendfl[MainStarter::flCounter][1] = tmpIPIntVec2[1];
					ipsendfl[MainStarter::flCounter][2] = tmpIPIntVec2[2];
					ipsendfl[MainStarter::flCounter][3] = tmpIPIntVec2[3];

					if (ipsstartfl[MainStarter::flCounter][0] > ipsendfl[MainStarter::flCounter][0]
						|| (ipsstartfl[MainStarter::flCounter][0] >= ipsendfl[MainStarter::flCounter][0]
						&& ipsstartfl[MainStarter::flCounter][1] > ipsendfl[MainStarter::flCounter][1]
						)
						|| (ipsstartfl[MainStarter::flCounter][0] >= ipsendfl[MainStarter::flCounter][0]
						&& ipsstartfl[MainStarter::flCounter][1] >= ipsendfl[MainStarter::flCounter][1]
						&& ipsstartfl[MainStarter::flCounter][2] > ipsendfl[MainStarter::flCounter][2]
						)
						|| (ipsstartfl[MainStarter::flCounter][0] >= ipsendfl[MainStarter::flCounter][0]
						&& ipsstartfl[MainStarter::flCounter][1] >= ipsendfl[MainStarter::flCounter][1]
						&& ipsstartfl[MainStarter::flCounter][2] >= ipsendfl[MainStarter::flCounter][2]
						&& ipsstartfl[MainStarter::flCounter][3] > ipsendfl[MainStarter::flCounter][3]
						)
						)
					{
						stt->doEmitionRedFoundData("[IP Loader]Wrong list format. Line-> [" + 
							QString::number(MainStarter::flCounter) + 
							"] String-> [" +
							QString(curIPCopy) + "]");
						return -1;
					};

					unsigned long ip1 = (ipsstartfl[MainStarter::flCounter][0] * 16777216) +
						(ipsstartfl[MainStarter::flCounter][1] * 65536) +
						(ipsstartfl[MainStarter::flCounter][2] * 256) +
						ipsstartfl[MainStarter::flCounter][3];
					unsigned long ip2 = (ipsendfl[MainStarter::flCounter][0] * 16777216) +
						(ipsendfl[MainStarter::flCounter][1] * 65536) +
						(ipsendfl[MainStarter::flCounter][2] * 256) +
						ipsendfl[MainStarter::flCounter][3];

					if (ip1 > ip2) {
						stt->doEmitionRedFoundData("Malformed input: check your range (" + 
							QString(curIP) + ")");
					}

					gTargets += ip2 - ip1 + 1;
					++MainStarter::flCounter;
				}
				else if (strstr(curIP, "/") != NULL)
				{
					if (strlen(curIP) > 18) {
						stt->doEmitionRedFoundData("[IP Loader]Wrong list format. Line-> [" +
							QString::number(MainStarter::flCounter) +
							"] String-> [" +
							QString(curIPCopy) +
							"]");
						continue;
					}
					unsigned int ip[4] = { 0 }, ip_min[4] = { 0 }, ip_max[4] = { 0 }, tmp1, tmp2;
					unsigned int netmask = atoi(strstr(curIP, "/") + 1);

					std::vector<int> tmpIPVec = Utils::splitToIntVector(curIP, '.');

					for (int i = 0; i < tmpIPVec.size(); ++i) ip[i] = tmpIPVec[i];

					/*Set the bytes which won't be changed*/
					for (tmp1 = tmp2 = netmask / 8; tmp1>0; tmp1--){
						ip_min[tmp1 - 1] = ip[tmp1 - 1];
						ip_max[tmp1 - 1] = ip[tmp1 - 1];
					}
					/*Set the bytes which should be 0ed or 255ed.*/
					for (tmp1 = tmp2, ++tmp2; tmp2< 4; tmp2++){
						ip_min[tmp2] = 0;
						ip_max[tmp2] = 255;
					}
					/* Finally set the one which has to be shifted.*/
					if (tmp1 < 4){
						tmp2 = 8 - netmask % 8;
						ip_min[tmp1] = ip[tmp1] >> tmp2;
						ip_min[tmp1] <<= tmp2;
						ip_max[tmp1] = ip_min[tmp1] + pow(2, tmp2) - 1;
					}
					
					ipsstartfl[MainStarter::flCounter][0] = ip_min[0];
					ipsstartfl[MainStarter::flCounter][1] = ip_min[1];
					ipsstartfl[MainStarter::flCounter][2] = ip_min[2];
					ipsstartfl[MainStarter::flCounter][3] = ip_min[3];

					ipsendfl[MainStarter::flCounter][0] = ip_max[0];
					ipsendfl[MainStarter::flCounter][1] = ip_max[1];
					ipsendfl[MainStarter::flCounter][2] = ip_max[2];
					ipsendfl[MainStarter::flCounter][3] = ip_max[3];

					unsigned long ip1 = (ip_min[0] * 16777216) +
						(ip_min[1] * 65536) +
						(ip_min[2] * 256) +
						ip_min[3];
					unsigned long ip2 = (ip_max[0] * 16777216) +
						(ip_max[1] * 65536) +
						(ip_max[2] * 256) +
						ip_max[3];

					if (ip1 > ip2) {
						stt->doEmitionRedFoundData("Malformed input: check your range (" +
							QString(curIP) + ")");
					}

					gTargets += ip2 - ip1 + 1;
					++MainStarter::flCounter;
				}
				else if (strstr(curIP, "RESTORE_IMPORT_SESSION") != NULL)
				{
					///DUMMY///
				}
				else
				{
					stt->doEmitionRedFoundData("[IP Loader]Wrong list format. Line-> [" + 
						QString::number(MainStarter::flCounter) + 
						"] String-> [" + 
						QString(curIPCopy) + 
						"]");
					return -1;
				};
				ZeroMemory(curIP, sizeof(curIP));
			};
		};
		gTargetsNumber = gTargets;

		stt->doEmitionYellowFoundData("List loader - [OK] (" + QString::number(gTargetsNumber + 1) + " hosts)");
		fclose(fl);
	}
	else stt->doEmitionRedFoundData("[IP Loader] Cannot open IP list.");
}
int MainStarter::loadTargets(const char *data) {

	if (gMode == 0) {
		std::vector<std::string> rangeVec;

		if (strstr(data, "/") != NULL) {

			unsigned int ip[4] = { 0 }, ip_min[4] = { 0 }, ip_max[4] = { 0 }, tmp1, tmp2;
			unsigned int netmask = atoi(strstr(data, "/") + 1);

			std::vector<int> tmpIPVec = Utils::splitToIntVector(data, '.');

			for (int i = 0; i < tmpIPVec.size(); ++i) ip[i] = tmpIPVec[i];

			/*Set the bytes which won't be changed*/
			for (tmp1 = tmp2 = netmask / 8; tmp1>0; tmp1--){
				ip_min[tmp1 - 1] = ip[tmp1 - 1];
				ip_max[tmp1 - 1] = ip[tmp1 - 1];
			}
			/*Set the bytes which should be 0ed or 255ed.*/
			for (tmp1 = tmp2, ++tmp2; tmp2< 4; tmp2++){
				ip_min[tmp2] = 0;
				ip_max[tmp2] = 255;
			}
			/* Finally set the one which has to be shifted.*/
			if (tmp1 < 4){
				tmp2 = 8 - netmask % 8;
				ip_min[tmp1] = ip[tmp1] >> tmp2;
				ip_min[tmp1] <<= tmp2;
				ip_max[tmp1] = ip_min[tmp1] + pow(2, tmp2) - 1;
			}

			char newRangeString[128] = { 0 };
			sprintf(newRangeString, "%u.%u.%u.%u-%u.%u.%u.%u", 
				ip_min[0], ip_min[1], ip_min[2], ip_min[3], ip_max[0], ip_max[1], ip_max[2], ip_max[3]);
			rangeVec = Utils::splitToStrVector(std::string(newRangeString), '-');
		}
		else rangeVec = Utils::splitToStrVector(data, '-');

		std::vector<int> ip1TmpVec = Utils::splitToIntVector(rangeVec[0], '.');
		std::vector<int> ip2TmpVec = Utils::splitToIntVector(rangeVec[1], '.');

		ipsstart[0] = ip1TmpVec[0];
		ipsstart[1] = ip1TmpVec[1];
		ipsstart[2] = ip1TmpVec[2];
		ipsstart[3] = ip1TmpVec[3];

		ipsend[0] = ip2TmpVec[0];
		ipsend[1] = ip2TmpVec[1];
		ipsend[2] = ip2TmpVec[2];
		ipsend[3] = ip2TmpVec[3];

		ip1 = (ipsstart[0] * 16777216) + 
			(ipsstart[1] * 65536) + 
			(ipsstart[2] * 256) + 
			ipsstart[3];
		ip2 = (ipsend[0] * 16777216) + 
			(ipsend[1] * 65536) + 
			(ipsend[2] * 256) + 
			ipsend[3];

		if (ip1 > ip2) {
			stt->doEmitionRedFoundData("Malformed input: check your range");
			return -1;
		}

		sprintf(finalIP, "%d.%d.%d.%d", 
			ipsend[0], ipsend[1], ipsend[2], ipsend[3]);

		gTargets = ip2 - ip1 + 1;
		gTargetsNumber = gTargets;
	}
	else if (gMode == 1) {
		strncpy(dnsTarget, data, 256);

		gTargets = ip2 - ip1 + 1;
		gTargetsNumber = gTargets;
	} 
	else {
		if (fileLoader(data) == -1) {
			stt->doEmitionRedFoundData("IP list is empty.");
			return -1;
		}
		sprintf(finalIP, "%d.%d.%d.%d", 
			ipsendfl[gflIndex][0], ipsendfl[gflIndex][1], ipsendfl[gflIndex][2], ipsendfl[gflIndex][3]);
	}

	return 0;
}
int MainStarter::loadPorts(const char *data, char delim) {
	portVector = Utils::splitToIntVector(data, delim);
	for (auto elem : portVector) {
		if (elem > 65535 | elem < 0) {
			stt->doEmitionRedFoundData("Malformed input: check your ports");
			return -1;
		}
	}
	return 0;
}

void MainStarter::saveBackupToFile()
{
	char saveStr[512] = { 0 };
	char saveBuffer[4096] = { 0 };
	char endStr[128] = { 0 };

	if (gMode == 0 || gMode == 1)
	{
		if (gMode == 1)
		{
			if (strlen(currentMask) == 0) {
				sprintf(endStr, "%s", currentIP);
			}
			else strcpy(endStr, currentMask);
		}
		else
		{
			if (strlen(finalIP) == 0) sprintf(endStr, "%s", currentIP);
			else sprintf(endStr, "%s-%s", currentIP, finalIP);
		};

		if (strlen(endStr) > 0)
		{
			sprintf(saveStr, "[SESSION]:%d %s %s %d %s\n",
				gMode, endStr, gTLD, gThreads, gPorts);
			strcat(saveBuffer, saveStr);
			ZeroMemory(saveStr, sizeof(saveStr));
		};
	}
	else if (gMode == -1)
	{
		char ipRange[128] = { 0 };

		if (MainStarter::flCounter > 0)
		{
			FILE *savingFile = fopen("tempIPLst.bk", "w");
			if (savingFile != NULL)
			{
				if (gflIndex < MainStarter::flCounter) {
					sprintf(ipRange, "%s-%d.%d.%d.%d\n",
						currentIP,
						ipsendfl[gflIndex][0],
						ipsendfl[gflIndex][1],
						ipsendfl[gflIndex][2],
						ipsendfl[gflIndex][3]);
					fputs(ipRange, savingFile);
					ZeroMemory(ipRange, sizeof(ipRange));
				}
				for (int tCounter = gflIndex + 1; tCounter < MainStarter::flCounter; ++tCounter)
				{
					sprintf(ipRange, "%d.%d.%d.%d-%d.%d.%d.%d\n",
						ipsstartfl[tCounter][0], 
						ipsstartfl[tCounter][1], 
						ipsstartfl[tCounter][2], 
						ipsstartfl[tCounter][3],
						ipsendfl[tCounter][0], 
						ipsendfl[tCounter][1], 
						ipsendfl[tCounter][2], 
						ipsendfl[tCounter][3]);

					fputs(ipRange, savingFile);

					ZeroMemory(ipRange, sizeof(ipRange));
				};
				fclose(savingFile);
			}
			else stt->doEmitionRedFoundData("[_saver] Cannot open file.");
		};

		sprintf(saveStr, "[SESSION]:%d RESTORE_IMPORT_SESSION %d %s\n", gMode, gThreads, gPorts);
		strcat(saveBuffer, saveStr);
		ZeroMemory(saveStr, sizeof(saveStr));
	}
	else {
		sprintf(saveStr, "[SESSION]: 0 1.1.1.1/32 0 -p80");
		strcat(saveBuffer, saveStr);
		ZeroMemory(saveStr, sizeof(saveStr));
	}

	sprintf(saveStr, "[NDBSERVER]:%s\n", trcSrv);
	strcat(saveBuffer, saveStr);
	ZeroMemory(saveStr, sizeof(saveStr));

	sprintf(saveStr, "[NDBSCRIPT]:%s\n", trcScr);
	strcat(saveBuffer, saveStr);
	ZeroMemory(saveStr, sizeof(saveStr));

	sprintf(saveStr, "[NDBPORT]:%s\n", trcSrvPortLine);
	strcat(saveBuffer, saveStr);
	ZeroMemory(saveStr, sizeof(saveStr));

	sprintf(saveStr, "[PROXY]:%s\n", trcProxy);
	strcat(saveBuffer, saveStr);
	ZeroMemory(saveStr, sizeof(saveStr));

	sprintf(saveStr, "[PROXYPORT]:%s\n", trcPort);
	strcat(saveBuffer, saveStr);
	ZeroMemory(saveStr, sizeof(saveStr));

	sprintf(saveStr, "[PING]:%s\n", gPingNScan ? "true" : "false");
	strcat(saveBuffer, saveStr);
	ZeroMemory(saveStr, sizeof(saveStr));

	sprintf(saveStr, "[SHUFFLE]:%s\n", gShuffle ? "true" : "false");
	strcat(saveBuffer, saveStr);
	ZeroMemory(saveStr, sizeof(saveStr));

	sprintf(saveStr, "[NSTRACK]:%s\n", trackerOK ? "true" : "false");
	strcat(saveBuffer, saveStr);
	ZeroMemory(saveStr, sizeof(saveStr));

	sprintf(saveStr, "[PING_TO]: %d\n", gPingTimeout);
	strcat(saveBuffer, saveStr);
	ZeroMemory(saveStr, sizeof(saveStr));

	sprintf(saveStr, "[THREAD_DELAY]: %d\n", Threader::gThreadDelay);
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

	std::string finalSaveStr(saveBuffer);
	std::ofstream file("restore");
	if (finalSaveStr.size() > 0) file << finalSaveStr;
	ZeroMemory(saveBuffer, strlen(saveBuffer));
}

bool saverRunning = false;
void MainStarter::saver()
{
	saverRunning = true;
	Sleep(1000);
	while (globalScanFlag)
	{
		savingBackUpFile = true;
		saveBackupToFile();
		savingBackUpFile = false;
		Sleep(10000);
	};
	saverRunning = false;
}

bool ipPerSecTimer = false;
void _IPPerSecTimer() {
	ipPerSecTimer = true;
	int ticks = 0;
	int ovrlIPs = 0;
	Sleep(50);

	while (globalScanFlag)
	{
		ovrlIPs += ipCounter;
		ipCounter = 0;

		sprintf(metaIPS, "%f.5", ovrlIPs / ++ticks);

		Sleep(1000);
	}
	ipPerSecTimer = false;
}

bool trackerRunning = false;
void _tracker() {
	trackerRunning = true;
	while (true) {
		while (globalScanFlag && !trackerOK) Sleep(1000);

		if (!globalScanFlag && jsonArr->size() == 0) break;
		char rBuffT[250000] = { 0 };
		char *msg = new char[4096];
		ZeroMemory(msg, sizeof(*msg));
		char ndbServer[64] = { 0 };
		char ndbScriptT[64] = { 0 };
		char ndbScript[64] = { 0 };

		sockaddr_in sockAddr;
		sockAddr.sin_family = AF_INET;
		sockAddr.sin_port = htons(atoi(trcSrvPortLine));

		sprintf(msg, "GET /%s HTTP/1.1\r\nHost: %s\r\nX-Nescav3: True\r\nContent-Type: application/x-www-form-urlencoded\r\nConnection: close\r\n\r\n",
			trcScr, trcSrv);

		HOSTENT *host;

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
		if (inet_addr(trcSrv) != INADDR_NONE) sockAddr.sin_addr.S_un.S_addr = inet_addr(trcSrv);
		else if (host = gethostbyname(trcSrv)) ((unsigned long*)&sockAddr.sin_addr)[0] = ((unsigned long**)host->h_addr_list)[0][0];
#else
		if (inet_addr(trcSrv) != INADDR_NONE) sockAddr.sin_addr.s_addr = inet_addr(trcSrv);
		else if (host = gethostbyname(trcSrv)) ((unsigned long*)&sockAddr.sin_addr)[0] = ((unsigned long**)host->h_addr_list)[0][0];
#endif
		SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

		if (connect(sock, (sockaddr*)&sockAddr, sizeof(sockAddr)) == -1)
		{
			CSSOCKET(sock);

			stt->doEmitionRedFoundData("[NS-Track] -Cannot connect to balancer! " + QString::number(WSAGetLastError()) + ".");

			continue;
		};

		if (send(sock, msg, strlen(msg), 0) == -1)
		{
			CSSOCKET(sock);

			stt->doEmitionRedFoundData("[NS-Track] -Cannot send to balancer! " + QString::number(WSAGetLastError()) + ".");

			continue;
		};

		ZeroMemory(rBuffT, sizeof(rBuffT));
		char rBuff[512] = { 0 };
		int test = recv(sock, rBuff, sizeof(rBuff), 0);
		strcpy(rBuffT, rBuff);

		while ((test = recv(sock, rBuff, sizeof(rBuff), 0)) != 0)
		{
			if (strlen(rBuffT) > 200000)
			{
				stt->doEmitionRedFoundData("[NS-Track] (Outer) -Large error received from server (>200000b) " + 
					QString::number(WSAGetLastError()) + ".");
				break;
			};
			strcat(rBuffT, rBuff);
		};

		if (test == -1)
		{
			CSSOCKET(sock);

			stt->doEmitionRedFoundData("[NS-Track] -Cannot recv from balancer! " + QString::number(WSAGetLastError()) + ".");

			continue;
		};

		char *t1;
		char *t2;
		if (strstr(rBuffT, "http://") != NULL)
		{
			t1 = strstr(rBuffT, "http://");
			if (strstr((char*)(t1 + 7), "/") != NULL)
			{
				t2 = strstr((char*)(t1 + 7), "/");
				int ln = t2 - t1 - 7;
				if (ln > 64)
				{
					CSSOCKET(sock);

					stt->doEmitionRedFoundData("[NS-Track] -Received server string is not valid!");

					continue;
				}
				else strncpy(ndbServer, (char*)(t1 + 7), ln);

				if (strlen(t2) > 64)
				{

					stt->doEmitionYellowFoundData("[NS-Track] -Fragmentation detected!");

					if (strstr(t2, "\r\n") != NULL)
					{
						char *t3 = strstr(t2, "\r\n");
						int y = (int)(t3 - t2);

						if (y > 64)
						{
							CSSOCKET(sock);

							stt->doEmitionRedFoundData("[NS-Track] -Received server string is not valid!");

							continue;
						}
						else
						{
							strncpy(ndbScriptT, t2, y);
							CSSOCKET(sock);

							stt->doEmitionGreenFoundData("[NS-Track] -OK! -Fragmented server string aquired! Starting NS-Track loop...");

							strncpy(ndbScript, ndbScriptT, strlen(ndbScriptT));
						};
					}
					else
					{
						CSSOCKET(sock);

						stt->doEmitionRedFoundData("[NS-Track] -Received server string is not valid!");

						continue;
					};
				}
				else
				{
					strcpy(ndbScriptT, t2);

					stt->doEmitionGreenFoundData("[NS-Track] -OK! -Server string aquired! Starting NS-Track loop...");

					CSSOCKET(sock);
					strncpy(ndbScript, ndbScriptT, strlen(ndbScriptT) - 2);
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

			while (true)
			{
				if (!globalScanFlag && jsonArr->size() == 0) break;
				if (!trackerOK) {
					Sleep(1000);
					continue;
				};

				if (jsonArr->size() > 0)
				{
					QJsonObject jsonKey;
					if (jsonArr == NULL) jsonArr = new QJsonArray();

					QJsonObject jsonMeta;
					if (gMode == 0) jsonMeta.insert("mode", QJsonValue(QString("IP")));					//
					else if (gMode == 1) jsonMeta.insert("mode", QJsonValue(QString("DNS")));			//Mode
					else if (gMode == -1) jsonMeta.insert("mode", QJsonValue(QString("Import")));		//
					jsonMeta.insert("range", QJsonValue(QString(metaRange)));
					jsonMeta.insert("current", QJsonValue(QString(currentIP)));
					if (gMode == 1) jsonMeta.insert("tld", QJsonValue(QString(gTLD)));					//TLD
					jsonMeta.insert("targets", QJsonValue(QString(metaTargets)));
					jsonMeta.insert("percent", QJsonValue(QString(metaPercent)));
					jsonMeta.insert("saved", QJsonValue(QString::number(saved)));
					jsonMeta.insert("found", QJsonValue(QString::number(found)));
					jsonMeta.insert("speed", QJsonValue(QString(metaIPS)));
					jsonMeta.insert("eta", QJsonValue(QString(metaETA)));
					jsonMeta.insert("threads", QJsonValue(QString::number(cons) + "/" + QString::number(gThreads)));
					jsonMeta.insert("bads", QJsonValue("-1"));
					jsonMeta.insert("version", QJsonValue(QString(gVER)));

					jsonArr->push_front(QJsonValue(jsonMeta));
					memset(trcPersKey + 32, '\0', 1);
					jsonKey.insert("key", QJsonValue(QString(trcPersKey)));
					jsonArr->push_front(jsonKey);

					QJsonDocument js;
					js.setArray(*jsonArr);
					QByteArray r = js.toJson();

					sockAddr.sin_family = AF_INET;
					sockAddr.sin_port = htons(atoi(trcSrvPortLine));

					if (msg != NULL)
					{
						delete[]msg;
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
					if (inet_addr(ndbServer) != INADDR_NONE) sockAddr.sin_addr.S_un.S_addr = inet_addr(ndbServer);
					else if (host = gethostbyname(ndbServer)) ((unsigned long*)&sockAddr.sin_addr)[0] = ((unsigned long**)host->h_addr_list)[0][0];
#else
					if (inet_addr(ndbServer) != INADDR_NONE) sockAddr.sin_addr.s_addr = inet_addr(ndbServer);
					else if (host = gethostbyname(ndbServer)) ((unsigned long*)&sockAddr.sin_addr)[0] = ((unsigned long**)host->h_addr_list)[0][0];
#endif
					sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

					if (gDebugMode)
					{
						stt->doEmitionDebugFoundData("Connecting to " + QString(ndbServer));
					};

					if (connect(sock, (sockaddr*)&sockAddr, sizeof(sockAddr)) == -1)
					{
						CSSOCKET(sock);


						stt->doEmitionRedFoundData("[NS-Track] connect() returned " + 
							QString::number(WSAGetLastError()) + ".");

						continue;
					};

					if (gDebugMode)
					{
						stt->doEmitionDebugFoundData("Sending!");
						stt->doEmitionDebugFoundData("Key: [" + QString(trcPersKey) + "]");
						stt->doEmitionDebugFoundData("MSG: [" + QString(msg) + "]");
					};

					if (send(sock, msg, strlen(msg), 0) == -1)
					{
						CSSOCKET(sock);

						stt->doEmitionRedFoundData("[NS-Track] send() returned " + 
							QString::number(WSAGetLastError()) + ".");

						continue;
					};

					ZeroMemory(rBuffT, sizeof(rBuffT));
					char msgR[32] = { 0 };

					if (gDebugMode)
					{
						stt->doEmitionDebugFoundData("Receiving...");
					};

					test = recv(sock, rBuff, 512, 0);

					if (gDebugMode)
					{
						stt->doEmitionDebugFoundData("Received: " + QString(rBuff));
					};

					strncpy(msgR, rBuff, 32);
					strcpy(rBuffT, rBuff);
					while (test > 0)
					{
						if (test <= 0) break;

						if (strlen(rBuffT) > 200000)
						{
							stt->doEmitionRedFoundData("[NS-Track] (Inner) Large error received from server (>200000b) " + 
								QString::number(WSAGetLastError()) + ".");
							break;
						};
						strcat(rBuffT, rBuff);
						test = recv(sock, rBuff, 512, 0);
						if (gDebugMode)
						{
							stt->doEmitionDebugFoundData("Received: " + QString(rBuff));
						};
					};

					if (test == -1)
					{
						CSSOCKET(sock);

						stt->doEmitionRedFoundData("[NS-Track] recv() returned " + QString::number(WSAGetLastError()) + ".");

						continue;
					};

					if (strstr(rBuffT, "201 Created") != NULL)
					{

						if (gDebugMode) stt->doEmitionYellowFoundData("[NS-Track] OK. Data saved!");
						stt->doEmitionDataSaved(true);
						Sleep(1000);
						stt->doEmitionDataSaved(false);

					}
					else if (strstr(rBuffT, "400 Bad Request") != NULL)
					{
						QString errorDef = Utils::GetNSErrorDefinition(rBuffT, "notify");
						if (errorDef == "Invalid access key") stt->doEmitionYellowFoundData("[NS-Track] [Key is unauthorized] A valid key is required.");
						else stt->doEmitionYellowFoundData("[NS-Track] FAIL! [400 Bad Request : " + errorDef + "]");

					}
					else
					{
						stt->doEmitionYellowFoundData("[NS-Track] FAIL! An error occured [" + QString(msgR) + "]");
					};

					ZeroMemory(msgR, sizeof(msgR));
					ZeroMemory(rBuffT, sizeof(rBuffT));
					ZeroMemory(msg, sizeof(*msg));

					if (msg != NULL)
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
			stt->doEmitionRedFoundData("[NS-Track] Balancer replied with invalid string.");
		};

		CSSOCKET(sock);
	}

	trackerRunning = false;
}

void verboseProgress(unsigned long target) {

	stt->doEmitionUpdateArc(gTargets);
	if (gTargets > 0) --gTargets;

	char targetNPers[128] = { 0 };
	float percent = (gTargetsNumber != 0 ? (100 - target / (double)gTargetsNumber * 100) : 0);

	sprintf(targetNPers, "%Lu (%.1f%%)", target, percent);

	sprintf(metaTargets, "%Lu", target);
	sprintf(metaPercent, "%.1f",
		percent);
}

void _connect() {

	std::string ip = "";
	std::unique_lock<std::mutex> lk;
	while (globalScanFlag) {
		lk = std::unique_lock<std::mutex>(Threader::m);
		Threader::cv.wait(lk, []{return Threader::ready; });

		if (!globalScanFlag || Threader::threadId > gThreads) {
			--Threader::threadId;
			lk.unlock();
			Threader::ready = true;
			Threader::cv.notify_all();
			return;
		}

		Threader::ready = false;

		if (!Threader::ipQueue.empty()) {
			ip = Threader::ipQueue.front();
			Threader::ipQueue.pop();
			lk.unlock();

			++ipCounter;

			++cons;
			Connector con;
			for (int i = 0; i < MainStarter::portVector.size(); ++i)
			{
				if (!globalScanFlag) break;
				if (con.connectToPort((char*)ip.c_str(), MainStarter::portVector[i]) == -2) break;
			};
			--cons;
			stt->doEmitionUpdateArc(gTargets);
		}
		else lk.unlock();
	}
}

void MainStarter::startIPScan(){

	switch (gShuffle) {
	case true: {
				   std::vector<std::string> ipVec;
				   struct in_addr tAddr;

				   for (unsigned long i = ip1; i <= ip2; ++i) {

					   if (!globalScanFlag) break;
					   unsigned long offset = ip2 - i;

					   tAddr.s_addr = ntohl(i);
					   ipVec.push_back(inet_ntoa(tAddr));

					   if (ipVec.size() >= (offset < 1000 ? offset : 1000)) {

						   std::random_shuffle(ipVec.begin(), ipVec.end());
						   while (ipVec.size() != 0) {

							   while (cons >= gThreads && globalScanFlag) Sleep(500);
							   if (!globalScanFlag) goto haters_gonna_hate_IPM;

							   ++indexIP;
							   strcpy(currentIP, ipVec[0].c_str());
							   ipVec.erase(ipVec.begin());

							   verboseProgress(gTargets);

							   Threader::fireThread(currentIP, (void*(*)(void))_connect);
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

						++indexIP;

						tAddr.s_addr = ntohl(i);
						strcpy(currentIP, inet_ntoa(tAddr));
						verboseProgress(gTargets);

						Threader::fireThread(currentIP, (void*(*)(void))_connect);
					}
					break;
	}
	}
}

char charAll[38] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '_', '-',
'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z' };
int _getPos(char l) {
	for (int i = 0; i < 38; ++i)
	{
		if (l == charAll[i]) return i;
	};
	return -1;
}
int _GetDNSFromMask(char *mask, char *saveMask, char *saveMaskEnder) {

	if (strstr(mask, "[") != NULL)
	{
		char maskEnd[1024] = { 0 };
		char maskRes[1024] = { 0 };
		char *ptr1 = strstr(mask, "[");
		char *ptr2 = strstr(ptr1, "]");
		int sz = ptr2 - ptr1;
		char chunk[8] = { 0 };
		strncpy(chunk, ptr1, sz + 1);
		int firstPos = _getPos(chunk[1]);
		int secondPos = _getPos(chunk[2]);

		if (firstPos > secondPos)
		{
			stt->doEmitionRedFoundData("Error at mask (Wrong letter sequence)");
			return -1;
		};

		char maskStart[1024] = { 0 };
		int startPosition = strstr(mask, "[") - mask;
		strncpy(maskStart, mask, startPosition);

		char maskEntry[1024] = { 0 };
		if (saveMaskEnder != NULL)
		{
			int startSavePosition = strstr(saveMaskEnder, "[") - saveMaskEnder;
			strncpy(maskEntry, saveMaskEnder, startSavePosition);
		};

		int szMask = strlen(mask);
		int szOffset = startPosition + 2;
		if (szMask != szOffset) strcpy(maskEnd, strstr(mask, "]") + 1);
		else ZeroMemory(maskEnd, sizeof(maskEnd));;

		char maskSaver[128] = { 0 };
		if (firstPos != -1 && secondPos != -1)
		{
			for (int i = firstPos; i <= secondPos; ++i)
			{
				if (!globalScanFlag) break;

				strcpy(maskSaver, saveMask);
				strcat(maskSaver, maskEntry);
				chunk[1] = charAll[i];
				strcat(maskSaver, chunk);
				strcpy(maskRes, maskStart);
				maskRes[startPosition] = charAll[i];
				strcat(maskRes, maskEnd);

				if (_GetDNSFromMask(maskRes, maskSaver, maskEnd) == -1) return -1;

				ZeroMemory(maskSaver, sizeof(maskSaver));
				ZeroMemory(maskRes, sizeof(maskRes));
			};
		};
	}
	else
	{
		strcpy(currentMask, saveMask);
		while (cons >= gThreads && globalScanFlag) Sleep(300);
		if (!globalScanFlag) return 0;

		++indexIP;
		sprintf(currentIP, "%s%s", mask, gTLD);
		verboseProgress(gTargets);

		Threader::fireThread(currentIP, (void*(*)(void))_connect);
	};
}
int _getChunkCount(char *data) {
	int firstPos = _getPos(data[1]);
	int secondPos = _getPos(data[2]);
	return secondPos - firstPos + 1;
}
void MainStarter::startDNSScan(){
	char dataEntry[1024] = { 0 };
	int innerCounter = 0;
	int sz = strlen(dnsTarget);

	for (int i = 0; i < sz; ++i)
	{
		if (dnsTarget[i] == '\\')
		{
			if (dnsTarget[i + 1] == 'd')
			{
				strcat(dataEntry, "[09]");
				++i;
				innerCounter += 4;
				continue;
			}
			else if (dnsTarget[i + 1] == 'w')
			{
				strcat(dataEntry, "[0z]");
				++i;
				innerCounter += 4;
				continue;
			}
			else if (dnsTarget[i + 1] == 'l')
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
				errStr += QString(dnsTarget).mid(0, i == 0 ? 0 : i);
				errStr += "<u>";
				errStr += QString(dnsTarget).mid(i, i == 0 ? i + 2 : i + 1);
				errStr += "</u>";
				errStr += QString(dnsTarget).mid(i + 2, strlen(dnsTarget));
				errStr += "\"";

				stt->doEmitionRedFoundData(errStr);
				return;
			};
		}
		else
		{
			memset(dataEntry + innerCounter++, dnsTarget[i], 1);
		};
	};

	memset(dataEntry + innerCounter + 1, '\0', 1);
	strcpy(currentMask, dataEntry);

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
					return;
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
			return;
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
	gTargetsNumber = gTargets;
	stt->doEmitionYellowFoundData("Starting DNS-scan...");
	stt->doEmitionChangeStatus("Scanning...");

	int y = _GetDNSFromMask(dataEntry, "", dataEntry);
	if (y == -1)
	{
		stt->doEmitionRedFoundData("DNS-Mode error");
	};
}

void MainStarter::startImportScan(){
	if (MainStarter::flCounter == 0)
	{
		stt->doEmitionRedFoundData("Empty IP list.");
		globalScanFlag = false;
		stt->doEmitionKillSttThread();
		return;
	};

	stt->doEmitionChangeStatus("Scanning...");
	for (gflIndex = 0; gflIndex < MainStarter::flCounter; ++gflIndex)
	{		
		sprintf(metaRange, "%d.%d.%d.%d-%d.%d.%d.%d",
			ipsstartfl[gflIndex][0], ipsstartfl[gflIndex][1], ipsstartfl[gflIndex][2], ipsstartfl[gflIndex][3], 
			ipsendfl[gflIndex][0], ipsendfl[gflIndex][1], ipsendfl[gflIndex][2], ipsendfl[gflIndex][3]);

		ip1 = (ipsstartfl[gflIndex][0] * 16777216) +
			(ipsstartfl[gflIndex][1] * 65536) +
			(ipsstartfl[gflIndex][2] * 256) +
			ipsstartfl[gflIndex][3];
		ip2 = (ipsendfl[gflIndex][0] * 16777216) +
			(ipsendfl[gflIndex][1] * 65536) +
			(ipsendfl[gflIndex][2] * 256) +
			ipsendfl[gflIndex][3];

		switch (gShuffle) {
		case true: {
					   std::vector<std::string> ipVec;
					   struct in_addr tAddr;

					   for (unsigned long i = ip1; i <= ip2; ++i) {

						   if (!globalScanFlag) break;
						   unsigned long offset = ip2 - i;

						   tAddr.s_addr = ntohl(i);
						   ipVec.push_back(inet_ntoa(tAddr));

						   if (ipVec.size() >= (offset < 1000 ? offset : 1000)) {

							   std::random_shuffle(ipVec.begin(), ipVec.end());
							   while (ipVec.size() != 0) {

								   while (cons >= gThreads && globalScanFlag) Sleep(500);
								   if (!globalScanFlag) goto haters_gonna_hate_IM;

								   ++indexIP;
								   strcpy(currentIP, ipVec[0].c_str());
								   ipVec.erase(ipVec.begin());
								   verboseProgress(gTargets);

								   Threader::fireThread(currentIP, (void*(*)(void))_connect);
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
							strcpy(currentIP, inet_ntoa(tAddr));
							verboseProgress(gTargets);
							Threader::fireThread(currentIP, (void*(*)(void))_connect);
						}
						break;
		};
		}
	}
}

void MainStarter::runAuxiliaryThreads() {

	FileUpdater::loadOnce();
	if (!FileUpdater::running) {
		std::thread lpThread(FileUpdater::updateLists);
		lpThread.detach();
	}
	if (!trackerRunning) {
		std::thread trackerThread(_tracker);
		trackerThread.detach();
	}
	if (!ipPerSecTimer) {
		std::thread timerThread(_IPPerSecTimer);
		timerThread.detach();
	}
	if (!saverRunning) {
		std::thread saverThread(&MainStarter::saver, this);
		saverThread.detach();
	}
}

void MainStarter::createResultFiles() {
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	bool res = CreateDirectoryA(RESULT_DIR_NAME, NULL);
	if(!res) stt->doEmitionRedFoundData("Failed to crete results.");
#else
	struct stat str = { 0 };
	if (stat(RESULT_DIR_NAME, &str) == -1) {
		mkdir(RESULT_DIR_NAME, 0700);
	}
#endif
}

void MainStarter::start(const char* targets, const char* ports) {
	
	curl_global_init(CURL_GLOBAL_ALL);
	
	createResultFiles();

	if (loadTargets(targets) == -1 ||
		loadPorts(ports, ',') == -1) {
		stt->doEmitionKillSttThread();
		return;
	}

	globalScanFlag = true;
	runAuxiliaryThreads();

	if (gMode == 0) startIPScan();
	else if (gMode == 1) startDNSScan();
	else startImportScan();

	stt->doEmitionYellowFoundData("Stopping threads...");
	stt->doEmitionChangeStatus("Stopping...");

	while (cons > 0 || jsonArr->size() > 0) Sleep(2000);

	stt->doEmitionGreenFoundData("Done. Saved <u>" + QString::number(saved) + 
		"</u> of <u>" + QString::number(found) + "</u> nodes.");
	stt->doEmitionChangeStatus("Idle");
	stt->doEmitionKillSttThread();
}