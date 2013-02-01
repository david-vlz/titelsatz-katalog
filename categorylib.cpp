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
        cout << "vc: " << valueCount << endl;
        values[valueCount++] = value;
    }
}

void ClCategory::allocValueSpace(int size)
{
    while ((valueCount + size) > maxValues) {
        maxValues *= 2;
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
//            cout << number << "." << i+1 << ":" << values[i] << endl;
            printAsTriple(number);
            cout << '.';
            printAsTriple(i+1);
            cout << ':' << values[i] << endl;
        }
    } else {
        cout << number << ":" << values[0] << endl;
    }
}

void ClCategory::printAsTriple(int i)
{
    for (int j = i; j < 100; j *= 10) {
        cout << '0';
    }
    cout << i;
}
