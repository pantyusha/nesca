#include <Threader.h>

std::vector<ThreadStruct> Threader::threadPool;

void Threader::createThreadPool(int poolSize, void *func, ST *st) {
    for(int i = 0; i < poolSize; ++i) {
        pthread_t thrc;
        pthread_create(&thrc, NULL, (void *(*)(void*))func, st);

        ThreadStruct threadStruct {
            &thrc,
            false
        };

        threadPool.push_back(threadStruct);
    }
}

void Threader::fireThread(ST *st) {

}
