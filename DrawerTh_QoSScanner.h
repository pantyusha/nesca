<<<<<<< HEAD
#ifndef DRAWERTH_QOSSCANNER_H
#define DRAWERTH_QOSSCANNER_H

#pragma once
#include "nesca_3.h"
#include "resource.h"

class DrawerTh_QoSScanner : public QThread
{
	Q_OBJECT

public:
	static void doEmitionAddLine();

public: signals: void sAddLine();

protected:
	void run();
};

extern DrawerTh_QoSScanner *dtQoS;
#endif // DRAWERTH_QOSSCANNER_H
=======
#ifndef DRAWERTH_QOSSCANNER_H
#define DRAWERTH_QOSSCANNER_H

#pragma once
#include "nesca_3.h"
#include "resource.h"

class DrawerTh_QoSScanner : public QThread
{
	Q_OBJECT

public:
	static void doEmitionAddLine();

public: signals: void sAddLine();

protected:
	void run();
};

extern DrawerTh_QoSScanner *dtQoS;
#endif // DRAWERTH_QOSSCANNER_H
>>>>>>> 5d30af667b7f24c6e91d2bf1888215f755b4378d
