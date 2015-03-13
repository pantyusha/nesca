#include <BasicAuth.h>

lopaStr BA::_BABrute(char *cookie,
                 char *ip,
                 int port,
                 char *pathT,
                 char *method)
{

}

lopaStr BA::_BALobby(char *cookie,
                     char *ip,
                     int port,
                     char *path,
                     char *method)
{
    while(BrutingThrds >= gMaxBrutingThreads) Sleep(700);

    BruteUtils::BConInc();
    lopaStr res = _BABrute(cookie, ip, port, path, method);
    BruteUtils::BConDec();

    return res;
}
