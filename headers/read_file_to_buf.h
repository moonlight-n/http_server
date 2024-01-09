#ifndef _READ_FILE_TO_BUF
#define _READ_FILE_TO_BUF 1

/**
 *  read_file_to_buf - читает файл в линейный буффер
 *
 *  filename:   Имя(путь) файла.
 *  buf:        Буфер, что будет заполнен.
*/
int read_file_to_buf(
    char *filename,
    char *buf);

#endif
