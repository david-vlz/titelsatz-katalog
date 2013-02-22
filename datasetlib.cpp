#include <iostream>

using namespace std;

#include <istream>
#include <string.h>

#include "dataset.h"
#include "category.h"


ClDataset::ClDataset()
{
    firstCategory = NULL;
    next = NULL;
}

ClDataset::~ClDataset()
{
    ClCategory *categoryOne = firstCategory;
    ClCategory *categoryTwo;
    if (categoryOne != NULL) {
        categoryTwo = categoryOne->getNext();
        delete categoryOne;
        while (categoryTwo != NULL) {
            categoryOne = categoryTwo;
            categoryTwo = categoryOne->getNext();
            delete categoryOne;
        }
    }
}

ClCategory *ClDataset::findCategory(int number)
{
    for (ClCategory *cat = firstCategory; cat != NULL; cat = cat->getNext()) {
        if (cat->getNumber() == number) {
            return cat;
        }
    }
    return NULL;
}

ClCategory *ClDataset::findCategory(char *value)
{
    for (ClCategory *cat = firstCategory; cat != NULL; cat = cat->getNext()) {
        if (cat->find(value) != NULL) {
            return cat;
        }
    }
    return NULL;
}

void ClDataset::toStream(ostream &stream)
{
    for (ClCategory *cat = firstCategory; cat != NULL; cat = cat->getNext()) {
        cat->toStream(stream);
    }
    stream << endl << endl;
//    stream << "999:" << endl << endl;
}

void ClDataset::allToStream(ostream &stream)
{
    for (ClDataset *dataset = this; dataset != NULL; dataset = dataset->getNext()) {
        dataset->toStream(stream);
    }
}
