#pragma region Decls
#pragma once
#include <QGridLayout>
#include <QFileDialog>
#include "nesca_3.h"
#include "CheckKey_Th.h"
#include "DrawerTh_QoSScanner.h"
#include "oIRC_Th.h"
#include "IRCPinger_Th.h"
#include "CheckProxy_Th.h"
#include "STh.h"
#include "msgcheckerthread.h"
#include "vercheckerthread.h"
#include "DrawerTh_HorNet.h"
#include "ActivityDrawerTh_HorNet.h"
#include "DrawerTh_GridQoSScanner.h"
#include "DrawerTh_ME2Scanner.h"
#include "DrawerTh_VoiceScanner.h"
#include "piestat.h"
#include <QMenu>
#include <QGraphicsSceneContextMenuEvent>
#include <QDesktopWidget>
#include <QtMultimedia/qsound.h>
#include <QtMultimedia/qsoundeffect.h>
#include <qscrollbar.h>
#include <qdesktopservices.h>
#include <qmessagebox.h>
#include "progressbardrawer.h"
#include "externFunctions.h"
#include "externData.h"

QDate date = QDate::currentDate();
int ver = 100*(100*(date.year()%100) + date.month()) + date.day();
#define VER 130706
#define WIDGET_WIDTH 500
//#define IRC_CHAN "iskopasi_lab01"

bool utfIRCFlag = true;
bool HTMLDebugMode = false;
bool code160 = false;
bool gDebugMode = false;
bool gNegDebugMode = false;
bool smBit_1 = false;
bool smBit_2 = false;
bool smBit_3 = false;
bool smBit_4 = false;
bool smBit_5 = false;
bool smBit_6 = false;
bool smBit_7 = false;
bool smBit_8 = false;
bool debugFileOK = false;
bool privateMsgFlag = false;

char inputStr[256] = {0};
bool proxyEnabledFlag = false;
bool disableBlink = false;
char gVER[16] = {0};
int nickFlag;
int offlineFlag;
bool OnlineMsgSentFlag = false;
int globalPinger = 0;

bool dFlag = false;
bool startFlag = false;
bool trackerOK = true;
char ircServer[32] = {0};
char ircPort[32] = {0};
char ircProxy[64] = {0};
char ircProxyPort[8] = {0};
char ircNick[32] = {0};
char trcPort[32] = {0};
char trcSrvPortLine[32] = {0};
char trcProxy[128] = {0};
char trcSrv[256] = {0};
char trcScr[256] = {0};
char trcPersKey[32] = {0};

SOCKET lSock;

VerCheckerThread *vct = new VerCheckerThread();
MSGCheckerThread *mct = new MSGCheckerThread();
STh *stt = new STh();
DrawerTh_HorNet *dtHN = new DrawerTh_HorNet();
DrawerTh_ME2Scanner *dtME2 = new DrawerTh_ME2Scanner();
DrawerTh_QoSScanner *dtQoS = new DrawerTh_QoSScanner();
DrawerTh_GridQoSScanner *dtGridQoS = new DrawerTh_GridQoSScanner();
CheckProxy_Th *chPTh = new CheckProxy_Th();
IRCPinger_Th *ircPTh = new IRCPinger_Th();
oIRC_Th *ircTh = new oIRC_Th();
CheckKey_Th *chKTh = new CheckKey_Th();
ActivityDrawerTh_HorNet *adtHN = new ActivityDrawerTh_HorNet();
DrawerTh_VoiceScanner *vsTh = new DrawerTh_VoiceScanner();
IRC_NMBlinker *irc_nmb = new IRC_NMBlinker();
PieStat *psTh = new PieStat();
ProgressbarDrawer *pbTh = new ProgressbarDrawer();

bool MapWidgetOpened = false;
bool connectedToIRC = false;
bool globalScanFlag;
float QoSStep = 1;
int MaxDataVal = 1;
QGraphicsScene *sceneGrid;
QGraphicsScene *sceneGrid2;
QGraphicsScene *sceneGraph;
QGraphicsScene *sceneUpper;
QGraphicsScene *sceneActivity;
QGraphicsScene *sceneActivityGrid;
QGraphicsScene *sceneTextPlacer;
QGraphicsScene *sceneVoice;
QGraphicsScene *pbScene;
QGraphicsScene *jobRangeVisualScene;

QString importFileName = "";
	
QSystemTrayIcon *tray;
	QTextCodec *codec = QTextCodec::codecForName("Windows-1251");

QWidget *qwm;
QString globalIRCText;
QString globalIRCRaw;
PopupMsgWidget *msgWdgt;

QPen rpen(QColor(255, 0, 0, 190), 0.1, Qt::DotLine);
QPen pen(QColor(130, 130, 130), 0.1, Qt::DotLine);
QPen pen2(QColor(130, 130, 130), 0.5);
QPen penQoS0(QColor(255, 255, 255), 0.8);
QPen penQoS1(QColor(255, 0, 0), 0.9);
QPen penQoS2(QColor(0, 255, 0), 0.8);
QPen penQoS3(QColor(255, 85, 0), 0.8);	//orange
QPen penQoS4(QColor(255, 0, 255), 0.8);	//violet
QPen penQoS5(QColor(0, 0, 255), 0.8);
QPen penQoS6(QColor(82,180,229), 0.8);	//sky-blue
QPen penQoS7(QColor(85,0,0), 0.8);	//dRed
QPen penQoSWhite(QColor(255,255,255), 0.2);
QPen penQoSBlack(QColor(0,0,0), 0.3);
QPen penBlack(QColor(0,0,0));
QPen pen2i;

QVector<QGraphicsPathItem*> vectOld;
QList<int> lstOfLabels;
QVector<int> actLst;
QVector<int> vAlivLst;
QVector<int> vAnomLst;
QVector<int> vWFLst;
QVector<int> vSuspLst;
QVector<int> vLowlLst;
QVector<int> vBALst;
QVector<int> vSSHLst;
QVector<int> vOvrlLst;
QVector<QPointF> vect;

QMenu *menuPS;

QVector<QRect> NodeDots;
QVector<QString> NodeAddrs;

int u = 1;

const nesca_3 *gthis;
bool BALogSwitched = false;
bool widgetIsHidden = false;
bool blinkFlag = false;
bool printDelimiter = false;
bool IRCLogToggled = true;
int PhraseIndex = 0;
QList<QString> PhraseLog;
bool ME2ScanFlag = true, QoSScanFlag = false, VoiceScanFlag = false, PieStatFlag = false;

Ui::nesca_3Class *ui = new Ui::nesca_3Class;


void setSceneArea()
{
	delete ui->graphicsVoice;
	ui->graphicsVoice = new PieStatView(ui->widget);
	ui->graphicsVoice->setObjectName(QStringLiteral("graphicsVoice"));
	ui->graphicsVoice->setGeometry(QRect(220, 265, 270, 100));
	ui->graphicsVoice->setStyleSheet(QStringLiteral("border:1px solid white;background-color: rgba(26, 26,26, 0);"));
	ui->graphicsVoice->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	ui->graphicsVoice->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	ui->graphicsVoice->setRenderHints(QPainter::TextAntialiasing);
	ui->graphicsVoice->setCacheMode(QGraphicsView::CacheNone);
	ui->graphicsVoice->raise();

	sceneGrid = new QGraphicsScene();
	sceneGrid2 = new QGraphicsScene();
	sceneGraph = new QGraphicsScene();
	sceneUpper = new QGraphicsScene();
	sceneUpper->setSceneRect(0, 0, ui->graphicLog_Upper->width(), ui->graphicLog_Upper->height());
	sceneGrid->setSceneRect(0, 0, ui->graphicLog_Upper->width(), ui->graphicLog_Upper->height());

	sceneActivity = new QGraphicsScene();
	sceneActivityGrid = new QGraphicsScene();
	sceneTextPlacer = new QGraphicsScene();
	sceneVoice = new QGraphicsScene();
	pbScene = new QGraphicsScene();
	jobRangeVisualScene = new QGraphicsScene();

	ui->graphicLog->setScene(sceneGrid);
	ui->graphicLog_2->setScene(sceneGraph);
	ui->graphicDelim->setScene(sceneGrid2);
	ui->graphicLog_Upper->setScene(sceneUpper);
	ui->graphicActivity->setScene(sceneActivity);
	ui->graphicActivityGrid->setScene(sceneActivityGrid);
	ui->graphicTextPlacer->setScene(sceneTextPlacer);
	ui->graphicsVoice->setScene(sceneVoice);
	ui->pbgv->setScene(pbScene);
	ui->jobRangeVisual->setScene(jobRangeVisualScene);
	
	ui->graphicLog->setSceneRect(0, 0, ui->graphicLog->width(), ui->graphicLog->height());
	ui->graphicLog_2->setSceneRect(0, 0, ui->graphicLog_2->width(), ui->graphicLog_2->height());
	ui->graphicDelim->setSceneRect(0, 0, ui->graphicDelim->width(), ui->graphicDelim->height());
	ui->graphicLog_Upper->setSceneRect(0, 0, ui->graphicLog_Upper->width(), ui->graphicLog_Upper->height());
	ui->graphicActivity->setSceneRect(0, 0, ui->graphicActivity->width(), ui->graphicActivity->height());
	ui->graphicActivityGrid->setSceneRect(0, 0, ui->graphicActivityGrid->width(), ui->graphicActivityGrid->height());
	ui->graphicTextPlacer->setSceneRect(0, 0, ui->graphicTextPlacer->width(), ui->graphicTextPlacer->height());
	ui->graphicsVoice->setSceneRect(0, 0, ui->graphicsVoice->width(), ui->graphicsVoice->height());
	ui->pbgv->setSceneRect(0, 0, ui->pbgv->width(), ui->pbgv->height());	
	ui->jobRangeVisual->setSceneRect(0, 0, ui->jobRangeVisual->width(), ui->jobRangeVisual->height());	
}

void setButtonStyleArea()
{
	ui->checkKeyBut->setStyleSheet(
		" #checkKeyBut {"
		"background-color: qlineargradient(spread:none, x1:1, y1:0, x2:1, y2:1, stop:0.681818 rgba(0, 0, 0, 250), stop:1 rgba(255, 255, 255, 130));"
		"color: #00b304;"
		"border: 0.5px solid qlineargradient(spread:reflect, x1:0.54, y1:0.488591, x2:0.54, y2:0, stop:0 rgba(255, 255, 255, 130), stop:1 rgba(0, 0, 0, 255));"
		"}"

		" #checkKeyBut:hover{"
		"background-color: qlineargradient(spread:none, x1:1, y1:0, x2:1, y2:1, stop:0.681818 rgba(0, 0, 0, 250), stop:1 rgba(255, 255, 255, 130));"
		"color: #1efd00;"
		"border: 0.5px solid qlineargradient(spread:reflect, x1:0.54, y1:0.488591, x2:0.54, y2:0, stop:0 rgba(255, 255, 255, 130), stop:1 rgba(0, 0, 0, 255));"
		"}"
		);

	ui->IRCConnectBut->setStyleSheet(
		" #IRCConnectBut {"
		"background-color: qlineargradient(spread:none, x1:1, y1:0, x2:1, y2:1, stop:0.681818 rgba(0, 0, 0, 250), stop:1 rgba(255, 255, 255, 130));"
		"color: #919191;"
		"border: 0.5px solid qlineargradient(spread:reflect, x1:0.54, y1:0.488591, x2:0.54, y2:0, stop:0 rgba(255, 255, 255, 130), stop:1 rgba(0, 0, 0, 255));"
		"}"

		" #IRCConnectBut:hover{"
		"background-color: qlineargradient(spread:none, x1:1, y1:0, x2:1, y2:1, stop:0.681818 rgba(0, 0, 0, 250), stop:1 rgba(255, 255, 255, 130));"
		"color: #D1D1D1;"
		"border: 0.5px solid qlineargradient(spread:reflect, x1:0.54, y1:0.488591, x2:0.54, y2:0, stop:0 rgba(255, 255, 255, 130), stop:1 rgba(0, 0, 0, 255));"
		"}"
		);

	ui->IRCCheckProxyBut->setStyleSheet(
		" #IRCCheckProxyBut {"
		"background-color: qlineargradient(spread:none, x1:1, y1:0, x2:1, y2:1, stop:0.681818 rgba(0, 0, 0, 250), stop:1 rgba(255, 255, 255, 130));"
		"color: #bfb800;"
		"border: 0.5px solid qlineargradient(spread:reflect, x1:0.54, y1:0.488591, x2:0.54, y2:0, stop:0 rgba(255, 255, 255, 130), stop:1 rgba(0, 0, 0, 255));"
		"}"

		" #IRCCheckProxyBut:hover{"
		"background-color: qlineargradient(spread:none, x1:1, y1:0, x2:1, y2:1, stop:0.681818 rgba(0, 0, 0, 250), stop:1 rgba(255, 255, 255, 130));"
		"color: #e9e100;"
		"border: 0.5px solid qlineargradient(spread:reflect, x1:0.54, y1:0.488591, x2:0.54, y2:0, stop:0 rgba(255, 255, 255, 130), stop:1 rgba(0, 0, 0, 255));"
		"}"
		);
	ui->importButton->setStyleSheet(
		" #importButton {"
		"background-color: qlineargradient(spread:none, x1:1, y1:0, x2:1, y2:1, stop:0.681818 rgba(0, 0, 0, 250), stop:1 rgba(255, 255, 255, 130));"
		"color: #00b304;"
		"border: 0.5px solid qlineargradient(spread:reflect, x1:0.54, y1:0.488591, x2:0.54, y2:0, stop:0 rgba(255, 255, 255, 130), stop:1 rgba(0, 0, 0, 255));"
		"}"

		" #importButton:hover{"
		"background-color: qlineargradient(spread:none, x1:1, y1:0, x2:1, y2:1, stop:0.681818 rgba(0, 0, 0, 250), stop:1 rgba(255, 255, 255, 130));"
		"color: #1efd00;"
		"border: 0.5px solid qlineargradient(spread:reflect, x1:0.54, y1:0.488591, x2:0.54, y2:0, stop:0 rgba(255, 255, 255, 130), stop:1 rgba(0, 0, 0, 255));"
		"}"
		);
	ui->startScanButton_3->setStyleSheet(
		" #startScanButton_3 {"
		"background-color: qlineargradient(spread:none, x1:1, y1:0, x2:1, y2:1, stop:0.681818 rgba(0, 0, 0, 250), stop:1 rgba(255, 255, 255, 130));"
		"color: #919191;"
		"border: 0.5px solid qlineargradient(spread:reflect, x1:0.54, y1:0.488591, x2:0.54, y2:0, stop:0 rgba(255, 255, 255, 130), stop:1 rgba(0, 0, 0, 255));"
		"}"

		" #startScanButton_3:hover{"
		"background-color: qlineargradient(spread:none, x1:1, y1:0, x2:1, y2:1, stop:0.681818 rgba(0, 0, 0, 250), stop:1 rgba(255, 255, 255, 130));"
		"color: #D1D1D1;"
		"border: 0.5px solid qlineargradient(spread:reflect, x1:0.54, y1:0.488591, x2:0.54, y2:0, stop:0 rgba(255, 255, 255, 130), stop:1 rgba(0, 0, 0, 255));"
		"}"
		);

	ui->startScanButton_4->setStyleSheet(
		" #startScanButton_4 {"
		"background-color: qlineargradient(spread:none, x1:1, y1:0, x2:1, y2:1, stop:0.681818 rgba(0, 0, 0, 250), stop:1 rgba(255, 255, 255, 130));"
		"color: #919191;"
		"border: 0.5px solid qlineargradient(spread:reflect, x1:0.54, y1:0.488591, x2:0.54, y2:0, stop:0 rgba(255, 255, 255, 130), stop:1 rgba(0, 0, 0, 255));"
		"}"

		" #startScanButton_4:hover{"
		"background-color: qlineargradient(spread:none, x1:1, y1:0, x2:1, y2:1, stop:0.681818 rgba(0, 0, 0, 250), stop:1 rgba(255, 255, 255, 130));"
		"color: #D1D1D1;"
		"border: 0.5px solid qlineargradient(spread:reflect, x1:0.54, y1:0.488591, x2:0.54, y2:0, stop:0 rgba(255, 255, 255, 130), stop:1 rgba(0, 0, 0, 255));"
		"}"
		);
}

void setSomeStyleArea()
{
	qApp->setStyleSheet(
		"QMenu{"
		"color:rgb(216, 216, 216);background-color: rgb(26, 26, 26); border: 1px solid white;"
		"}"		

		".a{"
		"color:rgb(216, 216, 216);background-color: rgb(26, 26, 26); border: 1px solid white;"
		"}"

		"QMenu::item {"
		"padding: 2px 25px 2px 25px;"
		"border: 1px solid transparent;"
		"}"

		"QMenu::item:disabled {"
		"color:rgb(35, 35, 35);background-color: rgb(26, 26, 26); border: 1px solid white;"
		"padding: 2px 25px 2px 25px;"
		"border: 1px solid transparent;"
		"}"

		"QMenu::item:selected:enabled {"
		"color:rgb(255, 255, 255);background-color: rgb(26, 26, 26); border: 1px solid white;"
		"border-color: rgb(250, 170, 60);"
		"background: rgba(250, 170, 60, 50);"
		"}"

		"QMenu::item:selected:disabled {"
		"color:rgb(35, 35, 35);background-color: rgb(26, 26, 26); border: 1px solid white;"
		"border-color: rgb(250, 170, 60);"
		"background: rgba(250, 170, 60, 50);"
		"}"

		"QMenu::separator {"
		"height: 2px;"
		"margin-left: 25px;"
		"}"

		"QMenu::indicator {"
		"width: 13px;"
		"height: 13px;"
		"}"
		
		" #widget {"
		"border: 1px solid #525252;"
		"border-radius: 0px;"
		"}"
		
		" #exitButton {"
		"color: #525252;"
		"border: 0px solid black;"
		"}"

		" #exitButton:hover {"
		"color: red;"
		"border: 0px solid black;"
		"}"

		" #exitButton:pressed {"
		"color: maroon;"
		"border: 0px solid black;"
		"}"

		" #trayButton {"
		"color: #525252;"
		"border: 0px solid black;"
		"}"

		" #trayButton:hover {"
		"color: red;"
		"border: 0px solid black;"
		"}"

		" #trayButton:pressed {"
		"color: maroon;"
		"border: 0px solid black;"
		"}"

		" #exitButton_2 {"
		"color: #525252;"
		"border: 0px solid black;"
		"}"

		" #exitButton_2:hover {"
		"color: red;"
		"border: 0px solid black;"
		"}"

		" #exitButton_2:pressed {"
		"color: maroon;"
		"border: 0px solid black;"
		"}"

		" QLineEdit {"
		"border: 1px solid #525252 !important;"
		"border-radius: 0px !important;"
		"padding: 0 8px !important;"
		"background: yellow !important;"
		"selection-background-color: darkgray !important;"
		"}"

		"QTabBar::tab {"
		"color: white;"
		"border: 1px solid #C4C4C3;"
		"border-bottom-color: #C2C7CB;"
		"border-top-left-radius: 4px;"
		"border-top-right-radius: 4px;"
		"padding: 1px 3px;"
		"margin-left: 1px;"
		"margin-bottom: 4px;"
		"}"

		"QTabBar::tab:selected {"
		"color: black;"
		"background-color: #505050;"
		"}"

		"QCheckBox::indicator:unchecked"
		"{"
		"background-color:black;"
		"border: 1px solid #313131;"
		"}"

		"QCheckBox::indicator:checked"
		"{"
		"background-color: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.500318, stop:0.107955 rgba(255, 255, 255, 255), stop:0.204545 rgba(255, 255, 255, 0));"
		"border: 1px solid #313131;"
		"}"
		);
	setButtonStyleArea();
}

void SetValidators()
{
	QRegExpValidator *validator = new QRegExpValidator(
		QRegExp("([\\d*|.|//|-])+"), 
		NULL
		);
	ui->ipLine->setValidator(validator);

	validator = new QRegExpValidator(QRegExp("(\\d{1,5}[,|-]\\s{0,1})+"), NULL);
	ui->portLine->setValidator(validator);
	ui->lineEditPort->setValidator(validator);

	validator = new QRegExpValidator(QRegExp("\\d{1,3}"), NULL);
	ui->importThreads->setValidator(validator);
	ui->threadLine->setValidator(validator);
	ui->lineEditThread->setValidator(validator);
	ui->iptoLine_value->setValidator(validator);
	ui->iptoLine_value_2->setValidator(validator);
	ui->iptoLine_value_3->setValidator(validator);
	ui->maxBrutingThrBox->setValidator(validator);

	validator = new QRegExpValidator(QRegExp("\\d{1,5}"), NULL);
	ui->PingTO->setValidator(validator);
	ui->threadDelayBox->setValidator(validator);
	
	validator = new QRegExpValidator(QRegExp("(\\w|-|\\.|\\[|\\]|\\\\)+"), NULL);
	ui->lineEditStartIPDNS->setValidator(validator);

	validator = new QRegExpValidator(QRegExp("(\\w|-|\\.)+((\\w|-|\\.)+)+"), NULL);
	ui->lineILVL->setValidator(validator);

	validator = new QRegExpValidator(QRegExp("\\d{1,5}"), NULL);
	ui->serverPortBox->setValidator(validator);
	ui->ircProxyPort->setValidator(validator);
	ui->trcSrvPortLine->setValidator(validator);
	
	validator = new QRegExpValidator(QRegExp("[a-zA-Z0-9]{32}"), NULL);
	ui->linePersKey->setValidator(validator);
}

