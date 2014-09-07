<<<<<<< HEAD
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
=======
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
>>>>>>> 5d30af667b7f24c6e91d2bf1888215f755b4378d
