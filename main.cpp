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
        cin.ignore();
        file.open(filename);
        if (!file) {
            cout << "Keine Datei mit diesem Namen vorhanden oder Datei konnte nicht geoeffnet werden." << endl;
            fileStatus = 0;
        } else {
            fileStatus = 1;
        }
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

