#include "DrawerTh_GridQoSScanner.h"

void DrawerTh_GridQoSScanner::doEmitAddLine()
{
	emit dtGridQoS->sAddLine();
};

void DrawerTh_GridQoSScanner::run()
{
	while(QoSScanFlag)
	{
		if(stt->isRunning() == true)
		{
			if(widgetIsHidden == false && tray->isVisible() == false) 
			{
				++QoSStep;
				emit dtGridQoS->doEmitAddLine();
			};
		};
		msleep(2000);
	};
};