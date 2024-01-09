#ifndef _CONTENTTYPE_BY_EXTENSION
#define _CONTENTTYPE_BY_EXTENSION 1

/**
 *  contenttype_by_extension - получание заголовка http "Content-Type"
 *
 *  filename:       Имя файла с расширением. Тип файла определяется по расширению.
 *  type_field:     Указатель на поле для заголовка, что будет заполненно в результате
 *                  работы функции.
 *
 *  Функция поддерживает определение html, css, png и jpeg.
 *
 *  Возвращает 0 при успешном определении. Если встречается неподдерживаемый или
 *  неизвестный тип, возвращает -1.
*/
int contenttype_by_extension(
    char *filename,
    char *type_field);

#endif
