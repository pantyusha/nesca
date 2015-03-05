#include <Connector.h>



int Connector::_sshConnect(char *user, char *pass, char *host, int port)
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
    //ssh_options_set(my_ssh_session, SSH_OPTIONS_LOG_VERBOSITY, &verbosity);
    //ssh_options_set(my_ssh_session, SSH_OPTIONS_LOG_VERBOSITY_STR, &verbosity);
    //ssh_options_set(my_ssh_session, SSH_OPTIONS_STRICTHOSTKEYCHECK, 0);
    int sshTimeout = gTimeOut + 1;
    ssh_options_set(my_ssh_session, SSH_OPTIONS_TIMEOUT, &sshTimeout);

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

char Connector::_get_ssh_banner(char *ip, int port)
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

int Connector::check_ssh_pass(char *user, char *pass, char *userPass, char *host, int port, std::string *buffer, const char *banner)
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

int Connector::_EstablishSSHConnection(char *host, int port, std::string *buffer, const char *banner)
{
    char login[32] = {0};
    char pass[32] = {0};
    char temp[64] = {0};
    isActive = 1;
    BConInc();
    int sz = 0;
    char *ptr1 = 0;
    int res = -1;
    for(int i = 0; i < MaxSSHPass; ++i)
    {
        if(globalScanFlag == false) break;
        strcpy(temp, sshlpLst[i]);
        ptr1 = strstr(temp, ":");
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
            BConDec();
            isActive = 0;
            return 0;
        }
        else if(res == -2)
        {
            BConDec();
            isActive = 0;
            return -2;
        };
        Sleep(500);
    };
    BConDec();
    isActive = 0;
    return -1;
}

QString strIP;
QString strPort;
int Connector::_SSHLobby(char *ip, int port, std::string *buffer)
{
    const char &banner = _get_ssh_banner(ip, port);
    if(strlen(&banner) > 0)
    {
        return _EstablishSSHConnection(ip, port, buffer, &banner);
    };
    return -1;
}

static size_t nWriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}
int Connector::nConnect(char *ip, int port, std::string *buffer,
                        const char *postData = NULL,
                        const std::vector<std::string> *customHeaders = NULL){

    CURL *curl = curl_easy_init();

    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, ip);
        curl_easy_setopt(curl, CURLOPT_PORT, port);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (X11; Linux x86_64; rv:35.0) Gecko/20100101 Firefox/35.0");
        curl_easy_setopt(curl, CURLOPT_HEADER, 1L);
        curl_easy_setopt(curl, CURLOPT_AUTOREFERER, 1L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, nWriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, buffer);
        curl_easy_setopt(curl, CURLOPT_PROXY, "--");
        curl_easy_setopt(curl, CURLOPT_PROXYPORT, 3128);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, gTimeOut);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, gTimeOut);

        if(postData != NULL) {

            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData);
        };

        if(customHeaders != NULL) {

            struct curl_slist *chunk = NULL;

            for(auto &ch : customHeaders) {

                chunk = curl_slist_append(chunk, *ch);
            }

            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
        }

        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    } else {
        stt->doEmitionRedFoundData("Curl error.");
        return -1;
    };

    return buffer->size();
}
int Connector::_ConnectToPort(char *ip, int port, char *hl)
{
    if(gPingNScan)
    {
        if(_pingMyTarget(ip) == 0)
        {
            return -2;
        };
    };

    std::string buffer;
    int size = 0;

    if(port == 22) size = _SSHLobby(ip, port, &buffer);
    else size = nConnect(ip, port, &buffer);

    if(size > 0)
    {
        ++Alive;
        ++found;
        stt->doEmitionChangeParsed(QString::number(saved) + "/" + QString::number(found));

        conSTR CSTR;
        CSTR.lowerBuff = new char[size + 1];
        CSTR.size = size;
        memcpy(CSTR.lowerBuff, buffer.c_str(), size);
        memset(CSTR.lowerBuff + size, '\0', 1);

        Lexems lx;
        lx._filler(port, (char *)buffer.c_str(), ip, size, &lx, hl);

        delete []CSTR.lowerBuff;
        CSTR.lowerBuff = NULL;
    };

    return 0;
}
