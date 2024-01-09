
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <fcntl.h>

#include <arpa/inet.h>
#include <netinet/in.h>

#include <sys/types.h>
#include <sys/stat.h>

#include "headers/function_main.h"
#include "headers/main.h"

int main(int argc, char *argv[]) {
  int file_mk;
  int queue_mk_fd;
  int thread_index = 0;
  int server_socket;
  int client_socket;
 
  struct sockaddr_in  client_address;
  socklen_t client_address_len = sizeof(client_address);

  file_mk = create_mkfifo(SERVER_FIFO);
  server_socket = create_socket(SER_PORT);
  create_threads(NUMBER_THREADS);
  
  /*Открываем дополнительный записывающий дескриптор
  чтобы некогда не получать конец файла*/
  if ((queue_mk_fd = open(SERVER_FIFO, O_WRONLY)) == -1){
    perror("open_W:");
    exit(EXIT_FAILURE);
  }

   
  printf("Server started on port 8080\n");

  // Accept incoming connections
  for(;;){
    
    if ((client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_address_len)) < 0) {
       perror("Failed to accept connection");
       exit(EXIT_FAILURE);
    }

    printf("New client connected: %s:%d\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));

   // передоем в очередь клиенский дискриптор
    if(write(queue_mk_fd, &client_socket, sizeof(client_socket)) != sizeof(client_socket) ){
      printf("Error writng to FIFO\n");
    }  
  }
	  
  // usleep(1000000);
    printf("server end\n");
    return 0;
}
