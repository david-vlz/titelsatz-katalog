#ifndef CHARBUFFER_H
#define CHARBUFFER_H

class ClCharBuffer
{
public:
    ClCharBuffer()
    {
        buffer[0] = '\0';
        counter = 0;
    }

    void push(char c) { buffer[counter++] = c; }

    char pop() { return buffer[--counter]; }

    void reset()
    {
        buffer[0] = '\0';
        counter = 0;
    }

    char *getString()
    {
        buffer[counter] = '\0';
        return buffer;
    }

private:
    char buffer[80];
    int counter;
};

#endif // CHARBUFFER_H
