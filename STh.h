#ifndef STH_H
#define STH_H

#pragma once
#include "nesca_3.h"
#include "resource.h"
#include "externFunctions.h"

extern int tMax;

class STh : public QThread
{
	Q_OBJECT
public:

public:
	static void doEmitionDataSaved(bool status);
	static void doEmitionStartScanIP();
	static void doEmitionStartScanDNS();
	static void doEmitionStartScanImport();
	static void doEmitionAddIncData(QString ip, QString str);
	static void doEmitionAddOutData(QString ip, QString str);
	static void doEmition_BAGreenData(QString str);
	static void doEmition_BARedData(QString str);
	static void doEmitionIPRANGE(QString str);
	static void doEmitionThreads(QString str);
	static void doEmitionIPS(QString str);
	static void doEmitionFoundData(QString str);
	static void doEmitionRedFoundData(QString str);
	static void doEmitionGreenFoundData(QString);
	static void doEmitionYellowFoundData(QString);
	static void doEmitionChangeStatus(QString);
	static void doEmitionTargetsLeft(QString);
	static void doEmitionKillSttThread();
	static void doEmitionChangeParsed(QString);
	static void doEmitionChangeBA(QString);
	static void doEmitionOffline(QString);
	static void doEmitionBAData(QString str);
	static void doEmitionDebugFoundData(QString);
	static void doEmitionShowRedVersion();

signals:
public: signals: void showRedVersion();
public: signals: void startScanIP();
public: signals: void startScanDNS();
public: signals: void startScanImport();
public: signals: void signalDataSaved(bool);
public: signals: void changeGreenBAData(QString);
public: signals: void changeRedBAData(QString);
public: signals: void changeBAData(QString);
public: signals: void changeOffline(QString);
public: signals: void changeBAValue(QString);
public: signals: void changeParsedValue(QString);
public: signals: void changeIpRange(QString);
public: signals: void changeThreads(QString);
public: signals: void changeIPS(QString);
public: signals: void changeFoundData(QString);
public: signals: void changeRedFoundData(QString);
public: signals: void changeGreenFoundData(QString);
public: signals: void changeYellowFoundData(QString);
public: signals: void changeDebugFoundData(QString);
public: signals: void changeStatus(QString);
public: signals: void changeTargetsLeft(QString);
public: signals: void killSttThread();
public: signals: void sIncData(QString, QString);
public: signals: void sOutData(QString, QString);

protected:
	void run();
};
extern STh *stt;
#endif // STH_H
