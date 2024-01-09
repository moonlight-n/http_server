#include <malloc.h>
#include <string.h>
#include "../headers/post_data.h"

void clear_tmp(
    char *tmp,
    int *counter);

post_data *parse_post(char *buf, int *count)
{
    post_data *data = calloc(1, sizeof(post_data));
    int data_counter = 0;

    char tmp[100] = {};
    int tmp_counter = 0;

    for (int i = 0; i <= strlen(buf); i++)
    {
        if (buf[i] == '=')
        {
            data[data_counter].key = calloc(1, tmp_counter + 1);
            strncpy(data[data_counter].key, tmp, tmp_counter);
            clear_tmp(tmp, &tmp_counter);
            continue;
        }

        if (buf[i] == '&')
        {
            data[data_counter].value = calloc(1, tmp_counter + 1);
            strncpy(data[data_counter].value, tmp, tmp_counter);
            data_counter++;
            data = realloc(data, (sizeof(post_data) * (data_counter + 1)));
            clear_tmp(tmp, &tmp_counter);
            continue;
        }

        if (i == strlen(buf))
        {
            data[data_counter].value = calloc(1, tmp_counter + 1);
            strncpy(data[data_counter].value, tmp, tmp_counter);
            clear_tmp(tmp, &tmp_counter);
            data_counter++;
            break;
        }

        tmp[tmp_counter] = buf[i];
        tmp_counter++;
    }

    *count = data_counter;
    return data;
}

void clear_tmp(char *tmp, int *counter)
{
    for (int i = 0; i < 100; i++)
    {
        tmp[i] = 0;
    }

    *counter = 0;
}
