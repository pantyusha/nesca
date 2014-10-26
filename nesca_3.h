#ifndef nesca_3_H
#define nesca_3_H

#pragma once
#include <QMainWindow>
#include <qapplication.h>
#include <qgraphicsitem.h>
#include <QGraphicsView> 
#include <QGraphicsScene> 
#include <QGraphicsItem>
#include "ui_nesca_3.h"
#include <QSystemTrayIcon.h>
#include <QtGui\qevent.h>
#include <qthread.h>
#include <qdatetime.h>
#include <qjsonobject.h>
#include <qjsonvalue.h>
#include <qjsonarray.h>
#include <qtextcodec.h>
#include <qjsondocument.h>
#include <libssh\libssh.h>

extern Ui::nesca_3Class *ui;
extern bool widgetIsHidden, IRCLogToggled, BALogSwitched, blinkFlag, disableBlink, debugFileOK, QOSWait,
	connectedToIRC, ME2ScanFlag, QoSScanFlag, VoiceScanFlag, PieStatFlag, proxyEnabledFlag,
	smBit_8, smBit_7, smBit_6, smBit_5, smBit_4, smBit_3, smBit_2, smBit_1, printDelimiter;
extern float QoSStep;
extern int MaxDataVal;
extern char endIP2[128], inputStr[256];
extern SOCKET lSock;

extern QVector<int> vAlivLst, vAnomLst, vWFLst, vSuspLst, vLowlLst, vBALst, vSSHLst, vOvrlLst, actLst;
extern QVector<QPointF> vect;
extern QList<int> lstOfLabels;
extern QSystemTrayIcon *tray;

extern void ShowMsgPopup(QString str);

class nesca_3 : public QMainWindow
{
	Q_OBJECT

public:
	nesca_3(QWidget *parent = 0);
	~nesca_3();

	void ConnectEvrthng();
	void ChangeLabelIpRange_Value(QString str);
	void ChangeLabelIPS_Value(QString str);
	void newListItem(QString str);
	static int perc;
private:

protected:
	bool eventFilter(QObject* obj, QEvent *event);
	void run();
	
	QString GetSSLContent(QString str);
	void SSLConnect(QString str);
	protected slots:
		void MaxBrutingThr_ChangeValue(QString str);
		void ThreadDelay_ChangeValue(QString val);
		void ChangePingerOK(bool val);
		void changeNSTrackLabel(bool status);
		void slotPBUpdate();
		void DNSLine_ValueChanged(QString str);
		void slotChangeCPModeToUTF();
		void slotChangeCPModeTo1251();
		void slotShowRedVersion();
		void ChangeDebugFileState(bool val);
		void ChangeTopic();
		void slotIRCGetTopic(QString str);
		void slotIRCOfflined();
		void slotUnhidePopup(QString str, QString senderNick);
		void slotItemClicked(QListWidgetItem* wi);
		void slotClearNickList();
		void slotAppendIRCNick(QString str);
		void slotShowNicks();
		void slotBlinkMessage();
		void IPScanSeq();
		void DNSScanSeq();
		void ImportScanSeq();
		void smReaction();
		void slotShowDataflow();
		void slotOutData(QString ip, QString str);
		void slotIncData(QString ip, QString str);
		void slotShowServerMsg(QString str);
		void slotSaveImage(QAction *qwe);
		void slotUpdatePie();
		void slotClearLogs();
		void slotRestartIRC();
		void slotDrawVoiceGrid(int factor);
		void slotDrawTextPlacers();
		void onLinkClicked(QUrl link);
		void CheckPersKey();
		void CheckPersKey(int val);
		void slotRestoreDefPorts();
		void ChangeNick();
		void setNickBox(QString str);
		void SaySmthng();
		void ChangeIRCRawLog();
		void switchToJobMode();
		void ConnectToIRCServer();
		void CheckProxy();
		void ChangeDispalyMode();
		void switchDataFields();
		void importAndScan();
		void slotQoSAddGrid();
		void slotVoiceAddLine();
		void slotDrawDelimLines();
		void slotDrawActivityLine(QString data);
		void slotDrawActivityGrid();
		void activateME2ScanScene();
		void activateQoSScanBut();
		void activateVoiceScanBut();
		void activatePieStatBut();
		void slotAddPolyLine();
		void slotQoSAddLine();
		void slotAddLine(int x1, int y1, int x2, int y2);
		void slotDrawGrid();
		void exitButtonClicked();
		void trayButtonClicked();
		void mouseMoveEvent(QMouseEvent * event);
		void mousePressEvent(QMouseEvent *event);
		void mouseReleaseEvent(QMouseEvent *event);
		void logoLabelClicked();
		void startScanButtonClicked();
		void startScanButtonClickedDNS();
		void saveOptions();
		void ChangeTrackerOK(bool val);
		void ChangeLabelThreads_ValueChanged(QString);
		void PingTO_ChangeValue(QString);
		void ChangeLabelTO_ValueChanged(QString);
		void appendErrText(QString str);
		void appendOKText(QString str);
		void appendDebugText(QString str);
		void appendNotifyText(QString str);
		void appendDefaultText(QString str);
		void appendRedIRCText(QString str);
		void appendGreenIRCText(QString str);
		void appendYellowIRCText(QString str);
		void appendDefaultIRCText(bool, bool, int, QString str, QString s);
		void appendDefaultIRCTextOut(QString str);
		void appendDefaultIRCRawTextInc(QString str);
		void appendDefaultIRCRawTextOut(QString str);
		void appendGreenBAData(QString str);
		void appendRedBAData(QString str);
		void STTTerminate();
		void playFcknSound();
private:
	QPoint dragPosition;
};
class PieStatView : public QGraphicsView
{
	Q_OBJECT
public:
	PieStatView(QWidget *parent = 0) : QGraphicsView(parent) {};
public:
	void contextMenuEvent(QContextMenuEvent *event);
};
class PopupMsgWidget : public QWidget
{
	Q_OBJECT
public:
	PopupMsgWidget(QWidget* parent = 0) : QWidget(parent)
	{	};

public: signals: void clicked(bool checked = false);
protected:
		bool switchWindows;
        void mousePressEvent(QMouseEvent *evt)
        {
			switchWindows = false;
			if (evt->button() == Qt::LeftButton)
			{
				switchWindows = true;
				oldPos = evt->globalPos();
				evt->accept();
			}
			else if (evt->button() == Qt::RightButton)
			{
				disableBlink = true;
				ui->newMessageLabel->setStyleSheet("color:rgba(255, 0, 0, 0);background-color: rgba(2, 2, 2, 0);");	
				this->hide();
			};
        }
        void mouseMoveEvent(QMouseEvent *evt)
        {
			switchWindows = false;
            const QPoint delta = evt->globalPos() - oldPos;
            move(x()+delta.x(), y()+delta.y());
            oldPos = evt->globalPos();
        }	
        void mouseReleaseEvent(QMouseEvent *evt)
        {
			if(switchWindows)
			{
				switchWindows = false;
				emit ui->IRCModeBut->clicked();
			};
        }

    private:
        QPoint oldPos;
};

#endif // nesca_3_H

