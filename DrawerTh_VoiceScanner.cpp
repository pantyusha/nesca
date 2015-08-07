#include "DrawerTh_VoiceScanner.h"
#include "externData.h"

void DrawerTh_VoiceScanner::doEmitAddLine()
{
	emit vsTh->sAddLine();
};
void DrawerTh_VoiceScanner::doEmitDrawGrid(int factor)
{
	emit vsTh->sDrawGrid(factor);
};
void DrawerTh_VoiceScanner::doEmitDrawTextPlacers()
{
	emit vsTh->sDrawTextPlacers();
}

void makeVoiceLine(int Al, int An, int Bd, int Sp, int Lo, int var, int Ovl, int WF, int SSH)
{
	if(vAlivLst.size() < 140) 
	{
		if(Al > 0) vAlivLst.push_back((Al + 1)*10);
	}
	else 
	{
		vAlivLst.pop_front();
		if(Al > 0) vAlivLst.push_back((Al + 1)*10);
	};
	
	if(vAnomLst.size() < 140) 
	{
		if(An > 0) vAnomLst.push_back((An + 1)*10);
	}
	else 
	{
		vAnomLst.pop_front();
		if(An > 0) vAnomLst.push_back((An + 1)*10);
	};

	if(vWFLst.size() < 140) 
	{
		if(Bd > 0) vWFLst.push_back((Bd + 1)*10);
	}
	else 
	{
		vWFLst.pop_front();
		if(Bd > 0) vWFLst.push_back((Bd + 1)*10);
	};

	if(vSuspLst.size() < 140) 
	{
		if(Sp > 0) vSuspLst.push_back((Sp + 1)*10);
	}
	else 
	{
		vSuspLst.pop_front();
		if(Sp > 0) vSuspLst.push_back((Sp + 1)*10);
	};

	if(vLowlLst.size() < 140) 
	{
		if(Lo > 0) vLowlLst.push_back((Lo + 1)*10);
	}
	else 
	{
		vLowlLst.pop_front();
		if(Lo > 0) vLowlLst.push_back((Lo + 1)*10);
	};

	if(vLowlLst.size() < 140) 
	{
		if(var > 0) vBALst.push_back((var + 1)*10);
	}
	else 
	{
		vBALst.pop_front();
		if(var > 0) vBALst.push_back((var + 1)*10);
	};

	if(vOvrlLst.size() < 140) 
	{
		if(Ovl > 0) vOvrlLst.push_back((Ovl + 1)*10);
	}
	else 
	{
		vOvrlLst.pop_front();
		if(Ovl > 0) vOvrlLst.push_back((Ovl + 1)*10);
	};
		
	if(vSSHLst.size() < 140) 
	{
		if(SSH > 0) vSSHLst.push_back((SSH + 1)*10);
	}
	else 
	{
		vSSHLst.pop_front();
		if(SSH > 0) vSSHLst.push_back((SSH + 1)*10);
	};
};
void DrawerTh_VoiceScanner::run()
{
	vsTh->doEmitDrawGrid(150);
	vsTh->doEmitDrawTextPlacers();
	while(VoiceScanFlag)
	{
		if(widgetIsHidden == false && tray->isVisible() == false)
		{
			makeVoiceLine(Alive, camerasC1, 0, other, 0, baCount, Overl, 0, ssh);
			Alive = 0;
			camerasC1 = 0;
			//WF = 0;
			filtered = 0;
			//Lowl = 0;
			baCount = 0;
			Overl = 0;
			other = 0;
			ssh = 0;

			vsTh->doEmitAddLine();
			msleep(150);
		}
		else
		{
			msleep(500);
			Alive = 0;
			camerasC1 = 0;
			//WF = 0;
			filtered = 0;
			//Lowl = 0;
			baCount = 0;
			Overl = 0;
			other = 0;
			ssh = 0;
		};
	};
};
