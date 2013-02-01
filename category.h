#ifndef CATEGORY_H
#define CATEGORY_H

class ClCategory
{
public:
    ClCategory();
    ClCategory(int initNumber);

    void setNumber(int i) { number = i; }
    int getNumber() { return number; }

    void setValue(char *value);
    void addValue(char *value);

    char *getValue(int index) { return values[index]; }
    char *getValue() { return values[0]; }
    char **getValues() { return values; }

    void setNext(ClCategory *category) { next = category; }
    ClCategory *getNext() { return next; }

    void print();

private:
    int number;
    int partNumber;

    char **values;
    int valueCount;
    int maxValues;
    bool multipleValuesPossible;
    void allocValueSpace(int size);

    ClCategory *next;

    void printAsTriple(int i);
};

#endif // CATEGORY_H
