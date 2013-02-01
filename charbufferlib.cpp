#include <iostream>

using namespace std;

#include <string.h>

#include "charbuffer.h"


ClCharBuffer::ClCharBuffer(int initSize)
{
    maxSize = initSize;
    buffer = new char[initSize];
    buffer[0] = '\0';
    counter = 0;
}

void ClCharBuffer::push(char c)
{
    buffer[counter++] = c;
    if (counter == maxSize) {
        maxSize *= 2;
        char *newBuffer = new char[maxSize];
        strcpy(newBuffer, getString());
        delete buffer;
        buffer = newBuffer;
    }
}
