#include <iostream>

using namespace std;

#include <fstream>

#include "catalogue.h"

int main()
{
    ifstream file;
    int fileStatus = 1;
    char filename[256];

    ClCatalogue *catalogue;
    int dialogueStatus = 1;

    do {
        cout << "Bitte geben sie den Namen der Datei ein, der die Katalogdatensätze enthaelt: " << endl;
        cin >> filename;
        cout << "\'" << filename << "\'" << endl;
        file.open(filename);
        if (!file) {
            cout << "Keine Datei mit diesem Namen vorhanden." << endl;
            fileStatus = 0;
        }
        cin.ignore();
    } while (fileStatus == 0);

    catalogue = new ClCatalogue;
    catalogue->load(file);
    cout << "Katalog geladen." << endl;
    file.close();

    while (dialogueStatus == 1) {
        dialogueStatus = catalogue->queryDialogue();
    }

    return dialogueStatus;
}

