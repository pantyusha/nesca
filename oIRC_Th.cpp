#include "oIRC_Th.h"
#include <QtMultimedia/qsound.h>
#include "externData.h"
#include "externFunctions.h"

int iWantToConnect = false;

#define MAX_IRC_RECV_LEN 2048
//#define IRC_CHAN "iskopasi_lab01"

void oIRC_Th::doEmitionPlayDckingSound()
{
	emit ircTh->notifyPlay();	
};
void oIRC_Th::doEmitUnhidePopup(QString str1, QString str2)
{
	emit ircTh->sUnhidePopup(str1, str2);
};
void oIRC_Th::doEmitChangeIRCData(bool pm, bool hlflag, int code, QString str, QString s)
{
	emit ircTh->changeIRCData(pm, hlflag, code, str, s);
};
void oIRC_Th::doEmitChangeRedIRCData(QString str)
{
	emit ircTh->changeRedIRCData(str);
};
void oIRC_Th::doEmitChangeGreenIRCData(QString str)
{
	emit ircTh->changeGreenIRCData(str);
};
void oIRC_Th::doEmitChangeYellowIRCData(QString str)
{
	emit ircTh->changeYellowIRCData(str);
};
void oIRC_Th::doEmitChangeRawIRCDataInc(QString str)
{
	emit ircTh->changeRawIRCDataInc(str);
};
void oIRC_Th::doEmitChangeRawIRCDataOut(QString str)
{
	emit ircTh->changeRawIRCDataOut(str);
};
void oIRC_Th::doEmitSetNick(QString str)
{
	emit ircTh->setNick(str);
};
void oIRC_Th::doEmitAddNick(QString str)
{
	emit ircTh->AddNick(str);
};
void oIRC_Th::doEmitClearNickList()
{
	emit ircTh->ClearNickList();
};
void oIRC_Th::doEmitIRCOfflined()
{
	emit ircTh->IRCOfflined();
};
void oIRC_Th::doEmitGetTopic(QString str)
{
	emit ircTh->getTopic(str);
};

QString GetNickColor(char *sn)
{
	QString str(sn);
	QString hexNick = str.toLocal8Bit().toHex();
	int origLen = str.size();
	int hln = hexNick.length();
	while(hln < 7) 
	{
		hexNick += "0";
		hln = hexNick.length();
	};
	
	QString nickColorStr = hexNick.mid(0, 6);
	QString nickBGColorStr = hexNick.mid(hexNick.size() - 6, hexNick.size());

    int nickColor = nickColorStr.toUInt(NULL, 16);
    int dim = QString::number(nickColor).length();
		
	nickColor += (7*origLen + nickColor*6 + 123456 - hln*hln*hln*hln + (int)(str[0].toLatin1())*123);
	nickColorStr.setNum(nickColor, 16);
	nickColorStr = nickColorStr.mid(nickColorStr.size() - 6, nickColorStr.size());
	
	int R = nickColorStr.mid(0, 2).toUInt(NULL, 16);
	int G = nickColorStr.mid(2, 2).toUInt(NULL, 16);
	int B = nickColorStr.mid(4, 2).toUInt(NULL, 16);
	
	if(R < 100 && G < 100 && B < 100) nickBGColorStr = "#fd7e31";
	else nickBGColorStr = "#000000";

	return nickColorStr + "; background-color: " + nickBGColorStr + ";";
}

bool doHL(char *rawData)
{
	if(strstr(rawData, ircNick) != NULL) return true;
	else return  false;
}

void _blinkNLine(QString tempData = "", QString senderNick = "")
{
	if(widgetIsHidden == false && tray->isVisible() == false)
	{
		disableBlink = false;
		if(irc_nmb->isRunning() == false) irc_nmb->start();
		ircTh->doEmitUnhidePopup(tempData, senderNick);

		if(printDelimiter) ircTh->doEmitChangeIRCData(false, false, 0, "------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------", "");
        printDelimiter = false;
	};
}

