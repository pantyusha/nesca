#ifndef FILEUPDATER_H
#define FILEUPDATER_H

#include <fstream>
#include <mutex>
#include <condition_variable>
#include <vector>

class FileUpdater {
public:
	static bool running;
    static long oldNegLstSize;
    static long oldLoginLstSize;
    static long oldPassLstSize;
    static long oldSSHLstSize;
    static long oldWFLoginLstSize;
	static long oldWFPassLstSize;
	static long oldFTPLoginLstSize;
	static long oldFTPPassLstSize;

    static bool ready;
    static std::condition_variable cv;
    static std::mutex filesUpdatingMutex;
    static std::unique_lock<std::mutex> lk;
	static int gNegativeSize;
	static std::vector<std::string> negativeVector;

public:
    static void updateLists();
    static void loadOnce();
    static void FUClear();
};

#endif // FILEUPDATER_H
