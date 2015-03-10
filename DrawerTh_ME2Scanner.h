#ifndef DRAWERTH_ME2SCANNER_H
#define DRAWERTH_ME2SCANNER_H

#define ME2YPOS 100
#define ME2GRADPOS 10
#include "STh.h"

class DrawerTh_ME2Scanner : public QThread
{
	Q_OBJECT

public:
    static QList<QGraphicsPathItem*> itmList;
	static QVector<QPointF> polyVect;

	static void doEmitionAddPolyLine();
	static void doEmitDrawTextPlacers();

public: signals: void sAddPolyLine();
public: signals: void sDrawTextPlacers();

protected:
	void run();
};
extern DrawerTh_ME2Scanner *dtME2;
#endif // DRAWERTH_ME2SCANNER_H
