<<<<<<< HEAD
#ifndef DRAWERTH_GRIDQOSSCANNER_H
#define DRAWERTH_GRIDQOSSCANNER_H

#pragma once
#include "nesca_3.h"
#include "resource.h"

class DrawerTh_GridQoSScanner : public QThread
{
	Q_OBJECT

public:
public: signals: void sAddLine();

public:
	void doEmitAddLine();

protected:
	void run();
};

extern DrawerTh_GridQoSScanner *dtGridQoS;
#endif // DRAWERTH_GRIDQOSSCANNER_H
=======
#ifndef DRAWERTH_GRIDQOSSCANNER_H
#define DRAWERTH_GRIDQOSSCANNER_H

#pragma once
#include "nesca_3.h"
#include "resource.h"

class DrawerTh_GridQoSScanner : public QThread
{
	Q_OBJECT

public:
public: signals: void sAddLine();

public:
	void doEmitAddLine();

protected:
	void run();
};

extern DrawerTh_GridQoSScanner *dtGridQoS;
#endif // DRAWERTH_GRIDQOSSCANNER_H
>>>>>>> 5d30af667b7f24c6e91d2bf1888215f755b4378d
