#ifndef NESCALOGICSTARTER_H
#define NESCALOGICSTARTER_H

class NLC {
public:
	NLC(){

	}
	~NLC(){
		/*FileUpdater::FUClear();
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
		if (starterIP != NULL)
		{
			for (int i = 0; i < flCounter; ++i) delete[]starterIP[i];
			delete[]starterIP;
			starterIP = NULL;
		};*/
	}
	int startScan(const char* args);

};

#endif // NESCALOGICSTARTER_H