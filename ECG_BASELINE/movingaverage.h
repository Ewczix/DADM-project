#ifndef MOVINGAVERAGE_H
#define MOVINGAVERAGE_H

#include "filter.h"
#include <QObject>

class MovingAverage : public Filter
{
    Q_OBJECT

    int window_size;

public:
    MovingAverage(int w = 5) : window_size(w) {}
    QVector<double> applyFilter(QVector<double> data);
};

#endif // MOVINGAVERAGE_H
