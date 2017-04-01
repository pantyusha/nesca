#pragma once
#ifndef STH_H
#define STH_H

#include "nesca_3.h"
#include "externFunctions.h"
#include "MainStarter.h"
#include "externData.h"

extern int tMax;

class STh : public QThread
{
	Q_OBJECT
private:
	QString target = "";
	QString ports = "";
	QList<int> coloredIndexes;

public:
	void addColoredIndex(int index)
	{
		coloredIndexes.push_back(index);
	}
	QList<int> getColoredIndexes()
	{
		return coloredIndexes;
	}

	void setMode(short mode);
	void setTarget(QString target);
	void setPorts(QString ports);

	static int baModelSize();

	static void doEmitionDataSaved(bool status);
	static void doEmitionStartScanIP();
	static void doEmitionStartScanDNS();
	static void doEmitionStartScanImport();
	static void doEmitionAddIncData(QString ip, QString str);
	static void doEmitionAddOutData(QString str);

	static void doEmitionFoundData(QString str);
	static void doEmitionRedFoundData(QString str);
	static void doEmitionGreenFoundData(QString);
	static void doEmitionYellowFoundData(QString);
	static void doEmitionFoundDataCustom(QString, QString);
    static void doEmitionKillSttThread();

	static void doEmitionDebugFoundData(QString);
	static void doEmitionShowRedVersion();
	static void doEmitionUpdateArc(unsigned long gTargets);
	static void doEmitionBlockButton(bool value);
	//BA TablelistView
	static void doEmitionChangeBARow(int index, QString loginPass, QString percentage);

signals:
public: signals: void showRedVersion();
public: signals: void startScanIP();
public: signals: void startScanDNS();
public: signals: void startScanImport();
public: signals: void signalDataSaved(bool);
		
public: signals: void changeFoundData(QString);
public: signals: void changeRedFoundData(QString);
public: signals: void changeGreenFoundData(QString);
public: signals: void foundDataCustom(QString, QString);
public: signals: void changeYellowFoundData(QString);
public: signals: void changeDebugFoundData(QString);
public: signals: void killSttThread();
public: signals: void sIncData(QString, QString);
public: signals : void sOutData(QString);
public: signals : void signalUpdateArc(unsigned long);
public: signals : void signalBlockButton(bool);
		//BA TablelistView
public: signals : void signalChangeBARow(int, QString, QString);

protected:
	void run();
};
extern STh *stt;
#endif // STH_H
