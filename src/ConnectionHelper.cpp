//
// Created by hmenn on 06.04.2017.
//


#include "../include/ConnectionHelper.h"
#include "../include/Requirements.h"


ConnectionHelper::ConnectionHelper() {

  serverfd = socket(AF_INET, SOCK_STREAM, 0);
  if (serverfd < 0) {
    throw SocketCreationException(CREATE_ERROR);
  }

  int a = 1;

  /*Set socket option to reusable*/
  setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR, &a, sizeof (int));

  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(Constants::PORT);
  serv_addr.sin_addr.s_addr = INADDR_ANY;

  if (bind(serverfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
    throw SocketCreationException(BINDING_ERROR);
  }

  if (listen(serverfd, 3) < 0) {
    throw SocketCreationException(LISTEN_ERROR);
  }

  fprintf(LOG_FD, "ConnectionHelper constucted. Port:%d now available.\n", Constants::PORT);
}

int ConnectionHelper::acceptConnection(){

  int socklen = sizeof(serv_addr);

  if ((socketfd = accept(serverfd, (struct sockaddr *) &serv_addr, (socklen_t *) &socklen)) < 0) {
    throw SocketCreationException(ACCEPT_ERROR);
  }

  return socketfd;

}

void ConnectionHelper::releaseConnection() {
  char buffer[Constants::MAX_BUFFER_SIZE];
 /* while (read(socketfd, buffer, Constants::MAX_BUFFER_SIZE) > 0) {
    printf("FreeSocket Read:%s\n", buffer);
    bzero(buffer, 250);
  } // free socket*/
  close(socketfd);
  sleep(1);
  //shutdown(socketfd,SHUT_RDWR);
  fprintf(LOG_FD,"Connection released!\n");

}

char *ConnectionHelper::readSocket(int byte) {

  char *buffer = (char *) calloc(sizeof(char), byte);
  int size = read(socketfd, buffer, byte);
  if (size > 0) {
    return buffer;
  }

  return NULL;
}

int ConnectionHelper::writeSocket(const char *msg) {
  int size;
  if ((size = write(socketfd, msg, strlen(msg))) < 0) {
    perror("write socket");
    throw InvalidConnectionException();
  }
  return size;
}

int ConnectionHelper::writeSocket1(char* msg) {
  int size;
  if ((size = write(socketfd, msg, sizeof(char))) < 0) {
    perror("write socket");
    throw InvalidConnectionException();
  }
  return size;
}

ConnectionHelper::~ConnectionHelper() {
  char buffer[250];
  bzero(buffer, 250);

  //shutdown(socketfd,SHUT_RDWR);
  //shutdown(serverfd,SHUT_RDWR);
  //close(socketfd);
  close(serverfd);
  printf("Socket-Port:%d closed.", Constants::PORT);
  sleep(1);
}

int ConnectionHelper::getSocketFD() const {
  return socketfd;
}
