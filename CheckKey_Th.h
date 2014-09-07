<<<<<<< HEAD
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
=======
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
>>>>>>> 5d30af667b7f24c6e91d2bf1888215f755b4378d
