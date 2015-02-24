#ifndef VERCHECKERTHREAD_H
#define VERCHECKERTHREAD_H

#pragma once 
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
