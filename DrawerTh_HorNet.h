<<<<<<< HEAD
#ifndef DRAWERTH_HORNET_H
#define DRAWERTH_HORNET_H

#pragma once
#include "nesca_3.h"
#include "resource.h"

class DrawerTh_HorNet : public QThread
{
	Q_OBJECT

public:
	static void doEmitionDrawGrid();
	static void doEmitionAddLine(int x1, int y1, int x2, int y2);
	static void doEmitionAddDelimLines();

public: signals: void sAddDelimLines();
public: signals: void sAddLine(int, int, int, int);
public: signals: void sDrawGrid();

protected:
	void run();
};

extern DrawerTh_HorNet *dtHN;
#endif // DRAWERTH_HORNET_H
=======
#ifndef DRAWERTH_HORNET_H
#define DRAWERTH_HORNET_H

#pragma once
#include "nesca_3.h"
#include "resource.h"

class DrawerTh_HorNet : public QThread
{
	Q_OBJECT

public:
	static void doEmitionDrawGrid();
	static void doEmitionAddLine(int x1, int y1, int x2, int y2);
	static void doEmitionAddDelimLines();

public: signals: void sAddDelimLines();
public: signals: void sAddLine(int, int, int, int);
public: signals: void sDrawGrid();

protected:
	void run();
};

extern DrawerTh_HorNet *dtHN;
#endif // DRAWERTH_HORNET_H
>>>>>>> 5d30af667b7f24c6e91d2bf1888215f755b4378d
