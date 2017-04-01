#include "FileUpdater.h"
#include "externFunctions.h"
#include "STh.h"
#include "mainResources.h"
#include "MainStarter.h"

char **loginLst, **passLst;
char **wfLoginLst, **wfPassLst;
char **ftpLoginLst, **ftpPassLst;
char **sshlpLst;

bool FileUpdater::running = false;
long FileUpdater::oldNegLstSize = 0;
long FileUpdater::oldLoginLstSize = 0;
long FileUpdater::oldPassLstSize = 0;
long FileUpdater::oldSSHLstSize = 0;
long FileUpdater::oldWFLoginLstSize = 0;
long FileUpdater::oldWFPassLstSize = 0;
long FileUpdater::oldFTPLoginLstSize = 0;
long FileUpdater::oldFTPPassLstSize = 0;
int FileUpdater::gNegativeSize = 0;
std::mutex FileUpdater::filesUpdatingMutex;
std::condition_variable FileUpdater::cv;
bool FileUpdater::ready = false;
std::unique_lock<std::mutex> FileUpdater::lk;
std::vector<std::string> FileUpdater::negativeVector;

void negativeLoader() {
	std::ifstream file(NEGATIVE_FN);
	std::string line;

	while (std::getline(file, line)) FileUpdater::negativeVector.push_back(line);
}

