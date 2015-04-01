#include "FileDownloader.h"
#include "Connector.h"
#include "FileUpdater.h"
#include "istream"

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

void checkWeb(const char *fileName, long *ptr, void *func(void)) {
    std::string buffer;
    Connector::nConnect(std::string("localhost/nesca/" + std::string(fileName)).c_str(), 8080, &buffer);

    std::cout<<buffer<<std::endl;
    int cl = getCL(&buffer);
    if(cl == -1) return;

    if(cl != *ptr) {
        std::ofstream out(fileName);
        out << buffer.substr(buffer.find("\r\n\r\n") + 4);
        out.close();

        stt->doEmitionFoundData("<font color=\"Pink\">File " + QString(fileName) + " downloaded.</font>");
     }
}

void loadNegatives(){
}

void FileDownloader::checkWebFiles() {
    //checkWeb("negatives.txt", &FileUpdater::oldNegLstSize, (void*(*)(void))loadNegatives);
    //checkWeb("login.txt", (void*(*)(void))loadLogins);
    //checkWeb("pass.txt", (void*(*)(void))loadPass);
    //checkWeb("sshpass.txt", (void*(*)(void))loadSSHPass);
    //checkWeb("wflogin.txt", (void*(*)(void))loadWFLogins);
    //checkWeb("wfpass.txt", (void*(*)(void))loadWFPass);
}

