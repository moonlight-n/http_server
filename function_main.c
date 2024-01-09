
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>
#include <pthread.h>
#include <fcntl.h>
#include <signal.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <sys/types.h>
#include <sys/stat.h>

#include "headers/function_main.h"
#include "headers/thread.h"

void create_threads(int number_threads){
  
  /*Делаем глобальный массив threads[] чтобы можно было дождатся и отработку всех потоков
  * и закрыть их */
  pthread_t thread_tid;

  int thread;
  
  for(int index = 0; index < number_threads; index++){
    thread = pthread_create(&thread_tid, NULL, thread_function, NULL);
    if(thread != 0 ){
      perror("pthread_create:");
      exit(EXIT_FAILURE);
    }
  }
}

int create_mkfifo(char* server_fifo){

  int file_mk;
  
  file_mk = mkfifo(server_fifo, S_IRUSR | S_IWUSR | S_IWGRP );

  if (file_mk == -1 && errno != EEXIST){
    perror("mkfifo:");
    exit(EXIT_FAILURE);
  }
}

int create_socket(char* ser_port)
{
  int serverFd;
  int yes;
  struct addrinfo hints;
  struct addrinfo* result;
  struct addrinfo* rp;

  char addrStrServer[ADDRSTRLEN];
  char hostServer[NI_MAXHOST];
  char serviceServer[NI_MAXSERV];
  
  /* Вызываем getaddrinfo(), чтобы получить список адресов,
   * к которым можем попытаться привязать наш сокет */
  memset(&hints, 0, sizeof(hints));
  hints.ai_flags      = AI_PASSIVE;
  hints.ai_family     = AF_UNSPEC; // выбераем семейчтво подключения 
  hints.ai_socktype   = SOCK_STREAM;


  if(getaddrinfo(NULL, ser_port, &hints, &result) != 0){
    perror("getaddrinfo");
    exit(EXIT_FAILURE);
  }

  /* Перебираем полученный список, пока не находим структуру с адресом,
     подходящую для создания и привязывания сокета */
  for(rp = result; rp != NULL; rp = rp->ai_next ){
   
    serverFd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
    if(serverFd == -1){
      //perror("socket");
      printf("%d\n", errno);
      continue;
    }

    if (setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
    {
      perror(" setsockopt\n");
      close(serverFd);
    }

    if(bind(serverFd, rp->ai_addr, rp->ai_addrlen) == 0){
      break;
    }
    
    /* Вызов bind() завершился неудачно; закрываем этот сокет и пробуем следующий адрес */
    close(serverFd);
  }
  
  freeaddrinfo(result);
  
  if(rp == NULL){
    printf("Could not bind socket to any address\n");
    perror("listen");
    exit(EXIT_FAILURE);
  }

  /* Переводим свой адрес в читаемый вид*/
  if(getnameinfo((struct sockaddr*) rp->ai_addr, rp->ai_addrlen,
		   hostServer, NI_MAXHOST, serviceServer, NI_MAXSERV, 0) == 0){
    snprintf(addrStrServer, ADDRSTRLEN, "(%s, %s)", hostServer, serviceServer);
  }
  
  if(listen(serverFd, MAX_CLIENTS) == -1){
    perror("listen");
    exit(EXIT_FAILURE);
  }

  return serverFd;
}
