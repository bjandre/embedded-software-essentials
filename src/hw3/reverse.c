#include <stdlib.h>

void swap(char *a, char *b);

void swap(char *a, char *b)
{
    char tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}

char reverse(char *str, int length)
{
    if (length < 0 || str == NULL) {
        return -1;
    }
    char *begin = str;
    char *end = str + length - 1;
    while (begin < end) {
        swap(begin, end);
        begin++;
        end--;
    }
    return 0;
}

