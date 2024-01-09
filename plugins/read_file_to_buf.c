#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "../headers/properties.h"

int read_file_to_buf(char *filename, char *buf)
{
    int fd = open(filename, O_RDONLY);
    if (fd == -1)
    {
        return -1;
    }

    int readed = 0;
    int countOfBytes = 0;
    while (1)
    {
        readed = read(fd, buf, MAX_BODY_SIZE);
        countOfBytes += readed;
        if (readed <= 0)
        {
            break;
        }
    }

    return countOfBytes;
}
