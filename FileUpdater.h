#ifndef FILEUPDATER_H
#define FILEUPDATER_H

#ifndef Sleep
#define Sleep(msecs) usleep((msecs)*1000)
#endif

#include <fstream>
#include "externData.h"
#include "externFunctions.h"

class FileUpdater {
public:
    static long oldNegLstSize;
    static long oldLoginLstSize;
    static long oldPassLstSize;
    static long oldSSHLstSize;
    static long oldWFLoginLstSize;
    static long oldWFPassLstSize;
    static bool negUpdated;
    static bool lUpdated;
    static bool pUpdated;
    static bool wflUpdated;
    static bool wfpUpdated;
    static bool sshlpUpdated;

public:
    static int updateLists();
    static void passLoginLoader();
    static void negativeLoader();
};

#endif // FILEUPDATER_H
