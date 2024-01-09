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

http_response* _personal_GET(http_request *request)
{
    http_response *response = calloc(1, sizeof(http_response));
    response->body = calloc(1, MAX_BODY_SIZE);
    error_processing(401, response);
    return response;
}

http_response *_personal_POST(http_request *request)
{
    http_response *response = calloc(1, sizeof(http_response));
    response->body = calloc(1, MAX_BODY_SIZE);

    int count = 0;
    post_data *data = parse_post(request->body, &count); 
    char *login;
    char *password;
    int isAccess = -1;

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
    }
    
    /* Очистка */
    for (int i = 0; i < count; i++)
    {
        free(data[i].key);
        free(data[i].value);
    }
    free(data);

    int fd = open("./db.txt", O_RDWR);
    if (fd != -1)
    {
        char static_buf[MAX_BODY_SIZE];
        if (read(fd, static_buf, MAX_BODY_SIZE) != -1)
        {
            char* data1 = static_buf;
            int offset;
            char login_from_db[32];
            char password_from_db[32];
            int access_from_db;

            while (sscanf(data1, "%s %s %d%n", login_from_db, password_from_db, 
                &access_from_db, &offset) == 3)
            {
                data1 += offset;
                if (strcmp(login_from_db, login) == 0 && 
                    strcmp(password_from_db, password) == 0)
                {
                    isAccess = access_from_db;
                    break;
                }
            }

            if (isAccess == -1)
            {
                int result = read_file_to_buf("./pages/incorrect.html", response->body);
                if (result == -1)
                {
                    error_processing(404, response);
                    return response;
                }

                code_200(response, result);
                return response;
            }

            if (isAccess == 0)
            {
                error_processing(403, response);
                return response;
            }

            if (isAccess == 1)
            {
                char *page_buff1 = calloc(1, MAX_BODY_SIZE);
                char *page_buff2 = calloc(1, MAX_BODY_SIZE);

                int result_buff1 = read_file_to_buf("./pages/personal1.html", page_buff1);
                if (result_buff1 == -1)
                {
                    error_processing(404, response);
                    return response;
                }

                int result_buff2 = read_file_to_buf("./pages/personal2.html", page_buff2);
                if (result_buff2 == -1)
                {
                    error_processing(404, response);
                    return response;
                }

                sprintf(response->body,"%s%s%s", page_buff1, login_from_db, page_buff2);
                int total_buff = result_buff1 + strlen(login) + result_buff2;

                code_200(response, total_buff);
                return response;
            }

        }
        else
        {
            error_processing(500, response);
            return response;
        }
        close(fd);
    }
    else
    {
        error_processing(500, response);
        return response;
    }
}

http_response* _personal_HEAD(http_request* request)
{
    http_response* response = calloc(1, sizeof(http_response));
    response->body = calloc(1, MAX_BODY_SIZE);
    error_processing(401, response);
    return response;
}
