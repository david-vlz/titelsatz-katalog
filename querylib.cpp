#include <iostream>

using namespace std;

#include <string.h>

#include "category.h"
#include "dataset.h"
#include "query.h"

ClQuery::ClQuery(ClDataset *data)
{
    base = data;
    firstCondition = NULL;
    nextCondition = NULL;
    lastCondition = NULL;
    logConn = andConnect;
    results = NULL;
    resultCount = 0;
    maxResults = 4;
}

ClQuery::~ClQuery()
{
    ClCategory *categoryOne = firstCondition;
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
    delete results;
}

void ClQuery::addCondition(int number, char *value)
{
    if (firstCondition == NULL) {
        firstCondition = new ClCategory(number);
        lastCondition =  firstCondition;
    } else {
        nextCondition = new ClCategory(number);
        lastCondition->setNext(nextCondition);
        lastCondition = lastCondition->getNext();
    }
    lastCondition->setValue(value);
}


void ClQuery::execute()
{
    ClCategory *possibleMatch;
    bool allConditionsMet = true;

    for (ClDataset *dataset = base; dataset != NULL; dataset = dataset->getNext()) {

        for (ClCategory *query = firstCondition; query != NULL; query = query->getNext()) {

            allConditionsMet = true;

            // Nummer abfragen
            possibleMatch = dataset->findCategory(query->getNumber());
            if (possibleMatch != NULL) {
                // Text abfragen
                if (possibleMatch->find(query->getValue()) != NULL) {
                    if (logConn == orConnect) {
                        addToResults(dataset);
                        break;
                    }
                } else if (logConn == andConnect) {
                    allConditionsMet = false;
                    break;
                }
            } else if (logConn == andConnect) {
                allConditionsMet = false;
                break;
            }
        }

        if ((logConn == andConnect) && allConditionsMet) {
            addToResults(dataset);
        }
    }
}


void ClQuery::toStream(ostream &stream)
{
    stream << endl << "=== Abfrage ===" << endl;
    bool firstIteration = true;
    for (ClCategory *query = firstCondition; query != NULL; query = query->getNext()) {
        if (!firstIteration) {
            if (logConn == andConnect) {
                stream << " UND";
            } else if (logConn == orConnect) {
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
            stream << "Ergbnis [" << i+1 << "]" << endl;
            stream << "-----------" << endl;
            results[i]->toStream(stream);
        }
    } else {
        stream << "Die Abfrage erbrachte keine Ergebnisse." << endl;
    }
}



void ClQuery::addToResults(ClDataset *positiveMatch)
{
    if (results == NULL) {
        results = new ClDataset*[maxResults];
    }
    if (resultCount == maxResults) {
        maxResults *= 2;
        ClDataset **newResults = new ClDataset*[maxResults];
        for (int i = 0; i < resultCount; i++) {
            newResults[i] = results[i];
        }
        delete results;
        results = newResults;
    }
    results[resultCount++] = positiveMatch;
}
