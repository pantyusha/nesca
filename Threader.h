#ifndef THREADER_H
#define THREADER_H

#include <mainResources.h>
#include <thread>

//typedef struct {
//    char argv[MAX_ADDR_LEN] = {0};
//} ST;

class Threader {

private:
    static int threadPoolSize;
    static void* lFunc;
public:
    static std::vector<char *> threadPool;
    static bool mayRun;

private:
    static int getFree();
    static int getFreeDataSlotId();
    static int getFreeThreadId();
public:
    static void recreateThreadPool(int poolSize);
    static void createThreadPool(int poolSize, void *func(int));
    static void fireThread(char *st);
    static void cleanUp();
};

#endif // THREADER_H
