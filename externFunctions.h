#ifndef EF_H
#define EF_H

extern std::string toLowerStr(const char *str);
extern int recvS(int lSock, char *recvBuffT, int len);
extern int sendS(int lSock, char *msg, int len, int mode);
extern void UserNickInit();
extern void GetNicks();
extern int startScan(char* argv);
extern void BConInc();
extern void BConDec();
extern QString GetNSErrorDefinition(const char *str, const char *defin);
extern void _SaveBackupToFile();
extern char* strstri(const char *_Str, const char *_SubStr);
extern void nCleanup();
extern void getSubStr(char *src, char *startStr, char *endStr, char *dest, int szDest);
extern void getSubStrEx(char *src, char *startStr, char *endStr, char *dest, int szDest);
extern std::string xcode(LPCSTR src, UINT srcCodePage, UINT dstCodePage);
extern void _SaveBackupToFile();
extern char *_findFirst(const char *str, char *delim);
//extern int nConnect(char *ip, int port, std::string *buffer);


#endif // EF_H