int sendS(int lSock, char *msg, int len, int mode)
{
    if(lSock == ENOTSOCK || lSock <= 0 || !connectedToIRC) {
        ircTh->doEmitChangeRedIRCData("Not connected.");
        return -1;
    }
    int b = send(lSock, msg, len, mode);
    if(b == -1) ircTh->doEmitChangeRedIRCData("[IRC: RecvS error - (" + QString::number(WSAGetLastError()) + ")]");
	else
	{
		Activity += len;
							
		ircTh->doEmitChangeRawIRCDataOut(QString::fromLocal8Bit(msg));

	};
	return b;
}

int recvS(int lSock, char *recvBuffT, int len)
{
	int b;
	char recvBuff[MAX_IRC_RECV_LEN] = {0};

	b = recv(lSock, recvBuff, sizeof(recvBuff), 0);
	if(b == -1)  ircTh->doEmitChangeRedIRCData("[IRC: RecvS error - (" + QString::number(WSAGetLastError()) + ")]");
	else
	{
		Activity += len;

		strcpy(recvBuffT, recvBuff);

		ircTh->doEmitChangeRawIRCDataInc(QString::fromLocal8Bit(recvBuff));

		ZeroMemory(recvBuff, sizeof(recvBuff));
	};
	return b;		
}

void UserNickInit()
{
	strcpy(ircNick, ui->ircNickBox->text().toLocal8Bit().data());
	char tempBuffUser[1024] = {0};
	strcpy(tempBuffUser, "USER  ");
	strcat(tempBuffUser, ircNick);
	strcat(tempBuffUser, " \"netstalker01\" *  : ");
	strcat(tempBuffUser, ircNick);
	strcat(tempBuffUser, "\r\n");

	char tempBuffNick[1024] = {0};

	strcpy(tempBuffNick, "NICK ");
	strcat(tempBuffNick, ircNick);
	strcat(tempBuffNick, "\r\n");

	sendS(lSock, tempBuffUser, strlen(tempBuffUser), 0);
	sendS(lSock, tempBuffNick, strlen(tempBuffNick), 0);

	memset(tempBuffUser, '0', sizeof(tempBuffUser));
	memset(tempBuffNick, '0', sizeof(tempBuffNick));
}

void GetNicks()
{
	char chanTemp[64] = {0};
	strcpy(chanTemp, "NAMES #");
	strcat(chanTemp, IRC_CHAN);
	strcat(chanTemp, "\r\n");
	sendS(lSock, chanTemp, strlen(chanTemp), 0);
}

char *GetServerName(char *buff)
{
	char *temp1 = NULL;
	int sz = 0;
	char name[128] = {0};
	if(strstr(buff, " ") != NULL)
	{
		temp1 = strstr(buff, " ");
		sz = temp1 - buff - 1;
		strncpy(name, buff + 1, (sz < 128 ? sz : 128));
	};

	return name;
}

