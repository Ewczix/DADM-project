#ifndef FILTER_H
#define FILTER_H

#include <QObject>

class Filter
{
    Q_OBJECT
public:
    Filter();
    virtual QVector<double> applyFilter(QVector<double> data);
};

#endif // FILTER_H
