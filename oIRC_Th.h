#ifndef OIRC_TH_H
#define OIRC_TH_H

#pragma once
#include "nesca_3.h"
#include "resource.h"
#include "irc_nmblinker.h"
#include "IRCPinger_Th.h"

class oIRC_Th : public QThread
{
	Q_OBJECT

public:
	void doEmitUnhidePopup(QString, QString);
	void doEmitChangeIRCData(bool, bool, int, QString, QString);
	void doEmitChangeRedIRCData(QString);
	void doEmitChangeGreenIRCData(QString);
	void doEmitChangeYellowIRCData(QString);
	void doEmitChangeRawIRCDataInc(QString);
	void doEmitChangeRawIRCDataOut(QString);
	void doEmitSetNick(QString);
	void doEmitAddNick(QString);
	void doEmitClearNickList();
	void doEmitIRCOfflined();
	void doEmitionPlayDckingSound();
	void doEmitGetTopic(QString str);

public: signals: void sUnhidePopup(QString, QString);
public: signals: void changeIRCData(bool, bool, int, QString, QString);
public: signals: void changeRedIRCData(QString);
public: signals: void changeGreenIRCData(QString);
public: signals: void changeYellowIRCData(QString);
public: signals: void changeRawIRCDataInc(QString);
public: signals: void changeRawIRCDataOut(QString);
public: signals: void setNick(QString);
public: signals: void AddNick(QString);
public: signals: void ClearNickList();
public: signals: void IRCOfflined();
public: signals: void notifyPlay();
public: signals: void getTopic(QString);

protected:
	void run();
};

extern oIRC_Th *ircTh;
#endif // OIRC_TH_H
