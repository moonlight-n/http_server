#ifndef FUNCTION_MAIN_H
#define FUNCTION_MAIN_H

#define ADDRSTRLEN (NI_MAXSERV + NI_MAXHOST + 10)
#define MAX_CLIENTS 5
#define BUFFER_SIZE 1024
#define NUMBER_THREADS 5

void create_threads(int number_threads);
int create_mkfifo(char* server_fifo);
int create_socket(char* ser_port);

#endif
