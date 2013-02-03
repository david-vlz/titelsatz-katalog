#include <iostream>

using namespace std;

#include <string.h>

#include "category.h"
#include "dataset.h"
#include "query.h"

ClQuery::ClQuery(ClDataset *data)
{
    base = data;
    firstQuery = NULL;
    nextQuery = NULL;
    lastQuery = NULL;
    connector = andConnect;
    results = NULL;
    resultCount = 0;
    maxResults = 4;
}

void ClQuery::addQuery(int number, char *value)
{
    if (firstQuery == NULL) {
        firstQuery = new ClCategory(number);
        lastQuery =  firstQuery;
    } else {
        nextQuery = new ClCategory(number);
        lastQuery->setNext(nextQuery);
        lastQuery = lastQuery->getNext();
    }
    lastQuery->setValue(value);
}


ClDataset** ClQuery::execute()
{
    ClCategory *possibleMatch;
    bool allConditionsMet = true;

    for (ClDataset *dataset = base; dataset != NULL; dataset = dataset->getNext()) {

        for (ClCategory *query = firstQuery; query != NULL; query = query->getNext()) {

            // Nummer abfragen
            possibleMatch = dataset->findCategory(query->getNumber());
            if (possibleMatch != NULL) {
                // Text abfragen
                if (possibleMatch->find(query->getValue()) != NULL) {
                    if (connector == orConnect) {
                        addToResults(dataset);
                        break;
                    }
                } else if (connector == andConnect) {
                    allConditionsMet = false;
                    break;
                }
            } else if (connector == andConnect) {
                allConditionsMet = false;
                break;
            }
        }

        if ((connector == andConnect) && allConditionsMet) {
            addToResults(dataset);
        }
    }
}


void ClQuery::toStream(ostream &stream)
{
    stream << endl << "=== Abfrage ===" << endl;
    bool firstIteration = true;
    for (ClCategory *query = firstQuery; query != NULL; query = query->getNext()) {
        if (!firstIteration) {
            if (connector == andConnect) {
                stream << " UND";
            } else if (connector == orConnect) {
                stream << " ODER";
            }
            stream << endl;
        } else {
            firstIteration = false;
        }
        stream << "Kategorie "
               << query->getNumber()
               << " mit Text \'"
               << query->getValue()
               << '\'';
    }
    stream << endl << endl;
    if (results != NULL) {
        stream << "=== Ergebnisse ===" << endl;
        for (int i = 0; i < resultCount; i++) {
            stream << "* Ergbnis [" << i+1 << "] *" << endl;
            results[i]->toStream(stream);
        }
    }
}



void ClQuery::addToResults(ClDataset *positiveMatch)
{
    if (results == NULL) {
        results = new ClDataset*[maxResults];
    }
    if (resultCount == maxResults) {
        maxResults *= 2;
        cout << "new max: " << maxResults << endl;
        ClDataset **newResults = new ClDataset*[maxResults];
        for (int i = 0; i < resultCount; i++) {
            newResults[i] = results[i];
        }
        delete results;
        results = newResults;
    }
    results[resultCount++] = positiveMatch;
}
