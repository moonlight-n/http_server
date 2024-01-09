#ifndef GEN_ERROR
#define GEN_ERROR 1
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "../headers/interface.h"

enum {
  CODE_OK_200 = 200,
  CODE_400 = 400,
  CODE_401 = 401,
  CODE_403 = 403,
  CODE_404 = 404,
  CODE_500 = 500,
  CODE_505 = 505
};

void error_processing(int code_error, http_response *response);
void code_200(http_response *response, int content_length);
void gen_error_400(http_response *response);
void gen_error_401(http_response *response);
void gen_error_403(http_response *response);
void gen_error_404(http_response *response);
void gen_error_500(http_response *response);
void gen_error_505(http_response *response);

#endif
