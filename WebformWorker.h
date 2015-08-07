#ifndef WEBFORMWORKER_H
#define WEBFORMWORKER_H

#include "Utils.h"
#include "Connector.h"
#include "mainResources.h"
#include "externFunctions.h"
#include "BruteUtils.h"
#include "STh.h"

class WFClass : BruteUtils {

private:
    int passCounter = 1;
    lopaStr doGetCheck(const char *ip, int port, char *actionVal, char *userVal, char *passVal, char *formVal);
	lopaStr doPostCheck(const char *ip, int port, char *actionVal, char *userVal, char *passVal, char *formVal);
    lopaStr parseResponse(const char *ip, const int port, const std::string *buffer, const char* formVal,
                          const char *login,
                          const char *pass);


public:
    WFClass(){
        if(gMaxBrutingThreads > 0) {
            while(BrutingThrds >= gMaxBrutingThreads) Sleep(700);

            //++WF;

			++BrutingThrds; 
			//BConInc();
            passCounter = 1;
        }
    }

    ~WFClass(){
        //BConDec();

		--BrutingThrds;
    }

    lopaStr _WFBrute(const char *ip,
                                int port,
                                char *methodVal,
                                char *actionVal,
                                char *userVal,
                                char *passVal,
                                char *formVal);
};

#endif // WEBFORMWORKER_H
