#ifndef FILEUPDATER_H
#define FILEUPDATER_H

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
    static void updateLists();
    static void loadOnce();
    static void FUClear();
};

#endif // FILEUPDATER_H
