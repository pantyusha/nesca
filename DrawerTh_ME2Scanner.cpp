#include "DrawerTh_ME2Scanner.h"
#include "STh.h"
#include "externData.h"
#include <WebformWorker.h>

void DrawerTh_ME2Scanner::doEmitDrawTextPlacers()
{
	emit dtME2->sDrawTextPlacers();
}

void DrawerTh_ME2Scanner::doEmitionAddPolyLine()
{
	emit dtME2->sAddPolyLine();
}

void MakePolygonLine(int gWidth)
{
	vect.clear();
	int x = 0;
	int tx = 0;
	int xtx = 0;
	QPointF qp(0, 20);
	vect.append(qp);
	int fact1 = 0,
		fact2 = 0,
		fact3 = 0,
		fact4 = 0,
		fact5 = 0,
		fact6 = 0,
		fact7 = 0;

	bool state = stt->isRunning();
	for(int i = 1; i < 130; ++i)
	{
		x = qrand() % 4 + 2 + i;
		xtx = x + tx;
		if(xtx > 1 && xtx < 31) 
		{
			qp = QPointF(xtx, state ? qrand() % 3 - 3 + 20 - AnomC1*2 - fact1 : 20);
			if(AnomC1 > 0) 
			{
				if(xtx < 16 ) fact1+=2;
				else fact1-=2;
			};
		}

		if(xtx > 34 && xtx < 72) 
		{
            qp = QPointF(xtx, state ? qrand() % 3 - 3 + 20 - WF*2 - fact2 : 20);

			if(WF > 0) 
			{
				if(xtx < 52 ) fact2+=2;
				else fact2-=2;
			};
		}

		if(xtx > 74 && xtx < 112) 
		{
			qp = QPointF(xtx, state ? qrand() % 3 - 3 + 20 - BA*2 - fact3 : 20);

			if(BA > 0) 
			{
				if(xtx < 92 ) fact3+=2;
				else fact3-=2;
			};
		}

		if(xtx > 114 && xtx < 152) 
		{
			qp = QPointF(xtx, state ? qrand() % 3 - 3 + 20 - Susp*2 - fact4 : 20);

			if(Susp > 0) 
			{
				if(xtx < 132 ) fact4+=2;
				else fact4-=2;
			};
		}

		if(xtx > 154 && xtx < 192) 
		{
			qp = QPointF(xtx, state ? qrand() % 3 - 3 + 20 - Overl*2 - fact5 : 20);

			if(Overl > 0) 
			{
				if(xtx < 172 ) fact5+=2;
				else fact5-=2;
			};
		}

		if(xtx > 194 && xtx < 232) 
		{
			qp = QPointF(xtx, state ? qrand() % 3 - 3 + 20 - Lowl*2 - fact6 : 20);

			if(Lowl > 0) 
			{
				if(xtx < 212 ) fact6+=2;
				else fact6-=2;
			};
		}

		if(xtx > 234 && xtx < 269) 
		{
			qp = QPointF(xtx, state ? qrand() % 3 - 3 + 20 - Alive*2 - fact7 : 20);

			if(Alive > 0) 
			{
				if(xtx < 252 ) fact7+=2;
				else fact7-=2;
			};
		};

		vect.append(qp);
		tx = x;
	};
	AnomC1 = 0;
	WF = 0;
	BA = 0;
	Filt = 0;
	Overl = 0;
	Lowl = 0;
	Alive = 0;
	Susp = 0;
	vect.append(QPointF(gWidth, 20));
}

void DrawerTh_ME2Scanner::run()
{
	int gWidth = ui->graphicLog->width();
	dtME2->doEmitDrawTextPlacers();
	while(ME2ScanFlag)
	{
		if(widgetIsHidden == false && tray->isVisible() == false)
		{
			MakePolygonLine(gWidth);
			dtME2->doEmitionAddPolyLine();
		}
		else
		{
			msleep(1000);
			AnomC1 = 0;
			WF = 0;
			BA = 0;
			Filt = 0;
			Overl = 0;
			Lowl = 0;
			Alive = 0;
			Susp = 0;
		};
        msleep(90);
	};
}