void nesca_3::slotDrawTextPlacers()
{
	QFont fnt;
	fnt.setFamily("Eurostile");
	if(ME2ScanFlag)
	{
        delete sceneTextPlacer;
		sceneTextPlacer = NULL;
		sceneTextPlacer = new QGraphicsScene();
		ui->graphicTextPlacer->setScene(sceneTextPlacer);

		sceneTextPlacer->clear();
		QPen penPT(QColor(255, 255, 255), 0.5, Qt::SolidLine, Qt::SquareCap, Qt::RoundJoin);

		ui->RedLabel->setVisible(false);
		ui->GreenLabel->setVisible(false);
		ui->CyanLabel->setVisible(false);
		ui->BlueLabel->setVisible(false);
		ui->PinkLabel->setVisible(false);
		ui->YellowLabel->setVisible(false);
		ui->WhiteLabel->setVisible(false);
		ui->DredLabel->setVisible(false);

		ui->labelAnomaly->setVisible(false);
		ui->labelBA->setVisible(false);
		ui->labelFiltered->setVisible(false);
		ui->labelBads->setVisible(false);
		ui->labelLowloads->setVisible(false);
		ui->labelOverloads->setVisible(false);
		ui->labelAlives->setVisible(false);
		ui->labelSSH->setVisible(false);

		fnt.setPixelSize(10);

		sceneTextPlacer->addLine(85, 110,  90, 100, penPT);
		sceneTextPlacer->addLine(23, 110,  83, 110, penPT);

		QGraphicsTextItem *item = sceneTextPlacer->addText("- Anomalies", fnt);
		item->setX(25);
		item->setY(94);
		item->setDefaultTextColor(QColor(255, 255, 255, 180));

		sceneTextPlacer->addLine(118, 120,  130, 100, penPT);
		sceneTextPlacer->addLine(23, 121,  117, 121, penPT);
		item = sceneTextPlacer->addText("- Webforms", fnt);
		item->setX(25);
		item->setY(105);
		item->setDefaultTextColor(QColor(255, 255, 255, 180));

		sceneTextPlacer->addLine(155, 130,  170, 100, penPT);
		sceneTextPlacer->addLine(23, 131,  154, 131, penPT);
		item = sceneTextPlacer->addText("- Basic Auth", fnt);
		item->setX(25);
		item->setY(115);
		item->setDefaultTextColor(QColor(255, 255, 255, 180));

		sceneTextPlacer->addLine(190, 140, 210, 100, penPT);
		sceneTextPlacer->addLine(23, 141,  189, 141, penPT);
	item = sceneTextPlacer->addText("- Suspicious", fnt);
		item->setX(25);
		item->setY(125);
		item->setDefaultTextColor(QColor(255, 255, 255, 180));

		sceneTextPlacer->addLine(230, 150, 250, 100, penPT);
		sceneTextPlacer->addLine(23, 151,  229, 151, penPT);
	item = sceneTextPlacer->addText("- Overloads", fnt);
		item->setX(25);
		item->setY(135);
		item->setDefaultTextColor(QColor(255, 255, 255, 180));

		sceneTextPlacer->addLine(270, 160, 290, 100, penPT);	
		sceneTextPlacer->addLine(23, 161,  269, 161, penPT);
	item = sceneTextPlacer->addText("- Lowloads", fnt);
		item->setX(25);
		item->setY(145);
		item->setDefaultTextColor(QColor(255, 255, 255, 180));

		sceneTextPlacer->addLine(310, 170, 330, 100, penPT);	
		sceneTextPlacer->addLine(23, 171, 309, 171, penPT);
		item = sceneTextPlacer->addText("- Alive", fnt);
		item->setX(25);
		item->setY(155);
		item->setDefaultTextColor(QColor(255, 255, 255, 180));
	}
	else if(VoiceScanFlag)
	{
        sceneTextPlacer->clear();
		
		fnt.setPixelSize(8);

		int h = ui->graphicLog->height();

        delete sceneTextPlacer;
		sceneTextPlacer = NULL;
		sceneTextPlacer = new QGraphicsScene();
		ui->graphicTextPlacer->setScene(sceneTextPlacer);

		for(int i = 0; i < h; i += 10)
		{
			QGraphicsTextItem *item = sceneTextPlacer->addText(QString::number(i/10 + 1) + "-", fnt);
			if(i != 90) item->setX(46);
			else item->setX(42);
			item->setY(h - i - 11);
			item->setDefaultTextColor(QColor(255, 255, 255, 180));
		};
	};
}

void nesca_3::slotDrawDelimLines()
{
	int gHeight = ui->graphicLog->height();
	QPen penDelim(QColor(255, 0, 0), 0.5);
	QPen penDelim2(QColor(255, 0, 0, 60), 1);

	QLinearGradient gradient(0, 0, 0, 100);
	gradient.setColorAt(0, QColor(255,0,0, 60));
	gradient.setColorAt(0.5, QColor(255,0,0, 40));
	gradient.setColorAt(1, QColor(255,0,0, 0));
	penDelim2.setBrush(gradient);

	sceneGrid2->addLine(33, gHeight, 33, gHeight - 3, penDelim);
	sceneGrid2->addLine(73, gHeight, 73, gHeight - 3, penDelim); 
	sceneGrid2->addLine(113, gHeight, 113, gHeight - 3, penDelim); 
	sceneGrid2->addLine(153, gHeight, 153, gHeight - 3, penDelim);
	sceneGrid2->addLine(193, gHeight, 193, gHeight - 3, penDelim);
	sceneGrid2->addLine(233, gHeight, 233, gHeight - 3, penDelim);

	sceneGrid2->addRect(1, 0, 31, gHeight - 3, penDelim2);
	sceneGrid2->addRect(34, 0, 38, gHeight - 3, penDelim2);
	sceneGrid2->addRect(74, 0, 38, gHeight - 3, penDelim2);
	sceneGrid2->addRect(114, 0, 38, gHeight - 3, penDelim2);
	sceneGrid2->addRect(154, 0, 38, gHeight - 3, penDelim2);
	sceneGrid2->addRect(194, 0, 38, gHeight - 3, penDelim2);
	sceneGrid2->addRect(234, 0, 35, gHeight - 3, penDelim2);
}

QList<int> gLOL0;
QList<int> gLOL1;
QList<int> gLOL2;
QList<int> gLOL3;
QList<int> gLOL4;
QList<int> gLOL5;
QList<int> gLOL6;
QList<int> gLOL7;
bool QOSWait = false;
void nesca_3::slotQoSAddLine()
{
	QOSWait = true;
	sceneGraph->clear();
	sceneGrid2->clear();
	sceneUpper->clear();

    float gHeight = ui->graphicLog->height();
	
	float fact = (float)100 / (float)MaxDataVal;
	
	if(QoSStep > 268) 
	{
		MaxDataVal = 1;
		QoSStep = 1;
		gLOL0.clear();
		gLOL1.clear();
		gLOL2.clear();
		gLOL3.clear();
		gLOL4.clear();
		gLOL5.clear();
		gLOL6.clear();
		gLOL7.clear();
	};
	gLOL0.push_back(lstOfLabels[0]);
	gLOL1.push_back(lstOfLabels[1]);
	gLOL2.push_back(lstOfLabels[2]);
	gLOL3.push_back(lstOfLabels[3]);
	gLOL4.push_back(lstOfLabels[4]);
	gLOL5.push_back(lstOfLabels[5]);
	gLOL6.push_back(lstOfLabels[6]);
	gLOL7.push_back(lstOfLabels[7]);

	int d0;
	int d1;
	int d2;
	int d3;
	int d4;
	int d5;
	int d6;
	int d7;

	for(int QoSStepi = 0; QoSStepi < QoSStep; ++QoSStepi)
	{
		if(QoSStepi < gLOL0.size()
			&& QoSStepi < gLOL1.size()
			&& QoSStepi < gLOL2.size()
			&& QoSStepi < gLOL3.size()
			&& QoSStepi < gLOL4.size()
			&& QoSStepi < gLOL5.size()
			&& QoSStepi < gLOL6.size()
			&& QoSStepi < gLOL7.size()
			)
		{
			d0 = gHeight - gLOL0[QoSStepi] * fact - 1;
			d1 = gHeight - gLOL1[QoSStepi] * fact - 1;
			d2 = gHeight - gLOL2[QoSStepi] * fact - 1;
			d3 = gHeight - gLOL3[QoSStepi] * fact - 1;
			d4 = gHeight - gLOL4[QoSStepi] * fact - 1;
			d5 = gHeight - gLOL5[QoSStepi] * fact - 1;
			d6 = gHeight - gLOL6[QoSStepi] * fact - 1;
			d7 = gHeight - gLOL7[QoSStepi] * fact - 1;

			sceneGraph->addLine(QoSStepi, gHeight, QoSStepi, d0, penQoS0);
			sceneGraph->addLine(QoSStepi, gHeight, QoSStepi, d1, penQoS1);
			sceneGraph->addLine(QoSStepi, gHeight, QoSStepi, d2, penQoS2);
			sceneGraph->addLine(QoSStepi, gHeight, QoSStepi, d3, penQoS3);
			sceneGraph->addLine(QoSStepi, gHeight, QoSStepi, d4, penQoS4);
			sceneGraph->addLine(QoSStepi, gHeight, QoSStepi, d5, penQoS5);
			sceneGraph->addLine(QoSStepi, gHeight, QoSStepi, d6, penQoS6);
			sceneGraph->addLine(QoSStepi, gHeight, QoSStepi, d7, penQoS7);
		};
	};
	QFont fnt;
	fnt.setFamily("Eurostile");
	fnt.setPixelSize(10);

	QGraphicsTextItem *item = sceneUpper->addText("Max = " + QString(std::to_string(MaxDataVal).c_str()), fnt);
	item->setX(215);
	item->setDefaultTextColor(Qt::white);
	QOSWait = false;
}

void nesca_3::slotQoSAddGrid()
{
	sceneGrid->clear();

    int gWidth = ui->graphicLog->width();
	if(MaxDataVal > 100) MaxDataVal = 100;
	float fact = (float)100/(float)MaxDataVal;

	float th = 0;
	for(int i = 0; i < 100; ++i)
	{
		th += fact;
		sceneGrid->addLine(0, th, gWidth, th, rpen);
	};
}

void nesca_3::setNickBox(QString str)
{
	ui->ircNickBox->setText(str);
}

void nesca_3::slotAddLine(int x1, int y1, int x2, int y2)
{
	sceneGrid->addLine(x1, y1, x2, y2, pen);
	if(sceneGrid->items().size() > 10) 
	{
		sceneGrid->removeItem(vectOld[10]);
		sceneGrid->items().pop_back();
	};
}

void nesca_3::slotAddPolyLine()
{
	if(ME2ScanFlag)
	{
        double uu = 0.0;
		QPainterPath path;
		if(vect.size() > 0) 
		{
			path.moveTo(vect[0]);
			for(int i = 1; i < vect.size(); ++i) 
			{
				path.lineTo(vect[i]);
			};
		};

		QGraphicsPathItem* itm = new QGraphicsPathItem(path);
		
        itm->setPen(pen2i);
        int vSz = sceneGraph->items().size();
        for(int i = 0; i < vSz; ++i)
        {
            sceneGraph->items()[i]->setY(u+i + 1);
            sceneGraph->items()[i]->setOpacity(1 - uu);
            uu+=0.027;
            u+=1;
        };
        sceneGraph->addItem(itm);

        if(vSz == 50)
        {
            sceneGraph->items().pop_back();
		};
		if(u > 10) u = 1;
	};
}

void nesca_3::slotDrawGrid()
{
	sceneGrid->clear();
}

void nesca_3::slotDrawActivityGrid()
{
	sceneActivityGrid->clear();
	QPen penActivity(QColor(170, 170, 170, 150), 0.1);

	int gHeight = ui->graphicActivityGrid->height();
	
	//Vertical
	for(int i = 0; i < 100; i+=10)
	{
		sceneActivityGrid->addLine(i, 0, i, gHeight, penActivity);
	};
}

void nesca_3::slotDrawActivityLine(QString data)
{
	sceneActivity->clear();

	QPen penActivity(QColor(255, 255, 255), 0.3);
	int as = 0;

	for(int i = 1; i < actLst.size(); ++i)
	{
        as += 2;
		sceneActivity->addLine(as - 2, 16 - actLst[i - 1], as, 16 - actLst[i], penActivity);
	};
	QFont fnt;
	fnt.setFamily("Eurostile");
	fnt.setPixelSize(9);
	QGraphicsTextItem *titem = sceneActivity->addText(data, fnt);
	titem->setX(43 - data.length()*2);
	titem->setY(-5);
	titem->setDefaultTextColor(QColor(255, 255, 255, 80));
}

void nesca_3::slotDrawVoiceGrid(int factor)
{
	sceneGrid->clear();
	QPen penActivity(QColor(170, 170, 170, factor), 0.1);
	QPen rpenActivity(QColor(255, 0, 0, factor), 0.1);

	int gWidth = ui->graphicLog->width();
	int gHeight = ui->graphicLog->height();

	//Horizontal
	int th = 0;
	for(int i = 0; i < 100; i+=5)
	{
		th = i - 2;
		if(i % 10 == 0) sceneGrid->addLine(0, th, gWidth, th, rpenActivity);
		else sceneGrid->addLine(0, th, gWidth, th, penActivity);
	};

	//Vertical
	for(int i = 0; i < 270; i+=10)
	{
		sceneGrid->addLine(i, 0, i, gHeight, penActivity);
	};
}

void nesca_3::activateME2ScanScene()
{
	if(ME2ScanFlag == false)
	{
		ui->me2ScanBut->setStyleSheet("color: rgb(216, 216, 216);background-color: rgba(2, 2, 2, 0);border: 1px solid rgba(255, 255, 255, 40);");
		ui->QoSScanBut->setStyleSheet("color: rgb(130, 130, 130);background-color: rgba(2, 2, 2, 0);border: 1px solid rgba(130, 130, 130, 80);");
		ui->VoiceScanBut->setStyleSheet("color: rgb(130, 130, 130);background-color: rgba(2, 2, 2, 0);border: 1px solid rgba(130, 130, 130, 80);");
		ui->PieStatBut->setStyleSheet("color: rgb(130, 130, 130);background-color: rgba(2, 2, 2, 0);border: 1px solid rgba(130, 130, 130, 80);");

		sceneUpper->clear();

		sceneGrid->clear();
		sceneGrid2->clear();
		sceneGraph->clear();
		sceneVoice->clear();
			
		vAlivLst.clear();
		vAnomLst.clear();
		vWFLst.clear();
		vSuspLst.clear();
		vLowlLst.clear();
		vBALst.clear();
		vOvrlLst.clear();
		vSSHLst.clear();
		vectOld.clear();

		ME2ScanFlag = true;
		QoSScanFlag = false;
		VoiceScanFlag = false;
		PieStatFlag = false;
		
		for(int i = 0; i < sceneGrid->items().size(); ++i)
		{
			sceneGrid->removeItem(sceneGrid->items()[i]);
			sceneGrid->items().clear();
		};
		for(int i = 0; i < sceneGrid2->items().size(); ++i)
		{
			sceneGrid2->removeItem(sceneGrid2->items()[i]);
			sceneGrid2->items().clear();
		};

		if(dtHN->isRunning() == false) 
		{
			dtHN->start();
		};
		if(dtME2->isRunning() == false) 
		{
			dtME2->start();
		};
	};
}

void nesca_3::activateQoSScanBut()
{
	if(QoSScanFlag == false)
	{
		ui->QoSScanBut->setStyleSheet("color: rgb(216, 216, 216);background-color: rgba(2, 2, 2, 0);border: 1px solid rgba(255, 255, 255, 40);");
		ui->PieStatBut->setStyleSheet("color: rgb(130, 130, 130);background-color: rgba(2, 2, 2, 0);border: 1px solid rgba(130, 130, 130, 80);");
		ui->VoiceScanBut->setStyleSheet("color: rgb(130, 130, 130);background-color: rgba(2, 2, 2, 0);border: 1px solid rgba(130, 130, 130, 80);");
		ui->me2ScanBut->setStyleSheet("color: rgb(130, 130, 130);background-color: rgba(2, 2, 2, 0);border: 1px solid rgba(130, 130, 130, 80);");
		
		ui->RedLabel->setVisible(true);
		ui->GreenLabel->setVisible(true);
		ui->CyanLabel->setVisible(true);
		ui->BlueLabel->setVisible(true);
		ui->PinkLabel->setVisible(true);
		ui->YellowLabel->setVisible(true);
		ui->WhiteLabel->setVisible(true);
		ui->DredLabel->setVisible(true);

		ui->labelAnomaly->setVisible(true);
		ui->labelBA->setVisible(true);
		ui->labelFiltered->setVisible(true);
		ui->labelBads->setVisible(true);
		ui->labelLowloads->setVisible(true);
		ui->labelOverloads->setVisible(true);
		ui->labelAlives->setVisible(true);
		ui->labelSSH->setVisible(true);
		
		vAlivLst.clear();
		vAnomLst.clear();
		vWFLst.clear();
		vSuspLst.clear();
		vLowlLst.clear();
		vBALst.clear();
		vOvrlLst.clear();
		vSSHLst.clear();
		vectOld.clear();

		ME2ScanFlag = false;
		QoSScanFlag = true;
		VoiceScanFlag = false;
		PieStatFlag = false;

		QoSStep = 1;
		MaxDataVal = 1;

		sceneGrid2->clear();
		sceneGraph->clear();
		sceneGrid->clear();
		sceneUpper->clear();
		sceneTextPlacer->clear();
		sceneVoice->clear();
		vect.clear();

		if(dtQoS->isRunning() == false) dtQoS->start();
		if(dtGridQoS->isRunning() == false) dtGridQoS->start();
	};
}

void nesca_3::activateVoiceScanBut()
{
	if(VoiceScanFlag == false)
	{
		ui->VoiceScanBut->setStyleSheet("color: rgb(216, 216, 216);background-color: rgba(2, 2, 2, 0);border: 1px solid rgba(255, 255, 255, 40);");
		ui->me2ScanBut->setStyleSheet("color: rgb(130, 130, 130);background-color: rgba(2, 2, 2, 0);border: 1px solid rgba(130, 130, 130, 80);");
		ui->QoSScanBut->setStyleSheet("color: rgb(130, 130, 130);background-color: rgba(2, 2, 2, 0);border: 1px solid rgba(130, 130, 130, 80);");
		ui->PieStatBut->setStyleSheet("color: rgb(130, 130, 130);background-color: rgba(2, 2, 2, 0);border: 1px solid rgba(130, 130, 130, 80);");

		ui->RedLabel->setVisible(true);
		ui->GreenLabel->setVisible(true);
		ui->CyanLabel->setVisible(true);
		ui->BlueLabel->setVisible(true);
		ui->PinkLabel->setVisible(true);
		ui->YellowLabel->setVisible(true);
		ui->WhiteLabel->setVisible(true);
		ui->DredLabel->setVisible(true);

		ui->labelAnomaly->setVisible(true);
		ui->labelBA->setVisible(true);
		ui->labelFiltered->setVisible(true);
		ui->labelBads->setVisible(true);
		ui->labelLowloads->setVisible(true);
		ui->labelOverloads->setVisible(true);
		ui->labelAlives->setVisible(true);
		ui->labelSSH->setVisible(true);

		ME2ScanFlag = false;
		QoSScanFlag = false;
		VoiceScanFlag = true;
		PieStatFlag = false;

		sceneGrid2->clear();
		sceneGraph->clear();
		sceneGrid->clear();
		sceneUpper->clear();
		sceneVoice->clear();

		if(vsTh->isRunning() == false) vsTh->start();
	};
}

