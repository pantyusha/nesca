#pragma once
#ifndef DRAWERTH_GRIDQOSSCANNER_H
#define DRAWERTH_GRIDQOSSCANNER_H

#include "STh.h"

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
