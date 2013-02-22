#include <iostream>

using namespace std;

#include <cstdlib>
#include <fstream>
#include <string.h>

#include "catalogue.h"
#include "charbuffer.h"


ClCatalogue::ClCatalogue()
{
    base = NULL;
    query = NULL;
}

ClCatalogue::~ClCatalogue()
{
    ClDataset *datasetOne = base;
    ClDataset *datasetTwo;
    if (datasetOne != NULL) {
        datasetTwo = datasetOne->getNext();
        delete datasetOne;
        while (datasetTwo != NULL) {
            datasetOne = datasetTwo;
            datasetTwo = datasetOne->getNext();
            delete datasetOne;
        }
    }
    delete query;
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

        // carriage returns ignorieren
        case '\r':
            break;

        case '\n':
            if (datasetStatus == outside) {
                buffer->reset();
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
    base = firstDataset;
    delete buffer;
}

int ClCatalogue::queryDialogue()
{
    ClCharBuffer *buffer = new ClCharBuffer(40);
    char input[512];
    int conditionCount = 0;
    int newCondition = 1;
    int number;
    char *value;
    query = new ClQuery(base);
    enum connector connector;

    cout << "Moechten sie eine Abfrage starten? [j/n]: ";
    copyInput(buffer, input);
    switch(isValidYesNoInput(input)) {

    case 1:

        while (newCondition == 1) {
            cout << "~~ Bedingung " << ++conditionCount << " ~~" << endl;
            cout << "Suche nach Kategorie-Nr.: ";
            copyInput(buffer, input);
            number =  isValidCategoryNumber(input);
            if (number == -1) {
                cout << "-> Bitte geben sie eine ganze Zahl von 0 bis 999 ein." << endl;
                conditionCount--;
                continue;
            }
            cout << "mit Text: ";
            value = getInput(buffer);
            query->addCondition(number, value);

            newCondition = -1;
            while (newCondition == -1) {
                cout << "Moechten sie eine weitere Bedingung hinzufuegen? [j/n]: ";
                copyInput(buffer, input);
                newCondition = isValidYesNoInput(input);
                if (newCondition == -1) {
                    cout << "-> Bitte antworten sie mit \'j\' fuer ja oder \'n\' fuer nein." << endl;
                }
            }

        }
        if (conditionCount > 1) {
            connector = invalid;
            while (connector == invalid) {
                cout << "Wie moechten sie die Bedingungen verknuepfen? [u,und,UND/o,oder,ODER]" << endl;
                copyInput(buffer, input);
                connector = isValidLogicalConnectorInput(input);
                if (connector == invalid) {
                    cout << "-> Bitte geben sie \'u\', \'und\', \'UND\' fuer eine und-Verknuepfung oder \'o\', \'oder\', \'ODER\' fuer eine oderVerknuepfung ein." << endl;
                }
            }
            query->setConnector(connector);
        }
        query->execute();
        query->print();
        return 1;

    case 0:
        cout << "Ciao!" << endl;
        return 0;

    default:
        cout << "-> Bitte antworten sie mit \'j\' fuer ja oder \'n\' fuer nein." << endl;
        return 1;
    }
    delete query;
    delete buffer;
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
    if (result && !((symbol != '.') || (symbol != ':'))) {
        result = false;
    }
    file.putback(symbol);
    for (count--; count >= 0; count--) {
        file.putback(linestart[count]);
    }
    return result;
}

char *ClCatalogue::getInput(ClCharBuffer *buffer) {
    char *result;
    buffer->reset();
    buffer->readInput('\n');
    result = strdup(buffer->getString());
    buffer->reset();
    return result;
}

void ClCatalogue::copyInput(ClCharBuffer *buffer, char *destination){
    buffer->reset();
    buffer->readInput('\n');
    strcpy(destination, buffer->getString());
    buffer->reset();
}

int ClCatalogue::isValidCategoryNumber(char *str)
{
    int number = -1;
    bool valid = true;
    for (unsigned int i = 0; i < strlen(str); i++) {
        if (!isdigit(str[i])) {
            valid = false;
            break;
        }
    }
    if (valid) {
        number = atoi(str);
        number = (number < 1000) ? number : -1;
    }
    return number;
}

int ClCatalogue::isValidYesNoInput(char *input)
{
    if (!strcmp(input, "j")) {
        return 1;
    } else if (!strcmp(input, "n")) {
        return 0;
    } else {
        return -1;
    }
}

connector ClCatalogue::isValidLogicalConnectorInput(char *input)
{
    if (!strcmp(input, "u") || !strcmp(input, "und") || !strcmp(input, "UND")) {
        return andConnect;
    } else if (!strcmp(input, "o") || !strcmp(input, "oder") || !strcmp(input, "ODER")) {
        return orConnect;
    } else {
        return invalid;
    }
}





