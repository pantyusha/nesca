#ifndef WEBFORMWORKER_H
#define WEBFORMWORKER_H

#include <Utils.h>
#include "Connector.h"
#include <mainResources.h>
#include <externFunctions.h>
#include <BruteUtils.h>
#include "STh.h"

class WFClass : BruteUtils {

private: static bool active;
    int passCounter = 1;
    lopaStr doGetCheck(char *ip, int port, char *actionVal, char *userVal, char *passVal, char *formVal);
    lopaStr doPostCheck(char *ip, int port, char *actionVal, char *userVal, char *passVal, char *formVal);
    lopaStr parseResponse(const char *ip, const int port, const std::string *buffer, const char* formVal,
                          const int *iIndex,
                          const int *jIndex);


public:
    WFClass(){

        while(BrutingThrds >= gMaxBrutingThreads) Sleep(700);

        ++WF;
        BConInc();
        this->active = true;
        passCounter = 1;
    }

    ~WFClass(){
        this->active = false;
        BConDec();
    }

    lopaStr _WFBrute(char *ip,
                                int port,
                                char *methodVal,
                                char *actionVal,
                                char *userVal,
                                char *passVal,
                                char *formVal);
};

#endif // WEBFORMWORKER_H
