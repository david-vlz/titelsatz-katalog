#include <iostream>

using namespace std;

#include <fstream>
#include <string.h>

#include "charbuffer.h"


void read(ifstream& file);
bool isAtCategoryLineStart(ifstream &file);

enum datasetStatus { inside, outside };
enum categoryStatus { number, subnumber, info };

int main()
{
    ifstream file;
    file.open("test-data-2.txt");
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

    for (file.get(symbol); !file.eof(); file.get(symbol)) {

        switch(symbol) {

        case ':':

            // Datensatzbeginn erkennen
            if (datasetStatus == outside) {

                if (!strcmp(buffer->getString(), "000")) {
                    datasetStatus = inside;
                    cout << "------ Beginn Datensatz ------" << endl;
                }
            }

            // Nichts unternehmen, wenn keine Kategorienummer erwartetet wird
            if (categoryStatus == info) {
                buffer->push(symbol);
                break;
            }

            // Kategorienummer behandeln
            if (datasetStatus == inside) {

                // Datensatzende behandeln
                if (!strcmp(buffer->getString(), "999")) {
                    datasetStatus = outside;
                    categoryStatus = number;
                    cout << "------ Ende Datensatz ------" << endl;
                }
                // Erweiterung einer Kategorienummer
                else if (categoryStatus == subnumber) {
                    categoryStatus = info;
                    cout << "UntNr.: '" << buffer->getString() << '\'' << endl;
                }
                // Normale Kategorienummer
                else {
                    categoryStatus = info;
                    cout << "KatNr.: \'" << buffer->getString() << '\'' << endl;
                }
            }
            buffer->reset();
            break;

        case '\n':

            // TODO:Auf fehlerhafte Eingaben zwischen Datensätzen prüfen
            if (datasetStatus == outside) {
                buffer->reset();
                break;
            }


            if (categoryStatus == info) {
                // Status der neuen Zeile prüfen
                if (isAtCategoryLineStart(file)) {

                    cout << "Inhalt: " << '\'' << buffer->getString() << '\'' << endl;

                    categoryStatus = number;
                    buffer->reset();
                } else {
                    // Nichts unternehmen, wenn die Zeile einen Informationsbereich fortsetzt
                    // Buffer wird weiter gefüllt
                    break;
                }
            } else {
                // TODO: Warnung bei Zeilenende nach leerer Kategorie ??
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
    char word[3];
    for (file.get(symbol), count = 0; count < 3; count++, file.get(symbol)) {
        word[count] = symbol;
        if (!std::isdigit(symbol)) {
            result = false;
            break;
        }
    }
    for (count -= 1, file.putback(symbol); count >= 0; count--) {
        file.putback(word[count]);
    }
    return result;
}
