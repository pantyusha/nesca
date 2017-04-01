#include "DrawerTh_HorNet.h"

void DrawerTh_HorNet::run()
{
	qsrand (QDateTime::currentMSecsSinceEpoch());
	int factor = 0;

	int gWidth = ui->graphicLog->width();
	int gHeight = ui->graphicLog->height();

	dtHN->doEmitionAddDelimLines();

	while(ME2ScanFlag)
	{
		if(widgetIsHidden == false && tray->isVisible() == false)
		{
			if (++factor > 9) factor = 0;
			dtHN->doEmitionDrawGrid();

			for(int i = 0; i < gHeight; i += 10) 
			{
				QApplication::processEvents();
				dtHN->doEmitionAddLine(0, i + factor, gWidth, i + factor);
				QApplication::processEvents();
			};
		};
		msleep(200);
	};
};

void DrawerTh_HorNet::doEmitionDrawGrid()
{
	emit dtHN->sDrawGrid();
};
void DrawerTh_HorNet::doEmitionAddLine(int x1, int y1, int x2, int y2)
{
	emit dtHN->sAddLine(x1, y1, x2, y2);
};
void DrawerTh_HorNet::doEmitionAddDelimLines()
{
	emit dtHN->sAddDelimLines();
};