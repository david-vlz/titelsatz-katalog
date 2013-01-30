#include <iostream>

using namespace std;

#include <fstream>
#include <string.h>


void read(ifstream& datei);

enum datasetStatus { inside, outside };
enum categoryStatus { number, info, noise };

int main()
{
    ifstream file;
    file.open("test-data.txt");
    read(file);
    file.close();

    return 0;
}


void read(ifstream& datei)
{
    char symbol;
    char buffer[80] = "\0";
    int counter;

    enum datasetStatus datasetStatus = outside;
    enum categoryStatus categoryStatus = noise;

    for (counter = 0, datei.get(symbol); !datei.eof(); datei.get(symbol))
    {
        switch(symbol){

        case ':':

            // Datensatzbeginn
            if (datasetStatus == outside) {
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
                    categoryStatus = noise;
                    cout << "------ Ende Datensatz ------" << endl;
                }
                // Normale Kategorienummer behandeln
                else {
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
            buffer[counter] = '\0';
            counter = 0;
//            cout << buffer << endl;
            break;

        default:
            buffer[counter] = symbol;
            counter += 1;
            break;

        }
    }
}