int jFlag1 = 0;
void __pinger(char *recvBuff)
{
	if(strstr(recvBuff, "PING") != NULL) 
	{
		char tmpa[128] = {0};

		if(strstr(recvBuff, "PING :") != NULL)
		{	
			if(strstr(strstr(recvBuff, "PING :") + strlen("PING :"), "\r\n") != NULL)
			{
				strcpy(tmpa, "PONG ");
				strncat(tmpa, strstr(recvBuff, "PING :") + strlen("PING "), strlen(strstr(recvBuff, "PING :") + strlen("PING ")) - strlen(strstr(strstr(recvBuff, "PING :") + strlen("PING :"), "\r\n")));
				strcat(tmpa, "\r\n");

				sendS(lSock, tmpa, strlen(tmpa), 0);
			} 
			else 
			{
				strcpy(tmpa, "PONG ");
				strncat(tmpa, strstr(recvBuff, ":"), 16);
				strcat(tmpa, "\r\n");

				sendS(lSock, tmpa, strlen(tmpa), 0);
			};


			if(ircPTh->isRunning() == false) ircPTh->start();
		};

		if(!jFlag1) //Channel-entering sequence
		{
			Sleep(500);
			char chanTemp[32] = {0};
			strcpy(chanTemp, "JOIN #");
			strcat(chanTemp, IRC_CHAN);
			strcat(chanTemp, "\r\n");
			sendS(lSock, chanTemp, strlen(chanTemp), 0);

			jFlag1 = 1;
		};

		globalPinger = 0;
		memset(tmpa, '\0', sizeof(tmpa));
	}
	if(strstr(recvBuff, "PONG") != NULL) 
	{
		char tmpa[128] = {0};
		
		if(!jFlag1) //Channel-entering sequence
		{
			Sleep(500);
			char chanTemp[32] = {0};
			strcpy(chanTemp, "JOIN #");
			strcat(chanTemp, IRC_CHAN);
			strcat(chanTemp, "\r\n");
			sendS(lSock, chanTemp, strlen(chanTemp), 0);

			jFlag1 = 1;
		};

		globalPinger = 0;
		memset(tmpa, '\0', sizeof(tmpa));
	}
	else if(strstr(recvBuff, "G :") != NULL)
	{
		char tmpa[128] = {0};
		if(strstr(recvBuff, "G :") != NULL)
		{	
			if(strstr(strstr(recvBuff, "G :") + strlen("G :"), "\r\n") != NULL)
			{
				strcpy(tmpa, "PONG ");
				strncat(tmpa, strstr(recvBuff, "G :") + strlen("G :"), strlen(strstr(recvBuff, "G :") + strlen("G :")) - strlen(strstr(strstr(recvBuff, "G :") + strlen("G :"), "\r\n")));
				strcat(tmpa, "\r\n");
				sendS(lSock, tmpa, strlen(tmpa), 0);
			} 
			else 
			{
				strcpy(tmpa, "PONG ");
				strncat(tmpa, strstr(recvBuff, ":"), 16);
				strcat(tmpa, "\r\n");
				sendS(lSock, tmpa, strlen(tmpa), 0);
			};
		};

		if(!jFlag1) //Channel-entering sequence
		{
			Sleep(500);
			char chanTemp[32] = {0};
			strcpy(chanTemp, "JOIN #");
			strcat(chanTemp, IRC_CHAN);
			strcat(chanTemp, "\r\n");
			sendS(lSock, chanTemp, strlen(chanTemp), 0);

			jFlag1 = 1;
		};

		globalPinger = 0;
		if(ircPTh->isRunning() == false) ircPTh->start(); 
		memset(tmpa, '\0', sizeof(tmpa));
	};
}

