#ifndef FILEDOWNLOADER_H
#define FILEDOWNLOADER_H

#include "Connector.h"

class FileDownloader {
private:
    static std::string lastModifiedNeg;
    static std::string lastModifiedL;
    static std::string lastModifiedP;
    static std::string lastModifiedSSH;
    static std::string lastModifiedWFL;
    static std::string lastModifiedWFP;
public:
    static std::string lastModified;
    static void checkWebFiles();
};

#endif // FILEDOWNLOADER_H
