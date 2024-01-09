#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>
#include <fcntl.h>
#include <dlfcn.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <sys/types.h>
#include <sys/stat.h>

#include "headers/function_thread.h"


/*  проверка на коректность запроса */
int checking_input_accuracy(char* request_str){

  char method[MAX_METHOD_LENGTH];
  char path[MAX_PATH_LENGTH];
  char protocol[MAX_PROTOCOL_LENGTH];

  sscanf(request_str, "%s %s %s", method, path, protocol);
      
  if(strcmp(protocol, "HTTP/1.1") == 0){
    for(int index = 0; index < strlen(path); index++){
      if(path[index] == '%'){
	return 400;
      }
    }
    return 0;
  }
  else{
    return 505;
  }  
  
}

/* функция получает запрос в виде строки и парсит его в структуру 
 * и возвращает указатель на эту структуру  */
 
http_request* parse_http_request(char* request_str){
  
    http_request* request = malloc(sizeof(http_request));
    memset(request, 0, sizeof(http_request));
   
    char* request_ptr = malloc(strlen(request_str));
    
    if(request == NULL || request_ptr == NULL){
      perror("malloc");
      exit(EXIT_FAILURE);
    }
    strcpy(request_ptr, request_str);
    
    // Parse request method, path, and protocol
    sscanf(request_ptr, "%s %s %s", request->method, request->path, request->protocol);

    // Parse headers
    char* line = strtok(request_ptr, "\r\n");
    while (line) {
        if (strstr(line, ":")) {
            request->headers[request->num_headers] = malloc(strlen(line) + 1);
            strcpy(request->headers[request->num_headers], line);
            request->num_headers++;
        }
        line = strtok(NULL, "\r\n");
    }

    char* body_start = strstr(request_str, "\r\n\r\n");
    
    printf("%s\n", request_str);
       
    if (body_start) {
        request->body = malloc(strlen(body_start) + 1);
        strcpy(request->body, body_start + 4);
    }

    free(request_ptr);
    
    return request;
}

/* функция получает распарсенный ответ и клиенский дескриптор
 * и отправляет ответ клиенту */
void send_response_from_server(int client_socket, http_response* response){
  
    char header[BUFFER_SIZE] = {0};
    const char* substr = "Content-Length: ";
    long num;
    // Add headers
    for (int i = 0; i < response->num_headers; i++) {
        strcat(header, response->headers[i]);
        strcat(header, "\r\n");
    }
    strcat(header, "\r\n");
   
    // Send response headers
    write(client_socket, header, strlen(header));

    if(strstr(response->headers[1], "image/png")){
      const char* num_str = strstr(response->headers[2], substr);

      if (num_str) {
        num_str += strlen(substr);
        num = strtol(num_str, NULL, 10);
        printf("\nContent length: %ld\n", num);
      }

      write(client_socket, response->body, num);
    }
    else{
      // Send response content
      write(client_socket, response->body, strlen(response->body));
    }

}

/* функция освобождает структуру http_request */
void free_http_request(http_request** request){
  http_request* request_ptr = *request;
  
  for(int index = 0; index < (request_ptr->num_headers); index++){
    free(request_ptr->headers[index]);
  }
  free(request_ptr->body);
  free(request_ptr);
  request_ptr = NULL;
}

/* функция освобождает структуру http_response */
void free_http_response(http_response** response){
  http_response* response_ptr = *response;
  
  for(int index = 0; index < (response_ptr->num_headers); index++){
    free(response_ptr->headers[index]);
  }
  free(response_ptr->body);
  free(response_ptr);
  response_ptr = NULL;

}

