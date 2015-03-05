#include "vercheckerthread.h"
#include "externData.h"
#include "mainResources.h"
#include "Connector.h"
#include "Utils.h"

void _checkVer()
{
    while(true) {
        char request[64] = {"http://nesca.d3w.org/version"};
        std::string buffer;
        std::vector<std::string> headerVector {"X-Nescav3: True"};
        Connector::nConnect(request, 80, &buffer, NULL, &headerVector);

        char *ptr1 = NULL;
        if(buffer.size() > 0)
        {
            if(Utils::ci_find_substr(buffer, std::string("\r\n\r\n")) != -1)
            {
                ptr1 = strstr((char*)buffer.c_str(), "\r\n\r\n");
                if(strcmp(gVER, ptr1 + 4) != 0)
                {
                    stt->doEmitionShowRedVersion();
                };
            };
        };

        vct->sleep(600000); //10 min
    };
    //vct->terminate();
}

void VerCheckerThread::run()
{
	_checkVer();
}
