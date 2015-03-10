#include "IRCPinger_Th.h"
#include "externData.h"

void IRCPinger_Th::doEmitChangeRedIRCData(QString str)
{
	emit ircPTh->changeRedIRCData(str);
};
void IRCPinger_Th::doEmitRestartIRC()
{
	emit ircPTh->RestartIRC();
};

void IRCPinger_Th::run()
{
	while(iWantToConnect)
	{
		if(globalPinger >= 360) //6min timeout
		{

			ircPTh->doEmitChangeRedIRCData("-//- Ping timeout. Reconnecting... ");


			ircPTh->doEmitRestartIRC();
			globalPinger = 0;
			msleep(10000);
		};
		++globalPinger;
		msleep(1000);
	};
};
