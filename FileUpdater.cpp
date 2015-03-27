#include "FileUpdater.h"
#include "STh.h"
#include "mainResources.h"

long FileUpdater::oldNegLstSize = 0;
long FileUpdater::oldLoginLstSize = 0;
long FileUpdater::oldPassLstSize = 0;
long FileUpdater::oldSSHLstSize = 0;
long FileUpdater::oldWFLoginLstSize = 0;
long FileUpdater::oldWFPassLstSize = 0;
std::mutex FileUpdater::filesUpdatingMutex;
std::condition_variable FileUpdater::cv;
bool FileUpdater::ready = false;
std::unique_lock<std::mutex> FileUpdater::lk;

void ReadUTF8(FILE* nFile, char *cp) {
    char buffFG[256] = {0};
    int i = 0;
    GlobalNegativeSize = 0;

    if(nFile != NULL)
    {
        while(fgets((char*)buffFG, sizeof(buffFG), nFile) != NULL)
        {
            if(buffFG[0] != '#' && buffFG[0] != ' ' && buffFG[0] != '\n' && buffFG[0] != '\r' && strcmp(buffFG, "") != 0 &&
                ((buffFG[0] == '/' && buffFG[1] == '/') == false) && ((buffFG[0] == '\t' && buffFG[1] == '\t' && buffFG[2] == '\t' && (buffFG[3] == 13 || buffFG[3] == 10 || buffFG[3] == '#')) == false)
                && (buffFG[0] == '\t' && buffFG[1] == '\t' && buffFG[2] == '\t' && (buffFG[3] == '/' && buffFG[4] == '/')) == false)
            {
                ++GlobalNegativeSize;
            }
            ZeroMemory(buffFG, sizeof(buffFG));
        };

        rewind(nFile);
        if(strcmp(cp, "UTF") == 0) fseek(nFile, 3, 0);

        GlobalNegatives = new char*[GlobalNegativeSize + 2];

        while(fgets(buffFG, sizeof(buffFG), nFile) != NULL)
        {
            if(buffFG[0] == '#' || buffFG[0] == ' ' || buffFG[0] == '\n' || buffFG[0] == '\r' || strcmp(buffFG, "") == 0 ||
                (buffFG[0] == '/' && buffFG[1] == '/'))
            {
                ZeroMemory(buffFG, sizeof(buffFG));
                continue;
            };

            if(buffFG[0] == '\t' && buffFG[1] == '\t' && buffFG[2] == '\t')
            {
                char buffFGT[256] = {0};
                strcpy(buffFGT, buffFG);
                char *ptr1 = strstr(buffFGT, "\t\t\t");
                ZeroMemory(buffFG, sizeof(buffFG));
                strcpy(buffFG, ptr1 + 3);
            };

            int bSz = strlen(buffFG);
            if((bSz == 2 && buffFG[0] == 13 && buffFG[1] == 10) || (bSz == 1 && (buffFG[0] == 13 || buffFG[0] == 10)))
            {
                ZeroMemory(buffFG, sizeof(buffFG));
                continue;
            };
            if(buffFG[bSz] == 13 || buffFG[bSz] == 10)
            {
                buffFG[bSz] = '\0';
            };
            if(buffFG[bSz - 1] == 13 || buffFG[bSz - 1] == 10)
            {
                buffFG[bSz - 1] = '\0';
            };
            if(buffFG[bSz - 2] == 13 || buffFG[bSz - 2] == 10)
            {
                buffFG[bSz - 2] = '\0';
            };

            if(strstr((char*)buffFG, "\n") != 0)
            {
                std::string res;
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
                res = xcode(buffFG, CP_UTF8, CP_ACP);
#else
                res = std::string(buffFG);
#endif
                int sz = res.size();
                GlobalNegatives[i] = new char[sz + 1];
                ZeroMemory(GlobalNegatives[i], sizeof(*GlobalNegatives[i]));
                memcpy(GlobalNegatives[i], toLowerStr(res.c_str()).c_str(), sz - 1);
                memset(GlobalNegatives[i] + sz - 1, '\0', 1);
                ++i;
            }
            else
            {
                std::string res;
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
                res = xcode(buffFG, CP_UTF8, CP_ACP);
#else
                res = std::string(buffFG);
#endif
                int sz = res.size();
                GlobalNegatives[i] = new char[sz + 1];
                ZeroMemory(GlobalNegatives[i], sizeof(*GlobalNegatives[i]));
                memcpy(GlobalNegatives[i], toLowerStr(res.c_str()).c_str(), sz);
                memset(GlobalNegatives[i] + sz, '\0', 1);
                ++i;
            };

            unsigned char buffcpy2[256] = {0};
            int sz = strlen((char*)buffFG);
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
            strncpy((char*)buffcpy2, xcode(buffFG, CP_ACP, CP_UTF8).c_str(), sz);
#else
            strncpy((char*)buffcpy2, buffFG, sz);
#endif
            ZeroMemory(buffFG, sizeof(buffFG));
        };

        if(FileUpdater::oldNegLstSize == 0) stt->doEmitionGreenFoundData("Negative list loaded (" + QString::number(GlobalNegativeSize) + " entries)");
        else stt->doEmitionFoundData("<font color=\"Pink\">Negative list updated (" + QString::number(GlobalNegativeSize) + " entries)</font>");

        ZeroMemory(buffFG, sizeof(buffFG));
        fclose(nFile);
    }
    else
    {
        stt->doEmitionRedFoundData("No negative list found");
        stt->doEmitionKillSttThread();
    };
}

