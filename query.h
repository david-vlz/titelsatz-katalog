#ifndef QUERY_H
#define QUERY_H

enum connector { andConnect, orConnect, invalid };

class ClQuery
{
public:
    ClQuery(ClDataset *data);
    ~ClQuery();

    void addCondition(int number, char *value);

    void setConnector(enum connector logicalConnector) { logConn = logicalConnector; }

    void execute();

    ClDataset **getResults() { return results; }

    void toStream(ostream &stream);
    void print() { toStream(cout); }

private:
    ClDataset *base;
    ClCategory *firstCondition;
    ClCategory *nextCondition;
    ClCategory *lastCondition;
    enum connector logConn;
    ClDataset **results;
    int resultCount;
    int maxResults;
    void addToResults(ClDataset *positiveMatch);
};


#endif // QUERY_H
