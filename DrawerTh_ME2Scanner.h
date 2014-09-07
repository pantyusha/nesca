<<<<<<< HEAD
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
=======
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
>>>>>>> 5d30af667b7f24c6e91d2bf1888215f755b4378d