void nesca_3::slotUpdatePie()
{
	sceneGraph->clear();
	vsTh->doEmitDrawGrid(70);

	int goods = saved;
	int nAlive = found - goods;
	float degree = (16*360) / (float)100;

	float perc0 = ((float)nAlive * 100) * degree;
	float perc1 = ((float)PieAnomC1/(float)(found) * 100) * degree;
	float perc2 = ((float)PieSusp/(float)(found) * 100) * degree;
	float perc3 = ((float)PieBA/(float)(found) * 100) * degree;
	float perc4 = ((float)PieLowl/(float)(found) * 100) * degree;
	float perc5 = ((float)PieWF/(float)(found) * 100) * degree;
	float perc6 = ((float)PieSSH/(float)(found) * 100) * degree;

	QFont fnt;
	fnt.setFamily("Eurostile");
	fnt.setPixelSize(10);

	QString dataSaved = "Saved:           "		+ QString::number(goods/(float)(found > 0 ? found : 1) * 100) + "%(" + QString::number(goods) + ")";
	QString dataParsed = "Filtered:         "	+ QString::number((found - goods)/(float)(found > 0 ? found : 1) * 100) + "%(" + QString::number(found - goods) + ")";
	QString dataOnline = "Online:           "	+ QString::number(found);
	QString dataAnom = "Anomalies:    "			+ QString::number(PieAnomC1/(float)(goods > 0 ? goods : 1) * 100) + "%";
	QString dataBA = "Basic Auth:    "			+ QString::number(PieBA/(float)(goods > 0 ? goods : 1) * 100) + "%";
	QString dataSusp = "Suspicious:    "		+ QString::number(PieSusp/(float)(goods > 0 ? goods : 1) * 100) + "%";
	QString dataLowl = "Lowload:       "		+ QString::number(PieLowl/(float)(goods > 0 ? goods : 1) * 100) + "%";
	QString dataWF = "WebForms:   "				+ QString::number(PieWF/(float)(goods > 0 ? goods : 1) * 100) + "%";
	QString dataSSH = "SSH:            "		+ QString::number(PieSSH/(float)(goods > 0 ? goods : 1) * 100) + "%";

	int dataX = 1;
	int dataY = 13;
	
	QGraphicsTextItem *titem = sceneGraph->addText(dataOnline, fnt);
	titem->setX(dataX);
	titem->setY(-5);
	titem->setDefaultTextColor(QColor(255, 255, 255, 130));

	titem = sceneGraph->addText(dataSaved, fnt);
	titem->setX(dataX);
	titem->setY(5);
	titem->setDefaultTextColor(QColor(255, 255, 255, 130));
	
	titem = sceneGraph->addText(dataParsed, fnt);
	titem->setX(dataX);
	titem->setY(15);
	titem->setDefaultTextColor(QColor(255, 255, 255, 255));

	titem = sceneGraph->addText(dataAnom, fnt);
	titem->setX(dataX);
	titem->setY(dataY + 17);
	titem->setDefaultTextColor(QColor("red"));

	titem = sceneGraph->addText(dataBA, fnt);
	titem->setX(dataX);
	titem->setY(dataY + 27);
	titem->setDefaultTextColor(Qt::darkCyan);

	titem = sceneGraph->addText(dataSusp, fnt);
	titem->setX(dataX);
	titem->setY(dataY + 37);
	titem->setDefaultTextColor(Qt::darkRed);

	titem = sceneGraph->addText(dataLowl, fnt);
	titem->setX(dataX);
	titem->setY(dataY + 47);
	titem->setDefaultTextColor(Qt::magenta);

	titem = sceneGraph->addText(dataWF, fnt);
	titem->setX(dataX);
	titem->setY(dataY + 56);
	titem->setDefaultTextColor(Qt::darkGray);

	titem = sceneGraph->addText(dataSSH, fnt);
	titem->setX(dataX);
	titem->setY(dataY + 66);
	titem->setDefaultTextColor(Qt::darkRed);

	int PieX = 155;
	int PieW = 112;

	//Alives
	QGraphicsEllipseItem* item = NULL;
	if(perc0 == 0)
	{
		item = new QGraphicsEllipseItem(PieX, 2, PieW, 97);
		item->setStartAngle(0);
		perc0 = 16*360;
		item->setSpanAngle(perc0);
		item->setBrush(QBrush(QColor(255, 255, 255, 10)));		
		sceneGraph->addItem(item);	
	}
	else
	{
		item = new QGraphicsEllipseItem(PieX, 2, PieW, 97);
		item->setStartAngle(0);
		item->setSpanAngle(perc0);
		item->setBrush(QBrush(Qt::white));
		sceneGraph->addItem(item);	
	};
	//Anomalies
	if(perc1 > 0)
	{
		item = new QGraphicsEllipseItem(PieX, 2, PieW, 97);
		item->setStartAngle(perc0);
		item->setSpanAngle(perc1);
		item->setBrush(QBrush(Qt::red));
		sceneGraph->addItem(item);
	};
	//Suspicious
	if(perc2 > 0)
	{
		item = new QGraphicsEllipseItem(PieX, 2, PieW, 97);
		item->setStartAngle(perc0 + perc1);
		item->setSpanAngle(perc2);
		item->setBrush(QBrush(Qt::darkRed));
		sceneGraph->addItem(item);
	};
	//BA
	if(perc3 > 0)
	{
		item = new QGraphicsEllipseItem(PieX, 2, PieW, 97);
		item->setStartAngle(perc0 + perc1 + perc2);
		item->setSpanAngle(perc3);
		item->setBrush(QBrush(Qt::darkCyan));
		sceneGraph->addItem(item);
	};
	//Lowl
	if(perc4 > 0)
	{
		item = new QGraphicsEllipseItem(PieX, 2, PieW, 97);
		item->setStartAngle(perc0 + perc1 + perc2 + perc3);
		item->setSpanAngle(perc4);
		item->setBrush(QBrush(Qt::magenta));
		sceneGraph->addItem(item);
	};
	//WF
	if(perc5 > 0)
	{
		item = new QGraphicsEllipseItem(PieX, 2, PieW, 97);
		item->setStartAngle(perc0 + perc1 + perc2 + perc3 + perc4);
		item->setSpanAngle(perc5);
		item->setBrush(QBrush(Qt::darkGray));
		sceneGraph->addItem(item);
	};
	//SSH
	if(perc6 > 0)
	{
		item = new QGraphicsEllipseItem(PieX, 2, PieW, 97);
		item->setStartAngle(perc0 + perc1 + perc2 + perc3 + perc4 + perc5);
		item->setSpanAngle(perc6);
		item->setBrush(QBrush(Qt::darkRed));
		sceneGraph->addItem(item);
	};
}

void nesca_3::activatePieStatBut()
{
	if(PieStatFlag == false)
	{
		ui->PieStatBut->setStyleSheet("color: rgb(216, 216, 216);background-color: rgba(2, 2, 2, 0);border: 1px solid rgba(255, 255, 255, 40);");
		ui->QoSScanBut->setStyleSheet("color: rgb(130, 130, 130);background-color: rgba(2, 2, 2, 0);border: 1px solid rgba(130, 130, 130, 80);");
		ui->VoiceScanBut->setStyleSheet("color: rgb(130, 130, 130);background-color: rgba(2, 2, 2, 0);border: 1px solid rgba(130, 130, 130, 80);");
		ui->me2ScanBut->setStyleSheet("color: rgb(130, 130, 130);background-color: rgba(2, 2, 2, 0);border: 1px solid rgba(130, 130, 130, 80);");
		
		ui->RedLabel->setVisible(false);
		ui->GreenLabel->setVisible(false);
		ui->CyanLabel->setVisible(false);
		ui->BlueLabel->setVisible(false);
		ui->PinkLabel->setVisible(false);
		ui->YellowLabel->setVisible(false);
		ui->WhiteLabel->setVisible(false);
		ui->DredLabel->setVisible(false);

		ui->labelAnomaly->setVisible(false);
		ui->labelBA->setVisible(false);
		ui->labelFiltered->setVisible(false);
		ui->labelBads->setVisible(false);
		ui->labelLowloads->setVisible(false);
		ui->labelOverloads->setVisible(false);
		ui->labelAlives->setVisible(false);
		ui->labelSSH->setVisible(false);

		ME2ScanFlag = false;
		QoSScanFlag = false;
		VoiceScanFlag = false;
		PieStatFlag = true;

		sceneGrid2->clear();
		sceneGraph->clear();
		sceneGrid->clear();
		sceneUpper->clear();
		sceneTextPlacer->clear();
		sceneVoice->clear();

		if(psTh->isRunning() == false) psTh->start();
	};
}

bool stopFirst;
void nesca_3::importAndScan()
{
	if(startFlag == false)
	{
		if(trackerOK)
		{
			if(ui->linePersKey->text().size() != 0)
			{
				CheckPersKey(2);
			}
			else
			{
				stt->doEmitionRedFoundData("Empty \"Personal key\" field. ");
			};
		}
		else
		{
			stt->doEmitionStartScanImport();
		};
	}
	else
	{
		ui->importButton->setStyleSheet(
			" QPushButton {"
			"background-color: qlineargradient(spread:none, x1:1, y1:0, x2:1, y2:1, stop:0.681818 rgba(0, 0, 0, 250), stop:1 rgba(255, 255, 255, 130));"
			"color: yellow;"
			"border: 0.5px solid qlineargradient(spread:reflect, x1:0.54, y1:0.488591, x2:0.54, y2:0, stop:0 rgba(255, 255, 255, 130), stop:1 rgba(0, 0, 0, 255));"
			"}"
			);

		ui->importButton->setText("Wait...");
		stt->doEmitionYellowFoundData("Trying to stop. Please, wait...");
		globalScanFlag = false;

		if(stopFirst == false)
		{
			stopFirst = true;
			globalScanFlag = false;
			ui->importButton->setStyleSheet(
				" QPushButton {"
				"background-color: qlineargradient(spread:none, x1:1, y1:0, x2:1, y2:1, stop:0.681818 rgba(0, 0, 0, 250), stop:1 rgba(255, 255, 255, 130));"
				"color: red;"
				"border: 0.5px solid qlineargradient(spread:reflect, x1:0.54, y1:0.488591, x2:0.54, y2:0, stop:0 rgba(255, 255, 255, 130), stop:1 rgba(0, 0, 0, 255));"
				"}"
				);

			ui->importButton->setText("STOP!");
			stt->doEmitionYellowFoundData("Trying to stop. Please, wait...");
			importFileName = "";
		}
		else
		{
			globalScanFlag = false;
			ui->importButton->setStyleSheet(
				" QPushButton {"
				"background-color: qlineargradient(spread:none, x1:1, y1:0, x2:1, y2:1, stop:0.681818 rgba(0, 0, 0, 250), stop:1 rgba(255, 255, 255, 130));"
				"color: yellow;"
				"border: 0.5px solid qlineargradient(spread:reflect, x1:0.54, y1:0.488591, x2:0.54, y2:0, stop:0 rgba(255, 255, 255, 130), stop:1 rgba(0, 0, 0, 255));"
				"}"
				);

			ui->importButton->setText("Wait...");
			stt->doEmitionYellowFoundData("Wait, killing threads...");
			STTTerminate();
		};
	};
}

void nesca_3::switchDataFields()
{
	if(ui->switcherBut->text() == "<")
	{
		BALogSwitched = true;
		ui->dataText->lower();
		ui->BAText->raise();
		ui->switcherBut->setText(">");
	}
	else
	{
		BALogSwitched = false;
		ui->dataText->raise();
		ui->BAText->lower();
		ui->switcherBut->setText("<");
	};
}

void nesca_3::slotBlinkMessage()
{
	if(blinkFlag) 
	{
		blinkFlag = false;
		ui->newMessageLabel->setStyleSheet("color:rgba(255, 0, 0, 0);background-color: rgba(2, 2, 2, 0);");	
		ui->IRCModeBut->setStyleSheet("color: rgb(216, 216, 216);background-color: rgba(2, 2, 2, 0);border: 1px solid rgba(255, 255, 255, 40);");
	}
	else 
	{
		blinkFlag = true;
		ui->newMessageLabel->setStyleSheet("color:rgba(255, 0, 0, 255);background-color: rgba(2, 2, 2, 0);");	
		ui->IRCModeBut->setStyleSheet("color: rgb(255, 0, 0);background-color: rgba(2, 2, 2, 0);border: 1px solid rgba(255, 0, 0, 255);");
	};
}

void nesca_3::ChangeDispalyMode()
{
	if(widgetIsHidden == false && tray->isVisible() == false)
	{
		ui->IRCModeChangerBut->setVisible(true);
		blinkFlag = false;
		widgetIsHidden = true;
		ui->newMessageLabel->setStyleSheet("color:rgba(255, 0, 0, 0);background-color: rgba(2, 2, 2, 0);");	
		ui->JobModeBut->setStyleSheet("color: rgb(216, 216, 216);background-color: rgba(2, 2, 2, 0);border: 1px solid rgba(255, 255, 255, 40);");
		ui->IRCModeBut->setStyleSheet("background-color: rgba(2, 2, 2, 0);border: 1px solid rgba(0, 214, 0, 40);color: rgb(0, 214, 0);");
		ui->widgetJOB->setGeometry(QRect(500, 44, 500, 730));
		ui->widgetIRC->setStyleSheet("background-color:qlineargradient(spread:pad, x1:0.541, y1:0.500364, x2:0.54, y2:0, stop:0 rgba(16, 16, 16, 255), stop:1 rgba(0, 0, 0, 255));");
		ui->widgetIRC->setGeometry(QRect(1, 44, 498, 730));
		ui->shoutBox->setFocus();
		QWidget::activateWindow();
		msgWdgt->hide();
	}
	else
	{
		ui->IRCModeBut->setStyleSheet("color: rgb(216, 216, 216);background-color: rgba(2, 2, 2, 0);border: 1px solid rgba(255, 255, 255, 40);");
		ui->IRCModeChangerBut->setVisible(false);
		disableBlink = false;
		blinkFlag = true;
		widgetIsHidden = false;
		printDelimiter = true;
		ui->widgetIRC->setGeometry(QRect(500, 44, 500, 730));
	};
}

void nesca_3::switchToJobMode()
{
	if(ui->widgetJOB->geometry().x() == 500)
	{
		ui->widgetIRC->setGeometry(QRect(500, 44, 500, 730));
		ui->IRCModeChangerBut->setVisible(false);
		widgetIsHidden = false;
		ui->widgetJOB->setGeometry(QRect(1, 44, 498, 730));
		ui->JobModeBut->setStyleSheet("background-color: rgba(2, 2, 2, 0);border: 1px solid rgba(0, 214, 0, 40);color: rgb(0, 214, 0);");
		ui->IRCModeBut->setStyleSheet("color: rgb(216, 216, 216);background-color: rgba(2, 2, 2, 0);border: 1px solid rgba(255, 255, 255, 40);");
	}
	else
	{
		ui->widgetJOB->setGeometry(QRect(500, 44, 500, 730));
		ui->JobModeBut->setStyleSheet("color: rgb(216, 216, 216);background-color: rgba(2, 2, 2, 0);border: 1px solid rgba(255, 255, 255, 40);");
	};
}

void nesca_3::CheckProxy()
{
	saveOptions();
	_SaveBackupToFile();
	QStringList qsl;
	if(ui->ircProxy->text().contains(":"))
	{
		qsl = ui->ircProxy->text().split(":");

		ui->ircProxy->setText(qsl[0]);
		ui->ircProxyPort->setText(qsl[1]);
	};

	chPTh->doEmitChangeYellowIRCData("Checking " + ui->ircProxy->text() + ":" + ui->ircProxyPort->text() + "...");
	chPTh->start();
}

void nesca_3::slotIRCOfflined()
{
	CSSOCKET(lSock);
	ui->IRCConnectBut->setStyleSheet(
		" #IRCConnectBut {"
		"background-color: qlineargradient(spread:none, x1:1, y1:0, x2:1, y2:1, stop:0.681818 rgba(0, 0, 0, 250), stop:1 rgba(255, 255, 255, 130));"
		"color: green;"
		"border: 0.5px solid qlineargradient(spread:reflect, x1:0.54, y1:0.488591, x2:0.54, y2:0, stop:0 rgba(255, 255, 255, 130), stop:1 rgba(0, 0, 0, 255));"
		"}");
	ui->IRCConnectBut->setText("Connect");
	ui->ircText->append("<span style=\"color:#efe100;\">[-//-] Disconnected.</span>");
}

void nesca_3::ConnectToIRCServer()
{
	saveOptions();
	_SaveBackupToFile();
	QStringList qsl;
	if(ui->ircServerBox->text().contains(":"))
	{
		qsl = ui->ircServerBox->text().split(":");

		ui->ircServerBox->setText(qsl[0]);
		ui->serverPortBox->setText(qsl[1]);
	};
	if(ui->ircProxy->text().contains(":"))
	{
		qsl = ui->ircProxy->text().split(":");

		ui->ircProxy->setText(qsl[0]);
		ui->ircProxyPort->setText(qsl[1]);
	};
	saveOptions();
	if(ui->ircServerBox->text() != "")
	{
		if(ui->serverPortBox->text() != "")
		{
			if(ui->IRCConnectBut->text() == "Connect")
			{
				if(!ircTh->isRunning()) 
				{
					if(ui->ircProxy->text() != "" && ui->ircProxyPort->text() != "") proxyEnabledFlag = 1;

					iWantToConnect = true;
					ui->IRCConnectBut->setStyleSheet(
						" #IRCConnectBut {"
						"background-color: qlineargradient(spread:none, x1:1, y1:0, x2:1, y2:1, stop:0.681818 rgba(0, 0, 0, 250), stop:1 rgba(255, 255, 255, 130));"
						"color: red;"
						"border: 0.5px solid qlineargradient(spread:reflect, x1:0.54, y1:0.488591, x2:0.54, y2:0, stop:0 rgba(255, 255, 255, 130), stop:1 rgba(0, 0, 0, 255));"
						"}");
					ui->IRCConnectBut->setText("Disconnect");
					if(proxyEnabledFlag) ui->ircText->append("<span style=\"color:#efe100;\">[*] Using proxy " + QString(ircProxy) + ":" + QString(ircProxyPort) + ".</span>");
                    else ui->ircText->append("<span style=\"color:#efe100;\">[*] No proxy selected! Connecting directly...</span>");
					ircTh->start();
				}
				else
				{
					ui->ircText->append("<span style=\"color:#efe100;\">[...] Wait...</span>");
				};
			}
			else
			{
				iWantToConnect = false;
				
					ui->IRCConnectBut->setStyleSheet(
							" #IRCConnectBut {"
							"background-color: qlineargradient(spread:none, x1:1, y1:0, x2:1, y2:1, stop:0.681818 rgba(0, 0, 0, 250), stop:1 rgba(255, 255, 255, 130));"
							"color: green;"
							"border: 0.5px solid qlineargradient(spread:reflect, x1:0.54, y1:0.488591, x2:0.54, y2:0, stop:0 rgba(255, 255, 255, 130), stop:1 rgba(0, 0, 0, 255));"
							"}");
					ui->IRCConnectBut->setText("Connect");
					ui->ircText->append("<span style=\"color:#efe100;\">[-//-] Disconnected.</span>");
								
				CSSOCKET(lSock);
			};
		}
		else
		{

			ui->ircText->append("<span style=\"color:red;background-color:#313131;\">No IRC port specified!</span>");

		};
	}
	else
	{

		ui->ircText->append("<span style=\"color:red;background-color:#313131;\">No IRC server specified!</span>");

	};
}

void nesca_3::ChangeIRCRawLog()
{
	if(ui->IRCModeChangerBut->text() == ":IRC raw")
	{
		IRCLogToggled = false;
		ui->IRCModeChangerBut->setText(":IRC log");
		ui->ircText->lower();
		ui->ircRaw->raise();
	}
	else
	{
		IRCLogToggled = true;
		ui->IRCModeChangerBut->setText(":IRC raw");
		ui->ircText->raise();
		ui->ircRaw->lower();
	};
}

void nesca_3::ChangeNick()
{
	strcpy(ircNick, ui->ircNickBox->text().toLocal8Bit().data());
    UserNickInit();
	ui->shoutBox->setFocus();
}

void nesca_3::CheckPersKey()
{
	emitIfOK = -1;
	saveOptions();
	_SaveBackupToFile();
	if(!chKTh->isRunning()) 
	{	
		stt->doEmitionYellowFoundData("[Key check] Starting checker-thread...");
		chKTh->start();
	}
	else
	{
        stt->doEmitionRedFoundData("Still ckecking your key, please wait...");
	};
}

void nesca_3::CheckPersKey(int val = -1)
{
	emitIfOK = val;
	saveOptions();
	_SaveBackupToFile();
	if(!chKTh->isRunning()) 
	{	
        stt->doEmitionYellowFoundData("[Key check] Starting checker-thread...");
		chKTh->start();
	}
	else
	{
        stt->doEmitionRedFoundData("Still ckecking your key, please wait...");
	};
}

void _finishNick()
{
	QList<QListWidgetItem *> ql = ui->nickList->findItems(ui->shoutBox->text(), Qt::MatchStartsWith);
	if(ql.size() > 0)
	{
		ui->shoutBox->setText(ql[0]->text() + ", ");
		ui->shoutBox->setFocus();
	};
}

bool nesca_3::eventFilter(QObject* obj, QEvent *event)
{
	if (obj == ui->shoutBox)
	{
		if (event->type() == QEvent::KeyPress)
		{
				QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
				if (keyEvent->key() == Qt::Key_Up)
				{
					ui->shoutBox->setText(PhraseLog[PhraseIndex]);
					++PhraseIndex;
					if(PhraseIndex >= PhraseLog.size()) PhraseIndex = 0;
					return true;
				}
				else if(keyEvent->key() == Qt::Key_Down)
				{
					--PhraseIndex;
					if(PhraseIndex < 0) PhraseIndex = PhraseLog.size() - 1;

					ui->shoutBox->setText(PhraseLog[PhraseIndex]);
					return true;
				}
				else if(keyEvent->modifiers() == Qt::ControlModifier)
				{
					privateMsgFlag = true;
					event->accept();
				}
				else if(keyEvent->key() == Qt::Key_Tab)
				{
					_finishNick();
					return true;
				};
		};
		return false;
	}
	else if (obj == ui->ircText)
	{
		if (event->type() == QEvent::KeyPress)
		{
			QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
			if(keyEvent->modifiers() == Qt::ControlModifier)
			{
				privateMsgFlag = true;
				event->accept();
                return true;
			};
            return false;
		}
		else if (event->type() == QEvent::KeyRelease)
		{
			privateMsgFlag = false;	
			event->accept();
            return true;
		};
        return false;
	}
	else if (obj == ui->widgetIRC)
	{
		if(MapWidgetOpened)
		{
			qwm->raise();
		};
		if (event->type() == QEvent::KeyPress)
		{
			QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
			if(keyEvent->modifiers() == Qt::ControlModifier)
			{
				privateMsgFlag = true;
				event->accept();
                return true;
			};
            return false;
		}
		else if (event->type() == QEvent::KeyRelease)
		{
			privateMsgFlag = false;	
			event->accept();
            return true;
		};
        return false;
	}
	else
	{
		if (event->type() == QEvent::KeyPress)
		{
			QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
			if(keyEvent->modifiers() == Qt::ControlModifier)
			{
				privateMsgFlag = true;
				event->accept();
                return true;
			};
            return false;
		}
		else if(event->type() == QEvent::KeyRelease)
		{
			privateMsgFlag = false;	
			event->accept();
            return true;
		};
        return false;
	};
}

void nesca_3::ChangeTopic()
{
	if(ui->topicLine->text().size() > 0)
	{
		char temp[2048] = {0};

		QString strf;
		
		QTextCodec *codec = QTextCodec::codecForName("Windows-1251");
		strf = codec->toUnicode(ui->topicLine->text().toUtf8());
		strcpy(temp, "TOPIC #");
		strcat(temp, IRC_CHAN);
		strcat(temp, " :");
		strncat(temp, strf.toLocal8Bit().data(), 256);
		strcat(temp, "\n");

		sendS(lSock, temp, strlen(temp), 0);
	};
}

