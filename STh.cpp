#include "STh.h"
#include "MainStarter.h"

//BA TablelistView
void STh::doEmitionChangeBARow(int index, QString loginPass, QString percentage)
{
	emit stt->signalChangeBARow(index, loginPass, percentage);
}

void STh::doEmitionShowRedVersion()
{
	emit stt->showRedVersion();	
}
void STh::doEmitionStartScanIP()
{
	emit stt->startScanIP();	
}
void STh::doEmitionStartScanDNS()
{
	emit stt->startScanDNS();		
}
void STh::doEmitionStartScanImport()
{
	emit stt->startScanImport();		
}
void STh::doEmitionAddIncData(QString(ip), QString str)
{
	emit stt->sIncData(ip, str);
}
void STh::doEmitionAddOutData( QString str)
{
	emit stt->sOutData(str);
}
void STh::doEmitionFoundData(QString str)
{
	emit stt->changeFoundData(str);
}

void STh::doEmitionRedFoundData(QString str)
{
	emit stt->changeRedFoundData(str);
}
void STh::doEmitionGreenFoundData(QString str)
{
	emit stt->changeGreenFoundData(str);
}
void STh::doEmitionFoundDataCustom(QString str, QString color)
{
	emit stt->foundDataCustom(str, color);
}
void STh::doEmitionYellowFoundData(QString str)
{
	emit stt->changeYellowFoundData(str);
}
void STh::doEmitionDebugFoundData(QString str)
{
	emit stt->changeDebugFoundData(str);
}
void STh::doEmitionKillSttThread()
{
	emit stt->killSttThread();	
}
void STh::doEmitionDataSaved(bool status)
{
    emit stt->signalDataSaved(status);
}
void STh::doEmitionUpdateArc(unsigned long gTargets)
{
	emit stt->signalUpdateArc(gTargets);
}
void STh::doEmitionBlockButton(bool value)
{
	emit stt->signalBlockButton(value);
}

void STh::setMode(short mode) {
	gMode = mode;
}
void STh::setTarget(QString target) {
	this->target = target;
}
void STh::setPorts(QString ports) {
	this->ports = ports;
}
void STh::run() 
{
	MainStarter ms;
	ms.start(this->target.toLocal8Bit().data(),
		this->ports.toLocal8Bit().data());
}
