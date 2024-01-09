#include <string.h>
#include "../headers/read_file_to_buf.h"
#include "../headers/interface.h"
#include "../headers/properties.h"
#include "../headers/gen_error.h"

http_response* _testpage_GET(http_request *request)
{
    http_response *response = calloc(1, sizeof(http_response));
    response->body = calloc(1, MAX_BODY_SIZE);
    
    int result = read_file_to_buf("./pages/testpage.html", response->body);
    if (result == -1)
    {
        error_processing(404, response);
        return response;
    }

    code_200(response, result);
    return response;
}

http_response* _testpage_HEAD(http_request *request)
{
    http_response *response = calloc(1, sizeof(http_response));
    response->body = calloc(1, 1);
    
    char static_buf[MAX_BODY_SIZE];
    int result = read_file_to_buf("./pages/testpage.html", static_buf);
    if (result == -1)
    {
        error_processing(404, response);
        return response;
    }

    code_200(response, result);
    return response;
}
