//
// Created by hmenn on 06.04.2017.
//


#include "../include/ConnectionHelper.h"
#include "../include/Requirements.h"


ConnectionHelper::ConnectionHelper(){

  serverfd = socket(AF_INET,SOCK_STREAM,0);
  if(serverfd<0){
    perror("Socket create");
    throw SocketCreationException();
  }

  bzero((char*)&serv_addr,sizeof(serv_addr));

  serv_addr.sin_family=AF_INET;
  serv_addr.sin_port=htons(Constants::PORT);
  serv_addr.sin_addr.s_addr=INADDR_ANY;

  if(bind(serverfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr))<0){
    perror("Socket bind");
    throw SocketCreationException();
  }

  if(listen(serverfd,3)<0){
    perror("Socket listen");
    throw SocketCreationException();
  }

  int socklen=sizeof(serv_addr);
  if((socketfd=accept(serverfd,(struct sockaddr*)&serv_addr,(socklen_t*)&socklen))<0){
    perror("Socket accept");
    throw SocketCreationException();
  }

  printf("ConnectionHelper constucted. Port:%d now available.\n",Constants::PORT);
}

void ConnectionHelper::listenPort(){
  char buffer[250];
  bzero(buffer,250);
  read(socketfd,buffer,250);
  printf("Receive: %s\n",buffer);
}


void ConnectionHelper::writePort(){
  char buffer[250];
  int size=0;
  bzero(buffer,250);
  sprintf(buffer,"Test");
  if((size = write(socketfd,buffer,strlen(buffer)))<0){
    perror("write socket");
    throw InvalidConnectionException();
  }
  printf("Write completed:%d\n",size);
}

ConnectionHelper::~ConnectionHelper(){
  char buffer[250];
  bzero(buffer,250);
  while (read(socketfd,buffer,250) > 0); // free socket
  //shutdown(socketfd,SHUT_RDWR);
  //shutdown(serverfd,SHUT_RDWR);
  close(socketfd);
  close(serverfd);
  printf("Socket-Port:%d closed.",Constants::PORT);
}
