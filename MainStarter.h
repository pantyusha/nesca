#ifndef MAINSTARTER_H
#define MAINSTARTER_H

#include <ctime>
#include <string>
#include <qjsonobject.h>
#include <qjsonvalue.h>
#include <qjsonarray.h>
#include <qjsondocument.h>
#include "externData.h"
#include "mainResources.h"
#include "STh.h"
#include "Threader.h"
#include "FileUpdater.h"
#include "Connector.h"
#include "Utils.h"
#include "IPRandomizer.h"
#include "HikvisionLogin.h"

class MainStarter {
private: char dnsTarget[256];
		 int ipsstart[4], ipsend[4];
		 static unsigned int **ipsstartfl, **ipsendfl;
		 unsigned long ip1, ip2;
		 static int gflIndex;

private:
	void startIPScan();
	void startDNSScan();
	void startImportScan();
	int loadPorts(const char *data, char delim);
	int loadTargets(const char *data);

public:
	static std::vector<int> portVector;
	static int flCounter;
	static bool savingBackUpFile;
	void saveBackupToFile();
	void saveBK();

public:
	MainStarter()
	{
		horLineFlag = false;
		PieCamerasC1 = 0, PieBA = 0, PieOther = 0, PieSSH = 0;
		camerasC1 = 0, baCount = 0, filtered = 0, Overl = 0, Alive = 0, Activity = 0, saved = 0, other = 0;
		BrutingThrds = 0;
		found = 0;
		gTargets = 0;
		cons = 0;
		found = 0;
		indexIP = 0;
		flCounter = 0;
		gflIndex = 0;
/*
		ZeroMemory(ipsstart, sizeof(ipsstart));
		ZeroMemory(ipsend, sizeof(ipsend));*/
		ipsstart[0] = 0;
		ipsend[0] = 0;
	}
	~MainStarter(){
		FileUpdater::FUClear();
		Threader::cleanUp();
		curl_global_cleanup();

		while (savingBackUpFile) Sleep(100);

		FileUpdater::negativeVector.clear();
		if (loginLst != NULL)
		{
			for (int i = 0; i < MaxLogin; ++i) delete[] loginLst[i];
			delete[] loginLst;
			loginLst = NULL;
		};
		if (passLst != NULL)
		{
			for (int i = 0; i < MaxPass; ++i) delete[] passLst[i];
			delete[] passLst;
			passLst = NULL;
		};
		if (wfPassLst != NULL)
		{
			for (int i = 0; i < MaxWFPass; ++i) delete[] wfPassLst[i];
			delete[] wfPassLst;
			wfPassLst = NULL;
		};
		if (wfLoginLst != NULL)
		{
			for (int i = 0; i < MaxWFLogin; ++i) delete[] wfLoginLst[i];
			delete[] wfLoginLst;
			wfLoginLst = NULL;
		};
		if (ftpPassLst != NULL)
		{
			for (int i = 0; i < MaxFTPPass; ++i) delete[] ftpPassLst[i];
			delete[] ftpPassLst;
			ftpPassLst = NULL;
		};
		if (ftpLoginLst != NULL)
		{
			for (int i = 0; i < MaxFTPLogin; ++i) delete[] ftpLoginLst[i];
			delete[] ftpLoginLst;
			ftpLoginLst = NULL;
		};
		if (sshlpLst != NULL)
		{
			for (int i = 0; i < MaxSSHPass; ++i) delete[] sshlpLst[i];
			delete[] sshlpLst;
			sshlpLst = NULL;
		};
		if (ipsstartfl != NULL)
		{
			for (int i = 0; i < flCounter; ++i) delete[] ipsstartfl[i];
			delete[] ipsstartfl;
			ipsstartfl = NULL;
		};
		if (ipsendfl != NULL)
		{
			for (int i = 0; i < flCounter; ++i) delete[] ipsendfl[i];
			delete[] ipsendfl;
			ipsendfl = NULL;
		};

		unBlockButtons();
	}

	void unBlockButtons();
	void runAuxiliaryThreads();
	void saver();
	int fileLoader(const char *fileName);
	static void createResultFiles();
	void start(const char* targets, const char* ports);
};

#endif // MAINSTARTER_H