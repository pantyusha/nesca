#ifndef DRAWERTH_ME2SCANNER_H
#define DRAWERTH_ME2SCANNER_H

#pragma once
#include "nesca_3.h"
#include "resource.h"

class DrawerTh_ME2Scanner : public QThread
{
	Q_OBJECT

public:
	static void doEmitionAddPolyLine();
	static void doEmitDrawTextPlacers();

public: signals: void sAddPolyLine();
public: signals: void sDrawTextPlacers();

protected:
	void run();
};
extern DrawerTh_ME2Scanner *dtME2;
#endif // DRAWERTH_ME2SCANNER_H
