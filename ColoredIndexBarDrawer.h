#ifndef CIPBD_H
#define CIPBD_H

#pragma once
#include "STh.h"
#include "externData.h"

class ColoredIndexBarDrawer : public QThread
{
	Q_OBJECT

public: signals : void drawPointerPB2(int);
public:
	static void doEmitionDrawPointerPB2(int pointer);
protected:
	void run();
};
extern ColoredIndexBarDrawer *pbTh2;

#endif // CIPBD_H
