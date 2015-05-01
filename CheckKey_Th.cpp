#include "CheckKey_Th.h"
#include "STh.h"
#include "externData.h"
#include "externFunctions.h"
#include "Connector.h"
#include "Utils.h"

int CheckKey_Th::isActiveKey = -1;

void getSubStrEx(const char *src, char *startStr, char *endStr, char *dest, int szDest)
{
	ZeroMemory(dest, szDest);
    char *ptr1 = strstri((const char*)src, startStr);
	if(ptr1 != NULL)
	{
        char *ptr2 = strstri((const char*)ptr1, endStr);
		if(ptr2 != NULL)
		{
			int szStartStr = strlen(startStr);
			int sz = ptr2 - ptr1 - szStartStr;
			strncpy(dest, ptr1 + szStartStr, sz < szDest ? sz : szDest);
		};
	};
}
void getSubStr(char *src, char *startStr, char *endStr, char *dest, int szDest)
{
	ZeroMemory(dest, szDest);
    char *ptr1 = strstri((const char*)src, startStr);
	if(ptr1 != NULL)
	{
        char *ptr2 = strstri((const char*)ptr1, endStr);
		if(ptr2 != NULL)
		{
			int sz = ptr2 - ptr1;
			strncpy(dest, ptr1, sz < szDest ? sz : szDest);
		};
	};
}

int emitIfOK = -1;
int KeyCheckerMain()
{
	int kLen = strlen(trcPersKey);
	if(kLen == 0)
	{
		stt->doEmitionRedFoundData("[Key check] Key field is empty.");
		return -1;
	}
	else if(kLen < 32)
	{
		stt->doEmitionRedFoundData("[Key check] Key length is not valid.");
		return -1;
	};

    std::vector<std::string> headerVector;
    headerVector.push_back("X-Nescav3: True");

	std::string buffer;
	Connector con;
    con.nConnect((std::string(trcSrv) + std::string(trcScr)).c_str(), std::stoi(trcSrvPortLine), &buffer, NULL, &headerVector);

    int hostStringIndex = buffer.find("\r\n\r\n");
    if(hostStringIndex != -1) {

        int s = buffer.find("http://", hostStringIndex);
        int e = buffer.find('/', s + 8);
		std::string url = buffer.substr(s, e - s);
		Connector con;
        con.nConnect((url + std::string("/api/checkaccount?key=") + std::string(trcPersKey)).c_str(),
                            std::stoi(trcSrvPortLine), &buffer, NULL, &headerVector);

        if(Utils::ustrstr(buffer, std::string("202 Accepted")) != -1) {
            stt->doEmitionGreenFoundData("Key is valid.");
			CheckKey_Th::isActiveKey = 1;
            if(emitIfOK == 0) stt->doEmitionStartScanIP();
            else if(emitIfOK == 1) stt->doEmitionStartScanDNS();
            else if(emitIfOK == 2) stt->doEmitionStartScanImport();
            return 1;
        } else if(Utils::ustrstr(buffer, std::string("400 Bad Request")) != -1) {
			QString errorDef = Utils::GetNSErrorDefinition(buffer.c_str(), "notify");
            if(errorDef == "Invalid access key") stt->doEmitionYellowFoundData("[NS-Track] Key is unauthorized. A valid key is required.");
			else stt->doEmitionYellowFoundData("[Key check] FAIL! [400 Bad Request : " + 
				Utils::GetNSErrorDefinition(buffer.c_str(), "notify") + "]");
        } else if(Utils::ustrstr(buffer, std::string("503 Bad Gateway")) != -1) {
            stt->doEmitionYellowFoundData("[Key check] 503 Backend not responding!");
        } else {
            char header[64] = {0};
            getSubStrEx(buffer.c_str(), "http/1.1 ", "\r\n", header, 64);
            stt->doEmitionRedFoundData("[Key check] FAIL! An error occured. (" + QString::number(WSAGetLastError()) + ") Header: <u>" + QString::fromLocal8Bit(header) + "</u>");
            if(gDebugMode) stt->doEmitionDebugFoundData(QString(buffer.c_str()));
        };
    } else {
        stt->doEmitionRedFoundData("[Key check] Cannot acquire host string.");
    }
    return -1;
}

void CheckKey_Th::run()
{
	KeyCheckerMain();
}
