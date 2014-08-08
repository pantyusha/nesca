#ifndef DARKMAP_H
#define DARKMAP_H

#pragma once
#include "nesca_3.h"
#include "resource.h"

class DarkMap : public QThread
{
	Q_OBJECT

public:
	static int cou;
	static int qwmGrWidth;
	static int qwmGrHeight;
	static void doEmitionDrawText();

public: signals: void sDrawText();

public:
	void doEmitDrawText();

protected:
	void run();
};

extern DarkMap *dmTh;

#endif // DARKMAP_H
