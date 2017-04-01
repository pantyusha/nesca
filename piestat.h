#pragma once
#ifndef PIESTAT_H
#define PIESTAT_H

#include "nesca_3.h"

class PieStat : public QThread
{
	Q_OBJECT

public:
public: signals: void sUpdatePie();

public:
	void doEmitUpdatePie();
protected:
	void run();
};
extern PieStat *psTh;

#endif // PIESTAT_H
