#include "DrawerTh_ME2Scanner.h"
#include "STh.h"
#include "externData.h"
#include "WebformWorker.h"
#include "MainStarter.h"

QList<QGraphicsPathItem*> DrawerTh_ME2Scanner::itmList;
QVector<QPointF> DrawerTh_ME2Scanner::polyVect;
int DrawerTh_ME2Scanner::vecSize;

void DrawerTh_ME2Scanner::doEmitDrawTextPlacers()
{
	emit dtME2->sDrawTextPlacers();
}

void DrawerTh_ME2Scanner::doEmitionAddPolyLine()
{
	emit dtME2->sAddPolyLine();
}

int MakePolygonLine(int gWidth)
{
	while (!DrawerTh_ME2Scanner::polyVect.empty()) Sleep(100);
    int x = 0;
    int tx = 0;
	int xtx = 0;
	QPointF qp(0, ME2YPOS);
	DrawerTh_ME2Scanner::polyVect.append(qp);
    int fact1 = 0,
		fact2 = 0,
		fact3 = 0,
		fact4 = 0,
		fact5 = 0,
		fact6 = 0,
		fact7 = 0;

	bool state = stt->isRunning();
	int activityVal = log(1 + Activity)/3 + 2;
	for(int i = 1; i < 136; ++i)
	{
        x = qrand() % 4 + i;
        xtx = x + tx;
		if(xtx > 1 && xtx < 31) 
		{
            qp = QPointF(xtx, state ? qrand() % activityVal + ME2YPOS - camerasC1 * 2 - fact1 : ME2YPOS);
			if (camerasC1 > 0)
			{
                if(xtx < 16 ) fact1+=2;
				else fact1-=2;
            };
		}

		if(xtx > 34 && xtx < 72) 
		{
            qp = QPointF(xtx, state ? qrand() % activityVal + ME2YPOS - /*WF*/0 * 2 - fact2 : ME2YPOS);

			if(/*WF*/0 > 0) 
			{
                if(xtx < 52 ) fact2+=2;
				else fact2-=2;
            };
		}

		if(xtx > 74 && xtx < 112) 
		{
			qp = QPointF(xtx, state ? qrand() % activityVal + ME2YPOS - baCount * 2 - fact3 : ME2YPOS);

			if (baCount > 0)
			{
                if(xtx < 92 ) fact3+=2;
				else fact3-=2;
            };
		}

		if(xtx > 114 && xtx < 152) 
		{
            qp = QPointF(xtx, state ? qrand() % activityVal + ME2YPOS - other * 2 - fact4 : ME2YPOS);

			if (other > 0)
			{
                if(xtx < 132 ) fact4+=2;
				else fact4-=2;
            };
		}

		if(xtx > 154 && xtx < 192) 
		{
            qp = QPointF(xtx, state ? qrand() % activityVal + ME2YPOS - Overl * 2 - fact5 : ME2YPOS);

			if(Overl > 0) 
			{
                if(xtx < 172 ) fact5+=2;
				else fact5-=2;
            };
		}

        if(xtx > 194 && xtx < 232)
		{
            qp = QPointF(xtx, state ? qrand() % activityVal + ME2YPOS - /*Lowl*/0 * 2 - fact6 : ME2YPOS);

			if(/*Lowl*/0 > 0) 
			{
                if(xtx < 212 ) fact6+=2;
				else fact6-=2;
            };
		}

        if(xtx > 234 && xtx < 278)
		{
            qp = QPointF(xtx, state ? qrand() % activityVal + ME2YPOS - Alive * 2 - fact7 : ME2YPOS);

			if(Alive > 0) 
			{
                if(xtx < 254 ) fact7+=1;
                else fact7-=1;
            };
		};

        DrawerTh_ME2Scanner::polyVect.append(qp);
        tx = x;
	};

	camerasC1 = 0;
	//WF = 0;
	baCount = 0;
	filtered = 0;
	Overl = 0;
	//Lowl = 0;
	Alive = 0;
	other = 0;

	DrawerTh_ME2Scanner::polyVect.append(QPointF(gWidth, ME2YPOS));
	return DrawerTh_ME2Scanner::polyVect.size();
}

void DrawerTh_ME2Scanner::run()
{
	int gWidth = ui->graphicLog->width();
	dtME2->doEmitDrawTextPlacers();

	while(ME2ScanFlag)
	{
		if (widgetIsHidden == false && tray->isVisible() == false)
		{
			if (itmList.count() < 39) {
				vecSize = MakePolygonLine(gWidth);
				dtME2->doEmitionAddPolyLine();
			}
		}
		else
		{
			msleep(1000);
			camerasC1 = 0;
			//WF = 0;
			baCount = 0;
			filtered = 0;
			Overl = 0;
			//Lowl = 0;
			Alive = 0;
			other = 0;
		};
        msleep(100);
	};

	itmList.clear();
}