void updateNegatives() {
	FileUpdater::negativeVector.clear();
    negativeLoader();
}
void updateLogin() {

    if(loginLst != NULL)
    {
        for(int i = 0; i < MaxLogin; ++i) delete []loginLst[i];
        delete []loginLst;
        loginLst = NULL;
    };

    MaxLogin = 0;

    char buffFG[32] = {0};

    FILE *loginList = fopen(LOGIN_FN, "r");

    if(loginList != NULL)
    {
        while(fgets(buffFG, 32, loginList) != NULL)
        {
            MaxLogin++;
            ZeroMemory(buffFG, sizeof(buffFG));
        };

        rewind(loginList);

        loginLst = new char*[MaxLogin];

        for(int j = 0; j < MaxLogin; j++)
        {
            loginLst[j] = new char[32];
        };

        int i = 0;
        while(fgets(buffFG, 32, loginList) != NULL)
        {
            memset(loginLst[i], 0, strlen(buffFG) + 1);

            if(strstr(buffFG, "\n") != NULL) strncat(loginLst[i++], buffFG, strlen(buffFG) - 1);
            else strncat(loginLst[i++], buffFG, strlen(buffFG));
            ZeroMemory(buffFG, sizeof(buffFG));
        };

        if(FileUpdater::oldLoginLstSize == 0) stt->doEmitionGreenFoundData("Login list loaded (" + QString::number(MaxLogin) + " entries)");
        else stt->doEmitionFoundData("<font color=\"Pink\">Login list updated (" + QString::number(MaxLogin) + " entries)</font>");

        fclose(loginList);
    }
    else
    {
        stt->doEmitionRedFoundData("No login list found");
        stt->doEmitionKillSttThread();
    };
}
void updatePass() {

    if(passLst != NULL)
    {
        for(int i = 0; i < MaxPass; ++i) delete []passLst[i];
        delete []passLst;
        passLst = NULL;
    };

    MaxPass = 0;

    char buffFG[32] = {0};

    FILE *passList = fopen(PASS_FN, "r");

    if(passList != NULL)
    {
        while(fgets(buffFG, 32, passList) != NULL)
        {
            MaxPass++;
            ZeroMemory(buffFG, sizeof(buffFG));
        };

        rewind(passList);

        passLst = new char*[MaxPass];

        for(int j = 0; j < MaxPass; j++)
        {
            passLst[j] = new char[32];
        };

        int i = 0;
        while(fgets(buffFG, 32, passList) != NULL)
        {
            memset(passLst[i], 0, strlen(buffFG) + 1);

            if(strstr(buffFG, "\n") != NULL) strncat(passLst[i++], buffFG, strlen(buffFG) - 1);
            else  strncat(passLst[i++], buffFG, strlen(buffFG));
            ZeroMemory(buffFG, sizeof(buffFG));
        };

        if(FileUpdater::oldPassLstSize == 0) stt->doEmitionGreenFoundData("Password list loaded (" + QString::number(MaxPass) + " entries)");
        else stt->doEmitionFoundData("<font color=\"Pink\">Password list updated (" + QString::number(MaxPass) + " entries)</font>");

        fclose(passList);
    }
    else
    {
        stt->doEmitionRedFoundData("No password list found");
        stt->doEmitionKillSttThread();
    };
}
void updateSSH() {

    if(sshlpLst != NULL)
    {
        for(int i = 0; i < MaxSSHPass; ++i) delete []sshlpLst[i];
        delete []sshlpLst;
        sshlpLst = NULL;
    };

    MaxSSHPass = 0;

    char buffFG[32] = {0};

	FILE *sshlpList = fopen(SSH_PASS_FN, "r");

    if(sshlpList != NULL)
    {
        while(fgets(buffFG, 32, sshlpList) != NULL)
        {
            ++MaxSSHPass;
            ZeroMemory(buffFG, sizeof(buffFG));
			buffFG[0] = 0;
        };

        rewind(sshlpList);

        sshlpLst = new char*[MaxSSHPass];

        for(int j = 0; j < MaxSSHPass; j++)
        {
            sshlpLst[j] = new char[32];
        };

        int i = 0;
        while(fgets(buffFG, 32, sshlpList) != NULL)
        {
            memset(sshlpLst[i], 0, strlen(buffFG) + 1);

            if(strstr(buffFG, "\n") != NULL) strncat(sshlpLst[i++], buffFG, strlen(buffFG) - 1);
			else strncat(sshlpLst[i++], buffFG, strlen(buffFG));
			ZeroMemory(buffFG, sizeof(buffFG));
        };

        if(FileUpdater::oldSSHLstSize == 0) stt->doEmitionGreenFoundData("SSH Password list loaded (" + QString::number(MaxSSHPass) + " entries)");
        else stt->doEmitionFoundData("<font color=\"Pink\">SSH list updated (" + QString::number(MaxSSHPass) + " entries)</font>");

        fclose(sshlpList);
    }
    else
    {
        stt->doEmitionRedFoundData("No password/login list found");
        stt->doEmitionKillSttThread();
    };
}
void updateWFLogin() {

    if(wfLoginLst != NULL)
    {
        for(int i = 0; i < MaxWFLogin; ++i) delete []wfLoginLst[i];
        delete []wfLoginLst;
        wfLoginLst = NULL;
    };

    MaxWFLogin = 0;

    char buffFG[32] = {0};

    FILE *wfLoginList = fopen(WF_LOGIN_FN, "r");

    if(wfLoginList != NULL)
    {
        while(fgets(buffFG, 32, wfLoginList) != NULL)
        {
			MaxWFLogin++;
			ZeroMemory(buffFG, sizeof(buffFG));
        };

        rewind(wfLoginList);

        wfLoginLst = new char*[MaxWFLogin];

        for(int j = 0; j < MaxWFLogin; j++)
        {
            wfLoginLst[j] = new char[32];
        };

        int i = 0;
        while(fgets(buffFG, 32, wfLoginList) != NULL)
        {
            memset(wfLoginLst[i], 0, strlen(buffFG) + 1);

            if(strstr(buffFG, "\n") != NULL) strncat(wfLoginLst[i++], buffFG, strlen(buffFG) - 1);
			else strncat(wfLoginLst[i++], buffFG, strlen(buffFG));
			ZeroMemory(buffFG, sizeof(buffFG));
        };

        if(FileUpdater::oldWFLoginLstSize == 0) stt->doEmitionGreenFoundData("WFLogin list loaded (" + QString::number(MaxWFLogin) + " entries)");
        else stt->doEmitionFoundData("<font color=\"Pink\">WFLogin list updated (" + QString::number(MaxWFLogin) + " entries)</font>");

        fclose(wfLoginList);
    }
}
void updateWFPass() {

    if(wfPassLst != NULL)
    {
        for(int i = 0; i < MaxWFPass; ++i) delete []wfPassLst[i];
        delete []wfPassLst;
        wfPassLst = NULL;
    };

    MaxWFPass = 0;

    char buffFG[32] = {0};

	FILE *wfPassList = fopen(WF_PASS_FN, "r");

    if(wfPassList != NULL)
    {
        while(fgets(buffFG, 32, wfPassList) != NULL)
        {
			MaxWFPass++;
			ZeroMemory(buffFG, sizeof(buffFG));
        };

        rewind(wfPassList);

        wfPassLst = new char*[MaxWFPass];

        for(int j = 0; j < MaxWFPass; j++)
        {
            wfPassLst[j] = new char[32];
        };

        int i = 0;
        while(fgets(buffFG, 32, wfPassList) != NULL)
        {
            memset(wfPassLst[i], 0, strlen(buffFG) + 1);

            if(strstr(buffFG, "\n") != NULL) strncat(wfPassLst[i++], buffFG, strlen(buffFG) - 1);
			else strncat(wfPassLst[i++], buffFG, strlen(buffFG));
			ZeroMemory(buffFG, sizeof(buffFG));
        };

        if(FileUpdater::oldWFPassLstSize == 0) stt->doEmitionGreenFoundData("WFPassword list loaded (" + QString::number(MaxWFPass) + " entries)");
        else stt->doEmitionFoundData("<font color=\"Pink\">WFPassword list updated (" + QString::number(MaxWFPass) + " entries)</font>");

        fclose(wfPassList);
    }
}
void updateFTPLogin() {

	if (ftpLoginLst != NULL)
	{
		for (int i = 0; i < MaxFTPLogin; ++i) delete[]ftpLoginLst[i];
		delete[]ftpLoginLst;
		ftpLoginLst = NULL;
	};

	MaxFTPLogin = 0;

	char buffFG[32] = { 0 };

	FILE *ftpLoginList = fopen(FTP_LOGIN_FN, "r");

	if (ftpLoginList != NULL)
	{
		while (fgets(buffFG, 32, ftpLoginList) != NULL)
		{
			MaxFTPLogin++;
			ZeroMemory(buffFG, sizeof(buffFG));
		};

		rewind(ftpLoginList);

		ftpLoginLst = new char*[MaxFTPLogin];

		for (int j = 0; j < MaxFTPLogin; j++)
		{
			ftpLoginLst[j] = new char[32];
		};

		int i = 0;
		while (fgets(buffFG, 32, ftpLoginList) != NULL)
		{
			memset(ftpLoginLst[i], 0, strlen(buffFG) + 1);

			if (strstr(buffFG, "\n") != NULL) strncat(ftpLoginLst[i++], buffFG, strlen(buffFG) - 1);
			else strncat(ftpLoginLst[i++], buffFG, strlen(buffFG));
			ZeroMemory(buffFG, sizeof(buffFG));
		};

		if (FileUpdater::oldFTPLoginLstSize == 0) stt->doEmitionGreenFoundData("FTP login list loaded (" + QString::number(MaxFTPLogin) + " entries)");
		else stt->doEmitionFoundData("<font color=\"Pink\">FTP login list updated (" + QString::number(MaxFTPLogin) + " entries)</font>");

		fclose(ftpLoginList);
	}
}
void updateFTPPass() {

	if (ftpPassLst != NULL)
	{
		for (int i = 0; i < MaxFTPPass; ++i) delete[]ftpPassLst[i];
		delete[]ftpPassLst;
		ftpPassLst = NULL;
	};

	MaxFTPPass = 0;

	char buffFG[32] = { 0 };

	FILE *ftpPassList = fopen(FTP_PASS_FN, "r");

	if (ftpPassList != NULL)
	{
		while (fgets(buffFG, 32, ftpPassList) != NULL)
		{
			++MaxFTPPass;
			ZeroMemory(buffFG, sizeof(buffFG));
		};

		rewind(ftpPassList);

		ftpPassLst = new char*[MaxFTPPass];

		for (int j = 0; j < MaxFTPPass; j++)
		{
			ftpPassLst[j] = new char[32];
		};

		int i = 0;
		while (fgets(buffFG, 32, ftpPassList) != NULL)
		{
			memset(ftpPassLst[i], 0, strlen(buffFG) + 1);

			if (strstr(buffFG, "\n") != NULL) strncat(ftpPassLst[i++], buffFG, strlen(buffFG) - 1);
			else strncat(ftpPassLst[i++], buffFG, strlen(buffFG));
			ZeroMemory(buffFG, sizeof(buffFG));
		};

		if (FileUpdater::oldFTPPassLstSize == 0) stt->doEmitionGreenFoundData("FTP password list loaded (" + QString::number(MaxFTPPass) + " entries)");
		else stt->doEmitionFoundData("<font color=\"Pink\">FTP password list updated (" + QString::number(MaxFTPPass) + " entries)</font>");

		fclose(ftpPassList);
	}
}
long getFileSize(const char *fileName) {
    std::ifstream in(fileName, std::ifstream::ate | std::ifstream::binary);
    return in.tellg();
}

