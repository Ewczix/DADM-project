#ifndef HRV_DFA_H
#define HRV_DFA_H

#include <QVector>
#include <utility>
#include <cmath>
#include <numeric>
#include <algorithm>
#include <iostream>

class HRV_DFA
{
    Q_OBJECT
private:
    double alpha1;
    double alpha2;

    //QVector<int> n1;
    //QVector<int> n2;

    double fluktuacja(const std::vector<double>& y, int n);
    std::pair<double, double> polyfit(const QVector<double>& x, const QVector<double>& y);

public:
    // Konstruktor
    HRV_DFA();
    // DFA
    void analyze(const std::vector<int>& rr_intervals); // const QVector<int>& n1, const QVector<int>& n2);
    // alpha1 i alpha2
    std::pair<double, double> getParams() const;
};

#endif // HRV_DFA_H




