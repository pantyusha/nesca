#ifndef FILEUPDATER_H
#define FILEUPDATER_H

#ifndef Sleep
#define Sleep(msecs) usleep((msecs)*1000)
#endif

#include <fstream>
#include <mutex>
#include <condition_variable>
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
    static bool ready;
    static std::condition_variable cv;
    static std::mutex filesUpdatingMutex;
    static std::unique_lock<std::mutex> lk;

public:
    static int updateLists();
    static void passLoginLoader();
    static void negativeLoader();
};

#endif // FILEUPDATER_H
