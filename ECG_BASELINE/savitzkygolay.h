#ifndef SAVITZKYGOLAY_H
#define SAVITZKYGOLAY_H

#include "filter.h"
#include <QObject>

class SavitzkyGolay : public Filter
{
    Q_OBJECT

    int window_size, poly_order;

public:
    SavitzkyGolay(int w = 21, int n = 6) : window_size(w), poly_order(n) {}
    QVector<double> applyFilter(QVector<double> data);
    QVector<double> coefficients();
};

#endif // SAVITZKYGOLAY_H
