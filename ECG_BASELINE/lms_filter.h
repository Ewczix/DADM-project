#ifndef LMS_FILTER_H
#define LMS_FILTER_H

#include <QObject>

class LMS_filter
{
    Q_OBJECT

    int window_size;
    double mu;

public:
    LMS_filter(int w = 6, double m = 0.007) : window_size(w), mu(m) {} // mu = 0.8 / fs * pi
    QVector<double> applyFilter(QVector<double> data);
    std::pair<double, QVector<double> > step(QVector<double> input, QVector<double> weights);
};

#endif // LMS_FILTER_H
