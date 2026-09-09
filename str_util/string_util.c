#include "string_util.h"
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

char* str_concat(const char* str1, const char* str2)
{
    // Handle NULL strings
    if (str1 == NULL) str1 = "";
    if (str2 == NULL) str2 = "";

    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);
    size_t total_len = len1 + len2 + 1;  // +1 for null terminator

    char* result = malloc(total_len);
    if (result == NULL) {
        return NULL;
    }

    strcpy(result, str1);
    strcat(result, str2);

    return result;
}

char* str_concat3(const char* str1, const char* str2, const char* str3)
{
    // Handle NULL strings
    if (str1 == NULL) str1 = "";
    if (str2 == NULL) str2 = "";
    if (str3 == NULL) str3 = "";

    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);
    size_t len3 = strlen(str3);
    size_t total_len = len1 + len2 + len3 + 1;

    char* result = malloc(total_len);
    if (result == NULL) {
        return NULL;
    }

    strcpy(result, str1);
    strcat(result, str2);
    strcat(result, str3);

    return result;
}

char* str_concat_multi(const char* str1, ...)
{
    va_list args;
    const char* current;
    size_t total_len = 0;
    char* result;
    char* result_ptr;

    // Calculate total length needed
    va_start(args, str1);
    current = str1;
    while (current != NULL) {
        total_len += strlen(current);
        current = va_arg(args, const char*);
    }
    va_end(args);

    // Allocate memory
    result = malloc(total_len + 1);
    if (result == NULL) {
        return NULL;
    }

    // Concatenate all strings
    result_ptr = result;
    va_start(args, str1);
    current = str1;
    while (current != NULL) {
        size_t len = strlen(current);
        memcpy(result_ptr, current, len);
        result_ptr += len;
        current = va_arg(args, const char*);
    }
    va_end(args);

    *result_ptr = '\0';

    return result;
}

char* str_append(char* str1, const char* str2)
{
    char* result;

    if (str2 == NULL) {
        str2 = "";
    }

    if (str1 == NULL) {
        result = malloc(strlen(str2) + 1);
        if (result == NULL) {
            return NULL;
        }
        strcpy(result, str2);
        return result;
    }

    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);

    result = malloc(len1 + len2 + 1);
    if (result == NULL) {
        free(str1);
        return NULL;
    }

    strcpy(result, str1);
    strcat(result, str2);
    free(str1);

    return result;
}
