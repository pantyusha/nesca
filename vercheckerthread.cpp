#include "vercheckerthread.h"
#include "externData.h"

void _checkVer()
{
	Connector con;
	conSTR CSTR;
	CSTR.lowerBuff = NULL;
	CSTR.size = 0;
	con._EstablishConnection("nesca.d3w.org", 80, "GET /version HTTP/1.1\r\nHost: nesca.d3w.org\r\nX-Nescav3: True\r\n\r\n", &CSTR, 1);

	char *ptr1 = NULL;
	if(CSTR.lowerBuff != NULL)
	{
		if(strstr(CSTR.lowerBuff, "\r\n\r\n") != 0)
		{
			ptr1 = strstr(CSTR.lowerBuff, "\r\n\r\n");
			if(strcmp(gVER, ptr1 + 4) != 0)
			{
				stt->doEmitionShowRedVersion();
			};
		};
		delete []CSTR.lowerBuff;
		CSTR.lowerBuff = NULL;
	};

	vct->terminate();
};

void VerCheckerThread::run()
{
	_checkVer();
};