void IRCLoop()
{
	nickFlag = 0;
	offlineFlag = 0;


	ircTh->doEmitChangeYellowIRCData("Connecting to IRC server " + QString(ircServer) + ":" + QString(ircPort) + "...");


	jFlag1 = 0;
		sockaddr_in addr;
		addr.sin_family = AF_INET;

		HOSTENT *host;  

		if(proxyEnabledFlag) 
		{
			addr.sin_port = htons(atoi(ircProxyPort));

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
			if(inet_addr(ircProxy) != INADDR_NONE) addr.sin_addr.S_un.S_addr = inet_addr(ircProxy);  
			else if(host = gethostbyname (ircProxy)) ((unsigned long*) &addr.sin_addr)[0] = ((unsigned long**)host->h_addr_list)[0][0];  
#else
			if(inet_addr(ircProxy) != INADDR_NONE) addr.sin_addr.s_addr = inet_addr(ircProxy);  
			else if(host=gethostbyname (ircProxy)) ((unsigned long*) &addr.sin_addr)[0] = ((unsigned long**)host->h_addr_list)[0][0];
#endif
		}
		else 
		{
			addr.sin_port = htons(atoi(ircPort));

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
			if(inet_addr(ircServer) != INADDR_NONE) addr.sin_addr.S_un.S_addr = inet_addr(ircServer);  
			else if(host = gethostbyname (ircServer)) ((unsigned long*) &addr.sin_addr)[0] = ((unsigned long**)host->h_addr_list)[0][0];  
#else
			if(inet_addr(ircServer) != INADDR_NONE) addr.sin_addr.s_addr = inet_addr(ircServer);  
			else if(host=gethostbyname (ircServer)) ((unsigned long*) &addr.sin_addr)[0] = ((unsigned long**)host->h_addr_list)[0][0];
#endif
		};

		for(int conCounter = 1; conCounter <= 100; ++conCounter)
		{
			char topicData[256] = {0};
			if(iWantToConnect == false) break;
			OnlineMsgSentFlag = false;

			lSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			
			if(proxyEnabledFlag)  ircTh->doEmitChangeYellowIRCData("Connecting to proxy " + QString(ircProxy) + "..."); 
			if(connect(lSock, (sockaddr*)&addr, sizeof(addr)) != SOCKET_ERROR) 
			{
				if(proxyEnabledFlag)  ircTh->doEmitChangeYellowIRCData("Connection to proxy " + QString(ircProxy) + " established."); 
				globalPinger = 0;

				char temprecvBuff[512] = {0};
				char tempSendMsg[512] = {0};

				if(proxyEnabledFlag) 
				{
					strcpy(tempSendMsg, "CONNECT ");
					strcat(tempSendMsg, ircServer);
					strcat(tempSendMsg, ":");
					strcat(tempSendMsg, ircPort);
					strcat(tempSendMsg, " HTTP/1.1\r\n\r\n");

					sendS(lSock, tempSendMsg, strlen(tempSendMsg), 0);

                    recvS(lSock, temprecvBuff, sizeof(temprecvBuff));

					if(strstr(temprecvBuff, "HTTP/1.1 200 OK") || strstr(temprecvBuff, "200 OK")
						|| strstr(temprecvBuff, "OK 200") || strstr(temprecvBuff, "200 Connection") )
					{
						ircTh->doEmitChangeYellowIRCData("Proxy accepted connection. Waiting for IRC reply...");
						sendS(lSock, "\r\n", strlen("\r\n"), 0);
					}
					else
					{
						ircTh->doEmitChangeRedIRCData("[IRC: Bad proxy reply.]");		
						break;
					};
				}
				else
				{
					sendS(lSock, "\r\n", strlen("\r\n"), 0);
				};

                UserNickInit();

				char recvBuffG[MAX_IRC_RECV_LEN] = {0};
				char serverRealName[256] = {0};
				bool nameLocked = false;
												
				char pTemp[32] = {0};
				strcpy(pTemp, "PRIV");
				strcat(pTemp, "MSG ");
                while(recvS(lSock, recvBuffG, MAX_IRC_RECV_LEN) > 0 && iWantToConnect)
				{
					if(strlen(recvBuffG) > 0)
					{
						char *recvBuff = recvBuffG;

						__pinger(recvBuff);		

						char comStr[512] = {0};
						char delimBf[512] = {0};
						strcpy(delimBf, ":");
						strcat(delimBf, serverRealName);
							
						char *Gtemp = recvBuff;
						while(strstr(Gtemp + 1, "\n") != NULL)
						{
								char *temp1 = NULL;
								if(strstr(Gtemp + 1, "\n") != NULL) temp1 = strstr(Gtemp + 1, "\n");
								else temp1 = Gtemp + strlen(Gtemp);
								int csz = temp1 - Gtemp - 1;
								strncpy(comStr, Gtemp, csz);
								Gtemp = strstr(Gtemp + 1, "\n");
								
								char privTemp[64] = {0};
								
								strcpy(privTemp, "PRIV");
								strcat(privTemp, "MSG #");
								strcat(privTemp, IRC_CHAN);
								strcat(privTemp, " :");

								char pTemp[32] = {0};
								strcpy(pTemp, " PRIV");
								strcat(pTemp, "MSG ");
								if(strstr(comStr, pTemp) == NULL)
								{
									char topicTemp[64] = {0};
									strcpy(topicTemp, "TOPIC #");
									strcat(topicTemp, IRC_CHAN);
									strcat(topicTemp, " :");
									if(strstr(comStr, serverRealName) != NULL && (strstr(comStr, "while we process your") != NULL || strstr(comStr, "Looking up your hostname") != NULL)
										) 
									{

										ircTh->doEmitChangeGreenIRCData("[OK] Connected to irc server: " + ui->ircServerBox->text()+ ":" + ui->serverPortBox->text() + ".");

										if(nameLocked == false)
										{
											nameLocked = true;
											strncpy(serverRealName, GetServerName(recvBuff), 128);
										};

										Sleep(500);
                                        UserNickInit();

										Sleep(500);
										char chanTemp[32] = {0};
										strcpy(chanTemp, "JOIN #");
										strcat(chanTemp, IRC_CHAN);
										strcat(chanTemp, "\r\n");
										sendS(lSock, chanTemp, strlen(chanTemp), 0);
									}
									else if(strstr(comStr, serverRealName) != NULL && (strstr(comStr, "ERROR :Closing Link:") != NULL || strstr(comStr, "ERROR :") != NULL) ) 
									{
										if(strstr(comStr, "Registration timed out") != NULL)
										{

											ircTh->doEmitChangeRedIRCData("-//- [!] Connection failure. (Registration timed out)");
											ircTh->terminate();

										}
										else
										{

											ircTh->doEmitChangeRedIRCData("-//- [!] Connection failure. (Closed link)");
											ircTh->terminate();

										};
									}
									else if(strstr(comStr, serverRealName) != NULL && strstr(comStr, "flooding") != NULL)
									{
										ircTh->doEmitChangeYellowIRCData("[" + QString::fromLocal8Bit("Óןûנüעו לוכ") + "] Flooding detected.");
									}
									else if((strstr(comStr, serverRealName) != NULL && strstr(comStr, " 332 ") != NULL)
										|| strstr(comStr, topicTemp) != NULL)
									{
										char chanTemp[32] = {0};
										strcpy(chanTemp, IRC_CHAN);
										strcat(chanTemp, " :");
										char *temp = strstr(comStr, chanTemp);
										strncpy(topicData, temp + strlen(chanTemp), 256);
										if(utfIRCFlag) 
										{
											QTextCodec *codec = QTextCodec::codecForName("Windows-1251");
											QByteArray wtf_s(QString::fromLocal8Bit(topicData).toStdString().c_str());
											QByteArray wtf = codec->fromUnicode(wtf_s);
											ircTh->doEmitGetTopic(wtf);
										}
										else ircTh->doEmitGetTopic(QString::fromLocal8Bit(topicData));
									}
									else if(strstr(comStr, "not channel operator") != NULL)
									{						
										ircTh->doEmitChangeRedIRCData("[Nope] You're not channel operator.");
										ircTh->doEmitGetTopic(QString::fromLocal8Bit(topicData));
									}
									else if(strstr(comStr, serverRealName) != NULL && (strstr(comStr, "353") != NULL || strstr(comStr, "End of /NAMES list") != NULL))
									{
										char *strMain = NULL;
										char *str1 = NULL;
										char *str2 = NULL;

										if(strstr(comStr, " 353 ") != NULL) strMain = strstr(comStr, " 353 ");
										if(strMain != NULL)
										{
											char cTemp[64] = {0};
											strcpy(cTemp, IRC_CHAN);
											strcat(cTemp, " :");

											if(strstri(strMain, cTemp) != NULL) str1 = strstri(strMain, cTemp);
											else 
											{
												char chanTemp[64] = {0};
												strcpy(chanTemp, IRC_CHAN);
												strcat(chanTemp, " : Error in /NAMES");

												str1 = chanTemp;
											};
											if(strstr(str1, ":") != NULL) str2 = strstr(str1 + 1, ":");

											char temp[MAX_IRC_RECV_LEN] = {0};

											int dsz = strlen(str2);
											if(dsz > 0) 
											{
												ircTh->doEmitClearNickList();
												strncpy(temp, str2, dsz);
												
												char *lex = strtok(temp + 1, " ");

												while(lex != NULL && lex != "" && lex != "\r" && lex != "\r\n" && *lex != 13)
												{
													ircTh->doEmitAddNick(QString::fromLocal8Bit(lex));
													lex = strtok(NULL, " ");
												};
											};
										};
									}
									else if(strstr(comStr, serverRealName) != NULL && (strstr(comStr, " 432 ") > 0 || strstr(comStr, "Erroneous Nickname") > 0))
									{

										ircTh->doEmitChangeRedIRCData("[Nope] Erroneous Nickname: Illegal characters.");

									}
									else if(strstr(comStr, serverRealName) != NULL && (strstr(comStr, " 433 ") > 0 || strstr(comStr, "Nickname is already") > 0) )
									{

										QTime time = QTime::currentTime();
										qsrand((uint)time.msec());
										ircTh->doEmitChangeRedIRCData("[Nope] Nickname is already in use.");
										ircTh->doEmitSetNick("ns_" + QString::number(qrand() % 8999 + 1000 ));

                                        UserNickInit();
									}
									else if(strstr(comStr, serverRealName) != NULL && (strstr(comStr, " 438 ") > 0 || strstr(comStr, "Nick change too") > 0))
									{

										ircTh->doEmitChangeRedIRCData("[Nope] You are changing nicks too fast.");

									}
									else if(strstr(comStr, serverRealName) != NULL && (strstr(comStr, "End of /NAMES list") != NULL || strstr(comStr, "End of /names list") != NULL
										|| strstr(comStr, "end of /NAMES list") != NULL || strstr(comStr, "end of /names list") != NULL)
										&& strstr(comStr, "353") == NULL)
									{
										ircTh->doEmitChangeRedIRCData("[IRC: NAMES! lol]");								
									}
									else if(strstr(comStr, "QUIT :Ping timeout") != NULL || strstr(comStr, "EOF From") != NULL
										|| strstr(comStr, "EOF from") != NULL || strstr(comStr, " QUIT :") != NULL)
									{
										if(strstr(comStr, ":") != NULL)
										{
											if(strstr(comStr, "!") != NULL)
											{
												if(strstr(comStr, "@") != NULL)
												{
													char *temp1 = strstr(comStr, ":");
													char *temp2 = strstr(temp1, "!");
													char leaverNick[32] = {0};

													int sz = temp2 - temp1;

													strncpy(leaverNick, temp1, (sz < 16 ? sz : 16));


													if(strstr(comStr, "QUIT :Ping timeout") != NULL) 
													{
														ircTh->doEmitChangeYellowIRCData("-//- " + QString(leaverNick) + " left channel (Ping timeout).");
														_blinkNLine(QString(leaverNick) + " left channel (Ping timeout)", "[Server]");
													}
													else 
													{
														ircTh->doEmitChangeYellowIRCData("-//- " + QString(leaverNick) + " left channel.");
														_blinkNLine(QString(leaverNick) + " left channel.", "[Server]");
													};

												};
											};
										};
									}
									else if(strstr(comStr, "NICK :") != NULL)
									{
										char *temp;
										char *temp2;
										char senderNick[32] = {0};
										if(strstr(comStr, ":") != NULL) temp = strstr(comStr, ":");
										if(strstr(temp, "!") != NULL) temp2 = strstr(comStr, "!");
										int nickLen = temp2 - temp - 1;
										if(nickLen > 0) strncpy(senderNick, temp + 1, nickLen);

										if(strstr(comStr, "NICK :") != NULL) temp = strstr(comStr, "NICK :");
										memset(temp + strlen(temp), '\0', 1);

										QString newNick = QString((char*)(temp + strlen("NICK :")));

										ircTh->doEmitChangeYellowIRCData("[" + QString(senderNick) + "] is now known as [" + newNick + "].");

										_blinkNLine("[" + QString(senderNick) + "] is now known as [" + newNick + "].", "[Server]");

									}
									else if(iWantToConnect && (strstr(comStr, "JOIN :#") > 0 || strstr(comStr, "Join :#") > 0
										|| strstr(comStr, "join :#") > 0))
									{
										char *temp;
										char *temp2;
										char senderNick[32] = {0};
										if(strstr(comStr, ":") != NULL) temp = strstr(comStr, ":");
										if(strstr(temp, "!") != NULL) temp2 = strstr(comStr, "!");
										int nickLen = temp2 - temp;
										if(nickLen > 0) strncpy(senderNick, temp + 1, nickLen - 1);


										if(QString::fromLocal8Bit(senderNick) != ui->ircNickBox->text()) 
										{
											ircTh->doEmitChangeYellowIRCData("[" + QString(senderNick) + "] joined the channel.");
											_blinkNLine("[" + QString(senderNick) + "] joined the channel.", "[Server]");
										}
										else 
										{
											ircTh->doEmitChangeYellowIRCData("You have joined the channel.");
											_blinkNLine("You have joined the channel.", "[Server]");
											if(!OnlineMsgSentFlag) //Sending data only once per connect
											{	
												OnlineMsgSentFlag = true;

												char temp[64] = {0};
												strcpy(temp, "PRIV");
												strcat(temp, "MSG #");
												strcat(temp, IRC_CHAN);
												strcat(temp, " :My version: v3_");
												strcat(temp, gVER);
												strcat(temp, "\n");
												sendS(lSock, temp, strlen(temp), 0);

												connectedToIRC = true;
											};
										};

									}
									else if(iWantToConnect && (strstr(comStr, "PART #") > 0 || strstr(comStr, "Part #") > 0
										|| strstr(comStr, "part #") > 0))
									{
										char *temp;
										char *temp2;
										char senderNick[32] = {0};
										if(strstr(comStr, ":") != NULL) temp = strstr(comStr, ":");
										if(strstr(temp, "!") != NULL) temp2 = strstr(comStr, "!");
										int nickLen = temp2 - temp;
										if(nickLen > 0) strncpy(senderNick, temp + 1, nickLen - 1);


										if(QString::fromLocal8Bit(senderNick) != ui->ircNickBox->text()) ircTh->doEmitChangeYellowIRCData("[" + QString(senderNick) + "] left the channel.");
										else ircTh->doEmitChangeYellowIRCData("You have left the channel.");

									};
								}
								else if(strstri(comStr, privTemp) != NULL)
								{
									char channelName[64] = {0};
									strcpy(channelName, "PRIV");
									strcat(channelName, "MSG #");
									strcat(channelName, IRC_CHAN);
									strcat(channelName, " :");

									char *tprv = comStr;
									char *temp = NULL;
									char *temp2 = NULL;

									__pinger(recvBuff);		

									char senderNick[32] = {0};
									if(strstr(tprv, ":") != NULL) temp = strstr(tprv, ":");
									if(strstr(temp, "!") != NULL) temp2 = strstr(tprv, "!");
									if(temp != NULL && temp2 != NULL)
									{
										if(strlen(temp) > 0 && strlen(temp2) > 0)
										{
											int nickLen = temp2 - temp;
											if(nickLen <= 32)
											{
												char *tempD = strstri(tprv, channelName);
												int nsz = strlen(channelName);

												if(tempD == NULL) break;
												char *temp4 = NULL;
												int dsz = 0;
												if(strstr(tempD, "\n") != NULL) 
												{
													temp4 = strstr(tempD, "\n");
													dsz = temp4 - tempD - nsz - 1;
												};

												char tempData[512] = {0};
												if(temp4 != NULL)
												{
													strncpy(tempData, tempD + nsz, (dsz == 0 ? strlen(temp4) : dsz));
												}
												else strcpy(tempData, tempD + nsz);

												if(nickLen > 0) strncpy(senderNick, temp + 1, nickLen - 1);

												if(strlen(tempData) > 0)
												{
													QString strf;
													strf = QString::fromLocal8Bit(tempData);
													_blinkNLine(strf, QString::fromLocal8Bit(senderNick));
													ircTh->doEmitionPlayDckingSound();
													bool HLFlag = doHL(strf.toLocal8Bit().data());
                                                    int cCode = 0;
													ircTh->doEmitChangeIRCData(false, HLFlag, cCode, strf, " <a href=\"nesca:" + QString::fromLocal8Bit(senderNick) + "\"><font style=\"color:#" + GetNickColor(senderNick) + "\">[" + QString::fromLocal8Bit(senderNick) + "]:</font></a>");

												};
											};
										};
									};

									ZeroMemory(senderNick, sizeof(senderNick));

								}
								else if(strstri( comStr, QString(QString(pTemp) + " " + QString(ircNick)).toLocal8Bit().data() ) != NULL)
								{			
									char *tprv = comStr;
									char *temp = NULL;
									char *temp2 = NULL;
									char senderNick[32] = {0};
									if(strstr(tprv, ":") != NULL) temp = strstr(tprv, ":");
									if(strstr(temp, "!") != NULL) temp2 = strstr(tprv, "!");
									if(temp != NULL && temp2 != NULL)
									{
										if(strlen(temp) > 0 && strlen(temp2) > 0)
										{
											int nickLen = temp2 - temp;
											if(nickLen <= 32)
											{
												char *tempD = strstri(tprv, QString(QString(pTemp) + " " + QString(ircNick)).toLocal8Bit().data());
												int nsz = QString(ircNick).size() + 10;

												if(tempD == NULL) break;
												char *temp4 = NULL;
												int dsz = 0;
												if(strstr(tempD, "\n") != NULL) 
												{
													temp4 = strstr(tempD, "\n");
													dsz = temp4 - tempD - nsz - 1;
												};

												char tempData[512] = {0};
												if(temp4 != NULL)
												{
													strncpy(tempData, tempD + nsz, (dsz == 0 ? strlen(temp4) : dsz));
												}
												else strcpy(tempData, tempD + nsz);

												if(nickLen > 0) strncpy(senderNick, temp + 1, nickLen - 1);

												if(strlen(tempData) > 0)
												{
													QString strf;
													strf = QString::fromLocal8Bit(tempData);
													_blinkNLine(strf, QString::fromLocal8Bit(senderNick));
													ircTh->doEmitionPlayDckingSound();
													bool HLFlag = doHL(strf.toLocal8Bit().data());
													int cCode = 0;

													ircTh->doEmitChangeIRCData(true, HLFlag, cCode, strf, "<a href=\"nesca:" + QString::fromLocal8Bit(senderNick) + "\"><font style=\"color:#" + GetNickColor(senderNick) + "\">[" + QString::fromLocal8Bit(senderNick) + "]:</font></a>");

												};
											};
										};
									};
									ZeroMemory(senderNick, sizeof(senderNick));
								};
								ZeroMemory(comStr, sizeof(comStr));
						};
						ZeroMemory(recvBuffG, MAX_IRC_RECV_LEN);
					};
				};

				if(iWantToConnect == true)
				{

					ircTh->doEmitChangeRedIRCData("[-//-] IRC server went offline.");

					_blinkNLine("IRC server offlined!", "[Server]");
					Sleep(5000);
					connectedToIRC == false;
				};
			}
			else
			{

				if(proxyEnabledFlag) ircTh->doEmitChangeRedIRCData("[-//-] Cannot connect to proxy. (" + QString::number(WSAGetLastError()) + ")" );
				else ircTh->doEmitChangeRedIRCData("[-//-] Connection failed. (" + QString::number(WSAGetLastError()) + ")" );
				

			};
			CSSOCKET(lSock);
		};
	//};
	CSSOCKET(lSock);
};

void oIRC_Th::run()
{
	IRCLoop();
	ircTh->doEmitIRCOfflined();
};
