#ifndef DRAWERTH_VOICESCANNER_H
#define DRAWERTH_VOICESCANNER_H

#pragma once
#include "nesca_3.h"
#include "resource.h"

class DrawerTh_VoiceScanner : public QThread
{
	Q_OBJECT

public:
public: signals: void sAddLine();
public: signals: void sDrawGrid();
public: signals: void sDrawTextPlacers();
public:
	void doEmitAddLine();
	void doEmitDrawGrid();
	void doEmitDrawTextPlacers();
protected:
	void run();
};
extern DrawerTh_VoiceScanner *vsTh;
#endif // DRAWERTH_VOICESCANNER_H
