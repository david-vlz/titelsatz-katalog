#include <iostream>

using namespace std;

#include <cstdlib>
#include <fstream>
#include <string.h>

#include "catalogue.h"
#include "charbuffer.h"


ClCatalogue::ClCatalogue()
{
    start = NULL;
    query = NULL;
}

void ClCatalogue::load(ifstream &file)
{
    char symbol;
    ClCharBuffer *buffer = new ClCharBuffer(81);

    enum datasetStatus datasetStatus = outside;
    enum categoryStatus categoryStatus = number;
    bool readingMultiValuedCategory = false;

    int categoryNumber = 0;
    char *categoryValue;

    ClCategory *firstCategory = NULL;
    ClCategory *category = NULL;
    ClCategory *nextCategory = NULL;

    ClDataset *firstDataset = NULL;
    ClDataset *dataset = NULL;
    ClDataset *nextDataset = NULL;

    for (file.get(symbol); !file.eof(); file.get(symbol)) {

        switch(symbol) {

        case ':':

            // Datensatzbeginn erkennen und Datensatz initialisieren
            if ((datasetStatus == outside) && (!strcmp(buffer->getString(), "000"))) {
                if (firstDataset == NULL) {
                    firstDataset = new ClDataset;
                    dataset = firstDataset;
                } else {
                    nextDataset = new ClDataset;
                    dataset->setNext(nextDataset);
                    dataset = dataset->getNext();
                }
                datasetStatus = inside;
            }

            // Nichts unternehmen, wenn keine Kategorienummer erwartetet wird
            if (categoryStatus == value) {
                buffer->push(symbol);
                break;
            }

            if (datasetStatus == inside) {
                // Datensatzende behandeln
                if (!strcmp(buffer->getString(), "999")) {
                    firstCategory = NULL;
                    datasetStatus = outside;
                    categoryStatus = number;
                }
                // Erweiterung einer Kategorienummer markieren
                else if (categoryStatus == partNumber) {
                    categoryStatus = value;
                }
                // Bei normaler Kategorienummer, Kategorie initialisieren
                else {
                    categoryNumber = atoi(buffer->getString());
                    if (firstCategory == NULL) {
                        firstCategory = new ClCategory(categoryNumber);
                        category = firstCategory;
                        dataset->setFirstCategory(firstCategory);
                    } else {
                        nextCategory = new ClCategory(categoryNumber);
                        category->setNext(nextCategory);
                        category = category->getNext();
                    }
                    readingMultiValuedCategory = false;
                    categoryStatus = value;
                }
            }
            buffer->reset();
            break;

        case '.':
            // Kategorie initialisieren, wenn neue Kategorienummer erwartet wird
            // und alte Kategorie nicht die gleiche Nummer trägt wie die gelesene
            categoryNumber = atoi(buffer->getString());
            if (categoryStatus == number) {
                if (categoryNumber != category->getNumber()) {
                    nextCategory = new ClCategory(categoryNumber);
                    category->setNext(nextCategory);
                    category = category->getNext();
                }
                readingMultiValuedCategory = true;
                categoryStatus = partNumber;
                buffer->reset();
            }
            // wenn keine Kategorienummer erwartet wird '.' in den Buffer schreiben
            else {
                buffer->push(symbol);
            }
            break;

        case '\r':
            break;

        case '\n':
            if (datasetStatus == outside) {
                buffer->reset();
                 // TODO: Hier auf fehlerhafte Eingaben zwischen Datensätzen prüfen
                break;
            }

            if (categoryStatus == value) {
                // Kategoriewert setzen, wenn die Datei nach dem Zeilenumbruch
                // durch eine Kategorinummer fortgesetzt wird
                // Nichts unternehmen, wenn die Zeile einen Informationsbereich fortsetzt
                if (isAtCategoryLineStart(file)) {
                    categoryValue = new char[strlen(buffer->getString())+1];
                    strcpy(categoryValue, buffer->getString());
                    if (readingMultiValuedCategory) {
                        category->addValue(categoryValue);
                    } else {
                        category->setValue(categoryValue);
                    }
                    categoryStatus = number;
                    buffer->reset();
                }
            }
            break;


        default:
            buffer->push(symbol);
            break;

        }
    }
    start = firstDataset;
}

int ClCatalogue::queryDialogue()
{
    char queryStatusInput;
    char conditonStatusInput;
    int conditionCount = 0;
    bool newCondition = true;
    int number;
    char *value;
    query = new ClQuery(start);
    char queryConnectInput;

    cout << "Moechten sie eine Abfrage starten? [j/n]: ";
    cin >> queryStatusInput;

    if (queryStatusInput == 'j') {

        while (newCondition) {
            cout << "~~ Bedingung " << ++conditionCount << " ~~" << endl;
            cout << "Suche nach Kategorie-Nr.: ";
            cin >> number;
            cout << "mit Text: ";
            value = new char[128];
            cin >> value;
            cout << "Möchten sie eine weitere Bedingung hinzufuegen? [j/n]: ";
            cin >> conditonStatusInput;
            if (conditonStatusInput == 'n') {
                newCondition = false;
            } else if (!(conditonStatusInput == 'j')) {
                // TODO
                return 1;
            }
            query->addQuery(number, value);
        }

        if (conditionCount > 1) {
            cout << "Wie moechten sie die Bedingungen verknuepfen? [u,und,UND/o,oder,ODER]" << endl;
            cin >> queryConnectInput;
            if (queryConnectInput == 'o') {
                query->setConnector(orConnect);
            } else if (queryConnectInput == 'u') {
                query->setConnector(andConnect);
            } else {
                //TODO
            }
        }

        query->execute();
        query->toStream(cout);

        return 1;
    } else if (queryStatusInput == 'n'){
        return 0;
    } else {
        //TODO
        return 1;
    }

}

bool ClCatalogue::isAtCategoryLineStart(ifstream &file)
{
    bool result = true;
    char symbol;
    int count;
    char linestart[3];
    for (file.get(symbol), count = 0; count < 3; count++, file.get(symbol)) {
        linestart[count] = symbol;
        if (!isdigit(symbol)) {
            result = false;
            break;
        }
    }
    if (result && (symbol != '.') && (symbol != ':')) {
        result = false;
    }
    file.putback(symbol);
    for (count--; count >= 0; count--) {
        file.putback(linestart[count]);
    }
    return result;
}



