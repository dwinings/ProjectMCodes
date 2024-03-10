//
// Created by johno on 7/2/2020.
//

#include "cstring.h"


char* strcat(char* destination, const char* source) {
    return _strcat(destination, source);
}


char* strcpy(char* destination, const char* source) {
    return _strcpy(destination, source);
}

char* strncpy(char* destination, const char* source, int length) {
    return _strncpy(destination, source, length);
}

int strcmp(const char* str1, const char* str2) {
    return _strcmp(str1, str2);
}

unsigned int strlen(const char* str, unsigned int maxLen) {
    unsigned int counter = 0;
    while (*str != '\0' && counter < maxLen) {
        counter++;
        str++;
    }

    return counter + 1; // +1 for the null byte
}

char* strstr(const char* buffer, const char* target) {
    return _strstr(buffer, target);
}

int atoi(const char* buffer) {
    return _atoi(buffer);
}

float atof(const char* buffer) {
    return _atof(buffer);
}