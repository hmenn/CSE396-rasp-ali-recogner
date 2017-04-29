//
// Created by hmenn on 06.04.2017.
//


#include "../include/ConnectionHelper.h"
#include "../include/Requirements.h"


ConnectionHelper::ConnectionHelper(){

  serverfd = socket(AF_INET,SOCK_STREAM,0);
  if(serverfd<0){
    throw SocketCreationException(CREATE_ERROR);
  }

  bzero((char*)&serv_addr,sizeof(serv_addr));
  serv_addr.sin_family=AF_INET;
  serv_addr.sin_port=htons(Constants::PORT);
  serv_addr.sin_addr.s_addr=INADDR_ANY;

  if(bind(serverfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr))<0){
    throw SocketCreationException(BINDING_ERROR);
  }

  if(listen(serverfd,3)<0){
    throw SocketCreationException(LISTEN_ERROR);
  }

  int socklen=sizeof(serv_addr);

  if((socketfd=accept(serverfd,(struct sockaddr*)&serv_addr,(socklen_t*)&socklen))<0){
    throw SocketCreationException(ACCEPT_ERROR);
  }

  fprintf(LOG_FD,"ConnectionHelper constucted. Port:%d now available.\n",Constants::PORT);
}

void ConnectionHelper::listenPort(){
  char buffer[250];

  printf("Started to listen port\n");
  while(1){
    bzero(buffer,250);
    int size = read(socketfd,buffer,250);
    printf("%d",size);
    if(size>0){
      printf("Receive: %s\n",buffer);
      writePort("ok");
    }else{
      break;
    }

  }
}

void ConnectionHelper::writePort(const char *msg){
  int size;
  if((size = write(socketfd,msg,strlen(msg)))<0){
    perror("write socket");
    throw InvalidConnectionException();
  }
  printf("Write completed:%d\n",size);
}

ConnectionHelper::~ConnectionHelper(){
  char buffer[250];
  bzero(buffer,250);

  while (read(socketfd,buffer,Constants::MAX_BUFFER) > 0){
    printf("FreeSocket Read:%s\n",buffer);
    bzero(buffer,250);
  } // free socket
  //shutdown(socketfd,SHUT_RDWR);
  //shutdown(serverfd,SHUT_RDWR);
  close(socketfd);
  close(serverfd);
  printf("Socket-Port:%d closed.",Constants::PORT);
}
