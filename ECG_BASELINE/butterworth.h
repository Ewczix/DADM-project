#ifndef BUTTERWORTH_H
#define BUTTERWORTH_H

#include "filter.h"
#include <QObject>

class Butterworth : public Filter
{
    Q_OBJECT

    double cutoff;
    int order;
    double sampling_rate;

public:
    Butterworth(double fc = 0.5, int o = 2, double fs = 360) : cutoff(fc), order(o), sampling_rate(fs){} // default sampling rate?
    QVector<double> applyFilter(QVector<double> data);
    std::pair<QVector<double>, QVector<double>> coefficients();
};

#endif // BUTTERWORTH_H
