#ifndef THREADER_H
#define THREADER_H

#include "mainResources.h"
#include "externData.h"
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>

class Threader {

public:
    static int threadId;
    static std::mutex m;
    static bool ready;
    static std::condition_variable cv;
    static std::queue<std::string> ipQueue;
	static int gThreadDelay;

public:
    static void fireThread(std::string ip, void *func(void));
    static void cleanUp();
};

#endif // THREADER_H
