//
// Created by Salvatore Rivieccio on 06/10/16.
//
#include "util.h"

char* m_strcat( char* dest, char* src )
{
    while (*dest) dest++;
    while ((*dest++ = *src++));
    return --dest;
}