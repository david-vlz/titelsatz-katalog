#ifndef CATALOGUE_H
#define CATALOGUE_H

#include "dataset.h"
#include "query.h"

enum datasetStatus { inside, outside };
enum categoryStatus { number, partNumber, value };

class ClCatalogue
{
public:
    ClCatalogue();

    void load(ifstream &file);

    int queryDialogue();

    ClDataset **search(int number, char *value);

    void print() { start->printAll(); }

    // DEBUG
    ClDataset *getStart() { return start; }

private:
    ClDataset *start;

    ClQuery *query;

    bool isAtCategoryLineStart(ifstream &file);
};

#endif // CATALOGUE_H
