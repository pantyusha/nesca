#pragma once
#ifndef DRAWERTH_VOICESCANNER_H
#define DRAWERTH_VOICESCANNER_H

#include "nesca_3.h"

class DrawerTh_VoiceScanner : public QThread
{
	Q_OBJECT

public:
public: signals: void sAddLine();
public: signals: void sDrawGrid(int factor);
public: signals: void sDrawTextPlacers();
public:
	void doEmitAddLine();
	void doEmitDrawGrid(int factor);
	void doEmitDrawTextPlacers();
protected:
	void run();
};
extern DrawerTh_VoiceScanner *vsTh;
#endif // DRAWERTH_VOICESCANNER_H
