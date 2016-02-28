#ifndef CONNECTOR_H
#define CONNECTOR_H

#include "STh.h"
#include "BruteUtils.h"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#include <iphlpapi.h>
#include <icmpapi.h>
#pragma comment(lib, "iphlpapi.lib")
//#pragma comment(lib,"libcurldll.a")
#endif

#include <openssl/err.h>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#define MUTEX_TYPE            HANDLE
#define MUTEX_SETUP(x)        (x) = CreateMutex(NULL, FALSE, NULL)
#define MUTEX_CLEANUP(x)      CloseHandle(x)
#define MUTEX_LOCK(x)         WaitForSingleObject((x), INFINITE)
#define MUTEX_UNLOCK(x)       ReleaseMutex(x)
#define THREAD_ID             GetCurrentThreadId()
#else
#include <pthread.h>
#define MUTEX_TYPE            pthread_mutex_t
#define MUTEX_SETUP(x)        pthread_mutex_init(&(x), NULL)
#define MUTEX_CLEANUP(x)      pthread_mutex_destroy(&(x))
#define MUTEX_LOCK(x)         pthread_mutex_lock(&(x))
#define MUTEX_UNLOCK(x)       pthread_mutex_unlock(&(x))
#define THREAD_ID             pthread_self()
#endif


class Connector {

public:
    int nConnect(const char* ip, const int port, std::string *buffer,
                        const char *postData = NULL,
                        const std::vector<std::string> *customHeaders = NULL,
						const std::string *lpString = NULL,
						bool digestMode = false,
						bool isRTSP = false, bool isDigest = true);
    int connectToPort(char *ip, int port);
	int checkIsDigestRTSP(const char *ip, std::string *buffer);
};
#endif // CONNECTOR_H
