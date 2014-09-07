<<<<<<< HEAD
#ifndef ACTIVITYDRAWERTH_HORNET_H
#define ACTIVITYDRAWERTH_HORNET_H

#pragma once
#include "nesca_3.h"

class ActivityDrawerTh_HorNet : public QThread
{
	Q_OBJECT

public: signals: void sDrawActivityLine(QString);
public: signals: void sDrawGrid();

public:
	static void doEmitDrawActivityLine(QString data);
	static void doEmitDrawGrid();
protected:
	void run();
};
extern ActivityDrawerTh_HorNet *adtHN;
#endif // ACTIVITYDRAWERTH_HORNET_H
=======
#ifndef ACTIVITYDRAWERTH_HORNET_H
#define ACTIVITYDRAWERTH_HORNET_H

#pragma once
#include "nesca_3.h"

class ActivityDrawerTh_HorNet : public QThread
{
	Q_OBJECT

public: signals: void sDrawActivityLine(QString);
public: signals: void sDrawGrid();

public:
	static void doEmitDrawActivityLine(QString data);
	static void doEmitDrawGrid();
protected:
	void run();
};
extern ActivityDrawerTh_HorNet *adtHN;
#endif // ACTIVITYDRAWERTH_HORNET_H
>>>>>>> 5d30af667b7f24c6e91d2bf1888215f755b4378d
