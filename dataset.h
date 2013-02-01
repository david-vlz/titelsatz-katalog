#ifndef DATASET_H
#define DATA

#include "category.h"

class ClDataset
{
public:
    void addCategory();
private:
    ClCategory *category;
    ClDataset *next;
};

#endif // DATASET_H
