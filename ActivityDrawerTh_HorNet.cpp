#include "ActivityDrawerTh_HorNet.h"
#include "externData.h"

void ActivityDrawerTh_HorNet::doEmitDrawActivityLine(QString data)
{
	emit adtHN->sDrawActivityLine(data);
}

void ActivityDrawerTh_HorNet::doEmitDrawGrid()
{
	emit adtHN->sDrawGrid();
}

void makeActLine(int val)
{
	if(actLst.size() < 50) actLst.push_back(val);
	else 
	{
		actLst.pop_front();
		actLst.push_back(val);
	};
}

void ActivityDrawerTh_HorNet::run()
{
	adtHN->doEmitDrawGrid();
	int maxAct = Activity + 1;
	int nm = 0;
	while (true)
	{
		if (maxAct < Activity) maxAct = Activity;
		if (maxAct > 1000) nm = maxAct -= 1000;
		else if (maxAct > 10) nm = maxAct -= 10;

		makeActLine(((float)Activity / (nm != 0 ? nm : 1)) * 10);
		adtHN->doEmitDrawActivityLine(QString::number(Activity) + "b");
		Activity = 0;
		msleep(200);
	};
}
