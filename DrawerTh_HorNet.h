#ifndef DRAWERTH_HORNET_H
#define DRAWERTH_HORNET_H

#include "nesca_3.h"

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