void updateList(const char *fileName, long *szPtr, void *funcPtr(void)) {
	if (!globalScanFlag) return;
    long sz = getFileSize(fileName);

    if(sz != *szPtr) {
        FileUpdater::lk = std::unique_lock<std::mutex> (FileUpdater::filesUpdatingMutex);
        funcPtr();
        *szPtr = sz;
        FileUpdater::lk.unlock();
        FileUpdater::ready = true;
        FileUpdater::cv.notify_one();
    }
}

void FileUpdater::updateLists() {
	running = true;
    while(globalScanFlag) {
        Sleep(30000);
        if(!globalScanFlag) break;
        loadOnce();
    }
	running = false;
}

void FileUpdater::loadOnce() {
	updateList(NEGATIVE_FN, &oldNegLstSize, (void*(*)(void))updateNegatives);
	updateList(LOGIN_FN, &oldLoginLstSize, (void*(*)(void))updateLogin);
	updateList(PASS_FN, &oldPassLstSize, (void*(*)(void))updatePass);
	updateList(SSH_PASS_FN, &oldSSHLstSize, (void*(*)(void))updateSSH);
	updateList(WF_LOGIN_FN, &oldWFLoginLstSize, (void*(*)(void))updateWFLogin);
	updateList(WF_PASS_FN, &oldWFPassLstSize, (void*(*)(void))updateWFPass);
	updateList(FTP_LOGIN_FN, &oldFTPLoginLstSize, (void*(*)(void))updateFTPLogin);
	updateList(FTP_PASS_FN, &oldFTPPassLstSize, (void*(*)(void))updateFTPPass);
}

void FileUpdater::FUClear() {
    running = false;
    oldNegLstSize = 0;
    oldLoginLstSize = 0;
    oldPassLstSize = 0;
	oldSSHLstSize = 0;
	oldWFLoginLstSize = 0;
	oldWFPassLstSize = 0;
	oldFTPLoginLstSize = 0;
	oldFTPPassLstSize = 0;
}