void nesca_3::SaySmthng()
{
	if(ui->shoutBox->text().size() > 0)
	{
		char temp[2048] = {0};
		globalPinger = 0;
		PhraseIndex = 0;
		if(PhraseLog.size() > 50)
		{
			PhraseLog.pop_back();
			if(ui->shoutBox->text() != PhraseLog[PhraseIndex]) PhraseLog.push_front(ui->shoutBox->text());
		}
		else 
		{
			if(ui->shoutBox->text() != PhraseLog[PhraseIndex]) PhraseLog.push_front(ui->shoutBox->text());
		};

		if(ui->shoutBox->text() == "/clear" || ui->shoutBox->text() == "clear" || ui->shoutBox->text() == "clr")
		{
			ui->ircText->clear();
		}
		else if(ui->shoutBox->text().contains("/me "))
		{
			QString strf;
			if(utfIRCFlag)
			{
				strf = codec->toUnicode(ui->shoutBox->text().toUtf8());
			}
			else strf = ui->shoutBox->text();
			
			strcpy(temp, "PRIV");
			strcat(temp, "MSG #");
			strcat(temp, IRC_CHAN);
			strcat(temp, " :");
			strcat(temp, "ACTION ");
			strcat(temp, strf.toLocal8Bit().data() + 3);
			strcat(temp, "\n");
			sendS(lSock, temp, strlen(temp), 0);
			appendDefaultIRCTextOut("--> " + ui->ircNickBox->text() + " " + strf.mid(3));
		}
		else if(ui->shoutBox->text().contains("/w "))
		{
			int lBoundary1 = ui->shoutBox->text().indexOf(" ");
			int lBoundary2 = ui->shoutBox->text().indexOf(",");
			int lBoundary = 0;
			if(lBoundary1 == -1)
			{
				lBoundary = lBoundary2;				
			}
			else if(lBoundary2 == -1)
			{
				lBoundary = lBoundary1;
			}
			else if(lBoundary1 > lBoundary2)
			{
				lBoundary = lBoundary2;
			}
			else
			{
				lBoundary = lBoundary1;
			};
			QString nickPG = ui->shoutBox->text().mid(lBoundary + 1);
			int rBoundary = nickPG.indexOf(" ");
			QString nickP = ui->shoutBox->text().mid(lBoundary + 1, rBoundary);
			QString msgP = nickPG.mid(rBoundary + 1);

			QString strf;
			if(utfIRCFlag)
			{
				strf = codec->toUnicode(msgP.toUtf8());
			}
			else strf = msgP;

			strcpy(temp, "PRIV");
			strcat(temp, "MSG ");
			strcat(temp, nickP.toLocal8Bit().data());
			strcat(temp, " :");
			strcat(temp, strf.toLocal8Bit().data());
			strcat(temp, "\n");
			sendS(lSock, temp, strlen(temp), 0);
			sendS(lSock, "PING 0", 6, 0);
			
			QString rData;
			QString rData2;
			
			if(utfIRCFlag) 
			{
				QString str = codec->toUnicode(msgP.toUtf8());
				rData2 = "<font color=\"#ffae00\">[" + QTime::currentTime().toString() + "]</font> <font color=\"#810000\">[PM -> " + nickP + "]:</font> " + msgP;
				ui->ircText->append(rData2);
				rData = "<font color=\"#ffae00\">[" + QTime::currentTime().toString() + "]</font> <font color=\"#810000\">[PM -> " + nickP + "]:</font> " + str;
				globalIRCText += rData + "\n";
			}
			else 
			{
				rData = "<font color=\"#ffae00\">[" + QTime::currentTime().toString() + "]</font> <font color=\"#810000\">[PM -> " + nickP + "]:</font> " + msgP;
				ui->ircText->append(rData);
				globalIRCText += rData + "\n";
			};
		}
		else if(ui->shoutBox->text().contains("/nick "))
		{
			int lBoundary = ui->shoutBox->text().indexOf(" ");
			
			QString nickPG = ui->shoutBox->text().mid(lBoundary + 1);
			int rBoundary = nickPG.indexOf(" ");
			QString nickP = ui->shoutBox->text().mid(lBoundary + 1, rBoundary);
			
			strcpy(temp, "NICK ");
			strcat(temp, nickP.toLocal8Bit().data());
			strcat(temp, "\n");
			ui->ircNickBox->setText(nickP);
            UserNickInit();
		}
		else 
		{
			QString strf;
			if(utfIRCFlag)
			{
				strf = codec->toUnicode(ui->shoutBox->text().toUtf8());
			}
			else strf = ui->shoutBox->text();
			if(IRCLogToggled) 
			{
				strcpy(temp, "PRIV");
				strcat(temp, "MSG #");
				strcat(temp, IRC_CHAN);
				strcat(temp, " :");
				strcat(temp, strf.toLocal8Bit().data());
				strcat(temp, "\n");
				appendDefaultIRCTextOut("[" + ui->ircNickBox->text() + "]: " + strf);
			}
			else
			{
				if(strf.contains("join "))
				{
					appendDefaultIRCRawTextOut("[JOIN command is not accepted]");
					ui->shoutBox->clear();
					return;
				}
				else
				{
					strcpy(temp, strf.toLocal8Bit().data());
					strcat(temp, "\n");
				};
			};

			sendS(lSock, temp, strlen(temp), 0);
		};

		ui->shoutBox->clear();
	};
}

void nesca_3::slotChangeCPModeToUTF()
{
	utfIRCFlag = true;
	ui->IRCUTFMode->setStyleSheet("background-color: rgba(2, 2, 2, 0);border: 1px solid rgba(0, 214, 0, 40);color: rgb(0, 214, 0);");
	ui->IRCCP1251Mode->setStyleSheet("background-color: rgba(2, 2, 2, 0);border: 1px solid rgba(255, 255, 255, 40);color: rgb(71, 71, 71);");
	
	QByteArray wtfR = codec->fromUnicode(globalIRCRaw.replace("\n", "<br>"));
	QByteArray wtfT = codec->fromUnicode(globalIRCText.replace("\n", "<br>"));
	ui->ircText->setHtml( QString(wtfT) );
	ui->ircRaw->setHtml( QString(wtfR) );
	ui->ircText->verticalScrollBar()->setValue(ui->ircText->verticalScrollBar()->maximum());
	ui->ircRaw->verticalScrollBar()->setValue(ui->ircRaw->verticalScrollBar()->maximum());
}

void nesca_3::slotChangeCPModeTo1251()
{
	utfIRCFlag = false;
	ui->IRCCP1251Mode->setStyleSheet("background-color: rgba(2, 2, 2, 0);border: 1px solid rgba(0, 214, 0, 40);color: rgb(0, 214, 0);");
	ui->IRCUTFMode->setStyleSheet("background-color: rgba(2, 2, 2, 0);border: 1px solid rgba(255, 255, 255, 40);color: rgb(71, 71, 71);");
		
	ui->ircText->setHtml(globalIRCText.replace("\n", "<br>"));
	ui->ircRaw->setHtml(globalIRCRaw.replace("\n", "<br>"));
	ui->ircText->verticalScrollBar()->setValue(ui->ircText->verticalScrollBar()->maximum());
	ui->ircRaw->verticalScrollBar()->setValue(ui->ircRaw->verticalScrollBar()->maximum());
}

void nesca_3::onLinkClicked(QUrl link)
{
	QString lnk = link.toString();
	
	if(lnk.contains("nesca:") == 1 && privateMsgFlag == false)
	{
		ui->shoutBox->setText(ui->shoutBox->text() + lnk.mid(6) + ", ");
		ui->shoutBox->setFocus();
	}
	else if(lnk.contains("nesca:") == 1 && privateMsgFlag)
	{
		ui->shoutBox->setText("/w " + lnk.mid(6) + " ");
		ui->shoutBox->setFocus();
	}
	else
	{
		QDesktopServices::openUrl(link);
	};
}

void nesca_3::slotRestartIRC()
{
	ircPTh->terminate();
	globalPinger = 0;
	CSSOCKET(lSock);
	ircTh->terminate();
	ircTh->start();
}

void nesca_3::slotClearLogs()
{
	ui->dataText->clear();
	ui->BAText->clear();
}

void nesca_3::slotIRCGetTopic(QString str)
{
	ui->topicLine->setText(str);
	QString rData;
	rData = "<font style=\"color:#c0ff00; text-decoration: underline;\">Topic: \"" + str + "\"</font>";
	globalIRCText += rData + "\n";
	ui->ircText->append(rData);
}

//unsigned char jpgHeader[623] = {
//	0xFF, 0xD8, 0xFF, 0xE0, 0x00, 0x10, 0x4A, 0x46, 0x49, 0x46, 0x00, 0x01, 0x01, 0x01, 0x00, 0x60, 
//	0x00, 0x60, 0x00, 0x00, 0xFF, 0xDB, 0x00, 0x43, 0x00, 0x02, 0x01, 0x01, 0x02, 0x01, 0x01, 0x02, 
//	0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x03, 0x05, 0x03, 0x03, 0x03, 0x03, 0x03, 0x06, 0x04, 
//	0x04, 0x03, 0x05, 0x07, 0x06, 0x07, 0x07, 0x07, 0x06, 0x07, 0x07, 0x08, 0x09, 0x0B, 0x09, 0x08, 
//	0x08, 0x0A, 0x08, 0x07, 0x07, 0x0A, 0x0D, 0x0A, 0x0A, 0x0B, 0x0C, 0x0C, 0x0C, 0x0C, 0x07, 0x09, 
//	0x0E, 0x0F, 0x0D, 0x0C, 0x0E, 0x0B, 0x0C, 0x0C, 0x0C, 0xFF, 0xDB, 0x00, 0x43, 0x01, 0x02, 0x02, 
//	0x02, 0x03, 0x03, 0x03, 0x06, 0x03, 0x03, 0x06, 0x0C, 0x08, 0x07, 0x08, 0x0C, 0x0C, 0x0C, 0x0C, 
//	0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 
//	0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 
//	0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0xFF, 0xC0, 
//	0x00, 0x11, 0x08, 0x00, 0x64, 0x00, 0x80, 0x03, 0x01, 0x22, 0x00, 0x02, 0x11, 0x01, 0x03, 0x11, 
//	0x01, 0xFF, 0xC4, 0x00, 0x1F, 0x00, 0x00, 0x01, 0x05, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 
//	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 
//	0x0A, 0x0B, 0xFF, 0xC4, 0x00, 0xB5, 0x10, 0x00, 0x02, 0x01, 0x03, 0x03, 0x02, 0x04, 0x03, 0x05, 
//	0x05, 0x04, 0x04, 0x00, 0x00, 0x01, 0x7D, 0x01, 0x02, 0x03, 0x00, 0x04, 0x11, 0x05, 0x12, 0x21, 
//	0x31, 0x41, 0x06, 0x13, 0x51, 0x61, 0x07, 0x22, 0x71, 0x14, 0x32, 0x81, 0x91, 0xA1, 0x08, 0x23, 
//	0x42, 0xB1, 0xC1, 0x15, 0x52, 0xD1, 0xF0, 0x24, 0x33, 0x62, 0x72, 0x82, 0x09, 0x0A, 0x16, 0x17, 
//	0x18, 0x19, 0x1A, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 
//	0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 
//	0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 
//	0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 
//	0x9A, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 
//	0xB8, 0xB9, 0xBA, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xD2, 0xD3, 0xD4, 0xD5, 
//	0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xF1, 
//	0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFF, 0xC4, 0x00, 0x1F, 0x01, 0x00, 0x03, 
//	0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 
//	0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0xFF, 0xC4, 0x00, 0xB5, 0x11, 0x00, 
//	0x02, 0x01, 0x02, 0x04, 0x04, 0x03, 0x04, 0x07, 0x05, 0x04, 0x04, 0x00, 0x01, 0x02, 0x77, 0x00, 
//	0x01, 0x02, 0x03, 0x11, 0x04, 0x05, 0x21, 0x31, 0x06, 0x12, 0x41, 0x51, 0x07, 0x61, 0x71, 0x13, 
//	0x22, 0x32, 0x81, 0x08, 0x14, 0x42, 0x91, 0xA1, 0xB1, 0xC1, 0x09, 0x23, 0x33, 0x52, 0xF0, 0x15, 
//	0x62, 0x72, 0xD1, 0x0A, 0x16, 0x24, 0x34, 0xE1, 0x25, 0xF1, 0x17, 0x18, 0x19, 0x1A, 0x26, 0x27, 
//	0x28, 0x29, 0x2A, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 
//	0x4A, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 
//	0x6A, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 
//	0x89, 0x8A, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 
//	0xA7, 0xA8, 0xA9, 0xAA, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xC2, 0xC3, 0xC4, 
//	0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xE2, 
//	0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 
//	0xFA, 0xFF, 0xDA, 0x00, 0x0C, 0x03, 0x01, 0x00, 0x02, 0x11, 0x03, 0x11, 0x00, 0x3F, 0x00
//};
//
int c = 1;

void nesca_3::slotSaveImage(QAction *qwe)
{
	QObject *smB = this->sender();
	int ci = ui->tabMainWidget->currentIndex();
    QTime QT = QTime::currentTime();
    QString t("*.png");

	if(smB == menuPS)
	{
		if(ME2ScanFlag || VoiceScanFlag)
		{
			QString fn = QString::number(QT.msec()) + "_" + (ME2ScanFlag ? QString("ME2") : QString("Voice")) + "_" + (ci == 0 ? ui->ipLine->text() : ui->lineEditStartIPDNS->text()) + ".png";
			int ax = 27;
			int ay = 2;
			int w = ui->graphicLog->width() + 30;
			int h = ui->graphicLog->height() + 30;
			int hOffset = 20;
			int spX = -10;
			int spY = 108;
			int wsp = 40;
			int hsp = 70;

			if(VoiceScanFlag) 
			{
				hOffset = -60;
				spX = 8;
				spY = -1;
				wsp = 0;
				hsp = -3;
			};

			QPixmap pixmap(ui->graphicTextPlacer->width() + 5, ui->graphicTextPlacer->height() + hOffset);
			QPainter painter(&pixmap);
			pixmap.fill(QColor(0, 0, 0));
			sceneTextPlacer->render(&painter, QRect(spX, spY, w + wsp, h + hsp));
			sceneUpper->render(&painter, QRect(ax, ay, w, h));
			sceneVoice->render(&painter, QRect(ax, ay, w, h));
			sceneGraph->render(&painter, QRect(ax, ay + 8, w - 2, h));
			sceneGrid->render(&painter, QRect(ax, ay, w, h));
			sceneGrid2->render(&painter, QRect(ax, ay, w, h));

            QString filename = QFileDialog::getSaveFileName(
			this, 
			tr("Save image"), 
			QDir::currentPath() + "/" + fn,
			".png",
            &t
			);
			if (filename != "") pixmap.save(filename);
		}
		else
		{
			QString fn = QString::number(QT.msec()) + "_" + (PieStatFlag ? "PieStat" : "QOS") + "_" + (ci == 0 ? ui->ipLine->text() : ui->lineEditStartIPDNS->text()) + ".png";

			QPixmap pixmap(ui->graphicLog->width(), ui->graphicLog->height());
			QPainter painter(&pixmap);
			pixmap.fill(Qt::black);

			sceneGrid->render(&painter);
			sceneGrid2->render(&painter);
			sceneGrid2->render(&painter);
			sceneUpper->render(&painter);
			sceneVoice->render(&painter);
			sceneGraph->render(&painter);

			painter.end();
		
			QString filename = QFileDialog::getSaveFileName(
				this,
				tr("Save image"),
				QDir::currentPath() + "/" + fn,
				".png",
                &t
				);
			if (filename != "") pixmap.save(filename);
		};
	};
}

void PieStatView::contextMenuEvent(QContextMenuEvent *event)
{
	menuPS = new QMenu;
	menuPS->addAction("Save image.");
	menuPS->popup(event->globalPos());

	connect(menuPS, SIGNAL(triggered(QAction *)), gthis, SLOT(slotSaveImage(QAction *)));
}

QLabel *msgLbl;
QLabel *msgDLbl;
QLabel *msgTLbl;
QLabel *msgNLbl;
void nesca_3::slotUnhidePopup(QString strD = "", QString senderNick = "")
{
	msgWdgt->show();
	QString str;
	if(utfIRCFlag) 
	{
		QByteArray wtf = codec->fromUnicode(strD);
		str = QString(wtf);
	}
	else str = strD;
	if(msgLbl != NULL) msgLbl->setText(str);
	if(msgNLbl != NULL) msgNLbl->setText(senderNick);
	if(msgDLbl != NULL) msgDLbl->setText(QTime::currentTime().toString());
}

void CreateMsgPopupWidget(QString str = "", QString senderNick = "")
{
	QDesktopWidget desk;

	int px_width = desk.width();
	int px_height = desk.height();

	msgWdgt = new PopupMsgWidget();
	msgWdgt->setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint|Qt::SubWindow);
	msgWdgt->setStyleSheet(
		"background-color:qlineargradient(spread:pad, x1:0.541, y1:0.500364, x2:0.54, y2:0, stop:0 rgba(16, 16, 16, 255), stop:1 rgba(0, 0, 0, 255)); border: 1px solid #525252;"
		);

	msgWdgt->setGeometry(px_width - 300, px_height - 100, 300, 100);

	QLabel *msgBLbl = new QLabel(msgWdgt);
	msgBLbl->setGeometry(0, 0, 300, 100);
	msgBLbl->setStyleSheet(
		"background-color: rgba(0, 0, 0, 0);"
		);
	msgBLbl->show();
	
	msgDLbl = new QLabel(msgWdgt);
	msgDLbl->setGeometry(0, 0, 300, 15);
	msgDLbl->setStyleSheet(
		"background-color: rgba(0, 0, 0, 70);"
		);
	msgDLbl->show();

	msgTLbl = new QLabel(msgWdgt);
	msgTLbl->setText("New message");
	msgTLbl->setStyleSheet(
		"color: rgb(216, 216, 216);background-color: rgba(2, 2, 2, 0);"
		);
	msgTLbl->show();

	msgDLbl = new QLabel(msgWdgt);
	msgDLbl->setGeometry(249, 0, 300, 15);
	msgDLbl->setText(QTime::currentTime().toString());
	msgDLbl->setStyleSheet(
		"color: rgb(216, 216, 216);background-color: rgba(2, 2, 2, 0);"
		);
	msgDLbl->show();

	msgLbl = new QLabel(msgWdgt);
	msgLbl->setText(str);
	msgLbl->setGeometry(10, 10, 290, 100);
	msgLbl->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
	msgLbl->setStyleSheet(
		"color: rgb(246, 246, 246);background-color: rgba(2, 2, 2, 0); border:none;font-weight: bold;"
		);
	msgLbl->show();

	msgNLbl = new QLabel(msgWdgt);
	msgNLbl->setText(senderNick);
	msgNLbl->setGeometry(35, 0, 249, 15);
	msgNLbl->setAlignment(Qt::AlignHCenter);
	msgNLbl->setStyleSheet(
		"color: rgb(246, 246, 246);background-color: rgba(2, 2, 2, 0); border:none;font-weight: bold;text-decoration:underline;"
		);
	msgNLbl->show();
}

QTextBrowser *SendData;
QTextBrowser *RecvData;
void nesca_3::slotShowDataflow()
{
	if(MapWidgetOpened == false)
	{
		MapWidgetOpened = true;
		ui->DataflowModeBut->setStyleSheet("background-color: rgba(2, 2, 2, 0);border: 1px solid rgba(0, 214, 0, 40);color: rgb(0, 214, 0);");
		qwm = new QWidget();
		qwm->setWindowFlags(Qt::FramelessWindowHint|Qt::SubWindow);
		qwm->setStyleSheet(
			"background-color:qlineargradient(spread:pad, x1:0.541, y1:0.500364, x2:0.54, y2:0, stop:0 rgba(16, 16, 16, 255), stop:1 rgba(0, 0, 0, 255));"
			"border: 1px solid #616161;");
		
		qwm->setGeometry(QMainWindow::x() + ui->widget->width() + 5, QMainWindow::y(), 480, 800);
		qwm->show();
		QFont fnt;
		fnt.setFamily("Eurostile");
		fnt.setPixelSize(10);

		SendData = new QTextBrowser(qwm);
		SendData->setGeometry(5, 5, 470, 370);
		SendData->setStyleSheet("color:rgb(150, 150, 150);");
		SendData->setFont(fnt);
		SendData->show();

		RecvData = new QTextBrowser(qwm);
		RecvData->setGeometry(5, 380, 470, 414);
		RecvData->setStyleSheet("color:rgb(150, 150, 150);");
		RecvData->setFont(fnt);
		RecvData->show();
	}
	else
	{
		ui->DataflowModeBut->setStyleSheet("color: rgb(216, 216, 216);background-color: rgba(2, 2, 2, 0);border: 1px solid rgba(255, 255, 255, 40);");
        delete SendData;
        delete RecvData;
        delete qwm;
		qwm = NULL;
		RecvData = NULL;
		SendData = NULL;
		MapWidgetOpened = false;
	};
}

QGraphicsPathItem *GiveMeGItem(QVector<QPointF> vAnomLst)
{
	QPolygonF qpf;
	QPainterPath path;
	for(int i = 0; i < vAnomLst.size(); ++i)
	{
		qpf.append(vAnomLst[i]);
	};
	if(qpf.size() > 0) 
	{
		path.moveTo(qpf[0]);
		for(int i = 1; i < qpf.size(); ++i) 
		{
			path.lineTo(qpf[i]);
		};
	};
	return new QGraphicsPathItem(path);
}

void nesca_3::slotVoiceAddLine()
{
	int as = 0;
	if(VoiceScanFlag)
	{
		sceneVoice->clear();

		for(int i = 1; i < vAlivLst.size(); ++i)
		{
			as += 2;
			sceneVoice->addLine(as - 2, 120 - vAlivLst[i - 1] - 1, as, 120 - vAlivLst[i] - 1, penQoS0);
		};
		as = 0;
			
		for(int i = 1; i < vAnomLst.size(); ++i)
		{
			as += 2;
			sceneVoice->addLine(as - 2, 120 - vAnomLst[i - 1] - 1, as, 120 - vAnomLst[i] - 1, penQoS1);
		};
		as = 0;
			
		for(int i = 1; i < vWFLst.size(); ++i)
		{
			as += 2;
			sceneVoice->addLine(as - 2, 120 - vWFLst[i - 1] - 1, as, 120 - vWFLst[i] - 1, penQoS2);
		};
		as = 0;
			
		for(int i = 1; i < vSuspLst.size(); ++i)
		{
			as += 2;
			sceneVoice->addLine(as - 2, 120 - vSuspLst[i - 1] - 1, as, 120 - vSuspLst[i] - 1, penQoS3);
		};
		as = 0;
			
		for(int i = 1; i < vLowlLst.size(); ++i)
		{
			as += 2;
			sceneVoice->addLine(as - 2, 120 - vLowlLst[i - 1] - 1, as, 120 - vLowlLst[i] - 1, penQoS4);
		};
		as = 0;
			
		for(int i = 1; i < vBALst.size(); ++i)
		{
			as += 2;
			sceneVoice->addLine(as - 2, 120 - vBALst[i - 1] - 1, as, 120 - vBALst[i] - 1, penQoS5);
		};
		as = 0;
			
		for(int i = 1; i < vOvrlLst.size(); ++i)
		{
			as += 2;
			sceneVoice->addLine(as - 2, 120 - vOvrlLst[i - 1], as, 120 - vOvrlLst[i] - 1, penQoS6);
		};
		as = 0;
						
		for(int i = 1; i < vSSHLst.size(); ++i)
		{
			as += 2;
			sceneVoice->addLine(as - 2, 120 - vSSHLst[i - 1], as, 120 - vSSHLst[i] - 1, penQoS7);
		};
	};
}

