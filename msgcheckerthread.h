#pragma once
#ifndef MSGCHECKERTHREAD_H
#define MSGCHECKERTHREAD_H

#include "nesca_3.h"

class MSGCheckerThread : public QThread
{
private: bool msgChkRunnning = false;
	Q_OBJECT
public:
	static void doEmitionShowNewMsg(QString str);
public: signals: void showNewMsg(QString);

protected:
	void run();
};
extern MSGCheckerThread *mct;

#endif // MSGCHECKERTHREAD_H
