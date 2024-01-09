/* Здесь надо подключить недостающие библиотеки*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>

#include <fcntl.h> // open
#include <sys/types.h> // open
#include <sys/stat.h> // open

#include "headers/thread.h"
#include "headers/function_thread.h"

/* тестовая функция обработке потока которая получает в аргументе дескриптор клиента*/
void* thread_function(){
  int header_error;
  int client_socket ;
  int queue_mk_fd;
  int bytes_request;
  http_request* request = NULL;
  http_response* response = NULL;

  char buffer[BUFFER_SIZE] = {0};
    
  if((queue_mk_fd = open(SERVER_FIFO, O_RDONLY)) == -1){
    perror("open_R:");
    exit(EXIT_FAILURE);
  }
  
  for(;;){
    
    // Ждем в очереди сообщений адрес и порт клиента
    if(read(queue_mk_fd, &client_socket, sizeof(client_socket)) != sizeof(client_socket)) {
      printf("Error read tid: %ld", pthread_self());// выводим идентификатор данного потока где произошла ашибка 
    }
    printf("\n\nthread_tid: %ld\n\n", pthread_self());
    printf("clientfd thread child = %d\n", client_socket);
    
    memset(buffer, 0, sizeof(buffer));
        
    // Read client request
    if(bytes_request = read(client_socket, buffer, BUFFER_SIZE) == EOF){
      printf("server close connection\n");
    }
    else if(bytes_request < 0){
      perror("read");
    }

    printf("%s\n", buffer);	

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    header_error = checking_input_accuracy(buffer);
    printf("header_error = %d\n", header_error);
    
    if(header_error == 0){
      
    request = parse_http_request(buffer);

    response = run_plugin(request);
    }
    else if(header_error == 505 ){
      continue;
    }
    else if(header_error == 400){
      if((response = malloc(sizeof(http_response))) == NULL){
	  perror("malloc");
	  exit(EXIT_FAILURE);
      }
      memset(response, 0, sizeof(http_response));
    
      char *body_error = "<html><body><h1>400 Bad Request-400</h1></body></html>";
      if((response->body = malloc(strlen(body_error))) == NULL ){
	  perror("malloc");
	  exit(EXIT_FAILURE);
      }
      strcpy(response->body, body_error);
      
      response->status_code = 400;

      response->num_headers = 4;
      for (int index = 0; index < response->num_headers; index++){
	if((response->headers[index] = malloc( sizeof(char) * 100)) == NULL){
	    perror("malloc");
	    exit(EXIT_FAILURE);
	}
      }
      
      sprintf(response->headers[0], "HTTP/1.1 %d %s", 400, "Bad Request");
      sprintf(response->headers[1], "Content-Type: %s", "text/html");
      sprintf(response->headers[2], "Content-Length: %ld", strlen(response->body));
      sprintf(response->headers[3], "Connection: close");  
    }
    
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    /* отплавка ответа пользователю*/
    send_response_from_server(client_socket,response);
    
    /* функции которая освобождает память структуры типа http_response, http_request*/
    if(header_error = 0){
      free_http_request(&request);
    }

    free_http_response(&response);
    
    request = NULL;
    response = NULL;
    
    if(close(client_socket) == -1){
      perror("close_socket: ");
      exit(EXIT_FAILURE);
    }   
  }
   
  pthread_exit(NULL);
}
