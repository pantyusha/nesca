#include "piestat.h"
#include "externData.h"

void PieStat::doEmitUpdatePie()
{
	emit psTh->sUpdatePie();
};

void PieStat::run()
{
	while(PieStatFlag)
	{
		psTh->doEmitUpdatePie();
		camerasC1 = 0;
		//WF = 0;
		baCount = 0;
		filtered = 0;
		Overl = 0;
		//Lowl = 0;
		Alive = 0;
		other = 0;
		ssh = 0;
		msleep(500);
	};
};
