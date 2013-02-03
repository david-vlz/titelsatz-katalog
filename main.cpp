#include <iostream>

using namespace std;

#include <cstdlib>
#include <fstream>
#include <string.h>

#include "category.h"
#include "charbuffer.h"
#include "dataset.h"


void read(ifstream& file);
bool isAtCategoryLineStart(ifstream &file);

enum datasetStatus { inside, outside };
enum categoryStatus { number, partNumber, value };

int main()
{
    ifstream file;
    file.open("sample-data.txt");
    if (!file) {
        cout << "Keine Datei mit diesem Namen vorhanden." << endl;
    }
    read(file);
    file.close();

    return 0;
}


void read(ifstream& file)
{
    char symbol;
    ClCharBuffer *buffer = new ClCharBuffer(81);

    enum datasetStatus datasetStatus = outside;
    enum categoryStatus categoryStatus = number;
    bool readingMultiValuedCategory = false;

    int categoryNumber = 0;
    char *categoryValue;

    ClCategory *first = NULL;
    ClCategory *category = NULL;
    ClCategory *next = NULL;

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
                    first = NULL;
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
                    if (first == NULL) {
                        first = new ClCategory(categoryNumber);
                        category = first;
                        dataset->setFirstCategory(first);
                    } else {
                        next = new ClCategory(categoryNumber);
                        category->setNext(next);
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
                    next = new ClCategory(categoryNumber);
                    category->setNext(next);
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


        case '\r':
            // Carriage returns ignorieren
            break;

        default:
            buffer->push(symbol);
            break;

        }
    }

    firstDataset->printAll();
}


bool isAtCategoryLineStart(ifstream& file)
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
