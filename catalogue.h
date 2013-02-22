#ifndef CATALOGUE_H
#define CATALOGUE_H

#include "dataset.h"
#include "query.h"
#include "charbuffer.h"

enum datasetStatus { inside, outside };
enum categoryStatus { number, partNumber, value };

class ClCatalogue
{
public:
    ClCatalogue();
    ~ClCatalogue();

    void load(ifstream &file);

    int queryDialogue();

    void print() { base->printAll(); }

private:
    ClDataset *base;
    ClQuery *query;

    bool isAtCategoryLineStart(ifstream &file);

    char *getInput(ClCharBuffer *buffer);
    void copyInput(ClCharBuffer *buffer, char *destination);

    int isValidCategoryNumber(char *number);
    int isValidYesNoInput(char *input);
    connector isValidLogicalConnectorInput(char *input);

};

#endif // CATALOGUE_H
