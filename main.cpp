#include <iostream>

using namespace std;

#include <fstream>
#include <string.h>

#include "charbuffer.h"


void read(ifstream& file);
int isAtCategoryLineStart(ifstream &file);

enum datasetStatus { inside, outside };
enum categoryStatus { number, info, end };

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
    ClCharBuffer *buffer = new ClCharBuffer;

    enum datasetStatus datasetStatus = outside;
    enum categoryStatus categoryStatus = end;

    for (file.get(symbol); !file.eof(); file.get(symbol)) {

        switch(symbol){

        case ':':

            // Datensatzbeginn
            if (datasetStatus == outside) {

                if (!strcmp(buffer->getString(), "000")) {
                    datasetStatus = inside;
                    cout << "------ Beginn Datensatz ------" << endl;
                }
            }

            // Kategorienummer behandeln, wenn im Datensatz
            if (datasetStatus == inside) {

                // Datensatzende behandeln
                if (!strcmp(buffer->getString(), "999")) {
                    datasetStatus = outside;
                    categoryStatus = end;
                    cout << "------ Ende Datensatz ------" << endl;
                }
                // Normale Kategorienummer behandeln
                else {
                    categoryStatus = info;
                    cout << "KatNr.: " << buffer->getString() << endl;
                }
            }
            buffer->reset();
            break;

        case '.':
            buffer->reset();
            break;

        case '\n':

            // TODO:Auf fehlerhafte Eingaben zwischen Datansätzen prüfen
            if (datasetStatus == outside) {
                buffer->reset();
                break;
            }


            if (categoryStatus == info) {
                // Status der neuen Zeile prüfen
                if (isAtCategoryLineStart(file)) {

                    cout << "Inhalt: " << buffer->getString() << endl;

                    categoryStatus = number;
                }
            }

            buffer->reset();

            break;

        default:
            buffer->push(symbol);
            break;

        }
    }
}

int isAtCategoryLineStart(ifstream& file)
{
    int result = 1;
    char symbol;
    int count;
    char word[3];
    for (file.get(symbol), count = 0; count < 3; count++, file.get(symbol)) {
        word[count] = symbol;
        if (!std::isdigit(symbol)) {
            result = 0;
            break;
        }
    }
    for (; count >= 0; count--) {
        file.putback(word[count]);
    }
    return result;
}
