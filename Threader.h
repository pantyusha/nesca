#ifndef THREADER_H
#define THREADER_H

#include <mainResources.h>

struct ThreadStruct{
    pthread_t *handler;
    bool busy;
};

class Threader {
private:
    static std::vector<ThreadStruct> threadPool;

public:
    static void createThreadPool(int poolSize, void *func, ST *st);
    static void fireThread(ST *st);
    static pthread_t getFreeThread();
};

#endif // THREADER_H
