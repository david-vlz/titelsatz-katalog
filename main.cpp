#include <iostream>

using namespace std;

#include <fstream>
#include <string.h>

#include "charbuffer.h"
#include "category.h"


void read(ifstream& file);
bool isAtCategoryLineStart(ifstream &file);

enum datasetStatus { inside, outside };
enum categoryStatus { number, partNumber, value };

int main()
{
//    ifstream file;
//    file.open("test-data-2.txt");
//    if (!file) {
//        cout << "Keine Datei mit diesem Namen vorhanden." << endl;
//    }
//    read(file);
//    file.close();

    ClCategory *cat = new ClCategory;
    cat->setNumber(23);
    cat->addValue("abc");
    cout << cat->getValue() << endl;
    cat->setValue("def");
    cout << cat->getValue() << endl;
    char *c = new char[3];
    strcpy(c, "ghi");
    cat->addValue(c);
    cat->addValue("gnlpft");
    cat->addValue("gnlpft");
    cat->addValue("gnlpft");
    cat->addValue("gnlepft");
    cat->addValue("gn88888lpft");
    cat->addValue("gnlpr23434ft");
    cat->addValue("gn1111lpft");
    cout << "-----" << endl;
    cat->print();

    return 0;
}


void read(ifstream& file)
{
    char symbol;
    ClCharBuffer *buffer = new ClCharBuffer(81);

    enum datasetStatus datasetStatus = outside;
    enum categoryStatus categoryStatus = number;

    for (file.get(symbol); !file.eof(); file.get(symbol)) {

        switch(symbol) {

        case ':':

            // Datensatzbeginn erkennen
            if ((datasetStatus == outside) && (!strcmp(buffer->getString(), "000"))) {
                datasetStatus = inside;
                cout << "====== Beginn Datensatz ======" << endl;
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
                    cout << "====== Ende Datensatz ======" << endl;
                }
                // Erweiterung einer Kategorienummer
                else if (categoryStatus == partNumber) {
                    categoryStatus = value;
                    cout << "UntNr.: '" << buffer->getString() << '\'' << endl;
                }
                // Normale Kategorienummer
                else {
                    categoryStatus = value;
                    cout << "KatNr.: \'" << buffer->getString() << '\'' << endl;
                }
            }
            buffer->reset();
            break;

        case '.':
            if (categoryStatus == number) {
                categoryStatus = partNumber;
                cout << "KatNr.: \'" << buffer->getString() << '\'' << endl;
                buffer->reset();
            } else {
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
                // Status der neuen Zeile prüfen
                if (isAtCategoryLineStart(file)) {
                    cout << "Inhalt: " << '\'' << buffer->getString() << '\'' << endl;
                    cout << "-------" << endl;
                    categoryStatus = number;
                    buffer->reset();
                } else {
                    // Nichts unternehmen, wenn die Zeile einen Informationsbereich fortsetzt
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
    for (count -= 1, file.putback(symbol); count >= 0; count--) {
        file.putback(linestart[count]);
    }
    return result;
}
