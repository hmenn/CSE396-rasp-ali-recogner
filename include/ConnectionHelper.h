//
// Created by hmenn on 06.04.2017.
//

#ifndef CONNECTION_HELPER
#define CONNECTION_HELPER

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

class ConnectionHelper {

public:
    ConnectionHelper();
    ~ConnectionHelper();
    void listenPort();
    void writePort();
    void startRemoteServer();
    void openArdConnection();
    bool state=false;
private:
    int serverfd;
    int socketfd;
    int clielen; // size of client address
    struct sockaddr_in serv_addr,cli_addr;
};


#endif //CSE396_RASP_ALI_RECOGNER_CONNECTIONHELPER_H
