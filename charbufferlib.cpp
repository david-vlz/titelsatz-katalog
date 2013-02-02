#include <iostream>

using namespace std;

#include <string.h>

#include "charbuffer.h"


ClCharBuffer::ClCharBuffer(int initSize)
{
    maxSize = (initSize < 2) ? 2 : initSize;
    buffer = new char[maxSize];
    buffer[0] = '\0';
    counter = 0;
}

void ClCharBuffer::push(char c)
{
    buffer[counter++] = c;
    if (counter == maxSize-1) {
        maxSize *= 2;
        char *newBuffer = new char[maxSize];
        strcpy(newBuffer, getString());
        delete buffer;
        buffer = newBuffer;
    }
}
