#pragma once
#ifndef DRAWERTH_QOSSCANNER_H
#define DRAWERTH_QOSSCANNER_H

#include "nesca_3.h"

class DrawerTh_QoSScanner : public QThread
{
	Q_OBJECT

public:
	static int MaxDataVal;
public:
	static void doEmitionAddLine();

public: signals: void sAddLine();

protected:
	void run();
};

extern DrawerTh_QoSScanner *dtQoS;
#endif // DRAWERTH_QOSSCANNER_H
