#ifndef FUNCTION_THREAD_H
#define FUNCTION_THREAD_H

#define MAX_METHOD_LENGTH 10
#define MAX_PATH_LENGTH 50
#define MAX_PROTOCOL_LENGTH 10
#define MAX_HEADER_LENGTH 100

/* структура которая получает плагин */
typedef struct {
    char method[MAX_METHOD_LENGTH];
    char path[MAX_PATH_LENGTH];
    char protocol[MAX_PROTOCOL_LENGTH];
    char* headers[MAX_HEADER_LENGTH];
    int num_headers;
    char* body;
} http_request;

/* структуру которую возвращает плагин*/
typedef struct {
    int status_code;
    char* headers[MAX_HEADER_LENGTH];
    int num_headers;
    char* body;
} http_response;

#define ADDRSTRLEN (NI_MAXSERV + NI_MAXHOST + 10)
#define BUFFER_SIZE 1024
#define SIZ_BUFFER_REQUEST 1024


/* Проверка на коректность запроса */
int checking_input_accuracy(char* request_str);

/* функция получает запрос в виде строки и парсит его в структуру 
 * и возвращает указатель на эту структуру  */
http_request* parse_http_request(char* request_str);

/* функция имитирует подключения плагина */
http_response* processing_request(http_request* request);
/* футкция плагина обработки гет запроса*/
http_response* handle_get_request_plagin(http_request* request); 

/* функция получает распарсенный ответ и клиенский дескриптор
 * и отправляет ответ клиенту */
void send_response_from_server(int client_socket, http_response* response);

/* функция освобождает структуру http_request */
void free_http_request(http_request** request);

/* функция освобождает структуру http_response */
void free_http_response(http_response** response);

/////////////////////////////////////////////////////////////////////////////
/* вызов и работа с плагином */

/* функция имитирует подключения плагина */
http_response* plugin_processing_request(http_request* request);

http_response* run_plugin(http_request* request);
#endif

