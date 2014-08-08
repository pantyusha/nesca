#pragma once
#include "IRCPinger_Th.h"

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
#pragma region QTGUI_Area
			ircPTh->doEmitChangeRedIRCData("-//- Ping timeout. Reconnecting... ");
#pragma endregion

			ircPTh->doEmitRestartIRC();
			globalPinger = 0;
			msleep(10000);
		};
		++globalPinger;
		msleep(1000);
	};
};