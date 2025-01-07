#ifndef HRV_DFA_H
#define HRV_DFA_H

#include <QVector>
#include <utility>

class HRV_DFA
{
private:
    double alpha1;
    double alpha2;

    const QVector<int> n1 = [] {
        QVector<int> temp;
        for (int i = 10; i <= 40; ++i) temp.append(i);
        return temp;
    }();

    const QVector<int> n2 = [] {
        QVector<int> temp;
        for (int i = 40; i <= 256; ++i) temp.append(i);
        return temp;
    }();

    double fluktuacja(const QVector<double>& y, int n);
    std::pair<double, double> polyfit(const QVector<double>& x, const QVector<double>& y);

public:
    // Konstruktor
    HRV_DFA();
    // DFA
    void analyze(const QVector<double>& rr_intervals);
    // alpha1 i alpha2
    std::pair<double, double> getParams() const;
};

#endif // HRV_DFA_H




