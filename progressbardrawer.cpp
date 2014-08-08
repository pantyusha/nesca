#include "progressbardrawer.h"
void ProgressbarDrawer::update()
{
	emit pbTh->upd();
};

int nesca_3::perc = 0;
void ProgressbarDrawer::run()
{
	while(globalScanFlag)
	{
		msleep(1000);
		nesca_3::perc = (unsigned long)100*indexIP/(gTargetsOverall == 0 ? 1 : gTargetsOverall);
		update();
	};
};