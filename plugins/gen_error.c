#include "../headers/gen_error.h"
#include "../headers/main.h"
#include "../headers/properties.h"

void error_processing(int code_error, http_response *response) {
  response->body = calloc(1, MAX_BODY_SIZE);
  response->status_code = code_error;

  switch (code_error) {
    case CODE_400:
      gen_error_400(response);
      break;
    case CODE_401:
      gen_error_401(response);
      break;
    case CODE_403:
      gen_error_403(response);
      break;
    case CODE_404:
      gen_error_404(response);
      break;
    case CODE_500:
      gen_error_500(response);
      break;
    case CODE_505:
      gen_error_505(response);
      break;
  }
}

void code_200(http_response *response, int content_length) {
  response->status_code = 200;
  response->num_headers = 4;
  response->headers[0] = calloc(1, 100);
  response->headers[1] = calloc(1, 100);
  response->headers[2] = calloc(1, 100);
  response->headers[3] = calloc(1, 100);

  sprintf(response->headers[0], "HTTP/1.1 %d OK", response->status_code);
  sprintf(response->headers[1], "Content-Type: text/html");
  sprintf(response->headers[2], "Content-Length: %d", content_length);
  sprintf(response->headers[3], "Connection: close");
}

void gen_error_400(http_response *response) {
  response->headers[0] = calloc(1, 100);
  response->headers[1] = calloc(1, 100);
  response->status_code = 400;
  response->num_headers = 2;
  sprintf(response->headers[0], "HTTP/1.1 %d Bad Request",
          response->status_code);
  sprintf(response->headers[1], "Connection: close");
  sprintf(response->body,
          "<html><head><title></title>400 Bad Request</head></html>");
}

void gen_error_401(http_response *response) {
  response->headers[0] = calloc(1, 100);
  response->headers[1] = calloc(1, 100);
  response->status_code = 401;
  response->num_headers = 2;
  sprintf(response->headers[0], "HTTP/1.1 %d Unauthorized",
          response->status_code);
  sprintf(response->headers[1], "Connection: close");
  sprintf(response->body,
          "<html><head><title></title>401 Unauthorized</head></html>");
}

void gen_error_403(http_response *response) {
  response->headers[0] = calloc(1, 100);
  response->headers[1] = calloc(1, 100);
  response->status_code = 403;
  response->num_headers = 2;
  sprintf(response->headers[0], "HTTP/1.1 %d Forbidden", response->status_code);
  sprintf(response->headers[1], "Connection: close");
  sprintf(response->body,
          "<html><head><title></title>403 Forbidden</head></html>");
}

void gen_error_404(http_response *response) {
  response->headers[0] = calloc(1, 100);
  response->headers[1] = calloc(1, 100);
  response->status_code = 404;
  response->num_headers = 2;
  sprintf(response->headers[0], "HTTP/1.1 %d File not found",
          response->status_code);
  sprintf(response->headers[1], "Connection: close");
  sprintf(response->body,
          "<html><head><title></title>404 Page not found</head></html>");
}

void gen_error_500(http_response *response) {
  response->headers[0] = calloc(1, 100);
  response->headers[1] = calloc(1, 100);
  response->status_code = 500;
  response->num_headers = 2;
  sprintf(response->headers[0], "HTTP/1.1 %d Internal Server Error",
          response->status_code);
  sprintf(response->headers[1], "Connection: close");
  sprintf(response->body,
          "<html><head><title></title>500 Internal Server Error</head></html>");
}

void gen_error_505(http_response *response) {
  response->headers[0] = calloc(1, 100);
  response->headers[1] = calloc(1, 100);
  response->status_code = 505;
  response->num_headers = 2;
  sprintf(response->headers[0], "HTTP/1.1 %d  Version Not Supported",
          response->status_code);
  sprintf(response->headers[1], "Connection: close");
  sprintf(
      response->body,
      "<html><head><title></title>505  Version Not Supported</head></html>");
}
