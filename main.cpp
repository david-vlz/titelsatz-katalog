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
//    ifstream file;
//    file.open("test-data-2.txt");
//    if (!file) {
//        cout << "Keine Datei mit diesem Namen vorhanden." << endl;
//    }
//    read(file);
//    file.close();

    ClCharBuffer *buffer;
    buffer = new ClCharBuffer;
    buffer->push('a');
    buffer->push('c');
    cout << buffer->getString() << endl;
    buffer->push('e');
    buffer->push('g');
    buffer->pop();
    cout << buffer->getString() << endl;

    return 0;
}


void read(ifstream& file)
{
    char symbol;
    char buffer[80] = "\0";
    int counter;

    enum datasetStatus datasetStatus = outside;
    enum categoryStatus categoryStatus = end;

    for (counter = 0, file.get(symbol); !file.eof(); file.get(symbol))
    {
        switch(symbol){

        case ':':

            // Datensatzbeginn
            if (datasetStatus == outside) {
                buffer[counter] = '\0';
                counter = 0;
                if (!strcmp(buffer, "000")) {
                    datasetStatus = inside;
                    cout << "------ Beginn Datensatz ------" << endl;
                }
            }

            // Kategorienummer behandeln, wenn im Datensatz
            if (datasetStatus == inside) {

                buffer[counter] = '\0';
                counter = 0;

                // Datensatzende behandeln
                if (!strcmp(buffer, "999")) {
                    datasetStatus = outside;
                    categoryStatus = end;
                    cout << "------ Ende Datensatz ------" << endl;
                }
                // Normale Kategorienummer behandeln
                else {
                    categoryStatus = info;
                    cout << "KatNr.: " << buffer << endl;
                }
            }

            break;

        case '.':
            buffer[counter] = '\0';
            counter = 0;
//            cout << buffer << endl;
            break;

        case '\n':

            // TODO:Auf fehlerhafte Eingaben zwischen Datansätzen prüfen
            if (datasetStatus == outside) {
                buffer[counter] = '\0';
                counter = 0;
                break;
            }


            if (categoryStatus == info) {
                // Status der neuen Zeile prüfen
                if (isAtCategoryLineStart(file)) {
                    buffer[counter] = '\0';
                    counter = 0;

                    cout << "Inhalt: " << buffer << endl;

                    categoryStatus = number;
                }
            }

//            cout << buffer << endl;
            break;

        default:
            buffer[counter] = symbol;
            counter += 1;
            break;

        }
    }
}

int isAtCategoryLineStart(ifstream& file)
{
    int result = 1;
    char c;
    int i;
    char word[3];
    for (file.get(c), i = 0; i < 3; i++, file.get(c)) {
        word[i] = c;
        if (!std::isdigit(c)) {
            result = 0;
            break;
        }
    }
    for (; i >= 0; i--) {
        file.putback(word[i]);
    }
    return result;
}
