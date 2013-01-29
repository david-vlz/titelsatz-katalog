#include <iostream>

using namespace std;

#include <fstream>


void read(ifstream& file);

int main()
{
    ifstream file;
    file.open("test-data.txt");
    read(file);

    return 0;
}


void read(ifstream& file)
{
    char symbol;
    char buffer[80];
    int counter;

    /**
      * Basis-Leseschleife
      */

    for (counter = 0, file.get(symbol); !file.eof(); file.get(symbol))
    {
        switch(symbol){
        case ':':
            buffer[counter] = '\0';
            counter = 0;
            cout << '\'' << buffer << '\'' << endl;
            break;
        case '.':
            buffer[counter] = '\0';
            counter = 0;
            cout << buffer << endl;
            break;
        case '\n':
            buffer[counter] = '\0';
            counter = 0;
            cout << buffer << endl;
            break;
        default:
            buffer[counter] = symbol;
            counter += 1;
            break;
        }
    }
}
