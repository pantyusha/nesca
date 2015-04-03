#ifndef STH_H
#define STH_H

#pragma once
#include "nesca_3.h"
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
	static void doEmitionAddOutData(QString str);
	static void doEmition_BAGreenData(QString str);
    static void doEmition_BARedData(QString str);
	static void doEmitionFoundData(QString str);
	static void doEmitionRedFoundData(QString str);
	static void doEmitionGreenFoundData(QString);
	static void doEmitionYellowFoundData(QString);
    static void doEmitionChangeStatus(QString);
    static void doEmitionKillSttThread();
	static void doEmitionBAData(QString str);
	static void doEmitionDebugFoundData(QString);
	static void doEmitionShowRedVersion();
    static void doEmitionUpdateArc(unsigned long gTargets);

signals:
public: signals: void showRedVersion();
public: signals: void startScanIP();
public: signals: void startScanDNS();
public: signals: void startScanImport();
public: signals: void signalDataSaved(bool);
public: signals: void changeGreenBAData(QString);
public: signals: void changeRedBAData(QString);
public: signals: void changeBAData(QString);
public: signals: void changeFoundData(QString);
public: signals: void changeRedFoundData(QString);
public: signals: void changeGreenFoundData(QString);
public: signals: void changeYellowFoundData(QString);
public: signals: void changeDebugFoundData(QString);
public: signals: void changeStatus(QString);
public: signals: void killSttThread();
public: signals: void sIncData(QString, QString);
public: signals: void sOutData(QString);
public: signals: void signalUpdateArc(unsigned long);

protected:
	void run();
};
extern STh *stt;
#endif // STH_H
