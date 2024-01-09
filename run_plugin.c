#include <dlfcn.h>
#include "headers/interface.h"
#include "headers/read_file_to_buf.h"
#include "headers/contenttype_by_extension.h"
#include "headers/properties.h"

/* Замена символов '/' на '_', чтобы не было конфликтов с файловой системой */
void replace(
    char *str,
    char *newstr);

/* Оставляет str без данных, передаваемых со строкой get */
void cut_path_from_get_url(char *str);

/* Процедура отправки файлов в обход плагина, если таковой не предусмотрен */
http_response *basic_GET(http_request *request);

http_response *run_plugin(http_request *request)
{
    char replacedPath[100] = {};
    replace(request->path, replacedPath);

    //  Поиск библиотеки
    char nameOfPlugin[100] = {};
    strcat(nameOfPlugin, "./plugins/plugin_");
    strcat(nameOfPlugin, replacedPath);
    strcat(nameOfPlugin, ".so");

    void *lib = dlopen(nameOfPlugin, RTLD_NOW);
    if (lib == 0)
    {
        if (strcmp(request->method, "GET") == 0)
        {
            return basic_GET(request);
        }

        http_response *response = calloc(1, sizeof(http_response));
        response->status_code = 404;
        response->body = calloc(1, 1);
        response->headers[0] = calloc(1, 100);
        response->headers[1] = calloc(1, 100);
        response->num_headers = 2;
        sprintf(response->headers[0], "HTTP/1.1 %d File not found", response->status_code);
        sprintf(response->headers[1], "Connection: close");
        return response;
    }

    //  Поиск обработчика метода
    char nameOfHandler[100] = {};
    strcat(nameOfHandler, replacedPath);
    strcat(nameOfHandler, "_");
    strcat(nameOfHandler, request->method);

    http_response *(*handler)(http_request *request) = dlsym(lib, nameOfHandler);
    if (handler != 0)
    {
        return handler(request);
    }

    //  Если метод не найден, выбрасываем код not implemented
    http_response *response = calloc(1, sizeof(http_response));

    /**
     *  Откровенный костыль. Body отправляется, даже при его отсутствии, и чтобы сервер
     *  не падал, сделано вот это непотребство.
     *  Требует переделки
    */
    response->body = calloc(1, 1);

    response->status_code = 501;
    response->headers[0] = calloc(1, 100);
    response->headers[1] = calloc(1, 100);
    response->num_headers = 2;
    sprintf(response->headers[0], "HTTP/1.1 %d Not implemented", response->status_code);
    sprintf(response->headers[1], "Connection: close");
    return response;
}

http_response *basic_GET(http_request *request)
{
    http_response *response = calloc(1, sizeof(http_response));
    response->body = calloc(1, MAX_BODY_SIZE);
    response->headers[0] = calloc(1, 100);
    response->headers[1] = calloc(1, 100);
    response->headers[2] = calloc(1, 100);
    response->headers[3] = calloc(1, 100);

    char filename[100] = {};
    strcat(filename, RESOURCE_PATH);
    strcat(filename, request->path);
    cut_path_from_get_url(filename);
    int readed_bytes = read_file_to_buf(filename, response->body);
    printf("%s\n", filename);
    if (readed_bytes == -1)
    {
        response->status_code = 404;
        response->num_headers = 2;
        sprintf(response->headers[0], "HTTP/1.1 %d File not found", response->status_code);
        sprintf(response->headers[1], "Connection: close");
        return response;
    }

    char contenttype[50] = {};
    int result = contenttype_by_extension(filename, contenttype);
    if (result != 0)
    {
        response->status_code = 404;
        response->num_headers = 2;
        sprintf(response->headers[0], "HTTP/1.1 %d File not found", response->status_code);
        sprintf(response->headers[1], "Connection: close");
        return response;
    }

    response->status_code = 200;
    response->num_headers = 4;
    sprintf(response->headers[0], "HTTP/1.1 %d OK", response->status_code);
    sprintf(response->headers[1], "Content-Type: %s", contenttype);
    sprintf(response->headers[2], "Content-Length: %d", readed_bytes);
    sprintf(response->headers[3], "Connection: close");
    return response;
}

void replace(char *str, char *newstr)
{
    for (int i = 0; i < strlen(str); i++)
    {
        //  В конечную строку не должны входить данные, передаваемые с GET-запросом
        if (str[i] == '?')
        {
            break;
        }

        if (str[i] == '/')
        {
            newstr[i] = '_';
        }
        else
        {
            newstr[i] = str[i];
        }
    }
}

void cut_path_from_get_url(char *str)
{
    char tmp[100] = {};
    for (int i = 0; i < strlen(str); i++)
    {
        //  В конечную строку не должны входить данные, передаваемые с GET-запросом
        if (str[i] == '?')
        {
            break;
        }

        tmp[i] = str[i];
    }

    strcpy(str, tmp);
}
