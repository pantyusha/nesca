#ifndef CONNECTOR_H
#define CONNECTOR_H

class Connector {

private:
    int _sshConnect(char *user, char *pass, char *host, int port);
    int _get_ssh_banner(char *ip, int port);
    int check_ssh_pass(char *user, char *pass, char *userPass,
                       char *host, int port, std::string *buffer, const char *banner);
    int _EstablishSSHConnection(char *host, int port, std::string *buffer, const char *banner);

public:
    static int nConnect(char *ip, int port, std::string *buffer,
                               const char *postData = NULL,
                               const std::vector<std::string> *customHeaders = NULL);
    static int _ConnectToPort(char *ip, int port, char *hl);
    static int _SSHLobby(char *ip, int port, std::string *buffer);
};
#endif // CONNECTOR_H
