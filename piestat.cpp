#include "piestat.h"

void PieStat::doEmitUpdatePie()
{
	emit psTh->sUpdatePie();
};

void PieStat::run()
{
	while(PieStatFlag)
	{
		psTh->doEmitUpdatePie();
		AnomC1 = 0;
		WF = 0;
		BA = 0;
		Filt = 0;
		Overl = 0;
		Lowl = 0;
		Alive = 0;
		Susp = 0;
		ssh = 0;
		msleep(500);
	};
};
