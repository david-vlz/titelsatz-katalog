#ifndef CHARBUFFER_H
#define CHARBUFFER_H

class ClCharBuffer
{
public:
    ClCharBuffer() { buffer = "\0"; counter = 0; }
    void push(char c) { buffer[counter++] = c; }
    char pop() { return buffer[counter--]; }
    void reset() { buffer[counter] = '\0'; counter = 0; }
    char *getString() { reset(); return buffer; }
private:
    char buffer[80];
    int counter;
};

#endif // CHARBUFFER_H
