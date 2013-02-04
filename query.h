#ifndef QUERY_H
#define QUERY_H

enum connector { andConnect, orConnect, invalid };

/**
  * Ein Abfrage besteht aus mehreren Instanzen von ClCategory,
  * die die abzufragenden Werte enthalten, sowie einer Angabe
  * der Art der logischen Verknüpfung dieser Werte
  * Durch Vergleich mit der Datenbasis ("base") mit diesen Abfragen
  * wird eine Liste von Titelsätzen ("results") erzeugt, die der Anfrage
  * entsprechen.
  */
class ClQuery
{
public:
    ClQuery(ClDataset *data);

    void addQuery(int number, char *value);

    void setConnector(enum connector logicalConnector) { logConn = logicalConnector; }

    void execute();

    void toStream(ostream &stream);
    void print() { toStream(cout); }

private:
    ClDataset *base;
    ClCategory *firstQuery;
    ClCategory *nextQuery;
    ClCategory *lastQuery;
    enum connector logConn;
    ClDataset **results;
    int resultCount;
    int maxResults;
    void addToResults(ClDataset *positiveMatch);
};


#endif // QUERY_H
