#pragma once
#include "CheckKey_Th.h"
#include "CheckProxy_Th.h"
#include "STh.h"
int emitIfOK = -1;
int KeyCheckerMain()
{
	int kLen = strlen(trcPersKey);
	if(kLen == 0)
	{
		stt->doEmitionRedFoundData("[Key check] Key field is empty.");
		return -1;
	}
	else if(kLen < 32)
	{
		stt->doEmitionRedFoundData("[Key check] Key length is not valid.");
		return -1;
	};
	char msg[1024] = {0};
	char ndbServer[64] = {0};
	char ndbScript[64] = {0};

	sockaddr_in sockAddr;  
	sockAddr.sin_family = AF_INET;  
	sockAddr.sin_port = htons(atoi(trcSrvPortLine));

	strcpy(msg, "GET ");
	strcat(msg, "/");
	strcat(msg, trcScr);
	strcat(msg, " HTTP/1.0\r\nHost: ");
	strcat(msg, trcSrv);
	strcat(msg, "\r\nX-Nescav3: True");
	strcat(msg, "\r\nConnection: close");
	strcat(msg, "\r\n\r\n");

	HOSTENT *host;  

#if defined(WIN32)
	if(inet_addr(trcSrv) != INADDR_NONE) sockAddr.sin_addr.S_un.S_addr = inet_addr(trcSrv);  
	else if(host=gethostbyname (trcSrv)) ((unsigned long*) &sockAddr.sin_addr)[0] = ((unsigned long**)host->h_addr_list)[0][0];  
#else
	if(inet_addr(trcSrv) != INADDR_NONE) sockAddr.sin_addr.s_addr = inet_addr(trcSrv);  
	else if(host=gethostbyname (trcSrv)) ((unsigned long*) &sockAddr.sin_addr)[0] = ((unsigned long**)host->h_addr_list)[0][0];
#endif
	SOCKET sock = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP ); 

	stt->doEmitionYellowFoundData("[Key check] Requesting server ip...");
	int test = connect(sock, (sockaddr*)&sockAddr, sizeof(sockAddr));
	if(test == -1)
	{
#pragma region QTGUI_Area
		stt->doEmitionRedFoundData("[Key check] -connect() returned. Cannot connect to balancer! " + QString::number(WSAGetLastError()) + ".");
		closesocket(sock);
		return -1;
#pragma endregion
	};

	test = send(sock, msg, strlen(msg), 0);

	if(test == -1)
	{
#pragma region QTGUI_Area
		stt->doEmitionRedFoundData("[Key check] -send() returned. Cannot send to balancer! " + QString::number(WSAGetLastError()) + ".");
		closesocket(sock);
		return -1;
#pragma endregion
	};

	ZeroMemory(msg, sizeof(msg));
	int ror = sizeof(msg);

	test = recv(sock, msg, sizeof(msg), 0);
	char buff[512] = {0};
	while((test = recv(sock, msg, sizeof(msg), 0)) != 0)
	{
		strcat(msg, buff);
	};

	if(test == -1)
	{
#pragma region QTGUI_Area
		stt->doEmitionRedFoundData("[Key check] -recv() returned. Cannot recv from balancer! " + QString::number(WSAGetLastError()) + ".");
		closesocket(sock);
		return -1;
#pragma endregion
	};

	char *t1;
	char *t2;
	if(strstr(msg, "http://") != NULL) 
	{
		t1 = strstr(msg, "http://");
		if(strstr((char*)(t1 + strlen("http://")), "/") != NULL)
		{
			t2 = strstr((char*)(t1 + strlen("http://")), "/");
			int ln = t2 - t1 - strlen("http://");
			if(ln > 64)
			{
#pragma region QTGUI_Area
				stt->doEmitionRedFoundData("[Key check] -Received server string is not valid!");
				closesocket(sock);
				return -1;
#pragma endregion
			}
			else strncpy(ndbServer, (char*)(t1 + strlen("http://")), ln);

			
			if(strlen(t2) > 64)
			{
	#pragma region QTGUI_Area
					stt->doEmitionYellowFoundData("[Key check] -Fragmentation detected!");
	#pragma endregion
				if(strstr(t2, "\r\n") != NULL)
				{
					char *t3 = strstr(t2, "\r\n");
					int y = (int)(t3 - t2);

					if(y > 64)
					{
						#pragma region QTGUI_Area
						stt->doEmitionRedFoundData("[Key check] -Received server string is not valid!");
						closesocket(sock);
						return -1;
						#pragma endregion
					}
					else
					{
						strncpy(ndbScript, t2, y);
					};
				}
				else
				{
					#pragma region QTGUI_Area
					stt->doEmitionRedFoundData("[Key check] -Received server string is not valid!");
					closesocket(sock);
					return -1;
					#pragma endregion
				};
			} else strcpy(ndbScript, t2);
		}
		else
		{
#pragma region QTGUI_Area
			stt->doEmitionRedFoundData("[Key check] -Cannot receive script value!");
			closesocket(sock);
			return -1;
#pragma endregion
		};

	ZeroMemory(msg, sizeof(msg));

#pragma region QTGUI_Area
	stt->doEmitionGreenFoundData("[Key check] -OK. -Server string aquired! Checking key...");
	closesocket(sock);
#pragma endregion

	sockAddr.sin_family = AF_INET;  
	sockAddr.sin_port = htons(atoi(trcSrvPortLine));
	strcpy(msg, "GET ");
	strcat(msg, "/api/checkaccount?key=");
	strncat(msg, trcPersKey, 32);
	strcat(msg, " HTTP/1.0\r\nHost: ");
	strcat(msg, ndbServer);
	strcat(msg, "\r\nConnection: close");
	strcat(msg, "\r\n\r\n");

#if defined(WIN32)
	if(inet_addr(ndbServer) != INADDR_NONE) sockAddr.sin_addr.S_un.S_addr = inet_addr(ndbServer);  
	else if(host=gethostbyname (ndbServer)) ((unsigned long*) &sockAddr.sin_addr)[0] = ((unsigned long**)host->h_addr_list)[0][0];  
#else
	if(inet_addr(ndbServer) != INADDR_NONE) sockAddr.sin_addr.s_addr = inet_addr(ndbServer);  
	else if(host=gethostbyname (ndbServer)) ((unsigned long*) &sockAddr.sin_addr)[0] = ((unsigned long**)host->h_addr_list)[0][0];
#endif
	sock = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP ); 

	int c = connect(sock, (sockaddr*)&sockAddr, sizeof(sockAddr));
	if(c == SOCKET_ERROR)
	{
		stt->doEmitionRedFoundData("[Key check] -Connection timeout.");
		closesocket(sock);
		return -1;
	};
	c = send(sock, msg, strlen(msg), 0);
	if(c == SOCKET_ERROR)
	{
		stt->doEmitionRedFoundData("[Key check] -Send error.");
		closesocket(sock);
		return -1;
	};
	ZeroMemory(msg, sizeof(msg));
	test = recv(sock, msg, 512, 0);

	if(strstr(msg, "202 Accepted") != NULL)
	{
#pragma region QTGUI_Area
		stt->doEmitionGreenFoundData("[Key check] -OK. Key is valid!");
#pragma endregion
		closesocket(sock);
		if(emitIfOK == 0) stt->doEmitionStartScanIP();
		else if(emitIfOK == 1) stt->doEmitionStartScanDNS();
		else if(emitIfOK == 2) stt->doEmitionStartScanImport();
		return 1;
	}
	else if(strstr(msg, "400 Bad Request") != NULL)
	{
#pragma region QTGUI_Area
						QString errorDef = GetNSErrorDefinition(msg, "notify");
						if(errorDef == "Invalid access key") stt->doEmitionYellowFoundData("[NS-Track] [Key is unauthorized] A valid key is required.");
						else stt->doEmitionYellowFoundData("[NS-Track] -FAIL! [400 Bad Request : " + GetNSErrorDefinition(msg, "notify") + "]");
#pragma endregion
		closesocket(sock);
		return -1;
	}
	else
	{
#pragma region QTGUI_Area
		stt->doEmitionYellowFoundData("[Key check] -FAIL! An error occured. (" + QString::number(WSAGetLastError()) + ")");
		if(gDebugMode) stt->doEmitionDebugFoundData(QString(msg));
#pragma endregion
		closesocket(sock);
		return -1;
	};

	ZeroMemory(msg, sizeof(msg));
	closesocket(sock);
	}
	else
	{
#pragma region QTGUI_Area
		stt->doEmitionRedFoundData("[Key check] -Balancer replied with invalid string.");
		if(gDebugMode) stt->doEmitionDebugFoundData(QString(msg));
		closesocket(sock);
		return -1;
#pragma endregion
	};
};

void CheckKey_Th::run()
{
	KeyCheckerMain();
};
