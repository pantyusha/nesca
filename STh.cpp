#include "STh.h"

void STh::doEmitionShowRedVersion()
{
	emit stt->showRedVersion();	
};
void STh::doEmitionStartScanIP()
{
	emit stt->startScanIP();	
};
void STh::doEmitionStartScanDNS()
{
	emit stt->startScanDNS();		
};
void STh::doEmitionStartScanImport()
{
	emit stt->startScanImport();		
};
void STh::doEmitionAddIncData(QString(ip), QString str)
{
	emit stt->sIncData(ip, str);
};
void STh::doEmitionAddOutData(QString ip, QString str)
{
	emit stt->sOutData(ip, str);
};
void STh::doEmitionIPRANGE(QString str)
{
	emit stt->changeIpRange(str);
}
void STh::doEmitionThreads(QString str)
{
	emit stt->changeThreads(str);
}
void STh::doEmitionIPS(QString str)
{
	emit stt->changeIPS(str);
}
void STh::doEmitionFoundData(QString str)
{
	emit stt->changeFoundData(str);
}
void STh::doEmitionBAData(QString str)
{
	emit stt->changeBAData(str);
}
void STh::doEmition_BARedData(QString str)
{
	emit stt->changeRedBAData(str);	
};
void STh::doEmition_BAGreenData(QString str)
{
	emit stt->changeGreenBAData(str);
}
void STh::doEmitionRedFoundData(QString str)
{
	emit stt->changeRedFoundData(str);
}
void STh::doEmitionGreenFoundData(QString str)
{
	emit stt->changeGreenFoundData(str);
}
void STh::doEmitionYellowFoundData(QString str)
{
	emit stt->changeYellowFoundData(str);
}
void STh::doEmitionDebugFoundData(QString str)
{
	emit stt->changeDebugFoundData(str);
}
void STh::doEmitionChangeStatus(QString str)
{
	emit stt->changeStatus(str);
}
void STh::doEmitionTargetsLeft(QString str)
{
	emit stt->changeTargetsLeft(str);
}
void STh::doEmitionKillSttThread()
{
	emit stt->killSttThread();	
};
void STh::doEmitionChangeParsed(QString str)
{
	emit stt->changeParsedValue(str);	
};
void STh::doEmitionChangeBA(QString str)
{
	emit stt->changeBAValue(str);	
};
void STh::doEmitionOffline(QString str)
{
	emit stt->changeOffline(str);		
};
void STh::doEmitionDataSaved(bool status)
{
	emit stt->signalDataSaved(status);		
};
void STh::run() 
{
	startScan(inputStr);
}