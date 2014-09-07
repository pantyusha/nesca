<<<<<<< HEAD
#include "DrawerTh_GridQoSScanner.h"
#include "STh.h"

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
=======
#include "DrawerTh_GridQoSScanner.h"
#include "STh.h"

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
>>>>>>> 5d30af667b7f24c6e91d2bf1888215f755b4378d
};