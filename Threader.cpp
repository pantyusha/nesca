#include <Threader.h>

std::vector<char*> Threader::threadPool;
int Threader::threadPoolSize;
bool Threader::mayRun;
void *Threader::lFunc;

void Threader::recreateThreadPool(int poolSize) {
    if(mayRun) {
        createThreadPool(poolSize, (void* (*)(int))lFunc);
    }
}

void Threader::createThreadPool(int poolSize, void *func(int)) {
    cleanUp();
    lFunc = (void*)func;
    threadPoolSize = poolSize;
    for(int i = 0; i < threadPoolSize; ++i) {

        char *res = NULL;
        threadPool.push_back(res);
        std::thread thr12(func, i);
        thr12.detach();
//#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
//            _beginthread((void(*)(void*))_connect, 0, (void *)i);
//#else
//            pthread_t thrc;
//            pthread_create(&thrc, NULL, (void *(*)(void*))func, (void *)i);
//#endif
        Sleep(1);
    }
    mayRun = true;
}

int Threader::getFreeDataSlotId() {
    for(int i = 0; i != threadPoolSize; ++i) {
        if(threadPool[i] == NULL) return i;
    }
    return -1;
}

int Threader::getFreeThreadId() {
    int res;
    while((res = getFreeDataSlotId()) < 0) Sleep(10);
    return res;
}

void Threader::cleanUp() {
    mayRun = false;
    //for(int i = 0; i != threadPoolSize; ++i) {
        //if(threadPool[i] != NULL) delete threadPool[i];
    //}
    threadPool.clear();
}

void Threader::fireThread(char *res) {
    //while(!mayRun) sleep(10);
    threadPool[getFreeThreadId()] = res;
}
