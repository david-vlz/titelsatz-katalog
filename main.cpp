#include <iostream>

using namespace std;

#include <fstream>

#include "catalogue.h"

int main()
{
    ifstream file;
    ClCatalogue *catalogue;
    int dialogueStatus = 1;

    file.open("Neugebauer.txt");
    if (!file) {
        cout << "Keine Datei mit diesem Namen vorhanden." << endl;
    }

    catalogue = new ClCatalogue;
    catalogue->load(file);
    cout << "Katalog geladen." << endl;
    file.close();

    while (dialogueStatus == 1) {
        dialogueStatus = catalogue->queryDialogue();
    }

    return dialogueStatus;
}

