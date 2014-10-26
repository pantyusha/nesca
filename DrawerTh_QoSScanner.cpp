#pragma once
#include "DrawerTh_QoSScanner.h"
#include "STh.h"
#include "externData.h"

int tMax;

void DrawerTh_QoSScanner::run()
{
	while(QoSScanFlag)
	{
		tMax = 0;
		
		if(stt->isRunning() == true && widgetIsHidden == false && tray->isVisible() == false && QOSWait == false) 
		{
			lstOfLabels.clear();
			lstOfLabels.append(Alive);
			lstOfLabels.append(AnomC1);
			lstOfLabels.append(WF);
			lstOfLabels.append(Susp);
			lstOfLabels.append(Lowl);
			lstOfLabels.append((int)BA);
			lstOfLabels.append(Overl);
			lstOfLabels.append(ssh);
			
			QList<int> lstOfLabelsCopy = lstOfLabels;
			qSort(lstOfLabelsCopy.begin(), lstOfLabelsCopy.end(), qGreater<float>());
			int curVal = lstOfLabelsCopy[0];
			if(curVal > MaxDataVal) MaxDataVal = curVal;
			if(curVal > tMax) tMax = curVal;
			
			dtQoS->doEmitionAddLine();
		}
		else
		{
			msleep(500);
			AnomC1 = 0;
			WF = 0;
			BA = 0;
			Filt = 0;
			Overl = 0;
			Lowl = 0;
			Alive = 0;
			Susp = 0;
			ssh = 0;
		};
		
		AnomC1 = 0;
		WF = 0;
		BA = 0;
		Filt = 0;
		Overl = 0;
		Lowl = 0;
		Alive = 0;
		Susp = 0;
		ssh = 0;
		msleep(2000);
	};
};

void DrawerTh_QoSScanner::doEmitionAddLine()
{
	emit dtQoS->sAddLine();
};