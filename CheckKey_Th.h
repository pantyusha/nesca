#ifndef CHECKKEY_TH_H
#define CHECKKEY_TH_H

#include "STh.h"

extern int emitIfOK;
class CheckKey_Th : public QThread
{
public: static int isActiveKey;
	Q_OBJECT

protected:
	void run();
};

extern CheckKey_Th *chKTh;
#endif // CHECKKEY_TH_H
