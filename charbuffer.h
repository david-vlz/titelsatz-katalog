#ifndef CHARBUFFER_H
#define CHARBUFFER_H

class ClCharBuffer
{
public:
    ClCharBuffer(int initSize);
    ~ClCharBuffer() { delete buffer; }

    void push(char c);

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

    void readInput(char stopCharacter);

private:
    char *buffer;
    int counter;
    int maxSize;
};

#endif // CHARBUFFER_H
