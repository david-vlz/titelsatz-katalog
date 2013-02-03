#ifndef QUERY_H
#define QUERY_H

enum connector { andConnect, orConnect };

class ClQuery
{
public:
    ClQuery(ClDataset *data);

    void addQuery(int number, char *value);

    void setConnector(enum connector logicalConnector) { connector = logicalConnector; }

    ClDataset **query();

    void toStream(ostream &stream);

private:
    ClDataset *base;
    ClCategory *firstQuery;
    enum connector connector;
    ClDataset **results;
};


#endif // QUERY_H
