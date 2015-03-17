#include <BasicAuth.h>

bool BA::checkOutput(const string *buffer) {
    if(Utils::ci_find_substr(*buffer, "200 ok") != -1 ||
            Utils::ci_find_substr(*buffer, "http/1.0 200") != -1 ||
            Utils::ci_find_substr(*buffer, "http/1.1 200") != -1
            ) {

        return true;
    }

    return false;
}

lopaStr BA::_BABrute(const char *ip, const int port) {
    string buffer;
    string lpString;
    lopaStr lps;
    ZeroMemory(lps.login, sizeof(lps.login));
    ZeroMemory(lps.pass, sizeof(lps.pass));
    ZeroMemory(lps.other, sizeof(lps.other));

    strcpy(lps.login, "UNKNOWN");

    for(int i = 0; i < MaxLogin; ++i) {
        for (int j = 0; j < MaxPass; ++j) {
            if (!globalScanFlag) return lps;

            lpString = string(loginLst[i]) + ":" + string(passLst[j]);

            Connector::nConnect(ip, port, &buffer, NULL, NULL, &lpString);

            if(checkOutput(&buffer)) {
                strcpy(lps.login, loginLst[i]);
                strcpy(lps.pass, passLst[j]);
                return lps;
            };

            Sleep(70);
        }
    }

    return lps;
}

lopaStr BA::_BALobby(const char *ip, const int port) {
    while(BrutingThrds >= gMaxBrutingThreads) Sleep(1000);

    BruteUtils::BConInc();
    const lopaStr &lps = _BABrute(ip, port);
    BruteUtils::BConDec();

    return lps;
}
