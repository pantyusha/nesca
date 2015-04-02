#include "FileDownloader.h"
#include "fstream"

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
    Connector::nConnect(std::string("http://nesca.d3w.org/files/" + std::string(fileName)).c_str(), 80, &buffer);

    const std::string &lm = getLM(&buffer);
    if(lm.size() == 0) return;

    if(lm.compare(*oldLM) != 0) {
        *oldLM = lm;
        //QString res(buffer.substr(buffer.find("\r\n\r\n") + 4).c_str());
        std::string res(buffer.substr(buffer.find("\r\n\r\n") + 4).c_str());
        //res.replace("\r\n", "\n");
        //QTextCodec *codec = QTextCodec::codecForName("Windows-1251");
        //res = codec->toUnicode(res.toLocal8Bit().data());
        std::ofstream out(fileName);
        out << std::string(res);
        out.close();

        stt->doEmitionFoundData("<font color=\"Pink\">File " + QString(fileName) + " downloaded.</font>");
     }
}

void FileDownloader::checkWebFiles() {
    while (true) {
        checkWeb("negatives.txt", &lastModifiedNeg);
        checkWeb("login.txt", &lastModifiedL);
        checkWeb("pass.txt", &lastModifiedP);
        checkWeb("sshpass.txt", &lastModifiedSSH);
        checkWeb("wflogin.txt", &lastModifiedWFL);
        checkWeb("wfpass.txt", &lastModifiedWFP);
        Sleep(600000);
    }
}

