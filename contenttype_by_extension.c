#include <string.h>
#include <stdbool.h>

int contenttype_by_extension(char *filename, char *type_field)
{
    char tmp[50] = {};
    char extension[5] = {};
    short extcounter = 0;
    bool isExtension = false;

    strcpy(tmp, filename);
    
    //  Счёт начинается с 1, чтобы запись "./файл.расшрирение" была принята корректно
    for (int i = 1; i < strlen(tmp); i++)
    {
        if (isExtension) 
        {
            extension[extcounter] = tmp[i];
            extcounter++;
        }

        if (tmp[i] == '.') 
        {
            isExtension = true;
        }
    }

    if (strcmp(extension, "html") == 0)
    {
        strcpy(type_field, "text/html");
    }
    else if (strcmp(extension, "css") == 0)
    {
        strcpy(type_field, "text/css");
    }
    else if (strcmp(extension, "png") == 0)
    {
        strcpy(type_field, "image/png");
    }
    else if (strcmp(extension, "jpeg") == 0)
    {
        strcpy(type_field, "image/jpeg");
    }
    else
    {
        return -1;
    }

    return 0;
}
