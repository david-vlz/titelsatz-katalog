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

    void print() { start->printAll(); }

private:
    ClDataset *start;
    ClQuery *query;

    bool isAtCategoryLineStart(ifstream &file);

    int isValidCategoryNumber(char *number);
    int isValidYesNoInput(char *input);
    connector isValidLogicalConnectorInput(char *input);

};

#endif // CATALOGUE_H
