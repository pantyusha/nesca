#include "ActivityDrawerTh_HorNet.h"
#include "STh.h"

void ActivityDrawerTh_HorNet::doEmitDrawActivityLine()
{
	emit adtHN->sDrawActivityLine();
};
void ActivityDrawerTh_HorNet::doEmitDrawGrid()
{
	emit adtHN->sDrawGrid();
};

void makeActLine(int val)
{
	if(actLst.size() < 50) actLst.push_back(val);
	else 
	{
		actLst.pop_front();
		actLst.push_back(val);
	};
};
void ActivityDrawerTh_HorNet::run()
{
	adtHN->doEmitDrawGrid();
	int maxAct = Activity + 1;
	while(true)
	{
		if(maxAct < Activity) maxAct = Activity;
		int nm = maxAct-=5;
		makeActLine(((float)Activity/(nm != 0 ? nm : 1)) * 10);
		stt->doEmitionSetActivityValue(QString::number(Activity) + "b");
		Activity = 0;
		adtHN->doEmitDrawActivityLine();
		msleep(130);
	};
};