#include "Connector.h"
#include "SSHAuth.h"
#include "Filter.h"


#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
int _pingMyTarget(const char *ip)
{
    HANDLE hIcmpFile;
    unsigned long ipaddr = INADDR_NONE;
    DWORD dwRetVal = 0;
    char SendData[32] = "Data Buffer";
    LPVOID ReplyBuffer = NULL;
    DWORD ReplySize = 0;

    ipaddr = inet_addr(ip);

    if (ipaddr == INADDR_NONE)
    {
        stt->doEmitionRedFoundData("[Pinger] INADDR_NONE! [" + QString(ip) + "]");
        return 0;
    }

    hIcmpFile = IcmpCreateFile();
    if (hIcmpFile == INVALID_HANDLE_VALUE)
    {
        stt->doEmitionRedFoundData("[Pinger] Unable to open handle. [" + QString::number(GetLastError()) + "]");
        return 0;
   }

    ReplySize = sizeof(ICMP_ECHO_REPLY) + sizeof(SendData);
    ReplyBuffer = (VOID*) malloc(ReplySize);
    if (ReplyBuffer == NULL)
    {
        stt->doEmitionRedFoundData("[Pinger] Unable to allocate memory.");
        return 0;
    }

    dwRetVal = IcmpSendEcho(hIcmpFile, ipaddr, SendData, sizeof(SendData),
        NULL, ReplyBuffer, ReplySize, gPingTimeout*1000);
    if (dwRetVal != 0) {
        PICMP_ECHO_REPLY pEchoReply = (PICMP_ECHO_REPLY)ReplyBuffer;
        struct in_addr ReplyAddr;
        ReplyAddr.S_un.S_addr = pEchoReply->Address;
        printf("\tSent icmp message to %s\n", "127.0.0.1");
        if (dwRetVal > 1)
        {
            if(gDebugMode) stt->doEmitionYellowFoundData("[Pinger] Received " + QString::number(dwRetVal) + " icmp message responses.");
        }
        else
        {
            if(gDebugMode) stt->doEmitionYellowFoundData("[Pinger] Received " + QString::number(dwRetVal) + " icmp message responses.");
        }

        if(gDebugMode) stt->doEmitionYellowFoundData("[Pinger] Received from: " + QString(inet_ntoa( ReplyAddr )) + "; Status = " + QString::number(pEchoReply->Status) + "; Roundtrip time = " + QString::number(pEchoReply->RoundTripTime) + "ms.");
        return 1;
    }
    else
    {
        printf("\tCall to IcmpSendEcho failed.\n");
        printf("\tIcmpSendEcho returned error: %ld\n", GetLastError() );
        if(gDebugMode) stt->doEmitionRedFoundData("[Pinger] Call to IcmpSendEcho failed. IcmpSendEcho returned error: " + QString::number(GetLastError()));
        return 0;
    };
}
#else
int _pingMyTarget(const char *ip)
{
    FILE *pipe = popen(("ping -w " + std::to_string(gPingTimeout) + " " + ip).c_str(), "r");
    if(!pipe) {
        stt->doEmitionRedFoundData("Ping pipe failed: cannot open pipe.");
        perror("pipe");
        return 0;
    }

    char buffer[128] = {0};
    std::string result;

    while(!feof(pipe)) {
        if(fgets(buffer, 128, pipe) != NULL){
            result += buffer;
        }
    }
    pclose(pipe);

    if(strstr((char*)result.c_str(), "100% packet loss") != NULL) return 0;
    return 1;
}
#endif

struct data {
  char trace_ascii; /* 1 or 0 */
};

static
int my_trace(CURL *handle, curl_infotype type,
             char *data, size_t size,
             void *userp)
{
  if (type == CURLINFO_HEADER_OUT) {
	  data[size] = '\0';
	  Activity += strlen(data);
	  stt->doEmitionAddOutData(QString(data));
  }

  return 0;
}

size_t nWriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
	size_t realsize = size * nmemb;
	if (((std::string*)userp)->size() > 180000) return -1;
	((std::string*)userp)->append((char*)contents, realsize);
	Activity += realsize;
	return realsize;

	//struct MemoryStruct *mem = (struct MemoryStruct *)userp;
	//if (mem->size > 180000) return -1;
	//size_t realsize = size * nmemb;
	//mem->memory = (char*)realloc(mem->memory, mem->size + realsize + 1);

	//if (mem->memory == NULL) {
	//	stt->doEmitionRedFoundData("not enough memory (realloc returned NULL)\n");
	//	return 0;
	//}

	//memcpy(&(mem->memory[mem->size]), contents, realsize);
	//mem->size += realsize;
	//mem->memory[mem->size] = 0;
	//Activity += realsize;
	//return realsize;
}

