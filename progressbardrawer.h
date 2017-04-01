#pragma once
#ifndef PROGRESSBARDRAWER_H
#define PROGRESSBARDRAWER_H

#include "nesca_3.h"

class ProgressbarDrawer : public QThread
{
	Q_OBJECT

public: signals: void upd();
public:
	void update();
protected:
	void run();
};
extern ProgressbarDrawer *pbTh;

#endif // PROGRESSBARDRAWER_H