void nesca_3::slotShowNicks()
{
	if(ui->nickShowBut->text() == "<")
	{
		ui->nickShowBut->setText(">");
		ui->nickList->raise();
		GetNicks();
	}
	else
	{
		ui->nickShowBut->setText("<");
		ui->nickList->lower();
	};
}

QRegExp _rOutProt(" HTTP/1.\\d+");
QRegExp _rOutPath(" /(\\w|\\.|,|/|:|-|_|\\?|!|\\@|#|\\$|%|\\^|&|\\*|\\(|\\)|=|\\+|<|>|;|:|\"|'|~|\\[|\\])* ");
QRegExp _rOutHost("Host: ((\\w|\\d|\\.|:|/)*)\\r\\n");
void nesca_3::slotOutData(QString ip, QString str)
{
	if(SendData != NULL) 
	{
		_rOutPath.indexIn(str);
		QString prot = _rOutPath.cap(0);
		if(prot.size() > 0) str.replace(prot, " <font color=\"Turquoise\">" + prot + "</font> ");
		_rOutHost.indexIn(str);
		prot = _rOutHost.cap(1);
		if(prot.size() > 0) str.replace(prot, " <font color=\"Turquoise\">" + prot + "</font> ");
		str.replace("GET ", "<font color=\"GoldenRod\">GET </font>");
		str.replace("POST ", "<font color=\"GoldenRod\">POST </font>");
		str.replace("Host: ", "<font color=\"GoldenRod\">Host: </font>");
		str.replace("Cookie: ", "<font color=\"GoldenRod\">Cookie: </font>");
		str.replace("Accept:", "<font color=\"GoldenRod\">Accept:</font>");
		str.replace("Accept-Language:", "<font color=\"GoldenRod\">Accept-Language:</font>");
		str.replace("Accept-Charset:", "<font color=\"GoldenRod\">Accept-Charset:</font>");
		str.replace("Accept-Encoding:", "<font color=\"GoldenRod\">Accept-Encoding:</font>");
		str.replace("User-Agent:", "<font color=\"GoldenRod\">User-Agent:</font>");
		str.replace("Connection:", "<font color=\"GoldenRod\">Connection:</font>");
		str.replace("Content-Length:", "<font color=\"GoldenRod\">Content-Length:</font>");
		str.replace("Authorization:", "<font color=\"GoldenRod\">Authorization:</font>");
		_rOutProt.indexIn(str);
		prot = _rOutProt.cap(0);
		if(prot.size() > 0) str.replace(prot, "<font color=\"GoldenRod\">" + prot + "</font>");
		str.replace("\r\n", "<br>");

		SendData->append("<font color=\"#F0FFFF\">[" + ip + "]</font><br>" + str + "<hr><br>");
	};
}

QRegExp _rIncProt("HTTP/1.\\d+ ");
QRegExp _rIncHost("Host: ((\\w|\\d|\\.|:|/)*)\\r\\n");
QRegExp _rIncTags1("&lt;.{1,8}&gt;");
QRegExp _rIncTagsClose("&lt;/.{1,8}&gt;");
QRegExp _rIncTags2("&lt;.{1,8} ");
QRegExp _rIncTags3("/&gt;");
QRegExp _rIncInnerTags("=&quot;((\\w|\\.|,|/|:|-|_|\\?|!|\\@|#|\\$|%|\\^|&|\\*|\\(|\\)|=|\\+|<|>|;|:|\"|'|~|\\s| )*)&quot;");
QRegExp _rIncScriptTag1("&lt;script&gt;.*&lt;/script&gt;");
QRegExp _rIncScriptTag2("&lt;script .*&lt;/script&gt;");
QRegExp _rIncStyleTag("&lt;style&gt;.*&lt;/style&gt;");

QRegExp _rIncRN("\r\n(_|-|=|.*)*:");
void nesca_3::slotIncData(QString ip, QString str)
{
	if(RecvData != NULL)
	{
		QStringList headNbody = str.split("\r\n\r\n");
		QString tStr;
		QString tagRes;
		QString prot;
		int posTag = 0;
		for(int i = 0; i < headNbody.size(); ++i)
		{
			if(headNbody[i].size() == 0) headNbody.erase(headNbody.begin() + i);
		};
		if(headNbody.size() == 0) return;
		if(headNbody.size() > 1)
		{
			str = headNbody[0];
			tStr = str;

			_rIncRN.setMinimal(true);
			while ((posTag = _rIncRN.indexIn(tStr, posTag)) != -1) 
			{	
				tagRes = _rIncRN.cap(0);
				if(tagRes.size() > 0) 
				{
					str.replace(tagRes, "<font color=\"GoldenRod\">" + tagRes + "</font>");
				};
				tStr.replace(tagRes, "");
			};
			str.replace("HTTP/1.1", "<font color=\"GoldenRod\">HTTP/1.1</font>");
			str.replace("\r\n", "<br>");
			RecvData->append("<font color=\"#F0FFFF\">[" + ip + "]</font><br>[HEAD]<br>" + str + "<hr><br>");




			_rIncTags1.setMinimal(true);
			_rIncInnerTags.setMinimal(true);

			RecvData->append("<font color=\"#F0FFFF\">[" + ip + "]</font><br>[BODY]<br>");
			for(int i = 1; i < headNbody.size(); ++i)
			{
				str = headNbody[i].toHtmlEscaped();
				tStr = str;

				posTag = 0;
				while ((posTag = _rIncTags1.indexIn(tStr, posTag)) != -1) 
				{	
					tagRes = _rIncTags1.cap(0);
					if(tagRes.size() > 0) 
					{
						if(tagRes.contains("script") == false && tagRes.contains("style") == false)
						{
							str.replace(tagRes, "<font color=\"SteelBlue\">" + tagRes + "</font>");
						};
						tStr.replace(tagRes, "");
					};
				};

				posTag = 0;
				while ((posTag = _rIncTagsClose.indexIn(tStr, posTag)) != -1) 
				{	
					tagRes = _rIncTagsClose.cap(0);
					if(tagRes.size() > 0) 
					{
						if(tagRes.contains("script") == false && tagRes.contains("style") == false)
						{
							str.replace(tagRes, "<font color=\"SteelBlue\">" + tagRes + "</font>");
						};
						tStr.replace(tagRes, "");
					};
				};

				posTag = 0;
				while ((posTag = _rIncTags2.indexIn(tStr, posTag)) != -1) 
				{	
					tagRes = _rIncTags2.cap(0);
					if(tagRes.size() > 0) 
					{
						if(tagRes.contains("script") == false && tagRes.contains("style") == false)
						{
							str.replace(tagRes, "<font color=\"SteelBlue\">" + tagRes + "</font>");
						};
						tStr.replace(tagRes, "");
					};
				};

				posTag = 0;
				while ((posTag = _rIncInnerTags.indexIn(tStr, posTag)) != -1) 
				{	
					tagRes = _rIncInnerTags.cap(0);
					if(tagRes.size() > 0) 
					{
						str.replace(tagRes, "<font color=\"OliveDrab\">" + tagRes + "</font>");
						tStr.replace(tagRes, "");
					};
				};

				_rIncTags3.indexIn(str);
				prot = _rIncTags3.cap(0);
				if(prot.size() > 0) str.replace(prot, "<font color=\"SteelBlue\">" + prot + "</font>");

				_rIncScriptTag1.indexIn(str);
				prot = _rIncScriptTag1.cap(0);
				if(prot.size() > 0) str.replace(prot, "<font color=\"Maroon\">" + prot + "</font>");
				_rIncScriptTag2.indexIn(str);
				prot = _rIncScriptTag2.cap(0);
				if(prot.size() > 0) str.replace(prot, "<font color=\"Maroon\">" + prot + "</font>");
				str.replace("&lt;/script&gt;", "<font color=\"Maroon\">&lt;/script&gt;</font>");

				_rIncStyleTag.indexIn(str);
				prot = _rIncStyleTag.cap(0);
				if(prot.size() > 0) str.replace(prot, "<font color=\"NavajoWhite\">" + prot + "</font>");
				str.replace("&lt;style&gt;", "<font color=\"NavajoWhite\">&lt;style&gt;</font>");

				str.replace("\n", "<br>");
				RecvData->append(str);
			};
			RecvData->append("<hr><br>");
		}
		else
		{
			str = headNbody[0];
			tStr = str;

			_rIncRN.setMinimal(true);
			while ((posTag = _rIncRN.indexIn(tStr, posTag)) != -1) 
			{	
				tagRes = _rIncRN.cap(0);
				if(tagRes.size() > 0) 
				{
					str.replace(tagRes, "<font color=\"GoldenRod\">" + tagRes + "</font>");
				};
				tStr.replace(tagRes, "");
			};
			str.replace("HTTP/1.1", "<font color=\"SteelBlue\">HTTP/1.1</font>");
			str.replace("\r\n", "<br>");
			RecvData->append("<font color=\"#F0FFFF\">[" + ip + "]</font><br>" + str + "<hr><br>");
		};
	};
}

#pragma region SM_Buts
void nesca_3::smReaction()
{
	QObject *smB = this->sender();

	if(smB == ui->secretMessageBut_1)
	{
		if(smBit_1 == false)
		{
			smBit_1 = true;
			ui->secretMessageBut_1->setStyleSheet("border: 0.5px solid #313131; background-color:qlineargradient(spread:pad, x1:0.511, y1:0.477727, x2:0.512, y2:1, stop:0 rgba(0, 0, 0, 255), stop:0.0113636 rgba(83, 83, 83, 255), stop:0.0568182 rgba(98, 98, 98, 255), stop:0.0625 rgba(13, 167, 0, 255), stop:1 rgba(4, 255, 0, 255));");
		}
		else
		{
			smBit_1 = false;
			ui->secretMessageBut_1->setStyleSheet("border: 0.5px solid #313131; background-color:qlineargradient(spread:pad, x1:0.511, y1:0.477727, x2:0.512, y2:0, stop:0 rgba(0, 0, 0, 255), stop:0.0113636 rgba(83, 83, 83, 255), stop:0.0568182 rgba(98, 98, 98, 255), stop:0.0625 rgba(167, 0, 0, 255), stop:1 rgba(255, 0, 0, 255))");
		};
	} 
	else if(smB == ui->secretMessageBut_2)
	{ 
		if(smBit_2 == false)
		{
			smBit_2 = true;
			ui->secretMessageBut_2->setStyleSheet("border: 0.5px solid #313131; background-color:qlineargradient(spread:pad, x1:0.511, y1:0.477727, x2:0.512, y2:1, stop:0 rgba(0, 0, 0, 255), stop:0.0113636 rgba(83, 83, 83, 255), stop:0.0568182 rgba(98, 98, 98, 255), stop:0.0625 rgba(13, 167, 0, 255), stop:1 rgba(4, 255, 0, 255));");
		}
		else
		{
			smBit_2 = false;
			ui->secretMessageBut_2->setStyleSheet("border: 0.5px solid #313131; background-color:qlineargradient(spread:pad, x1:0.511, y1:0.477727, x2:0.512, y2:0, stop:0 rgba(0, 0, 0, 255), stop:0.0113636 rgba(83, 83, 83, 255), stop:0.0568182 rgba(98, 98, 98, 255), stop:0.0625 rgba(167, 0, 0, 255), stop:1 rgba(255, 0, 0, 255))");
		};
	}
	else if(smB == ui->secretMessageBut_3)
	{
		if(smBit_3 == false)
		{
			smBit_3 = true;
			ui->secretMessageBut_3->setStyleSheet("border: 0.5px solid #313131; background-color:qlineargradient(spread:pad, x1:0.511, y1:0.477727, x2:0.512, y2:1, stop:0 rgba(0, 0, 0, 255), stop:0.0113636 rgba(83, 83, 83, 255), stop:0.0568182 rgba(98, 98, 98, 255), stop:0.0625 rgba(13, 167, 0, 255), stop:1 rgba(4, 255, 0, 255));");
		}
		else
		{
			smBit_3 = false;
			ui->secretMessageBut_3->setStyleSheet("border: 0.5px solid #313131; background-color:qlineargradient(spread:pad, x1:0.511, y1:0.477727, x2:0.512, y2:0, stop:0 rgba(0, 0, 0, 255), stop:0.0113636 rgba(83, 83, 83, 255), stop:0.0568182 rgba(98, 98, 98, 255), stop:0.0625 rgba(167, 0, 0, 255), stop:1 rgba(255, 0, 0, 255))");
		};
	} 
	else if(smB == ui->secretMessageBut_4)
	{
		if(smBit_4 == false)
		{
			smBit_4 = true;
			ui->secretMessageBut_4->setStyleSheet("border: 0.5px solid #313131; background-color:qlineargradient(spread:pad, x1:0.511, y1:0.477727, x2:0.512, y2:1, stop:0 rgba(0, 0, 0, 255), stop:0.0113636 rgba(83, 83, 83, 255), stop:0.0568182 rgba(98, 98, 98, 255), stop:0.0625 rgba(13, 167, 0, 255), stop:1 rgba(4, 255, 0, 255));");
		}
		else
		{
			smBit_4 = false;
			ui->secretMessageBut_4->setStyleSheet("border: 0.5px solid #313131; background-color:qlineargradient(spread:pad, x1:0.511, y1:0.477727, x2:0.512, y2:0, stop:0 rgba(0, 0, 0, 255), stop:0.0113636 rgba(83, 83, 83, 255), stop:0.0568182 rgba(98, 98, 98, 255), stop:0.0625 rgba(167, 0, 0, 255), stop:1 rgba(255, 0, 0, 255))");
		};
	}
	else if(smB == ui->secretMessageBut_5)
	{
		if(smBit_5 == false)
		{
			smBit_5 = true;
			ui->secretMessageBut_5->setStyleSheet("border: 0.5px solid #313131; background-color:qlineargradient(spread:pad, x1:0.511, y1:0.477727, x2:0.512, y2:1, stop:0 rgba(0, 0, 0, 255), stop:0.0113636 rgba(83, 83, 83, 255), stop:0.0568182 rgba(98, 98, 98, 255), stop:0.0625 rgba(13, 167, 0, 255), stop:1 rgba(4, 255, 0, 255));");
		}
		else
		{
			smBit_5 = false;
			ui->secretMessageBut_5->setStyleSheet("border: 0.5px solid #313131; background-color:qlineargradient(spread:pad, x1:0.511, y1:0.477727, x2:0.512, y2:0, stop:0 rgba(0, 0, 0, 255), stop:0.0113636 rgba(83, 83, 83, 255), stop:0.0568182 rgba(98, 98, 98, 255), stop:0.0625 rgba(167, 0, 0, 255), stop:1 rgba(255, 0, 0, 255))");
		};
	}
	else if(smB == ui->secretMessageBut_6)
	{
		if(smBit_6 == false)
		{
			smBit_6 = true;
			ui->secretMessageBut_6->setStyleSheet("border: 0.5px solid #313131; background-color:qlineargradient(spread:pad, x1:0.511, y1:0.477727, x2:0.512, y2:1, stop:0 rgba(0, 0, 0, 255), stop:0.0113636 rgba(83, 83, 83, 255), stop:0.0568182 rgba(98, 98, 98, 255), stop:0.0625 rgba(13, 167, 0, 255), stop:1 rgba(4, 255, 0, 255));");
		}
		else
		{
			smBit_6 = false;
			ui->secretMessageBut_6->setStyleSheet("border: 0.5px solid #313131; background-color:qlineargradient(spread:pad, x1:0.511, y1:0.477727, x2:0.512, y2:0, stop:0 rgba(0, 0, 0, 255), stop:0.0113636 rgba(83, 83, 83, 255), stop:0.0568182 rgba(98, 98, 98, 255), stop:0.0625 rgba(167, 0, 0, 255), stop:1 rgba(255, 0, 0, 255))");
		};
	}
	else if(smB == ui->secretMessageBut_7)
	{
		if(smBit_7 == false)
		{
			smBit_7 = true;
			ui->secretMessageBut_7->setStyleSheet("border: 0.5px solid #313131; background-color:qlineargradient(spread:pad, x1:0.511, y1:0.477727, x2:0.512, y2:1, stop:0 rgba(0, 0, 0, 255), stop:0.0113636 rgba(83, 83, 83, 255), stop:0.0568182 rgba(98, 98, 98, 255), stop:0.0625 rgba(13, 167, 0, 255), stop:1 rgba(4, 255, 0, 255));");
		}
		else
		{
			smBit_7 = false;
			ui->secretMessageBut_7->setStyleSheet("border: 0.5px solid #313131; background-color:qlineargradient(spread:pad, x1:0.511, y1:0.477727, x2:0.512, y2:0, stop:0 rgba(0, 0, 0, 255), stop:0.0113636 rgba(83, 83, 83, 255), stop:0.0568182 rgba(98, 98, 98, 255), stop:0.0625 rgba(167, 0, 0, 255), stop:1 rgba(255, 0, 0, 255))");
		};
	}
	else if(smB == ui->secretMessageBut_8)
	{
		if(smBit_8 == false)
		{
			smBit_8 = true;
			ui->secretMessageBut_8->setStyleSheet("border: 0.5px solid #313131; background-color:qlineargradient(spread:pad, x1:0.511, y1:0.477727, x2:0.512, y2:1, stop:0 rgba(0, 0, 0, 255), stop:0.0113636 rgba(83, 83, 83, 255), stop:0.0568182 rgba(98, 98, 98, 255), stop:0.0625 rgba(13, 167, 0, 255), stop:1 rgba(4, 255, 0, 255));");
		}
		else
		{
			smBit_8 = false;
			ui->secretMessageBut_8->setStyleSheet("border: 0.5px solid #313131; background-color:qlineargradient(spread:pad, x1:0.511, y1:0.477727, x2:0.512, y2:0, stop:0 rgba(0, 0, 0, 255), stop:0.0113636 rgba(83, 83, 83, 255), stop:0.0568182 rgba(98, 98, 98, 255), stop:0.0625 rgba(167, 0, 0, 255), stop:1 rgba(255, 0, 0, 255))");
		};
	};
	
	if(smBit_1 && smBit_2 && smBit_3 && smBit_4 && smBit_5 && smBit_6 && smBit_7 && smBit_8)
	{
		gDebugMode = true;
		stt->doEmitionDebugFoundData("[Debug mode ON]");		
	}
	else if(smBit_1 && smBit_2 == false && smBit_3 && smBit_4 == false && smBit_5 == false && smBit_6 == false && smBit_7 == false && smBit_8 == false)
	{
		code160 = true;
		stt->doEmitionDebugFoundData("[Tags-in-chat mode ON]");		
	}
	else if(smBit_1 == false && smBit_2 == false && smBit_3 == false && smBit_4 == false && smBit_5 == false && smBit_6 == false && smBit_7 && smBit_8)
	{
		gNegDebugMode = true;
		stt->doEmitionDebugFoundData("[Negative-hit mode ON]");		
	}
	else if(smBit_1 == false && smBit_2 && smBit_3 && smBit_4 && smBit_5 && smBit_6 && smBit_7 && smBit_8)
	{
		HTMLDebugMode = true;
		stt->doEmitionDebugFoundData("[HTML Debug mode ON]");
	}
	
	else
	{
		if(gDebugMode)
		{
			gDebugMode = false;
			stt->doEmitionDebugFoundData("[Debug mode OFF]");
		};
		if(code160)
		{
			code160 = false;
			stt->doEmitionDebugFoundData("[Tags-in-chat mode OFF]");
		};
		if(gNegDebugMode)
		{
			gNegDebugMode = false;
			stt->doEmitionDebugFoundData("[Negative-hit mode OFF]");
		};
		if(HTMLDebugMode)
		{
			HTMLDebugMode = false;
			stt->doEmitionDebugFoundData("[HTML Debug mode OFF]");
		};
	};
}

