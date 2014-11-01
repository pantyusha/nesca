#ifndef CHECKPROXY_TH_H
#define CHECKPROXY_TH_H

#pragma once
#include "nesca_3.h"

class CheckProxy_Th : public QThread
{
	Q_OBJECT

public: signals: void changeRedIRCData(QString);
public: signals: void changeGreenIRCData(QString);
public: signals: void changeYellowIRCData(QString);
public: signals: void changeRawIRCDataInc(QString);
public: signals: void changeRawIRCDataOut(QString);

public:
	void doEmitChangeRedIRCData(QString str);
	void doEmitChangeGreenIRCData(QString str);
	void doEmitChangeYellowIRCData(QString str);
	void doEmitChangeRawIRCDataOut(QString str);
	void doEmitChangeRawIRCDataInc(QString str);
protected:
	void run();
};
extern CheckProxy_Th *chPTh;
#endif // CHECKPROXY_TH_H
