#ifndef _INTERFACE
#define _INTERFACE 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_METHOD_LENGTH 10
#define MAX_PATH_LENGTH 50
#define MAX_PROTOCOL_LENGTH 10
#define MAX_HEADER_LENGTH 100

/* Структура, которую получает плагин */
typedef struct {
    char method[MAX_METHOD_LENGTH];
    char path[MAX_PATH_LENGTH];
    char protocol[MAX_PROTOCOL_LENGTH];
    char* headers[MAX_HEADER_LENGTH];
    int num_headers;
    char* body;
} http_request;

/* Структура, которую возвращает плагин */
typedef struct {
    int status_code;
    char* headers[MAX_HEADER_LENGTH];
    int num_headers;
    char* body;
} http_response;

#endif