void negativeLoader() {
    FILE *nFile = fopen("negatives.txt", "rb");

    if( nFile != NULL)
    {
        unsigned char b[3] = {0};
        fread(b,1,2, nFile);
        if( b[0] == 0xEF && b[1] == 0xBB)
        {
            fread(b,1,1,nFile); // 0xBF
            ReadUTF8(nFile, "UTF");
        }
        else
        {
            rewind(nFile);
            ReadUTF8(nFile, "1251");
        };
    }
}

void *updateNegatives() {
    if(GlobalNegatives != NULL)
    {
        for(int i = 0; i < GlobalNegativeSize; ++i) delete []GlobalNegatives[i];
        delete []GlobalNegatives;
        GlobalNegatives = NULL;
    };

    negativeLoader();
}
void *updateLogin() {

    if(loginLst != NULL)
    {
        for(int i = 0; i < MaxLogin; ++i) delete []loginLst[i];
        delete []loginLst;
        loginLst = NULL;
    };

    MaxLogin = 0;

    char buffFG[32] = {0};

    FILE *loginList = fopen("login.txt", "r");

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
void *updatePass() {

    if(passLst != NULL)
    {
        for(int i = 0; i < MaxPass; ++i) delete []passLst[i];
        delete []passLst;
        passLst = NULL;
    };

    MaxPass = 0;

    char buffFG[32] = {0};

    FILE *passList = fopen("pass.txt", "r");

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
void *updateSSH() {

    if(sshlpLst != NULL)
    {
        for(int i = 0; i < MaxSSHPass; ++i) delete []sshlpLst[i];
        delete []sshlpLst;
        sshlpLst = NULL;
    };

    MaxSSHPass = 0;

    char buffFG[32] = {0};
    ZeroMemory(buffFG, sizeof(buffFG));

    FILE *sshlpList = fopen("sshpass.txt", "r");

    if(sshlpList != NULL)
    {
        while(fgets(buffFG, 32, sshlpList) != NULL)
        {
            ++MaxSSHPass;
            ZeroMemory(buffFG, sizeof(buffFG));
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
void *updateWFLogin() {

    if(wfLoginLst != NULL)
    {
        for(int i = 0; i < MaxWFLogin; ++i) delete []wfLoginLst[i];
        delete []wfLoginLst;
        wfLoginLst = NULL;
    };

    MaxWFLogin = 0;

    char buffFG[32] = {0};
    ZeroMemory(buffFG, sizeof(buffFG));

    FILE *wfLoginList = fopen("wflogin.txt", "r");

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
void *updateWFPass() {

    if(wfPassLst != NULL)
    {
        for(int i = 0; i < MaxWFPass; ++i) delete []wfPassLst[i];
        delete []wfPassLst;
        wfPassLst = NULL;
    };

    MaxWFPass = 0;

    char buffFG[32] = {0};
    ZeroMemory(buffFG, sizeof(buffFG));

    FILE *wfPassList = fopen("wfpass.txt", "r");

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

long getFileSize(const char *fileName) {
    std::ifstream in(fileName, std::ifstream::ate | std::ifstream::binary);
    return in.tellg();
}

void updateList(const char *fileName, long *szPtr, void *funcPtr(void)) {
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

int FileUpdater::updateLists() {
    while(globalScanFlag) {
        Sleep(60000);
        if(!globalScanFlag) break;
        loadOnce();
    }
}

int FileUpdater::loadOnce() {
    updateList("negatives.txt", &oldNegLstSize, updateNegatives);
    updateList("login.txt", &oldLoginLstSize, updateLogin);
    updateList("pass.txt", &oldPassLstSize, updatePass);
    updateList("sshpass.txt", &oldSSHLstSize, updateSSH);
    updateList("wflogin.txt", &oldWFLoginLstSize, updateWFLogin);
    updateList("wfpass.txt", &oldWFPassLstSize, updateWFPass);
}

void FileUpdater::FUClear() {
    oldNegLstSize = 0;
    oldLoginLstSize = 0;
    oldPassLstSize = 0;
    oldSSHLstSize = 0;
    oldWFLoginLstSize = 0;
    oldWFPassLstSize = 0;
}
