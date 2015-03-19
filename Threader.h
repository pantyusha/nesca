#ifndef THREADER_H
#define THREADER_H

#include <mainResources.h>
#include <externData.h>
#include <thread>
#include <chrono>
#include <future>
#include <mutex>
#include <condition_variable>

class Threader {

private:
public:
    static std::vector<char *> threadPool;
    static int threadId;
    static std::condition_variable cv;
    static std::vector<bool> readyPool;

private:
    static int getFreeDataSlotId();
    static int getFreeThreadId();
public:
    static void fireThread(char *st,
                           void* func(int,std::condition_variable*));
    static void cleanUp();
};

#endif // THREADER_H
