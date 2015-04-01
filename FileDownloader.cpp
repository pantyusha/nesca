#include "FileDownloader.h"
#include "Connector.h"
#include "FileUpdater.h"
#include "istream"

bool FileDownloader::running = false;

int getCL(std::string *buffer) {

    std::size_t pos1 = buffer->find("Content-Length:");
    if(pos1 == std::string::npos) {
        stt->doEmitionFoundData("<font color=\"Pink\">Cannot find Content-Length.</font>");
        return -1;
    }
    int pos2 = buffer->find("\r\n", pos1);
    if(pos2 == std::string::npos) {
        stt->doEmitionFoundData("<font color=\"Pink\">Weird reply.</font>");
        return -1;
    }

    std::string res = buffer->substr(pos1 + 15, pos2 - pos1 - 15);
    return stoi(res);
}

void checkWeb(const char *fileName, long *ptr) {
    std::string buffer;
    Connector::nConnect(std::string("http://nesca.d3w.org/files/" + std::string(fileName)).c_str(), 80, &buffer);

    int cl = getCL(&buffer);
    if(cl == -1) return;

    if(cl != *ptr) {
		QString res(buffer.substr(buffer.find("\r\n\r\n") + 4).c_str());
		res.replace("\r\n", "\n");
		QTextCodec *codec = QTextCodec::codecForName("Windows-1251");
		res = codec->toUnicode(res.toLocal8Bit().data());
		std::ofstream out(fileName);
		out << std::string(res.toLocal8Bit().data());
        out.close();

        stt->doEmitionFoundData("<font color=\"Pink\">File " + QString(fileName) + " downloaded.</font>");
     }
}

void FileDownloader::checkWebFiles() {
	running = true;
	while (globalScanFlag) {
		checkWeb("negatives.txt", &FileUpdater::oldNegLstSize);
		checkWeb("login.txt", &FileUpdater::oldLoginLstSize);
		checkWeb("pass.txt", &FileUpdater::oldPassLstSize);
		checkWeb("sshpass.txt", &FileUpdater::oldSSHLstSize);
		checkWeb("wflogin.txt", &FileUpdater::oldWFLoginLstSize);
		checkWeb("wfpass.txt", &FileUpdater::oldWFPassLstSize);
		Sleep(600000);
	}
	running = false;
}