void nesca_3::IPScanSeq()
{
	if(ui->ipLine->text() != "")
	{
		if(ui->portLine->text() != "")
		{
			ui->labelParsed_Value->setText("0/0");
			stopFirst = false;
			saveOptions();
			ui->tabMainWidget->setTabEnabled(1, false);
			ui->tabMainWidget->setTabEnabled(2, false);

			QString rangeData = ui->ipLine->text();
			if(rangeData.indexOf("-") > 0)
			{
				rangeData = ui->ipLine->text();
			}
			else
			{
				if(rangeData.indexOf("/") < 0)
				{
					rangeData = ui->ipLine->text() + "-" + ui->ipLine->text();
				}
				else
				{
					rangeData = ui->ipLine->text();
				};
			};

			strcpy(inputStr, ("DUMMY|0|" + rangeData + "|" + ui->threadLine->text() + "|-p" + ui->portLine->text().replace(" ", "")).toLocal8Bit().data());

			stt->start();
			startFlag = true;
			globalScanFlag = true;
			ui->startScanButton_3->setText("Stop");
			ui->startScanButton_3->setStyleSheet(
				" QPushButton {"
				"background-color: qlineargradient(spread:none, x1:1, y1:0, x2:1, y2:1, stop:0.681818 rgba(0, 0, 0, 250), stop:1 rgba(255, 255, 255, 130));"
				"color: red;"
				"border: 0.5px solid qlineargradient(spread:reflect, x1:0.54, y1:0.488591, x2:0.54, y2:0, stop:0 rgba(255, 255, 255, 130), stop:1 rgba(0, 0, 0, 255));"
				"}"
				);
			ui->dataText->clear();
		}
		else
		{
			stt->doEmitionRedFoundData("No ports specified!");				
		};
	}
	else
	{
		if(stopFirst == false)
		{
			stopFirst = true;
			globalScanFlag = false;
			ui->startScanButton_3->setStyleSheet(
				" QPushButton {"
				"background-color: qlineargradient(spread:none, x1:1, y1:0, x2:1, y2:1, stop:0.681818 rgba(0, 0, 0, 250), stop:1 rgba(255, 255, 255, 130));"
				"color: red;"
				"border: 0.5px solid qlineargradient(spread:reflect, x1:0.54, y1:0.488591, x2:0.54, y2:0, stop:0 rgba(255, 255, 255, 130), stop:1 rgba(0, 0, 0, 255));"
				"}"
				);

			ui->startScanButton_3->setText("STOP!");
			stt->doEmitionYellowFoundData("Trying to stop. Please, wait...");
		}
		else
		{
			globalScanFlag = false;
			ui->startScanButton_3->setStyleSheet(
				" QPushButton {"
				"background-color: qlineargradient(spread:none, x1:1, y1:0, x2:1, y2:1, stop:0.681818 rgba(0, 0, 0, 250), stop:1 rgba(255, 255, 255, 130));"
				"color: yellow;"
				"border: 0.5px solid qlineargradient(spread:reflect, x1:0.54, y1:0.488591, x2:0.54, y2:0, stop:0 rgba(255, 255, 255, 130), stop:1 rgba(0, 0, 0, 255));"
				"}"
				);

			ui->startScanButton_3->setText("Wait...");
			stt->doEmitionYellowFoundData("Wait, killing threads...");
			STTTerminate();
		};
	};
}

void nesca_3::DNSScanSeq()
{
	if(ui->lineEditStartIPDNS->text() != "")
	{
		if(ui->lineEditPort->text() != "")
		{
			ui->labelParsed_Value->setText("0/0");
			if(ui->lineEditStartIPDNS->text().indexOf(".") > 0)
			{
				QStringList lst = ui->lineEditStartIPDNS->text().split(".");
				ui->lineEditStartIPDNS->setText(lst[0]);
				QString topLevelDomainStr;
				for(int i = 1; i < lst.size(); ++i)
				{
					topLevelDomainStr += ".";
					topLevelDomainStr += lst[i];
				};
				ui->lineILVL->setText(topLevelDomainStr);
			};

			stopFirst = false;
			saveOptions();

			ui->tabMainWidget->setTabEnabled(0, false);
			ui->tabMainWidget->setTabEnabled(2, false);
			
			strcpy(inputStr, ("DUMMY|1|" + ui->lineEditStartIPDNS->text() + "|" + ui->lineILVL->text() + "|" + ui->lineEditThread->text() + "|-p" + ui->lineEditPort->text().replace(" ", "")).toLocal8Bit().data());

			stt->start();
			startFlag = true;
			globalScanFlag = true;
			ui->startScanButton_4->setText("Stop");
			ui->startScanButton_4->setStyleSheet(
				" QPushButton {"
				"background-color: qlineargradient(spread:none, x1:1, y1:0, x2:1, y2:1, stop:0.681818 rgba(0, 0, 0, 250), stop:1 rgba(255, 255, 255, 130));"
				"color: red;"
				"border: 0.5px solid qlineargradient(spread:reflect, x1:0.54, y1:0.488591, x2:0.54, y2:0, stop:0 rgba(255, 255, 255, 130), stop:1 rgba(0, 0, 0, 255));"
				"}"
				);
			ui->dataText->clear();
		}
		else
		{
			stt->doEmitionRedFoundData("No ports specified!");	
		};
	}
	else
	{
		stt->doEmitionRedFoundData("Wrong mask input.");
	};
}

void nesca_3::ImportScanSeq()
{
	QString fileName;

	if(importFileName.size() == 0) fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
		"",
		tr("Files (*.txt)")
		);
	else fileName = importFileName;
	if(fileName != "")
	{
		ui->tabMainWidget->setTabEnabled(0, false);
		ui->tabMainWidget->setTabEnabled(1, false);

		strcpy(inputStr, ("DUMMY|-f|" + fileName + "|" + ui->importThreads->text() + "|-p" + ui->importPorts->text().replace(" ", "")).toLocal8Bit().data());	

		globalScanFlag = true;
		stt->start();
		startFlag = true;
		stopFirst = false;
		pbTh->start();
		ui->importButton->setText("Stop");
		ui->importButton->setStyleSheet(
			" #importButton {"
			"background-color: qlineargradient(spread:none, x1:1, y1:0, x2:1, y2:1, stop:0.681818 rgba(0, 0, 0, 250), stop:1 rgba(255, 255, 255, 130));"
			"color: red;"
			"border: 0.5px solid qlineargradient(spread:reflect, x1:0.54, y1:0.488591, x2:0.54, y2:0, stop:0 rgba(255, 255, 255, 130), stop:1 rgba(0, 0, 0, 255));"
			"}"
			);
		ui->dataText->clear();
	}
	else
	{
		stt->doEmitionYellowFoundData("Empty filename.");
	};
}

QLabel *smsgLbl;
QLabel *smsgNLbl;
void nesca_3::slotShowServerMsg(QString str)
{
	 QMessageBox msgBox;
	 msgBox.setWindowFlags			( Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint|Qt::SubWindow );
	 msgBox.setStyleSheet("border: 0.5px solid #7c7c7c; background-color:rgba(25, 25, 25, 255); color:#7c7c7c;");
	 msgBox.setText("Server message");
	 msgBox.setInformativeText(str);
	 msgBox.setContentsMargins(0, 0, 25, 15);
	 msgBox.setStandardButtons(QMessageBox::Ok);
	 msgBox.setDefaultButton(QMessageBox::Ok);
	 msgBox.button(QMessageBox::Ok)->setStyleSheet(
				"background-color: qlineargradient(spread:none, x1:1, y1:0, x2:1, y2:1, stop:0.681818 rgba(0, 0, 0, 250), stop:1 rgba(255, 255, 255, 130));"
				"color: gray;"
				"border: 0.5px solid qlineargradient(spread:reflect, x1:0.54, y1:0.488591, x2:0.54, y2:0, stop:0 rgba(255, 255, 255, 130), stop:1 rgba(0, 0, 0, 255));"
				"width: 20px;"
				"position: absolute;"
				);
	 msgBox.exec();
}

void nesca_3::DNSLine_ValueChanged()
{
	if(globalScanFlag == 0) ui->startScanButton_4->setText("Start");	
}

void nesca_3::slotRestoreDefPorts()
{
	int ci = ui->tabMainWidget->currentIndex();

	if(ci == 0)
	{
		ui->portLine->setText(PORTSET);
	}
	else if(ci == 1)
	{
		ui->lineEditPort->setText(PORTSET);
	}
	else if(ci == 2)
	{
		ui->importPorts->setText(PORTSET);
	};
}

QGraphicsTextItem *textItem = NULL;
QGraphicsRectItem* pbItem = NULL;
QGraphicsRectItem* pbBlackRectItem = NULL;
QPen pbPen(QColor(227, 227, 227, 150));
QFont pbPointerFont;
void nesca_3::slotPBUpdate()
{
	int val = this->perc;
	if(textItem != NULL)
	{
		delete textItem;
		textItem = NULL;
	};
	if(pbItem != NULL)
	{
		delete pbItem;
		pbItem = NULL;
	};
	if(pbBlackRectItem != NULL)
	{
		delete pbBlackRectItem;
		pbBlackRectItem = NULL;
	};
	pbScene->clear();
	QLinearGradient grad1(0, 0, 0, 110);

	if(val < 33) grad1.setColorAt(0.1, QColor(207, 0, 0));
	else if( val < 66 ) grad1.setColorAt(0.1, QColor(247, 244, 0));
	else if( val < 99 ) grad1.setColorAt(0.1, QColor(0, 207, 0));

	QLinearGradient gradBlack(0, 0, 0, 110);
	gradBlack.setColorAt(0.1, QColor(0, 0, 0));
	pbBlackRectItem = new QGraphicsRectItem(0,0,5,99);
	pbBlackRectItem->setBrush(gradBlack);
	pbScene->addItem(pbBlackRectItem);

	pbItem = new QGraphicsRectItem(0,0,5,val);
	pbItem->setBrush(grad1);
	pbScene->addItem(pbItem);
	
	textItem = pbScene->addText("- " + QString::number(val) + "%", pbPointerFont);
	textItem->setX(2);
	textItem->setY(val - 10);
	textItem->setDefaultTextColor(QColor(255, 255, 255, 180));

	pbScene->addLine(4, 11, 6, 11, pbPen);
	pbScene->addLine(4, 22, 6, 22, pbPen);
	pbScene->addLine(0, 33, 8, 33, pbPen);
	pbScene->addLine(4, 44, 6, 44, pbPen);
	pbScene->addLine(4, 55, 6, 55, pbPen);
	pbScene->addLine(0, 66, 8, 66, pbPen);
	pbScene->addLine(4, 77, 6, 77, pbPen);
	pbScene->addLine(4, 88, 6, 88, pbPen);
}

void nesca_3::changeNSTrackLabel(bool status)
{
	if(status) ui->NSTrackStatusLabel->setStyleSheet("background-color: green; border: 1px solid white;");
	else ui->NSTrackStatusLabel->setStyleSheet("background-color: black; border: 1px solid white;");
}

#pragma region "Signal assignments"
void nesca_3::ConnectEvrthng()
{
	connect ( pbTh, SIGNAL(upd()), this, SLOT(slotPBUpdate()));
	connect ( ui->secretMessageBut_1, SIGNAL( clicked() ), this, SLOT( smReaction() ) );
	connect ( ui->secretMessageBut_2, SIGNAL( clicked() ), this, SLOT( smReaction() ) );
	connect ( ui->secretMessageBut_3, SIGNAL( clicked() ), this, SLOT( smReaction() ) );
	connect ( ui->secretMessageBut_4, SIGNAL( clicked() ), this, SLOT( smReaction() ) );
	connect ( ui->secretMessageBut_5, SIGNAL( clicked() ), this, SLOT( smReaction() ) );
	connect ( ui->secretMessageBut_6, SIGNAL( clicked() ), this, SLOT( smReaction() ) );
	connect ( ui->secretMessageBut_7, SIGNAL( clicked() ), this, SLOT( smReaction() ) );
	connect ( ui->secretMessageBut_8, SIGNAL( clicked() ), this, SLOT( smReaction() ) );
	connect ( ui->nickList, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(slotItemClicked(QListWidgetItem*)));
	connect ( ui->nickShowBut, SIGNAL( clicked() ), this, SLOT( slotShowNicks() ) );
	connect ( ui->ircText, SIGNAL( anchorClicked(QUrl) ), this, SLOT( onLinkClicked(QUrl) ) );
	connect ( ui->ircRaw, SIGNAL( anchorClicked(QUrl) ), this, SLOT( onLinkClicked(QUrl) ) );
	connect ( ui->dataText, SIGNAL( anchorClicked(QUrl) ), this, SLOT( onLinkClicked(QUrl) ) );
	connect ( ui->ircProxy, SIGNAL( returnPressed() ), this, SLOT( CheckProxy() ) );
	connect ( ui->ircProxyPort, SIGNAL( returnPressed() ), this, SLOT( CheckProxy() ) );
	connect ( ui->shoutBox, SIGNAL( returnPressed() ), this, SLOT( SaySmthng() ) );
	connect ( ui->ircNickBox, SIGNAL( returnPressed() ), this, SLOT( ChangeNick() ) );
	connect ( ui->ircServerBox, SIGNAL( returnPressed() ), this, SLOT( ConnectToIRCServer() ) );
	connect ( ui->serverPortBox, SIGNAL( returnPressed() ), this, SLOT( ConnectToIRCServer() ) );
	connect ( ui->checkKeyBut, SIGNAL( clicked() ), this, SLOT( CheckPersKey() ) );
	connect ( ui->DataflowModeBut, SIGNAL( clicked() ), this, SLOT( slotShowDataflow() ) );
	connect ( ui->IRCConnectBut, SIGNAL( clicked() ), this, SLOT( ConnectToIRCServer() ) );
	connect ( ui->IRCModeChangerBut, SIGNAL( clicked() ), this, SLOT( ChangeIRCRawLog() ) );
	connect ( ui->JobModeBut, SIGNAL( clicked() ), this, SLOT( switchToJobMode() ) );
	connect ( ui->IRCModeBut, SIGNAL( clicked() ), this, SLOT( ChangeDispalyMode() ) );
	connect ( ui->clearLogBut, SIGNAL( clicked() ), this, SLOT( slotClearLogs() ) );
	connect ( ui->IRCUTFMode, SIGNAL( clicked() ), this, SLOT( slotChangeCPModeToUTF() ) );
	connect ( ui->IRCCP1251Mode, SIGNAL( clicked() ), this, SLOT( slotChangeCPModeTo1251() ) );
	connect ( mct, SIGNAL(showNewMsg(QString)), this, SLOT(slotShowServerMsg(QString)));
	connect ( tray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(trayButtonClicked()));
	connect ( ui->exitButton, SIGNAL( clicked() ), this, SLOT( exitButtonClicked() ) );
	connect ( ui->trayButton, SIGNAL( clicked() ), this, SLOT( trayButtonClicked() ) );
	connect ( ui->switcherBut, SIGNAL( clicked() ), this, SLOT( switchDataFields() ) );
	connect ( ui->importButton, SIGNAL( clicked() ), this, SLOT( importAndScan() ) );
	connect ( ui->startScanButton_3, SIGNAL( clicked() ), this, SLOT( startScanButtonClicked() ) );
	connect ( ui->startScanButton_4, SIGNAL( clicked() ), this, SLOT( startScanButtonClickedDNS() ) );
	connect ( ui->shuffle_onoff, SIGNAL(toggled(bool)), this, SLOT(ChangeShuffle(bool)));
	connect ( ui->trackerOnOff, SIGNAL(toggled(bool)), this, SLOT(ChangeTrackerOK(bool)));
	connect ( ui->pingingOnOff, SIGNAL( toggled(bool) ), this, SLOT( ChangePingerOK(bool) ) );
	connect ( ui->debugFileOnOff, SIGNAL( toggled(bool) ), this, SLOT( ChangeDebugFileState(bool) ) );
	connect ( ui->importThreads, SIGNAL( textChanged(QString) ), this, SLOT( ChangeLabelThreads_ValueChanged(QString) ) );
	connect ( ui->threadLine, SIGNAL( textChanged(QString) ), this, SLOT( ChangeLabelThreads_ValueChanged(QString) ) );
	connect ( ui->PingTO, SIGNAL( textChanged(QString) ), this, SLOT( PingTO_ChangeValue(QString) ) );
	connect ( ui->threadDelayBox, SIGNAL( textChanged(QString) ), this, SLOT( ThreadDelay_ChangeValue(QString) ) );
	connect ( ui->maxBrutingThrBox, SIGNAL( textChanged(QString) ), this, SLOT( MaxBrutingThr_ChangeValue(QString) ) );
	connect ( ui->lineEditThread, SIGNAL( textChanged(QString) ), this, SLOT( ChangeLabelThreads_ValueChanged(QString) ) );
	connect ( ui->iptoLine_value, SIGNAL( textChanged(QString) ), this, SLOT( ChangeLabelTO_ValueChanged(QString) ) );
	connect ( ui->iptoLine_value_2, SIGNAL( textChanged(QString) ), this, SLOT( ChangeLabelTO_ValueChanged(QString) ) );
	connect ( ui->iptoLine_value_3, SIGNAL( textChanged(QString) ), this, SLOT( ChangeLabelTO_ValueChanged(QString) ) );
	connect ( ui->restoreDefaultPorts1, SIGNAL( clicked() ), this, SLOT( slotRestoreDefPorts() ) );
	connect ( ui->restoreDefaultPorts2, SIGNAL( clicked() ), this, SLOT( slotRestoreDefPorts() ) );
	connect ( ui->restoreDefaultPorts3, SIGNAL( clicked() ), this, SLOT( slotRestoreDefPorts() ) );
	connect ( ui->IRCCheckProxyBut, SIGNAL( clicked() ), this, SLOT( CheckProxy() ) );
	connect ( ui->lineEditStartIPDNS, SIGNAL( textChanged(QString) ), this, SLOT( DNSLine_ValueChanged(QString) ) );
	connect ( ui->ipLine, SIGNAL(				returnPressed() ), this, SLOT(		startScanButtonClicked() ) );
	connect ( ui->threadLine, SIGNAL(			returnPressed() ), this, SLOT(		startScanButtonClicked() ) );
	connect ( ui->portLine, SIGNAL(				returnPressed() ), this, SLOT(		startScanButtonClicked() ) );
	connect ( ui->lineEditStartIPDNS, SIGNAL(	returnPressed() ), this, SLOT(		startScanButtonClickedDNS() ) );
	connect ( ui->lineILVL, SIGNAL(				returnPressed() ), this, SLOT(		startScanButtonClickedDNS() ) );
	connect ( ui->lineEditPort, SIGNAL(			returnPressed() ), this, SLOT(		startScanButtonClickedDNS() ) );
	connect ( ui->lineEditThread, SIGNAL(		returnPressed() ), this, SLOT(		startScanButtonClickedDNS() ) );
	connect ( ui->logoLabel, SIGNAL( clicked() ), this, SLOT( logoLabelClicked() ) );
	connect ( ui->me2ScanBut, SIGNAL( clicked() ), this, SLOT( activateME2ScanScene() ) );
	connect ( ui->QoSScanBut, SIGNAL( clicked() ), this, SLOT( activateQoSScanBut() ) );
	connect ( ui->VoiceScanBut, SIGNAL( clicked() ), this, SLOT( activateVoiceScanBut() ) );
	connect ( ui->PieStatBut, SIGNAL( clicked() ), this, SLOT( activatePieStatBut() ) );
	connect ( ui->topicLine, SIGNAL( returnPressed() ), this, SLOT( ChangeTopic() ) );
	connect ( stt, SIGNAL(showRedVersion()), this, SLOT(slotShowRedVersion()));
	connect ( stt, SIGNAL(startScanIP()), this, SLOT(IPScanSeq()));
	connect ( stt, SIGNAL(startScanDNS()), this, SLOT(DNSScanSeq()));
	connect ( stt, SIGNAL(startScanImport()), this, SLOT(ImportScanSeq()));
	connect ( stt, SIGNAL(sIncData(QString, QString)), this, SLOT(slotIncData(QString, QString)));
	connect ( stt, SIGNAL(sOutData(QString, QString)), this, SLOT(slotOutData(QString, QString)));
	connect ( stt, SIGNAL(changeDebugFoundData(QString)), this, SLOT(appendDebugText(QString)));
	connect ( stt, SIGNAL(changeYellowFoundData(QString)), this, SLOT(appendNotifyText(QString)));
	connect ( stt, SIGNAL(changeRedFoundData(QString)), this, SLOT(appendErrText(QString)));
	connect ( stt, SIGNAL(changeGreenFoundData(QString)), this, SLOT(appendOKText(QString)));
	connect ( stt, SIGNAL(killSttThread()), this, SLOT(STTTerminate()));
	connect ( stt, SIGNAL(changeParsedValue(QString)), ui->labelParsed_Value, SLOT(setText(QString)));		
	connect ( stt, SIGNAL(changeIpRange(QString)), ui->labelIpRange_Value, SLOT(setText(QString)));
	connect ( stt, SIGNAL(changeThreads(QString)), ui->labelThreads_Value, SLOT(setText(QString)));
	connect ( stt, SIGNAL(changeIPS(QString)), ui->labelIPS_Value, SLOT(setText(QString)));
	connect ( stt, SIGNAL(changeFoundData(QString)), this, SLOT(appendDefaultText(QString)));
	connect ( stt, SIGNAL(changeBAValue(QString)), ui->labelBAThreads_Value, SLOT(setText(QString)));
	connect ( stt, SIGNAL(changeStatus(QString)), ui->labelStatus_Value, SLOT(setText(QString)));
	connect ( stt, SIGNAL(changeTargetsLeft(QString)), ui->labelTargetsLeft_Value, SLOT(setText(QString)));
	connect ( stt, SIGNAL(changeOffline(QString)), ui->labelOffline_Value, SLOT(setText(QString)));
	connect ( stt, SIGNAL(changeBAData(QString)), ui->BAText, SLOT(append(QString)));
	connect ( stt, SIGNAL(changeGreenBAData(QString)), this, SLOT(appendGreenBAData(QString)));
	connect ( stt, SIGNAL(changeRedBAData(QString)), this, SLOT(appendRedBAData(QString)));
	connect ( stt, SIGNAL(signalDataSaved(bool)), this, SLOT(changeNSTrackLabel(bool)));
	connect ( chPTh, SIGNAL(changeGreenIRCData(QString)), this, SLOT(appendGreenIRCText(QString)));
	connect ( chPTh, SIGNAL(changeRedIRCData(QString)), this, SLOT(appendRedIRCText(QString)));
	connect ( chPTh, SIGNAL(changeYellowIRCData(QString)), this, SLOT(appendYellowIRCText(QString)));
	connect ( chPTh, SIGNAL(changeIRCData(bool, bool, int, QString, QString)), this, SLOT(appendDefaultIRCText(bool, bool, int, QString, QString)));
	connect ( ircTh, SIGNAL(getTopic(QString)), this, SLOT(slotIRCGetTopic(QString)) );
	connect ( ircTh, SIGNAL(IRCOfflined()), this, SLOT(slotIRCOfflined()) );
	connect ( ircTh, SIGNAL(sUnhidePopup(QString, QString)), this, SLOT(slotUnhidePopup(QString, QString)));
	connect ( ircTh, SIGNAL(ClearNickList()), this, SLOT(slotClearNickList()));
	connect ( ircTh, SIGNAL(AddNick(QString)), this, SLOT(slotAppendIRCNick(QString)));
	connect ( ircTh, SIGNAL(changeGreenIRCData(QString)), this, SLOT(appendGreenIRCText(QString)));
	connect ( ircTh, SIGNAL(changeRedIRCData(QString)), this, SLOT(appendRedIRCText(QString)));
	connect ( ircPTh, SIGNAL(changeRedIRCData(QString)), this, SLOT(appendRedIRCText(QString)));
	connect ( ircPTh, SIGNAL(RestartIRC()), this, SLOT(slotRestartIRC()));
	connect ( ircTh, SIGNAL(changeYellowIRCData(QString)), this, SLOT(appendYellowIRCText(QString)));
	connect ( ircTh, SIGNAL(changeIRCData(bool, bool, int, QString, QString)), this, SLOT(appendDefaultIRCText(bool, bool, int, QString, QString)));
	connect ( ircTh, SIGNAL(changeRawIRCDataInc(QString)), this, SLOT(appendDefaultIRCRawTextInc(QString)));
	connect ( ircTh, SIGNAL(changeRawIRCDataOut(QString)), this, SLOT(appendDefaultIRCRawTextOut(QString)));
	connect ( chPTh, SIGNAL(changeRawIRCDataInc(QString)), this, SLOT(appendDefaultIRCRawTextInc(QString)));
	connect ( chPTh, SIGNAL(changeRawIRCDataOut(QString)), this, SLOT(appendDefaultIRCRawTextOut(QString)));
	connect ( ircTh, SIGNAL(setNick(QString)), this, SLOT(setNickBox(QString)));
	connect ( ircTh, SIGNAL(changeIRCDataOut(QString)), this, SLOT(appendDefaultIRCTextOut(QString)));
	connect ( ircTh, SIGNAL(notifyPlay()), this, SLOT(playFcknSound()));
	connect ( adtHN, SIGNAL(sDrawActivityLine(QString)), this, SLOT(slotDrawActivityLine(QString)));
	connect ( adtHN, SIGNAL(sDrawGrid()), this, SLOT(slotDrawActivityGrid()));
	connect ( dtHN, SIGNAL(sAddDelimLines()), this, SLOT(slotDrawDelimLines()));
	connect ( dtHN, SIGNAL(sDrawGrid()), this, SLOT(slotDrawGrid()));
	connect ( dtHN, SIGNAL(sAddLine(int, int, int, int)), this, SLOT(slotAddLine(int, int, int, int)));
	connect ( dtME2, SIGNAL(sAddPolyLine()), this, SLOT(slotAddPolyLine()));
	connect ( dtME2, SIGNAL(sDrawTextPlacers()), this, SLOT(slotDrawTextPlacers()));
	connect ( dtQoS, SIGNAL(sAddLine()), this, SLOT(slotQoSAddLine()));
	connect ( dtGridQoS, SIGNAL(sAddLine()), this, SLOT(slotQoSAddGrid()));
	connect ( dtGridQoS, SIGNAL(sAddDelimLines()), this, SLOT(slotQoSDrawDelimLines()));
	connect ( vsTh, SIGNAL(sAddLine()), this, SLOT(slotVoiceAddLine()));
	connect ( vsTh, SIGNAL(sDrawGrid(int)), this, SLOT(slotDrawVoiceGrid(int)));
	connect ( vsTh, SIGNAL(sDrawTextPlacers()), this, SLOT(slotDrawTextPlacers()));
	connect ( psTh, SIGNAL(sUpdatePie()), this, SLOT(slotUpdatePie()) );
	connect ( irc_nmb, SIGNAL(sBlinkMessage()), this, SLOT(slotBlinkMessage()) );
}

