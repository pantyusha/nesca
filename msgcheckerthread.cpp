#include "msgcheckerthread.h"

void MSGCheckerThread::doEmitionShowNewMsg(QString str)
{
	emit mct->showNewMsg(str);
};

void _getNewMsg()
{
	Connector con;
	conSTR CSTR;
	CSTR.lowerBuff = NULL;
	CSTR.size = 0;
	char request[256] = {0};
	strcpy(request, "GET /mailbox?key=");
	strncat(request, trcPersKey, 32);
	strcat(request, " HTTP/1.1\r\nHost: nesca.d3w.org\r\nX-Nescav3: True\r\n\r\n");
	con._EstablishConnection("nesca.d3w.org", 80, request, &CSTR, 1);

	char *ptr1 = NULL;
	if(CSTR.lowerBuff != NULL)
	{
		if(strstr(CSTR.lowerBuff, "\r\n\r\n") != NULL && strstr(CSTR.lowerBuff, "HTTP/1.1 404 Not Found") == NULL && strstr(CSTR.lowerBuff, "HTTP/1.1 502 Bad Gateway") == NULL && strstr(CSTR.lowerBuff, "HTTP/1.1 400 Bad Request") == NULL && strstr(CSTR.lowerBuff, "\r\n\r\nEmpty") == NULL)
		{
			ptr1 = strstr(CSTR.lowerBuff, "\r\n\r\n");
			if(strlen(ptr1 + 4) != 0)
			{
				mct->doEmitionShowNewMsg(QString(ptr1 + 4));
			};
		};
		delete []CSTR.lowerBuff;
		CSTR.lowerBuff = NULL;
	};
};

void MSGCheckerThread::run() 
{
	for(;;)
	{
		Sleep(60000);
		_getNewMsg();
	};
};
