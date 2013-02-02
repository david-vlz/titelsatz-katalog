#include <iostream>

using namespace std;

#include <fstream>
#include <string.h>
#include <cstdlib>

#include "charbuffer.h"
#include "category.h"


void read(ifstream& file);
bool isAtCategoryLineStart(ifstream &file);

enum datasetStatus { inside, outside };
enum categoryStatus { number, partNumber, value };

int main()
{
    ifstream file;
    file.open("test-data.txt");
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

    char *val;

    ClCategory *first = NULL;
    ClCategory *category = NULL;
    ClCategory *next = NULL;

    for (file.get(symbol); !file.eof(); file.get(symbol)) {

        switch(symbol) {

        case ':':

            // Datensatzbeginn erkennen
            if ((datasetStatus == outside) && (!strcmp(buffer->getString(), "000"))) {
                datasetStatus = inside;
            }

            // Nichts unternehmen, wenn keine Kategorienummer erwartetet wird
            if (categoryStatus == value) {
                buffer->push(symbol);
                break;
            }

            // Kategorienummer behandeln
            if (datasetStatus == inside) {

                // Datensatzende behandeln
                if (!strcmp(buffer->getString(), "999")) {
                    datasetStatus = outside;
                    categoryStatus = number;
                }
                // Erweiterung einer Kategorienummer
                else if (categoryStatus == partNumber) {
                    categoryStatus = value;
                }
                // Bei normaler Kategorienummer, Kategorie initialisieren
                else {
                    if (first == NULL) {
                        first = new ClCategory(atoi(buffer->getString()));
                        category = first;
                    } else {
                        next = new ClCategory(atoi(buffer->getString()));
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
            if (categoryStatus == number) {
                next = new ClCategory(atoi(buffer->getString()));
                category->setNext(next);
                category = category->getNext();
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
                if (isAtCategoryLineStart(file)) {
                    val = new char[strlen(buffer->getString())+1];
                    strcpy(val, buffer->getString());
                    if (readingMultiValuedCategory) {
                        category->addValue(val);
                    } else {
                        category->setValue(val);
                    }
                    categoryStatus = number;
                    buffer->reset();
                }
                // Nichts unternehmen, wenn die Zeile einen Informationsbereich fortsetzt
                else {
                    // Buffer wird weiter gefüllt
                    break;
                }
            } else {
                // TODO: Warnung bei Zeilenende nach leerer Kategorie, wenn Puffer gefüllt ??
            }
            break;


        case '\r':
            // Ignore carriage returns, TODO: check if those should be there in the first place
            break;

        default:
            buffer->push(symbol);
            break;

        }
    }
    for (ClCategory *c = first; c != NULL; c = c->getNext()) {
        c->print();
    }
}

// TODO: Auf korrektes Endsymbol testen, '.' oder ';'
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
    for (count -= 1, file.putback(symbol); count >= 0; count--) {
        file.putback(linestart[count]);
    }
    return result;
}
