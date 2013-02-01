#ifndef CATEGORY_H
#define CATEGORY_H

class ClCategory
{
public:
    ClCategory();

    void setValue(char *value);
    void addValue(char *value);

    char *getValue() { return values[0]; }
    char **getValues() { return values; }

private:
    int number;
    int partNumber;
    char **values;
    int valueCount;
    int maxValues;
    ClCategory *next;
};

#endif // CATEGORY_H
