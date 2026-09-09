#ifndef STRING_UTIL_H
#define STRING_UTIL_H

#include <stddef.h>

/**
 * Concatenate two strings and return a newly allocated string.
 * The caller is responsible for freeing the returned pointer.
 *
 * @param str1 First string (can be NULL, treated as empty)
 * @param str2 Second string (can be NULL, treated as empty)
 * @return Newly allocated concatenated string, or NULL on memory error
 */
char* str_concat(const char* str1, const char* str2);

/**
 * Concatenate three strings and return a newly allocated string.
 * The caller is responsible for freeing the returned pointer.
 *
 * @param str1 First string (can be NULL)
 * @param str2 Second string (can be NULL)
 * @param str3 Third string (can be NULL)
 * @return Newly allocated concatenated string, or NULL on memory error
 */
char* str_concat3(const char* str1, const char* str2, const char* str3);

/**
 * Concatenate multiple strings and return a newly allocated string.
 * Last argument must be NULL to mark the end of the list.
 *
 * Example: str_concat_multi("Hello", " ", "World", NULL)
 *
 * @return Newly allocated concatenated string, or NULL on memory error
 */
char* str_concat_multi(const char* str1, ...);

/**
 * Append str2 to str1, freeing str1 and returning a newly allocated result.
 * Useful for building strings incrementally in a loop.
 *
 * Example:
 *   char* result = malloc(1);
 *   result[0] = '\0';
 *   result = str_append(result, "data");
 *   result = str_append(result, ",");
 *   free(result);
 *
 * @param str1 String to append to (will be freed)
 * @param str2 String to append
 * @return Newly allocated string, or NULL on memory error
 */
char* str_append(char* str1, const char* str2);

#endif
