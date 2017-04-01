#include "FileDownloader.h"
#include "mainResources.h"
#include "fstream"
#include <algorithm>

std::string FileDownloader::lastModifiedNeg = "";
std::string FileDownloader::lastModifiedL = "";
std::string FileDownloader::lastModifiedP = "";
std::string FileDownloader::lastModifiedSSH = "";
std::string FileDownloader::lastModifiedWFL = "";
std::string FileDownloader::lastModifiedWFP = "";

std::string getLM(std::string *buffer) {

    std::size_t pos1 = buffer->find("Last-Modified:");
    if(pos1 == std::string::npos) {
        stt->doEmitionFoundData("<font color=\"Pink\">Cannot find Last-Modified.</font>");
        return "";
    }
    int pos2 = buffer->find("\r\n", pos1);
    if(pos2 == std::string::npos) {
        stt->doEmitionFoundData("<font color=\"Pink\">Weird reply.</font>");
        return "";
    }

    std::string res = buffer->substr(pos1 + 15, pos2 - pos1 - 15);
    return res;
}

void checkWeb(const char *fileName, std::string *oldLM) {
	std::string buffer;
	Connector con;
    con.nConnect(std::string("http://nesca.d3w.org/files/" + std::string(fileName)).c_str(), 80, &buffer);

    const std::string &lm = getLM(&buffer);
    if(lm.size() == 0) return;

    if(lm.compare(*oldLM) != 0) {
        *oldLM = lm;
        std::string res(buffer.substr(buffer.find("\r\n\r\n") + 4).c_str());
		res.erase(std::remove(res.begin(), res.end(), '\r'), res.end());
        std::ofstream out(fileName);
        out << std::string(res);
        out.close();

        stt->doEmitionFoundData("<font color=\"Pink\">File " + QString(fileName) + " downloaded.</font>");
     }
}

void FileDownloader::checkWebFiles() {
    while (true) {
        checkWeb(NEGATIVE_FN, &lastModifiedNeg);
		checkWeb(LOGIN_FN, &lastModifiedL);
		checkWeb(PASS_FN, &lastModifiedP);
		checkWeb(SSH_PASS_FN, &lastModifiedSSH);
		checkWeb(WF_LOGIN_FN, &lastModifiedWFL);
		checkWeb(WF_PASS_FN, &lastModifiedWFP);
        Sleep(600000);
    }
}

