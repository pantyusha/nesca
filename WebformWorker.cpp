#include <WebformWorker.h>

lopaStr WFClass::parseResponse(const char *ip,
                               const int port,
                               const std::string *buffer,
                               const char* formVal,
                               const int *iIndex,
                               const int *jIndex) {

    lopaStr result = {"UNKNOWN", "UNKNOWN", "UNKNOWN"};

    if(buffer->size() != 0)
    {
        if(Utils::ci_find_substr(*buffer, std::string(formVal)) == -1
                && Utils::ci_find_substr(*buffer, std::string("denied")) == -1
                && Utils::ci_find_substr(*buffer, std::string("Location:")) == -1
                && Utils::ci_find_substr(*buffer, std::string("Authentication required")) == -1
                && Utils::ci_find_substr(*buffer, std::string("invalid")) == -1
                && Utils::ci_find_substr(*buffer, std::string("err")) == -1
                && Utils::ci_find_substr(*buffer, std::string(".href")) == -1
                && Utils::ci_find_substr(*buffer, std::string(".replace")) == -1
                && Utils::ci_find_substr(*buffer, std::string(".location")) == -1
                && Utils::ci_find_substr(*buffer, std::string("501 not implemented")) == -1
                && Utils::ci_find_substr(*buffer, std::string("http-equiv")) == -1
                && Utils::ci_find_substr(*buffer, std::string("busy")) == -1
                && Utils::ci_find_substr(*buffer, std::string("later")) == -1
                && Utils::ci_find_substr(*buffer, std::string("forbidden")) == -1
                ) {

            if(*iIndex == 0) return result;

            stt->doEmition_BAGreenData("[+] " + QString(ip) + ":" + QString::number(port) + " - WF pass: " +
                                       QString(wfLoginLst[*iIndex]) + ":" + QString(wfPassLst[*jIndex]));
            strcpy(result.login, wfLoginLst[*iIndex]);
            strcpy(result.pass, wfPassLst[*jIndex]);
            return result;

        } else {

            if(Utils::ci_find_substr(*buffer, std::string("501 not implemented")) != -1) stt->doEmitionRedFoundData("<a href=\"http://" + QString(ip) + ":" + QString::number(port) + "\"><font color=\"#c3c3c3\">" + QString(ip) + ":" + QString::number(port) + "</font></a> - [WF]: 501 Not Implemented.");

            if(Utils::ci_find_substr(*buffer, std::string("404 not found")) != -1) stt->doEmitionRedFoundData("<a href=\"http://" + QString(ip) + ":" + QString::number(port) + "\"><font color=\"#c3c3c3\">" + QString(ip) + ":" + QString::number(port) + "</font></a> - [WF]: 404 Not Found.");

            return result;
        }
    }
    else return result;
}

lopaStr WFClass::doGetCheck(char *ip,
                                  int port,
                                  char *actionVal,
                                  char *userVal,
                                  char *passVal,
                                  char *formVal) {

    lopaStr result = {"UNKNOWN", "UNKNOWN", "UNKNOWN"};
    int passCounter = 0;
    int firstCycle = 0;

    for(int i = 0; i < MaxWFLogin; ++i)
    {
        if(globalScanFlag == false) break;
        for(int j = firstCycle; j < MaxWFPass; ++j)
        {
            if(globalScanFlag == false) break;

            int rSize = strlen(ip) + strlen(actionVal) + strlen(userVal) + strlen(wfLoginLst[i]) + strlen(passVal) + strlen(wfPassLst[j]) + 4;

            if(rSize > 256) {
                stt->doEmitionRedFoundData("[WF] Wrong request size! (" + QString(ip) + ":" + QString::number(port) + ")");
                return result;
            };

            char nip[256] = {0};
            sprintf(nip, "%s%s?%s=%s&%s=%s", ip, actionVal, userVal, wfLoginLst[i], passVal, wfPassLst[j]);

            std::string buffer;
            Connector::nConnect(nip, port, &buffer);

            if(BALogSwitched) stt->doEmitionBAData("Checked WF: " + QString(ip) + ":" + QString::number(port) + "; login/pass: "+ QString(wfLoginLst[i]) + ":" + QString(wfPassLst[j]) + ";	- Progress: (" + QString::number((passCounter/(double)(MaxWFPass*MaxWFLogin)) * 100).mid(0, 4) + "%)");
            ++passCounter;

            result = parseResponse(ip, port, &buffer, formVal, &i, &j);
            if(i == 0) ++i;
        }
        firstCycle = 1;
    }

    return result;
}

lopaStr WFClass::doPostCheck(char *ip,
                                   int port,
                                   char *actionVal,
                                   char *userVal,
                                   char *passVal,
                                   char *formVal) {

    lopaStr result = {"UNKNOWN", "UNKNOWN", "UNKNOWN"};
    int passCounter = 0;
    int firstCycle = 0;

    for(int i = 0; i < MaxWFLogin; ++i)
    {
        if(globalScanFlag == false) break;
        for(int j = firstCycle; j < MaxWFPass; ++j)
        {
            if(globalScanFlag == false) break;

            int rSize = strlen(ip) + strlen(actionVal) + strlen(userVal) + strlen(wfLoginLst[i]) + strlen(passVal) + strlen(wfPassLst[j]) + 4;

            if(rSize > 256) {
                stt->doEmitionRedFoundData("[WF] Wrong request size! (" + QString(ip) + ":" + QString::number(port) + ")");
                return result;
            };

            char nip[256] = {0};
            char postData[256] = {0};
            sprintf(nip, "%s%s", ip, actionVal);
            sprintf(postData, "%s=%s&%s=%s", userVal, wfLoginLst[i], passVal, wfPassLst[j]);

            std::string buffer;
            Connector::nConnect(nip, port, &buffer, postData);

            if(BALogSwitched) stt->doEmitionBAData("Checked WF: " + QString(ip) + ":" + QString::number(port) + "; login/pass: "+ QString(wfLoginLst[i]) + ":" + QString(wfPassLst[j]) + ";	- Progress: (" + QString::number((passCounter/(double)(MaxWFPass*MaxWFLogin)) * 100).mid(0, 4) + "%)");
            ++passCounter;

            return parseResponse(ip, port, &buffer, formVal, &i, &j);
            if(i == 0) ++i;
        }
        firstCycle = 1;
    }

    return result;
}

lopaStr WFClass::_WFBrute( char *ip,
                                int port,
                                char *methodVal,
                                char *actionVal,
                                char *userVal,
                                char *passVal,
                                char *formVal) {

    lopaStr result = {"UNKNOWN", "UNKNOWN", "UNKNOWN"};

    if(strstri(methodVal, "get") != NULL) {
        result = doGetCheck(ip, port, actionVal, userVal, passVal, formVal);
    } else if(strstri(methodVal, "post") != NULL) {
        result = doPostCheck(ip, port, actionVal, userVal, passVal, formVal);
    } else {
        stt->doEmitionFoundData("<a href=\"http://" + QString(ip) +
                                ":" + QString::number(port)
                                + "\"><font color=\"#c3c3c3\">" +
                                QString(ip) + ":" + QString::number(port) +
                                "</font></a> - [WF]: Unknown method.");
    };

    return result;
}
