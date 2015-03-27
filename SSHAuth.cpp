#include "SSHAuth.h"
#include "FileUpdater.h"

int _sshConnect(char *user, char *pass, const char *host, int port)
{
    char hostStr[128] = {0};
    ZeroMemory(hostStr, sizeof(hostStr));
    strcpy(hostStr, user);
    strcat(hostStr, "@");
    strcat(hostStr, host);

    ssh_session my_ssh_session = ssh_new();
    if (my_ssh_session == NULL)
    {
        ssh_free(my_ssh_session);
        return -1;
    };

    ssh_options_set(my_ssh_session, SSH_OPTIONS_HOST, hostStr);
    ssh_options_set(my_ssh_session, SSH_OPTIONS_PORT, &port);
    ssh_options_set(my_ssh_session, SSH_OPTIONS_STRICTHOSTKEYCHECK, 0);
    ssh_options_set(my_ssh_session, SSH_OPTIONS_GSSAPI_DELEGATE_CREDENTIALS, 0);
    ssh_options_set(my_ssh_session, SSH_OPTIONS_TIMEOUT, &(gTimeOut + 1));

    int rc = ssh_connect(my_ssh_session);

    if (rc != SSH_OK)
    {
        ssh_disconnect(my_ssh_session);
        ssh_free(my_ssh_session);
        ++offlines;
        return -2;
    }
    else
    {
        rc = ssh_userauth_password(my_ssh_session, NULL, pass);
        if (rc != SSH_AUTH_SUCCESS)
        {
            ssh_disconnect(my_ssh_session);
            ssh_free(my_ssh_session);
            return -1;
        };
    };
    ssh_disconnect(my_ssh_session);
    ssh_free(my_ssh_session);
    ++ssh;
    return 0;
}

char _get_ssh_banner(const char *ip, int port)
{
    char recvBuff[256] = {0};
    std::string buffer;
    Connector::nConnect(ip, port, &buffer);

    int sz = buffer.size();

    if(sz != 0)
    {
        strncpy(recvBuff, buffer.c_str(), sz < 256 ? sz : 256);
    };

    return *recvBuff;
}

int check_ssh_pass(char *user, char *pass, char *userPass, const char *host, int port, std::string *buffer, const char *banner)
{
    int res = -1;
    if(BALogSwitched) stt->doEmitionBAData("Probing SSH: " + QString(user) + ":" + QString(pass) + "@" + QString(host) + ":" + QString::number(port));
    res = _sshConnect(user, pass, host, port);
    if(res == 0)
    {
        stt->doEmition_BAGreenData("[+] SSH: " + QString(user) + ":" + QString(pass) + "@" + QString(host));
        buffer->append(userPass);
        buffer->append("@");
        buffer->append(host);
        buffer->append("|+|");
        buffer->append(banner);
        return 0;
    };
    return res;
}

int SSHBrute(const char* host, int port, std::string *buffer, const char *banner)
{
    char login[32] = {0};
    char pass[32] = {0};
    char temp[64] = {0};
    BruteUtils::BConInc();
    int sz = 0;
    char *ptr1 = 0;
    int res = -1;

    for(int i = 0; i < MaxSSHPass; ++i)
    {
        FileUpdater::cv.wait(FileUpdater::lk, []{return FileUpdater::ready;});
        if(!globalScanFlag) break;
        strcpy(temp, sshlpLst[i]);
        ptr1 = strstr(temp, ":");

        if (ptr1 == NULL) {
			stt->doEmitionRedFoundData("[SSH]Wrong format: " + QString(temp));
			return -1;
		}

        sz = ptr1 - temp;
        strncpy(login, temp, sz);
        strcpy(pass, ptr1 + 1);

        res = check_ssh_pass(login, pass, temp, host, port, buffer, banner);

        ZeroMemory(login, sizeof(login));
        ZeroMemory(pass, sizeof(pass));
        ZeroMemory(temp, sizeof(temp));

        if(res == 0)
        {
            if(i == 0) return -2; //Failhit
            BruteUtils::BConDec();
            return 1;
        }
        else if(res == -2)
        {
            BruteUtils::BConDec();
            return -2;
        };

        Sleep(500);
    };
    BruteUtils::BConDec();
    return -1;
}

QString strIP;
QString strPort;
int SSHAuth::SSHLobby(const char *ip, int port, std::string *buffer)
{
    const char &banner = _get_ssh_banner(ip, port);
    if(strlen(&banner) > 0)
    {
        return SSHBrute(ip, port, buffer, &banner);
    };
    return -1;
}
