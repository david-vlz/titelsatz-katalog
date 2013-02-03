#include <iostream>

using namespace std;

#include <string.h>

#include "category.h"

ClCategory::ClCategory()
{
    number = -1;
    partNumber = -1;
    values = NULL;
    valueCount = 0;
    maxValues = 1;
    multipleValuesPossible = false;
    next = NULL;
}

ClCategory::ClCategory(int initNumber)
{
    number = initNumber;
    partNumber = -1;
    values = NULL;
    valueCount = 0;
    maxValues = 1;
    multipleValuesPossible = false;
    next = NULL;
}

void ClCategory::setValue(char *value)
{
    if (valueCount == 0) {
        values = new char*[1];
        values[valueCount++] = value;
    } else {
        values[0] = value;
    }
}

void ClCategory::addValue(char *value)
{
    multipleValuesPossible = true;
    if (valueCount == 0) {
        setValue(value);
    } else {
        allocValueSpace(1);
        values[valueCount++] = value;
    }
}

char *ClCategory::find(char *value)
{
    for (int i = 0; i < valueCount; i++) {
        if (strstr(values[i], value)) {
            return values[i];
        }
    }
    return NULL;
}

void ClCategory::allocValueSpace(int sizeToAdd)
{
    bool needToReallocate = false;
    while ((valueCount + sizeToAdd) > maxValues) {
        maxValues *= 2;
        needToReallocate = true;
    }
    if (needToReallocate) {
        char **newValues = new char*[maxValues];
        for (int i = 0; i < valueCount; i++) {
            newValues[i] = values[i];
        }
        delete values;
        values = newValues;
    }
}

void ClCategory::print()
{
    if (multipleValuesPossible) {
        for (int i = 0; i < valueCount; i++) {
            printAsTriple(number);
            cout << '.';
            printAsTriple(i+1);
            cout << ':' << values[i] << endl;
        }
    } else {
        printAsTriple(number);
        cout << ':' << values[0] << endl;
    }
}

void ClCategory::printAsTriple(int i)
{
    if (i == 0) {
        cout << "000";
        return;
    }
    for (int j = i; j < 100; j *= 10) {
        cout << '0';
    }
    cout << i;
}
