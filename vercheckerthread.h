#pragma once
#ifndef VERCHECKERTHREAD_H
#define VERCHECKERTHREAD_H

#include "STh.h"

class VerCheckerThread : public QThread
{
	Q_OBJECT

public:
	
protected:
	void run();	
};

extern VerCheckerThread *vct;
#endif // VERCHECKERTHREAD_H
