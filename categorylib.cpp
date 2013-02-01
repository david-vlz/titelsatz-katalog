#include <iostream>

using namespace std;

#include "category.h"

ClCategory::ClCategory()
{
    number = -1;
    partNumber = -1;
    values = NULL;
    valueCount = 0;
    maxValues = 1;
    next = NULL;
}

void ClCategory::setValue(char *value)
{
    if (values == NULL) {
        values = new char*[1];
        values[0] = value;
    } else {
        values[0] = value;
    }
}

void ClCategory::addValue(char *value)
{
    if (values == NULL) {
        setValue(value);
    } else {
        if
    }
}
