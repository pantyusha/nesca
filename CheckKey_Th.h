#ifndef CHECKKEY_TH_H
#define CHECKKEY_TH_H

#pragma once
#include "nesca_3.h"
#include "resource.h"
extern int emitIfOK;
class CheckKey_Th : public QThread
{
	Q_OBJECT

protected:
	void run();
};

extern CheckKey_Th *chKTh;
#endif // CHECKKEY_TH_H
