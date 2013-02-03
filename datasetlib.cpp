#include <iostream>

using namespace std;

#include <string.h>

#include "dataset.h"
#include "category.h"


ClDataset::ClDataset()
{
    firstCategory = NULL;
    next = NULL;
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


