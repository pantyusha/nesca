#ifndef MAINSTARTER_H
#define MAINSTARTER_H

#include <string>
#include "externData.h"
#include "mainResources.h"
#include "Threader.h"
#include "FileUpdater.h"

class MainStarter {
private: char dnsTarget[256];
		 int ipsstart[4], ipsend[4];
		 unsigned int **ipsstartfl = NULL, **ipsendfl = NULL;
		 unsigned long ip1, ip2;
		 int gflIndex = 0;

private:
	void startIPScan();
	void startDNSScan();
	void startImportScan();
	int loadPorts(const char *data, char delim);
	int loadTargets(const char *data);

public:
	static std::vector<int> portVector;
	static int flCounter;

public:
	MainStarter(short mode, 
		const char* targets,
		const char* ports,
		const char* tld = nullptr
		)
	{
		horLineFlag = false;
		PieAnomC1 = 0, PieWF = 0, PieBA = 0, PieSusp = 0, PieLowl = 0, PieSSH = 0;
		AnomC1 = 0, baCount = 0, Filt = 0, Overl = 0, Lowl = 0, Alive = 0, Activity = 0, saved = 0, Susp = 0,
			WF = 0, offlines = 0;
		BrutingThrds = 0;
		found = 0;
		gTargets = 0;
		cons = 0;
		found = 0;
		indexIP = 0;
		flCounter = 0;
		gflIndex = 0;

		ZeroMemory(ipsstart, sizeof(ipsstart));
		ZeroMemory(ipsend, sizeof(ipsend));

		if (loadTargets(targets) == -1 || 
			loadPorts(ports, ',') == -1) return;

		curl_global_init(CURL_GLOBAL_ALL);

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
		CreateDirectoryA(RESULT_DIR_NAME, NULL);
#else
		struct stat str = { 0 };
		if (stat(RESULT_DIR_NAME, &str) == -1) {
			mkdir(RESULT_DIR_NAME, 0700);
		}
#endif

	}
	~MainStarter(){
		FileUpdater::FUClear();
		Threader::cleanUp();
		curl_global_cleanup();

		while (__savingBackUpFile) Sleep(100);

		if (loginLst != NULL)
		{
			for (int i = 0; i < MaxLogin; ++i) delete[]loginLst[i];
			delete[]loginLst;
			loginLst = NULL;
		};
		if (passLst != NULL)
		{
			for (int i = 0; i < MaxPass; ++i) delete[]passLst[i];
			delete[]passLst;
			passLst = NULL;
		};
		if (GlobalNegatives != NULL)
		{
			for (int i = 0; i < GlobalNegativeSize; ++i) delete[]GlobalNegatives[i];
			delete[]GlobalNegatives;
			GlobalNegatives = NULL;
		};
		if (wfPassLst != NULL)
		{
			for (int i = 0; i < MaxWFPass; ++i) delete[]wfPassLst[i];
			delete[]wfPassLst;
			wfPassLst = NULL;
		};
		if (wfLoginLst != NULL)
		{
			for (int i = 0; i < MaxWFLogin; ++i) delete[]wfLoginLst[i];
			delete[]wfLoginLst;
			wfLoginLst = NULL;
		};
		if (sshlpLst != NULL)
		{
			for (int i = 0; i < MaxSSHPass; ++i) delete[]sshlpLst[i];
			delete[]sshlpLst;
			sshlpLst = NULL;
		};
		if (ipsstartfl != NULL)
		{
			for (int i = 0; i < flCounter; ++i) delete[]ipsstartfl[i];
			delete[]ipsstartfl;
			ipsstartfl = NULL;
		};
		if (ipsendfl != NULL)
		{
			for (int i = 0; i < flCounter; ++i) delete[]ipsendfl[i];
			delete[]ipsendfl;
			ipsendfl = NULL;
		};
	}

	void runAuxiliaryThreads();
	void saver();
	void saveBackupToFile();
	void fileLoader(const char *fileName);
	void start();
};

#endif // MAINSTARTER_H