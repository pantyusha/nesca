#include <Threader.h>

std::vector<char*> Threader::threadPool;
std::vector<std::condition_variable*> cvPool;
std::vector<bool> Threader::readyPool;
int Threader::threadId = 0;

int Threader::getFreeDataSlotId() {
    int tps = threadPool.size();
    for(int i = 0; i != (gThreads > tps ? tps : gThreads); ++i) {
        if(threadPool[i] == NULL) return i;
    }
    return -1;
}

int Threader::getFreeThreadId() {
    int res;
    while((res = getFreeDataSlotId()) < 0) Sleep(50);
    return res;
}

void Threader::fireThread(char *res, void *func(int,std::condition_variable*)) {
    if(threadId < gThreads) {
        threadPool.push_back(res);
        std::condition_variable cv;
        cvPool.push_back(&cv);
        std::thread workerThread(func, threadId++, &cv);
        workerThread.detach();
        readyPool.push_back(true);
        cv.notify_one();
    } else {
        int id = getFreeThreadId();
        threadPool[id] = res;
        readyPool[id] = true;
        cvPool[id]->notify_one();
    }
}

void Threader::cleanUp() {
    threadId = 0;
    threadPool.clear();
    cvPool.clear();
}
