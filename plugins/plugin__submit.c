#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../headers/post_data.h"
#include "../headers/parse_post.h"
#include "../headers/interface.h"
#include "../headers/properties.h"
#include "../headers/read_file_to_buf.h"
#include "../headers/gen_error.h"

http_response* _submit_GET(http_request *request)
{
    http_response *response = calloc(1, sizeof(http_response));
    response->body = calloc(1, MAX_BODY_SIZE);
    
    int result = read_file_to_buf("./pages/submit.html", response->body);
    if (result == -1)
    {
        error_processing(404, response);
        return response;
    }

    code_200(response, result);
    return response;
}

http_response *_submit_POST(http_request *request)
{
    http_response *response = calloc(1, sizeof(http_response));
    response->body = calloc(1, MAX_BODY_SIZE);

    int count = 0;
    post_data *data = parse_post(request->body, &count); 
    char *login;
    char *password;
    int isAccess = 0;

    //  Поиск переменных в пришедших от клиента данных POST и работа с ними
    for (int i = 0; i < count; i++)
    {
        if (strcmp(data[i].key, "login") == 0)
        {
            login = data[i].value;
        }

        if (strcmp(data[i].key, "password") == 0)
        {
            password = data[i].value;
        }

        if (strcmp(data[i].key, "personal") == 0)
        {
            isAccess = 1;
        }
    }


    int fd = open("./db.txt", O_RDWR | O_CREAT | O_APPEND, S_IRWXU | S_IRWXG | S_IRWXO);
    if (fd != -1)
    {
        write(fd, login, strlen(login));
        write(fd, " ", 1);
        write(fd, password, strlen(password));
        write(fd, " ", 1);
        if (isAccess == 1)
        {
            write(fd, "1", 1);
        }
        else
        {
            write(fd, "0", 1);
        }
        write(fd, "\n", 1);
        close(fd);

        int result = read_file_to_buf("./pages/submit.html", response->body);
        if (result == -1)
        {
            error_processing(404, response);
            return response;
        }

        code_200(response, result);

        /* Очистка */
        for (int i = 0; i < count; i++)
        {
            free(data[i].key);
            free(data[i].value);
        }
        free(data);
        return response;
    }
    else
    {
        /* Очистка */
        for (int i = 0; i < count; i++)
        {
            free(data[i].key);
            free(data[i].value);
        }
        free(data);

        error_processing(500, response);
        return response;
    }
}

http_response* _submit_HEAD(http_request *request)
{
    http_response *response = calloc(1, sizeof(http_response));
    response->body = calloc(1, 1);
    
    char static_buf[MAX_BODY_SIZE];
    int result = read_file_to_buf("./pages/submit.html", static_buf);
    if (result == -1)
    {
        error_processing(404, response);
        return response;
    }

    code_200(response, result);
    return response;
}