void RestoreSession()
{
	FILE *resFile = fopen("restore", "r");
	char resStr[128] = {0};
	char *lex;

	if(resFile != NULL)
	{

		stt->doEmitionYellowFoundData("Previous session file found! Restoring...");

		while(fgets(resStr, 128, resFile) != NULL)
		{
			if(strstr(resStr, "[SESSION]:") != NULL)
			{
				
				lex = strtok(strstr(resStr, "[SESSION]:") + strlen("[SESSION]:"), " ");
				gMode = atoi(lex);
				lex = strtok(NULL, " ");
				if (lex == nullptr) {
					stt->doEmitionRedFoundData("Restore file is corrupted.");
					return;
				}

				if(gMode == 0)
				{
					if(strstr(lex, "-") != NULL)
					{
						strncpy(gRange, lex, (int)(strstr(lex, "-") - lex));
						strcat(gRange, "-");
						strcat(gRange, strstr(lex, "-") + 1);

						lex = strtok(NULL, " ");
						gThreads = atoi(lex);

						ui->threadLine->setText(QString(lex));
						ui->ipLine->setText(QString(gRange));
						ui->tabMainWidget->setCurrentIndex(0);
					}
					else if(strstr(lex, "/") != NULL)
					{
						strncpy(gRange, lex, (int)(strstr(lex, "/") - lex));
						strcat(gRange, "/");
						strcat(gRange, strstr(lex, "/") + 1);

						lex = strtok(NULL, " ");
						gThreads = atoi(lex);

						ui->threadLine->setText(QString(lex));
						ui->ipLine->setText(QString(gRange));
						ui->tabMainWidget->setCurrentIndex(0);
					};
				}
				else if(gMode == 1)
				{
					ui->lineEditStartIPDNS->setText(QString(lex));
					lex = strtok(NULL, " ");
					strcpy(gFirstDom, lex);

					lex = strtok(NULL, " ");
					gThreads = atoi(lex);
					ui->lineEditThread->setText(QString(lex));
					ui->lineILVL->setText(QString(gFirstDom));
					ui->tabMainWidget->setCurrentIndex(1);
					ui->startScanButton_4->setText("RESTORE");
				}
				else if(gMode == -1)
				{
					lex = strtok(NULL, " ");
					gThreads = atoi(lex);

					ui->importThreads->setText(QString(lex));
					
					FILE *testFile = fopen("tempIPLst.bk", "r");
					if(testFile != NULL) 
					{
						ui->importButton->setText("RESTORE");
						importFileName = "tempIPLst.bk";
						fclose(testFile);
					};
					ui->tabMainWidget->setCurrentIndex(2);			
				};

				lex = strtok(NULL, " ");

				if(lex != NULL && strstr(lex, "-p") != NULL)
				{
					QString PortString = "";

					char *tPorts = strstr(lex, "-p");
					int strln = strlen(tPorts);
					char *fPorts = new char[strln];
					strncpy(fPorts, tPorts + 2, strln);
					PortString = QString(fPorts);

					PortString.replace("\n", "");

					ui->lineEditPort->setText(PortString);
					ui->portLine->setText(PortString);
					ui->importPorts->setText(PortString);
					
					delete []fPorts;
				}
				else
				{
					ui->lineEditPort->setText(PORTSET);
					ui->portLine->setText(PORTSET);
				};
			};

			if(strstr(resStr, "[NDBSERVER]:") != NULL)	
			{
				lex = strstr(resStr, "[NDBSERVER]:") + strlen("[NDBSERVER]:");

				if(strlen(lex) > 1) 
				{
					lex[strlen(lex) - 1] = '\0';
					ui->lineTrackerSrv->setText(QString(lex));
				};
			}
			else if(strstr(resStr, "[NDBSCRIPT]:") != NULL)	
			{
				lex = strstr(resStr, "[NDBSCRIPT]:") + strlen("[NDBSCRIPT]:");

				if(strlen(lex) > 1) 
				{
					lex[strlen(lex) - 1] = '\0';
					ui->lineTrackerScr->setText(QString(lex));
				};
			}
			else if(strstr(resStr, "[NDBPORT]:") != NULL)	
			{
				lex = strstr(resStr, "[NDBPORT]:") + strlen("[NDBPORT]:");

				if(strlen(lex) > 1) 
				{
					lex[strlen(lex) - 1] = '\0';
					ui->trcSrvPortLine->setText(QString(lex));
				};
			}
			else if(strstr(resStr, "[PROXY]:") != NULL)		
			{
				lex = strstr(resStr, "[PROXY]:") + strlen("[PROXY]:");

				if(strlen(lex) > 1) 
				{
					lex[strlen(lex) - 1] = '\0';
				};
			}
			else if(strstr(resStr, "[PROXYPORT]:") != NULL)	
			{
				lex = strstr(resStr, "[PROXYPORT]:") + strlen("[PROXYPORT]:");

				if(strlen(lex) > 1) 
				{
					lex[strlen(lex) - 1] = '\0';
				};
			}
			else if(strstr(resStr, "[PING]:") != NULL)	
			{
				lex = strstr(resStr, "[PING]:") + strlen("[PING]:");

				if(strlen(lex) > 1) 
				{
					lex[strlen(lex) - 1] = '\0';
					ui->pingingOnOff->setChecked(strcmp(lex, "true") == 0 ? true : false);
				};
			}
			else if(strstr(resStr, "[PING_TO]:") != NULL)	
			{
				lex = strstr(resStr, "[PING_TO]:") + strlen("[PING_TO]:");

				if(strlen(lex) > 1) 
				{
					lex[strlen(lex) - 1] = '\0';
					ui->PingTO->setText(QString(lex));
				};
			}
			else if(strstr(resStr, "[THREAD_DELAY]:") != NULL)	
			{
				lex = strstr(resStr, "[THREAD_DELAY]:") + strlen("[THREAD_DELAY]:");

				if(strlen(lex) > 1) 
				{
					lex[strlen(lex) - 1] = '\0';
					ui->threadDelayBox->setText(QString(lex));
				};
			}
			else if(strstr(resStr, "[TIMEOUT]:") != NULL)	
			{
				lex = strstr(resStr, "[TIMEOUT]:") + strlen("[TIMEOUT]:");

				if(strlen(lex) > 1) 
				{
					lex[strlen(lex) - 1] = '\0';
					ui->iptoLine_value->setText(QString(lex));
					ui->iptoLine_value_2->setText(QString(lex));
					ui->iptoLine_value_3->setText(QString(lex));
				};
			}
			else if(strstr(resStr, "[MAXBTHR]:") != NULL)	
			{
				lex = strstr(resStr, "[MAXBTHR]:") + strlen("[MAXBTHR]:");

				if(strlen(lex) > 1) 
				{
					lex[strlen(lex) - 1] = '\0';
					ui->maxBrutingThrBox->setText(QString(lex));
				};
			}
			else if(strstr(resStr, "[PERSKEY]:") != NULL)	
			{
				lex = strstr(resStr, "[PERSKEY]:") + strlen("[PERSKEY]:");

				if(strlen(lex) > 1) 
				{
					lex[strlen(lex) - 1] = '\0';
					ui->linePersKey->setText(QString(lex));
				};
			}
			else if(strstr(resStr, "[IRCSERVER]:") != NULL)	
			{
				lex = strstr(resStr, "[IRCSERVER]:") + strlen("[IRCSERVER]:");

				if(strlen(lex) > 1) 
				{
					lex[strlen(lex) - 1] = '\0';
					ui->ircServerBox->setText(QString(lex));
				};
			}
			else if(strstr(resStr, "[IRCPORT]:") != NULL)	
			{
				lex = strstr(resStr, "[IRCPORT]:") + strlen("[IRCPORT]:");

				if(strlen(lex) > 1) 
				{
					lex[strlen(lex) - 1] = '\0';
					ui->serverPortBox->setText(QString(lex));
				};
			}
			else if(strstr(resStr, "[IRCPROXY]:") != NULL)	
			{
				lex = strstr(resStr, "[IRCPROXY]:") + strlen("[IRCPROXY]:");

				if(strlen(lex) > 1) 
				{
					lex[strlen(lex) - 1] = '\0';
					ui->ircProxy->setText(QString(lex));
				};
			}
			else if(strstr(resStr, "[IRCPROXYPORT]:") != NULL)	
			{
				lex = strstr(resStr, "[IRCPROXYPORT]:") + strlen("[IRCPROXYPORT]:");

				if(strlen(lex) > 1) 
				{
					lex[strlen(lex) - 1] = '\0';
					ui->ircProxyPort->setText(QString(lex));
				};
			}
			else if(strstr(resStr, "[IRCNICK]:") != NULL)	
			{
				lex = strstr(resStr, "[IRCNICK]:") + strlen("[IRCNICK]:");

				if(strlen(lex) > 1) 
				{
					lex[strlen(lex) - 1] = '\0';
					ui->ircNickBox->setText(QString(lex));
				}
				else
				{
					ui->ircNickBox->setText("ns_" + QString::number(qrand() % 8999 + 1000 ));
				};
			};
		};
		fclose(resFile);


		stt->doEmitionGreenFoundData("Previous session restored.");

	};
}

void CreateVerFile()
{
	FILE *vf = fopen("version", "w");

	if(vf != NULL)
	{
		fputs(gVER, vf);
		fclose(vf);
	};
}

char *GetVer()
{
	int dver = 0;
    int tver = 0;

	dver = __DATE__[9] - 48;
	dver *= 10;
	dver += __DATE__[10] - 48;
	dver *= 100;
	
	if(__DATE__[0] == 'J' && __DATE__[1] == 'a') dver += 1;
	else if(__DATE__[0] == 'F') dver += 2;
	else if(__DATE__[0] == 'M' && __DATE__[1] == 'a' && __DATE__[2] == 'r') dver += 3;
	else if(__DATE__[0] == 'A' && __DATE__[1] == 'p') dver += 4;
	else if(__DATE__[0] == 'M' && __DATE__[1] == 'a' && __DATE__[2] == 'y') dver += 5;
	else if(__DATE__[0] == 'J' && __DATE__[1] == 'u' && __DATE__[2] == 'n') dver += 6;
	else if(__DATE__[0] == 'J' && __DATE__[1] == 'u' && __DATE__[2] == 'l') dver += 7;
	else if(__DATE__[0] == 'A' && __DATE__[1] == 'u') dver += 8;
	else if(__DATE__[0] == 'S') dver += 9;
	else if(__DATE__[0] == 'O') dver += 10;
	else if(__DATE__[0] == 'N') dver += 11;
	else if(__DATE__[0] == 'D') dver += 12;
	
	if(__DATE__[4] != ' ')
	{
		dver *= 10;
		dver += __DATE__[4] - 48;
	}
	else dver *= 10;
	dver *= 10;
	dver += __DATE__[5] - 48;

	tver = __TIME__[0] - 48;
	tver *= 10;
	tver += __TIME__[1] - 48;
	tver *= 10;
	tver += __TIME__[3] - 48;
	tver *= 10;
	tver += __TIME__[4] - 48;

	char db[32] = {0};
	char tb[32] = {0};
	sprintf(db, "%X", dver);
	strcat(db, "-");
	sprintf(tb, "%X", tver);
	strcat(db, tb);

	return db;
}

void nesca_3::slotShowRedVersion()
{
	ui->rVerLabel->show();
}

void _startVerCheck()
{
	vct->start();
}

void _startMsgCheck()
{
	mct->start();
}


static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

	nesca_3::nesca_3(QWidget *parent) : QMainWindow(parent)
{
	setWindowFlags			( Qt::FramelessWindowHint );
	this->hide();

	gthis = this;
	ui->setupUi(this);
	ui->widgetIRC->installEventFilter(this);
	ui->shoutBox->installEventFilter(this);
	setSomeStyleArea();
	ui->IRCModeChangerBut->setVisible(false);
	ui->dataText->setOpenExternalLinks(true);
	ui->dataText->setOpenLinks(false);
	ui->rVerLabel->hide();
	setSceneArea();
		
	tray = new QSystemTrayIcon(QIcon(":/nesca_3/Z:/nesca.ico"), this);
	tray->hide();

	SetValidators();
	ConnectEvrthng();

	QTime time = QTime::currentTime();
	qsrand((uint)time.msec());
	
	QLinearGradient gradient(0, 0, 0, 20);
	gradient.setColorAt(0,		QColor(0,255,0));
	gradient.setColorAt(0.5,	QColor(100,100,100, 100));
	gradient.setColorAt(1,		QColor(255,255,255, 60));
	pen2i.setBrush(gradient);
	
	ui->ircNickBox->setText("nsa_" + QString::number(qrand() % 8999 + 1000));
	
	char rVer[32] = {0};
	strcpy(rVer, GetVer());
	QString QVER = QString(rVer);
	QByteArray ba = QVER.toLatin1();
	strcpy(gVER, ba.data());
	ui->logoLabel->setToolTip("v3-" + QVER);
	ui->logoLabel->setStyleSheet("color:white; border: none;background-color:black;");
	ui->newMessageLabel->setStyleSheet("color:rgba(255, 0, 0, 0);background-color: rgba(2, 2, 2, 0);");

	CreateVerFile();

	RestoreSession();
	saveOptions();

	PhraseLog.push_back("");
	CreateMsgPopupWidget();

	dtHN->start();
	dtME2->start();
	adtHN->start();

	pbPointerFont.setFamily("Eurostile");
	pbPointerFont.setPixelSize(8);
	
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	WSADATA wsda;
	if (WSAStartup(0x0101, &wsda)) 
	{
		
		stt->doEmitionRedFoundData("WSAStartup failed.");
		stt->doEmitionKillSttThread();
		
	};
#endif

	_startVerCheck();
	_startMsgCheck();

//      curl = curl_easy_init();
//      if(curl) {
//        curl_easy_setopt(curl, CURLOPT_URL, "http://www.google.com");
//        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
//        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
//        res = curl_easy_perform(curl);
//        curl_easy_cleanup(curl);

//        std::cout << readBuffer << std::endl;
//      }

//        CURLcode res;
//        std::string readBuffer;
//        CURL *curl = curl_easy_init();

//        if (curl)
//        {
//            stt->doEmitionFoundData( curl_easy_strerror(curl_easy_setopt(curl, CURLOPT_URL, "http://www.google.com")));
//            stt->doEmitionFoundData( curl_easy_strerror(curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback)));
//            stt->doEmitionFoundData( curl_easy_strerror(curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer)));
//            stt->doEmitionFoundData( curl_easy_strerror(curl_easy_setopt(curl, CURLOPT_PROXY, "cache.fors.ru")));
//            stt->doEmitionFoundData( curl_easy_strerror(curl_easy_setopt(curl, CURLOPT_PROXYPORT, 3128)));
//            stt->doEmitionFoundData( curl_easy_strerror(curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L)));
//            stt->doEmitionFoundData( curl_easy_strerror(curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, gTimeOut)));
//            stt->doEmitionFoundData( curl_easy_strerror(curl_easy_setopt(curl, CURLOPT_TIMEOUT, gTimeOut)));

//            //stt->doEmitionFoundData( curl_easy_strerror(curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L)));
//           // stt->doEmitionFoundData( curl_easy_strerror(curl_easy_setopt(curl, CURLOPT_URL, "http://www.google.com")));
//           // stt->doEmitionFoundData( curl_easy_strerror(curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, gTimeOut)));
//           // stt->doEmitionFoundData( curl_easy_strerror(curl_easy_setopt(curl, CURLOPT_NOBODY, true)));
//           // stt->doEmitionFoundData( curl_easy_strerror(curl_easy_setopt(curl, CURLOPT_PROXY, "cache.fors.ru")));
//            //stt->doEmitionFoundData( curl_easy_strerror(curl_easy_setopt(curl, CURLOPT_PROXYPORT, 3128)));
//            //stt->doEmitionFoundData( curl_easy_strerror(curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_NTLM)));
//            //stt->doEmitionFoundData( curl_easy_strerror(curl_easy_setopt(curl, CURLOPT_VERBOSE, true)));
//            //stt->doEmitionFoundData( curl_easy_strerror(curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback)));
//            //stt->doEmitionFoundData( curl_easy_strerror(curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer)));

//            res = curl_easy_perform(curl);
//            curl_easy_cleanup(curl);
//        };

//        stt->doEmitionFoundData(QString(readBuffer.c_str()));
	//float step = 0;
	//QPen iprvPenRegular(QColor(51, 51, 51, 100));
	//QPen iprvPen(QColor(51, 51, 51, 100));
	//while(step < 480)
	//{
	//	jobRangeVisualScene->addLine(step, 0, step, 41, iprvPenRegular);
	//	step += 30;
	//};
	//QPen iprvPenComplete(QColor(51, 51, 51, 100));
	//while(step < 480)
	//{
	//	jobRangeVisualScene->addLine(step, 0, step, 41, iprvPen);
	//	step += 30;
	//};
}

void nesca_3::playFcknSound()
{
	QSound::play("00000036.wav");	
}

void nesca_3::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton) {
		dragPosition = event->globalPos() - frameGeometry().topLeft();
		event->accept();
	};
}

void nesca_3::mouseReleaseEvent(QMouseEvent * event)
{
	if (event->modifiers() == Qt::ControlModifier) {
		privateMsgFlag = false;
		event->accept();
	};
}

void nesca_3::mouseMoveEvent(QMouseEvent * event)
{
	QPoint CPos = (event->globalPos() - dragPosition);
	QWidget::move(CPos);
	if(qwm != NULL) qwm->move(CPos.x() + WIDGET_WIDTH + 5, CPos.y());
}

void nesca_3::exitButtonClicked()
{
	globalScanFlag = false;
	stt->terminate();
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	WSACleanup();
#endif
	qApp->quit();
}

void nesca_3::trayButtonClicked()
{
	if(tray->isVisible())
	{
		show();
		tray->hide();		
	}
	else
	{
		hide();
		tray->show();
	};
}

void nesca_3::ChangeShuffle(bool val)
{
	gShuffle = val;
}

void nesca_3::ChangeTrackerOK(bool val)
{
	trackerOK = val;
}

void nesca_3::ChangePingerOK(bool val)
{
	ui->PingTO->setEnabled(val);
	gPingNScan = val;
	if(val == false)
	{
		ui->PingTO->setStyleSheet("color: rgb(116, 116, 116);background-color: rgb(56, 56, 56);border:none;");
	}
	else
	{
		ui->PingTO->setStyleSheet("color: rgb(216, 216, 216);background-color: rgb(56, 56, 56);border:none;");
	};
}

void nesca_3::ChangeDebugFileState(bool val)
{
	debugFileOK = val;
}

