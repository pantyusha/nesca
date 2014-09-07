#ifndef IRCPINGER_TH_H
#define IRCPINGER_TH_H

#pragma once
#include "nesca_3.h"
#include "resource.h"
extern int iWantToConnect;

class IRCPinger_Th : public QThread
{
	Q_OBJECT

public:
	static void doEmitChangeRedIRCData(QString str);
	static void doEmitRestartIRC();

public: signals: void changeRedIRCData(QString);
public: signals: void RestartIRC();

protected:
	void run();
};
extern IRCPinger_Th *ircPTh;
#endif // IRCPINGER_TH_H
