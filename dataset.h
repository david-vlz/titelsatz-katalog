#ifndef DATASET_H
#define DATA

#include "category.h"

class ClDataset
{
public:
    ClDataset();

    void setFirstCategory(ClCategory *category) { firstCategory = category;  }
    ClCategory *getFirstCategory() { return firstCategory; }

    void setNext(ClDataset *category) { next = category; }
    ClDataset *getNext() { return next; }

    ClCategory *findCategory(int number);
    ClCategory *findCategory(char *value);

    ClDataset *searchDescendent(int number);
    ClDataset *searchDescendent(char *value);
    ClDataset *searchDescendent(int number, char *value);

    void print() { toStream(cout); }
    void printAll();

    void toStream(ostream &stream);
    void allToStream(ostream &stream);

private:
    ClCategory *firstCategory;
    ClDataset *next;
};

#endif // DATASET_H