void nesca_3::saveOptions()
{
	int ci = ui->tabMainWidget->currentIndex();



	if(ci == 0) 
	{
		ZeroMemory(saveStartIP, sizeof(saveStartIP));
		ZeroMemory(endIP2, sizeof(endIP2));
		ZeroMemory(top_level_domain, sizeof(top_level_domain));
		ZeroMemory(gPorts, sizeof(gPorts));
		gMode = 0;
		gThreads = ui->threadLine->text().toInt();
		int indexof = ui->ipLine->text().indexOf("-");
		if(indexof > 0) 
		{
			strncpy(saveStartIP, ui->ipLine->text().toLocal8Bit().data(), indexof); 
			if(ui->ipLine->text().indexOf("/") < 0) strcpy(endIP2, ui->ipLine->text().toLocal8Bit().data()); 
		}
		else 
		{
			if(ui->ipLine->text().indexOf("/") < 0) 
			{
				strcpy(saveStartIP, ui->ipLine->text().toLocal8Bit().data());
				strcpy(endIP2, ui->ipLine->text().toLocal8Bit().data()); 
				strcat(endIP2, "-"); 
				strcat(endIP2, ui->ipLine->text().toLocal8Bit().data()); 
			}
			else
			{
				strncpy(saveStartIP, ui->ipLine->text().toLocal8Bit().data(), ui->ipLine->text().indexOf("/")); 
			};
		};

		strncpy(gPorts, ("-p" + ui->portLine->text()).toLocal8Bit().data(), 65536);
	}
	else if(ci == 1) 
	{
		ZeroMemory(saveStartIP, sizeof(saveStartIP));
		ZeroMemory(endIP2, sizeof(endIP2));
		ZeroMemory(top_level_domain, sizeof(top_level_domain));
		ZeroMemory(gPorts, sizeof(gPorts));
		gMode = 1;
		gThreads = ui->lineEditThread->text().toInt();
		int indexof = ui->lineEditStartIPDNS->text().indexOf("/");
		if(indexof > 0)
		{
			strncpy(saveStartIP, ui->lineEditStartIPDNS->text().toLocal8Bit().data(), indexof); 
			strcpy(endIP2, ui->lineEditStartIPDNS->text().toLocal8Bit().data()); 
		}
		else
		{
			strcpy(saveStartIP, ui->lineEditStartIPDNS->text().toLocal8Bit().data()); 
			strcpy(endIP2, ui->lineEditStartIPDNS->text().toLocal8Bit().data()); 
			strcat(endIP2, "/"); 
			strcat(endIP2, ui->lineEditStartIPDNS->text().toLocal8Bit().data()); 
		};
		
		strcpy(top_level_domain, ui->lineILVL->text().toLocal8Bit().data()); 
		
		strncpy(gPorts, ("-p" + ui->lineEditPort->text()).toLocal8Bit().data(), 65536);
	};

	strcpy(trcSrv, ui->lineTrackerSrv->text().toLocal8Bit().data());
	strcpy(trcScr, ui->lineTrackerScr->text().toLocal8Bit().data());
	strncpy(trcPersKey, ui->linePersKey->text().toLocal8Bit().data(), 32);
	strcpy(trcSrvPortLine, ui->trcSrvPortLine->text().toLocal8Bit().data());

	strcpy(ircServer, ui->ircServerBox->text().toLocal8Bit().data());
	strcpy(ircPort, ui->serverPortBox->text().toLocal8Bit().data());
	strcpy(ircProxy, ui->ircProxy->text().toLocal8Bit().data());
	strcpy(ircProxyPort, ui->ircProxyPort->text().toLocal8Bit().data());
	strcpy(ircNick, ui->ircNickBox->text().toLocal8Bit().data());
}

void nesca_3::STTTerminate()
{
	importFileName = "";
	startFlag = false;
	globalScanFlag = false;

	stt->terminate();

	while(__savingBackUpFile) Sleep(100);

	nCleanup();
	ui->tabMainWidget->setTabEnabled(0, true);
	ui->tabMainWidget->setTabEnabled(1, true);
	ui->tabMainWidget->setTabEnabled(2, true);
	ui->tabMainWidget->setTabEnabled(3, true);
	stt->doEmitionThreads(QString::number(0) + "/" + QString::number(gThreads));
	stt->doEmitionIPS("0");
	stt->doEmitionChangeBA("0");
	BrutingThrds = 0;
	cons = 0;
	setButtonStyleArea();
	ui->startScanButton_3->setText("Start");
	ui->startScanButton_4->setText("Start");
	ui->importButton->setText("Import&&Scan");
	ui->labelStatus_Value->setText("Idle");
}

bool dfLocked = false;
void writeDebugFile(QString str)
{
	while(dfLocked) Sleep(10);
	QDate qd;
	char b[8] = {0};
	char fn[64] = {0};
	strcpy(fn, "./output_");
    sprintf(b, "%d", qd.currentDate().day());
    strcat(fn, b);
	strcat(fn, "_");
    sprintf(b, "%d", qd.currentDate().month());
    strcat(fn, b);
	strcat(fn, "_");
    sprintf(b, "%d", qd.currentDate().year());
    strcat(fn, b);
	strcat(fn, ".txt");

	FILE *df = fopen(fn, "a");
	if(df != NULL)
	{
		dfLocked = true;
		fputs(str.toLocal8Bit().data(), df);
		fclose(df);
		dfLocked = false;
	}
	else
	{
 		stt->doEmitionRedFoundData("[DEBUG: Cannot open " + QString(fn) + "]");
	};
}

void nesca_3::startScanButtonClicked()
{
	if(startFlag == false)
	{
		if(trackerOK)
		{
			if(ui->linePersKey->text().size() != 0)
			{
				CheckPersKey(0);
			}
			else
			{
				stt->doEmitionRedFoundData("Empty \"Personal key\" field. ");
			};
		}
		else
		{
			stt->doEmitionStartScanIP();		
		};
	}
	else
	{
		if(stopFirst == false)
		{
			stopFirst = true;
			globalScanFlag = false;
			ui->startScanButton_3->setStyleSheet(
				" QPushButton {"
				"background-color: qlineargradient(spread:none, x1:1, y1:0, x2:1, y2:1, stop:0.681818 rgba(0, 0, 0, 250), stop:1 rgba(255, 255, 255, 130));"
				"color: red;"
				"border: 0.5px solid qlineargradient(spread:reflect, x1:0.54, y1:0.488591, x2:0.54, y2:0, stop:0 rgba(255, 255, 255, 130), stop:1 rgba(0, 0, 0, 255));"
				"}"
				);

			ui->startScanButton_3->setText("STOP!");
			stt->doEmitionYellowFoundData("Trying to stop. Please, wait...");
		}
		else
		{
			globalScanFlag = false;
			ui->startScanButton_3->setStyleSheet(
				" QPushButton {"
				"background-color: qlineargradient(spread:none, x1:1, y1:0, x2:1, y2:1, stop:0.681818 rgba(0, 0, 0, 250), stop:1 rgba(255, 255, 255, 130));"
				"color: yellow;"
				"border: 0.5px solid qlineargradient(spread:reflect, x1:0.54, y1:0.488591, x2:0.54, y2:0, stop:0 rgba(255, 255, 255, 130), stop:1 rgba(0, 0, 0, 255));"
				"}"
				);

			ui->startScanButton_3->setText("Wait...");
			stt->doEmitionYellowFoundData("Wait, killing threads...");
			STTTerminate();
		};
	};
}

void nesca_3::startScanButtonClickedDNS()
{
	if(startFlag == false)
	{
		if(trackerOK)
		{
			if(ui->linePersKey->text().size() != 0)
			{
				CheckPersKey(1);
			}
			else
			{
				stt->doEmitionRedFoundData("Empty \"Personal key\" field. ");
			};
		}
		else
		{
			stt->doEmitionStartScanDNS();				
		};
	}
	else
	{
		if(stopFirst == false)
		{
			stopFirst = true;
			globalScanFlag = false;
			ui->startScanButton_4->setStyleSheet(
				" #startScanButton_4 {"
				"background-color: qlineargradient(spread:none, x1:1, y1:0, x2:1, y2:1, stop:0.681818 rgba(0, 0, 0, 250), stop:1 rgba(255, 255, 255, 130));"
				"color: red;"
				"border: 0.5px solid qlineargradient(spread:reflect, x1:0.54, y1:0.488591, x2:0.54, y2:0, stop:0 rgba(255, 255, 255, 130), stop:1 rgba(0, 0, 0, 255));"
				"}"
				);

			ui->startScanButton_4->setText("STOP!");
			stt->doEmitionYellowFoundData("Trying to stop. Please, wait...");
		}
		else
		{
			globalScanFlag = false;
			ui->startScanButton_4->setStyleSheet(
				" #startScanButton_4 {"
				"background-color: qlineargradient(spread:none, x1:1, y1:0, x2:1, y2:1, stop:0.681818 rgba(0, 0, 0, 250), stop:1 rgba(255, 255, 255, 130));"
				"color: yellow;"
				"border: 0.5px solid qlineargradient(spread:reflect, x1:0.54, y1:0.488591, x2:0.54, y2:0, stop:0 rgba(255, 255, 255, 130), stop:1 rgba(0, 0, 0, 255));"
				"}"
				);

			ui->startScanButton_4->setText("Wait...");
			stt->doEmitionYellowFoundData("Killing threads...");
			STTTerminate();
		};
	};
}

void nesca_3::logoLabelClicked()
{
	QDesktopServices::openUrl(QUrl("http://nesca.d3w.org/"));
}

void nesca_3::ChangeLabelTO_ValueChanged(QString str)
{
	gTimeOut = str.toInt();
}

void nesca_3::ChangeLabelThreads_ValueChanged(QString str)
{
	gThreads = str.toInt();
}

void nesca_3::PingTO_ChangeValue(QString str)
{
	gPingTimeout = str.toInt();
}

void nesca_3::ThreadDelay_ChangeValue(QString str)
{
	gThreadDelay = str.toInt();
}

void nesca_3::MaxBrutingThr_ChangeValue(QString str)
{
	gMaxBrutingThreads = str.toInt();
}

void nesca_3::appendRedBAData(QString str)
{
	ui->BAText->append("<span style=\"color:red;background-color:#313131;\">" + QString::fromUtf8(str.toLocal8Bit().data()) + "</span>");
}

void nesca_3::appendGreenBAData(QString str)
{
	ui->BAText->append("<span style=\"color:#06ff00;\">" + str + "</span>");
}

void nesca_3::appendDefaultText(QString str)
{
	ui->dataText->append("<p style=\"color: #a1a1a1;\">[" + QTime::currentTime().toString() + "] " + str + "</p>");
	if(debugFileOK) writeDebugFile(str + "\n");
}

void nesca_3::appendErrText(QString str)
{
	ui->dataText->append("<span style=\"color:red;background-color:#313131;\">[" + QTime::currentTime().toString() + "]" + QString::fromUtf8(str.toLocal8Bit().data()) + "</span>");

	if(stt->isRunning() == false)
	{
		startFlag = false;
		stt->doEmitionChangeStatus("Idle");
		stt->doEmitionIPS("0");
		ui->startScanButton_3->setText("Start");
		stt->terminate();
	};
	if(debugFileOK) writeDebugFile(str + "\n");
}

void nesca_3::appendOKText(QString str)
{
	ui->dataText->append("<span style=\"color:#06ff00;\">[" + QTime::currentTime().toString() + "][OK] " + str + "</span>");
	if(debugFileOK) writeDebugFile(str + "\n");
}

void nesca_3::appendNotifyText(QString str)
{
	ui->dataText->append("<span style=\"color:#efe100;\">[" + QTime::currentTime().toString() + "][*] " + str + "</span>");
	if(debugFileOK) writeDebugFile(str + "\n");
}

void nesca_3::appendDebugText(QString str)
{
	ui->dataText->append("<span style=\"color:#0084ff;\">[DEBUG] " + str + "</span>");
	if(debugFileOK) writeDebugFile(str + "\n");
}


QRegExp r("[ht|f]{0,2}tp[s]{0,1}://(\\w|\\.|,|/|:|-|_|\\?|!|\\@|#|\\$|%|\\^|&|\\*|=|\\+|<|>|;|:|\"|'|~|\\[|\\])*[\\s|\\t]{0,1}");
QRegExp under("((.+|(.+$)))");
QRegExp boldr("((.+|(.+$)))");
QRegExp colr("(\\d+[,\\d+]{0,2})");
QString GetColorCode(int mode, QString str)
{
	QRegExp c("(\\d{0,2})");
	QRegExp bg(",(\\d{0,2})");
	QString col;

	if(mode == 0)
	{
		c.indexIn(str);
		col = c.cap(1);
	}
	else
	{
		bg.indexIn(str);
		col = bg.cap(1);
	};

	QString result;

	int icol = col.toInt();
	if(icol == 0) result = "#AFAFAF";
	else if(icol == 1) result = "black";
	else if(icol == 2) result = "darkblue";
	else if(icol == 3) result = "green";
	else if(icol == 4) result = "red";
	else if(icol == 5) result = "darkred";
	else if(icol == 6) result = "blueviolet";
	else if(icol == 7) result = "darkorange";
	else if(icol == 8) result = "yellow";
	else if(icol == 9) result = "lightgreen";
	else if(icol == 10) result = "darkcyan";
	else if(icol == 11) result = "teal";
	else if(icol == 12) result = "blue";
	else if(icol == 13) result = "pink";
	else if(icol == 14) result = "#4d4d4d";
	else if(icol == 15) result = "gray";
	else result = "#AFAFAF";

	return result;
}

void nesca_3::appendDefaultIRCTextOut(QString str)
{
	bool thisIsUrl = false;
	r.indexIn(str);

	QString link = r.cap(0);

	if(link.size() != 0) 
	{
		str.replace("<", "&lt;");
		link.replace("<", "&lt;");
		str.replace(r, "<a href=\"" + link.trimmed() + "\"><span style=\"color: #717171;\">" + link.trimmed() + "</span></a> ");
		thisIsUrl = true;
	};

	QString rData;
	if(code160 || thisIsUrl) rData = "<font color=\"white\">[" + QTime::currentTime().toString() + "] " + str.trimmed() + "</font>";
	else rData = "<font color=\"white\">[" + QTime::currentTime().toString() + "] " + str.trimmed().toHtmlEscaped() + "</font>";
	
	globalIRCText += rData + "\n";
	if(utfIRCFlag) 
	{
		QByteArray wtf = codec->fromUnicode(rData);
		ui->ircText->append(QString(wtf));
	}
	else ui->ircText->append(rData);

	thisIsUrl = false;
}

void nesca_3::appendDefaultIRCText(bool pm, bool hlflag, QString str, QString senderNick)
{
	bool thisIsUrl = false;
	
#pragma region Color-handler
	int pos = 0;
	QString colRes;
	while ((pos = colr.indexIn(str, pos)) != -1) 
	{	
		colRes = colr.cap(1);
		if(colRes.indexOf(",") > 0) str.replace(colRes, "</font><font style=\"color: " + GetColorCode(0, colRes) + ";background-color: " + GetColorCode(1, colRes) + ";\">");
		else str.replace(colRes, "</font><font style=\"color: " + GetColorCode(0, colRes) + ";\">");
		pos += colr.matchedLength();
		thisIsUrl = true;
	};


#pragma region Bold-handler
	int posBold = 0;
	QString boldRes;
	boldr.setMinimal(true);
	while ((posBold = boldr.indexIn(str, posBold)) != -1) 
	{	
		boldRes = boldr.cap(0);
		QString boldResTrimmed = boldRes;
		boldResTrimmed.replace("", "");
		str.replace(boldRes, "<font style=\"font-weight: bold;\">" + boldResTrimmed + "</font>");
		posBold += boldr.matchedLength();
		thisIsUrl = true;
	};


#pragma region underline-handler
	int posUnder = 0;
	QString underRes;
	under.setMinimal(true);
	while ((posUnder = under.indexIn(str, posUnder)) != -1) 
	{	
		underRes = under.cap(0);
		QString underResTrimmed = underRes;
		underResTrimmed.replace("", "");
		str.replace(underRes, "<font style=\"text-decoration: underline;\">" + underResTrimmed + "</font>");
		posUnder += under.matchedLength();
		thisIsUrl = true;
	};

	
#pragma region link-handler
	r.indexIn(str);
	QString link = r.cap(0);
	if(link.size() != 0) 
	{
		str.replace("<", "&lt;");
		link.replace("<", "&lt;");
		str.replace(r, "<a href=\"" + link.trimmed() + "\"><span style=\"color: #717171;\">" + link.trimmed() + "</span></a> ");
		thisIsUrl = true;
	};

	
	str.replace("", "</font>");
	str.replace("", "</font>");
	
	char rawData[512] = {0};
	strncpy(rawData, str.toLocal8Bit().data(), 512);

	if(strstr(rawData, "") != NULL || strstr(rawData, "") != NULL || 
		strstr(rawData, "") != NULL || strstr(rawData, "") != NULL
		) thisIsUrl = true;

	QString rData;
	if(strstr(rawData, "ACTION") != NULL)
	{
		senderNick.replace("[", "");
		senderNick.replace("]:", "");
		str.replace("ACTION", "");
		str.replace("", "");
		rData = "<font color=\"#ffae00\">[" + QTime::currentTime().toString() + "]</font> -->" + senderNick + "<font style=\"color:#darkgray;background-color:#003f5f;\"> " + str.trimmed().toHtmlEscaped() + "</font>";
		globalIRCText += rData + "\n";
		if(utfIRCFlag) 
		{
			QByteArray wtf = codec->fromUnicode(rData);
			ui->ircText->append(QString(wtf));
		}
		else ui->ircText->append(rData);
	}
	else
	{	
		if(hlflag)
		{
			if(code160 || thisIsUrl) rData = "<font color=\"#ffae00\">[" + QTime::currentTime().toString() + "]</font> " + (pm?" <font color=\"#810000\">[PM]</font> ":"") + senderNick + "<font style=\"color:#AFAFAF;background-color:#003f5f;\"> " + str + "</font>";
			else rData = "<font color=\"#ffae00\">[" + QTime::currentTime().toString() + "]</font> " + (pm?" <font color=\"#810000\">[PM]</font> ":"") + senderNick + "<font style=\"color:#AFAFAF;background-color:#003f5f;\"> " + str.trimmed().toHtmlEscaped() + "</font>";
		}
		else
		{
			if(code160 || thisIsUrl) rData = "<font color=\"#ffae00\">[" + QTime::currentTime().toString() + "]</font> " + (pm?" <font color=\"#810000\">[PM]</font> ":"") + senderNick + "<font style=\"color:#AFAFAF;\"> " + str + "</font>";
			else rData = "<font color=\"#ffae00\">[" + QTime::currentTime().toString() + "]</font> " + (pm?" <font color=\"#810000\">[PM]</font> ":"") + senderNick + "<font color=\"#AFAFAF\"> " + str.trimmed().toHtmlEscaped() + "</font>";
		};
		
		globalIRCText += rData + "\n";
		if(utfIRCFlag) 
		{
			QByteArray wtf = codec->fromUnicode(rData);
			ui->ircText->append(QString(wtf));
		}
		else ui->ircText->append(rData);
	};
}

void nesca_3::appendRedIRCText(QString str)
{
	QString rData;
	rData = "<font style=\"color:red;background-color:#313131;\">[" + QTime::currentTime().toString() + "] " + str + "</font>";
	globalIRCText += rData + "\n";
	ui->ircText->append(rData);
}

void nesca_3::appendGreenIRCText(QString str)
{
	QString rData;
	rData = "<font style=\"color:#06ff00;\">[" + QTime::currentTime().toString() + "] " + str + "</font>";
	globalIRCText += rData + "\n";
	ui->ircText->append(rData);
}

void nesca_3::appendYellowIRCText(QString str)
{
	QString rData;
	rData = "<font style=\"color:#efe100;\">[" + QTime::currentTime().toString() + "] " + "[*] " + str.trimmed() + "</font>";
	globalIRCText += rData + "\n";
	ui->ircText->append(rData);
}

void nesca_3::appendDefaultIRCRawTextInc(QString str)
{
	QString rData;
	if(code160) rData = "[" + QTime::currentTime().toString() + "] " + str;
	else rData = "[" + QTime::currentTime().toString() + "] " + str.toHtmlEscaped();
	globalIRCRaw += rData + "\n";
	if(utfIRCFlag) 
	{
		QByteArray wtf = codec->fromUnicode(rData);
		ui->ircRaw->append(QString(wtf));
	}
	else ui->ircRaw->append(rData);
}

void nesca_3::appendDefaultIRCRawTextOut(QString str)
{
	QString rData;
	if(code160) rData = "<font color=\"#dbdbdb\">[" + QTime::currentTime().toString() + "] " + str.trimmed() + "</font>";
	else rData = "<font color=\"#dbdbdb\">[" + QTime::currentTime().toString() + "] " + str.trimmed().toHtmlEscaped() + "</font>";
	globalIRCRaw += rData + "\n";
	if(utfIRCFlag) 
	{
		QByteArray wtf = codec->fromUnicode(rData);
		ui->ircRaw->append(QString(wtf));
	}
	else ui->ircRaw->append(rData);
}

void nesca_3::slotClearNickList()
{
	ui->nickList->clear();
}

void nesca_3::slotAppendIRCNick(QString str)
{
	if(str.size() > 0 && str != " ") 
	{
		ui->nickList->addItem(str.remove("@"));
		ui->nickList->setGeometry(ui->nickList->x(), ui->nickList->y(), ui->nickList->width(), ui->nickList->count() * 17 + 5);
	};
}

void nesca_3::slotItemClicked(QListWidgetItem* wi)
{
	if(privateMsgFlag == false)
	{
		QString pText = ui->shoutBox->text();
		ui->shoutBox->setText(pText + wi->text() + ", ");
		ui->shoutBox->setFocus();
	}
	else
	{
		ui->shoutBox->setText("/w " + wi->text().remove("@") + " ");
		ui->shoutBox->setFocus();
	};
}

nesca_3::~nesca_3()
{
	delete[] ui;
}
