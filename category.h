#ifndef CATEGORY_H
#define CATEGORY_H

enum matchingStatus { full, partialText, none };

class ClCategory
{
public:
    ClCategory();
    ClCategory(int initNumber);
    ~ClCategory();

    void setNumber(int i) { number = i; }
    int getNumber() { return number; }

    void setValue(char *value);
    void addValue(char *value);

    char *getValue(int index) { return values[index]; }
    char *getValue() { return values[0]; }
    char **getValues() { return values; }

    void setNext(ClCategory *category) { next = category; }
    ClCategory *getNext() { return next; }

    char *find(char *text);

    void toStream(ostream &stream);

    void print() { toStream(cout); }

private:
    int number;
    int partNumber;

    char **values;
    int valueCount;
    int maxValues;
    bool multipleValuesPossible;
    void reallocateValueSpace(int sizeToAdd);

    ClCategory *next;

    void numberAsTripleToStream(int i, ostream &stream);
    void valueToStream(char *value, ostream &stream);
};

#endif // CATEGORY_H
