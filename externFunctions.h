#ifndef EF_H
#define EF_H

extern std::string toLowerStr(const char *str);
extern int startScan(char* argv);
extern QString GetNSErrorDefinition(const char *str, const char *defin);
extern void _SaveBackupToFile();
extern char* strstri(const char *_Str, const char *_SubStr);
extern void nCleanup();
extern void getSubStr(char *src, char *startStr, char *endStr, char *dest, int szDest);
extern void getSubStrEx(char *src, char *startStr, char *endStr, char *dest, int szDest);
extern void _SaveBackupToFile();
extern char *_findFirst(const char *str, char *delim);


#endif // EF_H
