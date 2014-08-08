#ifndef ACTIVITYDRAWERTH_HORNET_H
#define ACTIVITYDRAWERTH_HORNET_H

#pragma once
#include "nesca_3.h"

class ActivityDrawerTh_HorNet : public QThread
{
	Q_OBJECT

public: signals: void sDrawActivityLine();
public: signals: void sDrawGrid();

public:
	static void doEmitDrawActivityLine();
	static void doEmitDrawGrid();
protected:
	void run();
};
extern ActivityDrawerTh_HorNet *adtHN;
#endif // ACTIVITYDRAWERTH_HORNET_H
