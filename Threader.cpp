#include <Threader.h>

int Threader::threadId = 0;
std::mutex Threader::m;
bool Threader::ready = false;
std::condition_variable Threader::cv;
std::queue<std::string> Threader::ipQueue;

void Threader::fireThread(std::string ip, void *func(void)) {

    ipQueue.push(ip);
    if(threadId < gThreads) {
        ++threadId;
        std::thread workerThread(func);
        workerThread.detach();
    }

    std::unique_lock<std::mutex> lk(m);
    ready = true;
    Threader::cv.notify_one();
    Sleep(gThreadDelay);
}

//std::queue<std::string> empty;
void Threader::cleanUp() {
    //std::swap( ipQueue, empty );
    ipQueue = {};
    threadId = 0;
}
