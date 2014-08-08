#include "CheckProxy_Th.h"

void CheckProxy_Th::doEmitChangeRedIRCData(QString str)
{
	emit chPTh->changeRedIRCData(str);
};
void CheckProxy_Th::doEmitChangeGreenIRCData(QString str)
{
	emit chPTh->changeGreenIRCData(str);
};
void CheckProxy_Th::doEmitChangeYellowIRCData(QString str)
{
	emit chPTh->changeYellowIRCData(str);
};
void CheckProxy_Th::doEmitChangeRawIRCDataInc(QString str)
{
	emit chPTh->changeRawIRCDataInc(str);
};
void CheckProxy_Th::doEmitChangeRawIRCDataOut(QString str)
{
	emit chPTh->changeRawIRCDataOut(str);
};

void CheckProxyLogic()
{
	destroychPThFlag = false;
	QString str1 = ui->ircProxyPort->text();
	QString str2 = ui->ircProxy->text();
	strcpy(ircProxy, str2.toUtf8().data());
	strcpy(ircProxyPort, str1.toUtf8().data());

	int err, yes = 1;
	SOCKET pSock;
	char precvBuff[2048] = {0};
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(ircProxyPort));

	HOSTENT *host;  

#if defined(WIN32)
	if(inet_addr(ircProxy) != INADDR_NONE) addr.sin_addr.S_un.S_addr = inet_addr(ircProxy);  
	else if(host = gethostbyname (ircProxy)) ((unsigned long*) &addr.sin_addr)[0] = ((unsigned long**)host->h_addr_list)[0][0];  
#else
	if(inet_addr(ircProxy) != INADDR_NONE) addr.sin_addr.s_addr = inet_addr(ircProxy);  
	else if(host=gethostbyname (ircProxy)) ((unsigned long*) &addr.sin_addr)[0] = ((unsigned long**)host->h_addr_list)[0][0];
#endif
	pSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	setsockopt(pSock, SOL_SOCKET, SO_REUSEADDR, (char*)&yes, sizeof(int));

	if(pSock == INVALID_SOCKET) 
	{
#pragma region QTGUI_Area
		chPTh->doEmitChangeRedIRCData("CheckProxy: -INVALID SOCKET.");
#pragma endregion
	}
	else
	{
		if(connect(pSock, (sockaddr*)&addr, sizeof(addr)) != SOCKET_ERROR)
		{
#pragma region QTGUI_Area
		chPTh->doEmitChangeRawIRCDataOut(QString::fromLocal8Bit("CONNECT 2ip.ru HTTP/1.1\r\n\r\n"));
#pragma endregion
			send(pSock, "CONNECT 2ip.ru HTTP/1.1\r\n\r\n", strlen("CONNECT 2ip.ru HTTP/1.1\r\n\r\n"), 0);

			while(recv(pSock, precvBuff, sizeof(precvBuff), 0) > 0) 
			{
#pragma region QTGUI_Area
		chPTh->doEmitChangeRawIRCDataInc(QString::fromLocal8Bit(precvBuff));
#pragma endregion
				if( (strstr(precvBuff, "HTTP/1.1 200 OK") || strstr(precvBuff, "200 OK")
					|| strstr(precvBuff, "OK 200") || strstr(precvBuff, "200 Connection")
					)
					&& (strlen(precvBuff) < 150) 
					&& strstr(precvBuff, "404 File Not Found") == NULL
					&& 
					(
					strstr(precvBuff, "Invalid Request") == NULL
					|| strstr(precvBuff, "Invalid request") == NULL || strstr(precvBuff, "invalid request") == NULL
					|| strstr(precvBuff, "400 Bad Request") == NULL || strstr(precvBuff, " 400 bad request") == NULL
					) 
					)
				{
					
#pragma region QTGUI_Area
		chPTh->doEmitChangeRawIRCDataOut(QString::fromLocal8Bit("GET / HTTP/1.1\r\nHost: 2ip.ru\r\n\r\n"));
#pragma endregion
					send(pSock, "GET / HTTP/1.1\r\nHost: 2ip.ru\r\n\r\n", strlen("GET / HTTP/1.1\r\nHost: 2ip.ru\r\n\r\n"), 0);
					ZeroMemory(precvBuff, sizeof(precvBuff));
					while(recv(pSock, precvBuff, sizeof(precvBuff), 0) > 0) 
					{
						
#pragma region QTGUI_Area
		chPTh->doEmitChangeRawIRCDataInc(QString::fromLocal8Bit(precvBuff));
#pragma endregion
						if(strstr(precvBuff, "404 File Not Found") == NULL && strstr(precvBuff, "Invalid Request") == NULL
							&& strstr(precvBuff, "Invalid request") == NULL && strstr(precvBuff, "invalid request") == NULL
							&& strstr(precvBuff, "400 Bad Request") == NULL && strstr(precvBuff, "400 bad request") == NULL
							&& strstr(precvBuff, "404 Not") == NULL && strstr(precvBuff, "404 not") == NULL
							&& strstr(precvBuff, "500 Internal") == NULL && strstr(precvBuff, "500 internal") == NULL
							&& strstr(precvBuff, "401 Unauthorized") == NULL && strstr(precvBuff, "401 unauthorized") == NULL
							&& strstr(precvBuff, "InvalidUrl") == NULL && strstr(precvBuff, "invalidurl") == NULL
							&& strstr(precvBuff, "Invalid Url") == NULL && strstr(precvBuff, "invalid url") == NULL
							&& strstr(precvBuff, "Gateway Timeout") == NULL && strstr(precvBuff, "Gateway timeout") == NULL
							&& strstr(precvBuff, "gateway timeout") == NULL
							)
						{

#pragma region QTGUI_Area
							chPTh->doEmitChangeGreenIRCData("[OK] Success! Now using " + QString(ircProxy) + ":" + QString(ircProxyPort) + ".");
#pragma endregion
							proxyEnabledFlag = 1;

							closesocket(pSock);
							break;
						}
						else
						{
#pragma region QTGUI_Area
							chPTh->doEmitChangeRedIRCData("[Fail] " + QString(ircProxy) + ":" + QString(ircProxyPort) + " - is not CONNECT proxy? Try another one.");
#pragma endregion
							proxyEnabledFlag = 0;

							closesocket(pSock);
							break;
						};
					};
					break;
				} 
				else
				{
#pragma region QTGUI_Area
					chPTh->doEmitChangeRedIRCData("[Fail] " + QString(ircProxy) + ":" + QString(ircProxyPort) + " - is not CONNECT proxy? Try another one.");
#pragma endregion
					proxyEnabledFlag = 0;

					closesocket(pSock);
					break;
				};
			};
		}
		else
		{
#pragma region QTGUI_Area
			chPTh->doEmitChangeRedIRCData("[Fail] Cannot connect to " + QString(ircProxy) + ":" + QString(ircProxyPort) + ".");
#pragma endregion
			proxyEnabledFlag = 0;			
		};
	};	
};

void CheckProxy_Th::run()
{
	CheckProxyLogic();
};
