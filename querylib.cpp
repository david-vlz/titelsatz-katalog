#include <iostream>

using namespace std;

#include "category.h"
#include "dataset.h"
#include "query.h"

ClQuery::ClQuery(ClDataset *data)
{
    base = data;
    firstQuery = NULL;
    connector = andConnect;
    results = NULL;
}












void ClQuery::toStream(ostream &stream)
{
    stream << "=== Abfrage ===" << endl;
    bool firstIteration = true;
    for (ClCategory *query = firstQuery; query != NULL; query = query->getNext()) {
        if (!firstIteration) {
            if (connector == andConnect) {
                stream << " UND";
            } else if (connector == orConnect) {
                stream << " ODER";
            }
            stream << endl;
        }
        stream << "Kategorie Nr. "
               << query->getNumber()
               << "enthält "
               << query->getValue();
    }
}
