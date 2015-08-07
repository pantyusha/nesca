#include "DrawerTh_QoSScanner.h"
#include "STh.h"
#include "externData.h"

int DrawerTh_QoSScanner::MaxDataVal = 1;

void DrawerTh_QoSScanner::run()
{
	while(QoSScanFlag)
    {
		if(stt->isRunning() == true && widgetIsHidden == false && tray->isVisible() == false && QOSWait == false) 
		{
			lstOfLabels.clear();
			lstOfLabels.append(Alive);
			lstOfLabels.append(camerasC1);
			//lstOfLabels.append(WF);
			lstOfLabels.append(other);
			//lstOfLabels.append(Lowl);
			lstOfLabels.append(baCount);
			lstOfLabels.append(Overl);
			lstOfLabels.append(ssh);
			
			QList<int> lstOfLabelsCopy = lstOfLabels;
            qSort(lstOfLabelsCopy.begin(), lstOfLabelsCopy.end(), qGreater<float>());
            if(lstOfLabelsCopy[0] > MaxDataVal) MaxDataVal = lstOfLabelsCopy[0];
			
			dtQoS->doEmitionAddLine();
		}
		else
		{
			msleep(500);
			camerasC1 = 0;
			//WF = 0;
			baCount = 0;
			filtered = 0;
			Overl = 0;
			//Lowl = 0;
			Alive = 0;
			other = 0;
			ssh = 0;
		};
		
		camerasC1 = 0;
		//WF = 0;
		baCount = 0;
		filtered = 0;
		Overl = 0;
		//Lowl = 0;
		Alive = 0;
		other = 0;
		ssh = 0;
		msleep(2000);
	};
}

void DrawerTh_QoSScanner::doEmitionAddLine()
{
	emit dtQoS->sAddLine();
}
