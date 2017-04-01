#pragma once
#ifndef ACTIVITYDRAWERTH_HORNET_H
#define ACTIVITYDRAWERTH_HORNET_H

#include "STh.h"

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