int Connector::nConnect(const char* ip, const int port, std::string *buffer,
                        const char *postData,
                        const std::vector<std::string> *customHeaders,
                        const std::string *lpString,
						bool digestMode){
	buffer->clear();
	int res = 0;
	CURL *curl = curl_easy_init();

	if (curl != NULL)
	{
		curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
		if (MapWidgetOpened) {
			struct data config;
			config.trace_ascii = 1; /* enable ascii tracing */
			curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, my_trace);
			curl_easy_setopt(curl, CURLOPT_DEBUGDATA, &config);
			curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
		}
		curl_easy_setopt(curl, CURLOPT_URL, ip);
		curl_easy_setopt(curl, CURLOPT_PORT, port);
		curl_easy_setopt(curl, CURLOPT_USERAGENT,
			"Mozilla/5.0 (X11; Linux x86_64; rv:35.0) Gecko/20100101 Firefox/35.0");
		curl_easy_setopt(curl, CURLOPT_HEADER, 1L);
		curl_easy_setopt(curl, CURLOPT_AUTOREFERER, 1L);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, nWriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, buffer);
		int proxyPort = std::atoi(gProxyPort);
		if (proxyPort > 0 && proxyPort < 65535) curl_easy_setopt(curl, CURLOPT_PROXYPORT, proxyPort);
		curl_easy_setopt(curl, CURLOPT_PROXY, gProxyIP);
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, gTimeOut);
		curl_easy_setopt(curl, CURLOPT_TIMEOUT, gTimeOut + 3);

		if (postData != NULL) curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData);

		if (customHeaders != NULL) {

			struct curl_slist *chunk = NULL;
			for (auto &ch : *customHeaders) chunk = curl_slist_append(chunk, ch.c_str());
			curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
		}
		
		if (lpString != NULL) {
			curl_easy_setopt(curl, CURLOPT_UNRESTRICTED_AUTH, 1L);
			curl_easy_setopt(curl, CURLOPT_FTPLISTONLY, 1L);
			curl_easy_setopt(curl, CURLOPT_USERPWD, lpString->c_str());
			if (digestMode)
			{
				curl_easy_setopt(curl, CURLOPT_HTTPAUTH, (long)CURLAUTH_DIGEST);
				res = curl_easy_perform(curl);
				
				if (port != 21 && lpString != NULL) {
					int pos = Utils::ustrstr(*buffer, "\r\n\r\n");
					if (pos != -1) {
						*buffer = buffer->substr(pos + 4);
					}
				}
			}
			else res = curl_easy_perform(curl);
		}
		else res = curl_easy_perform(curl);
		
		int sz = buffer->size();
		if (res == CURLE_OK || (port == 21 && sz > 0)) {
			if (MapWidgetOpened) stt->doEmitionAddIncData(QString(ip), QString(buffer->c_str()));
			Activity += sz;

			curl_easy_cleanup(curl);
			return sz;
		} else {
			curl_easy_cleanup(curl);
			if (res == 6) return -2;
			if (res == 56 && port == 8000) return 1;														//Hikvision
			else if (res != 28 &&
				res != 7 &&
				res != 13 &&
				res != 67 &&
				res != 52 &&
				res != 56 &&
				res != 55 &&
				res != 35 &&
				res != 19 &&
                res != 23) 
			{
				if (res == 5) {
					stt->doEmitionRedFoundData("Couldn't resolve proxy. The given proxy host could not be resolved. ");
					return -2;
				} else if (res == 8) {
					stt->doEmitionFoundData("Strange ftp reply. (" + 
						QString::number(res) + ") " + QString(ip) + 
						":" + QString::number(port));
					return -2;
				}
				else if (res == 18) {
					stt->doEmitionFoundData("Inappropriate file size. (" +
						QString::number(res) + ") " + QString(ip) +
						":" + QString::number(port));
					return -2;
				}
				else stt->doEmitionRedFoundData("CURL error: (" + QString::number(res) + ") " + 
					QString(ip) + ":" + QString::number(port));
			};
			
			if (res == 23 && sz > 0) {
				return sz;
			}
			else return -1;
		}

		if (MapWidgetOpened) stt->doEmitionAddIncData(QString(ip), QString(buffer->c_str()));
		return sz;
    } else {
        stt->doEmitionRedFoundData("Curl error.");
        return -1;
	};
}

int Connector::connectToPort(char* ip, int port)
{
    if(gPingNScan)
    {
        if(_pingMyTarget(ip) == 0) return -2;
    };

    std::string buffer;
    int size = 0;

	if (port == 22) size = SSHAuth::SSHLobby(ip, port, &buffer);			//SSH
    else size = nConnect(ip, port, &buffer);

    if(size > 0)
    {
        ++Alive;//ME2
		++found;//PieStat
        Lexems lx;
		lx.filler(ip, port, &buffer, size, &lx);
	}
	else if (size == -2) return -2;
    return 0;
}
