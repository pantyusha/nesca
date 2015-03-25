#include "Threader.h"

int Threader::threadId = 0;
std::mutex Threader::m;
bool Threader::ready = false;
std::condition_variable Threader::cv;
std::queue<std::string> Threader::ipQueue;

void Threader::fireThread(std::string ip, void *func(void)) {

	std::unique_lock<std::mutex> lk(m);
	ipQueue.push(ip);
    if(threadId < gThreads) {
        ++threadId;
        std::thread workerThread(func);
        workerThread.detach();
    }

    ready = true;
    Threader::cv.notify_one();
    Sleep(gThreadDelay);
}

void Threader::cleanUp() {
	std::unique_lock<std::mutex> lk(m);
	lk.unlock();
	lk.release();
    threadId = 0;
}
