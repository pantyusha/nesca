#include "progressbardrawer.h"
#include "externData.h"

ProgressbarDrawer *pbTh;

void ProgressbarDrawer::update()
{
	emit pbTh->upd();
};

int nesca_3::perc = 0;
void ProgressbarDrawer::run()
{
	globalScanFlag = true;
	while(globalScanFlag)
	{
		msleep(1000);
		nesca_3::perc = (unsigned long)100*indexIP/(gTargetsNumber == 0 ? 1 : gTargetsNumber);
		update();
	};
};