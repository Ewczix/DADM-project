#ifndef HRV_DFA_H
#define HRV_DFA_H
#include <QVector>
#include <utility>
#include <QPair>

class HRV_DFA
{
public:
    // Konstruktor
    HRV_DFA();
    // DFA
    void analyze(const QVector<double>& rr_intervals);
    // alpha1 i alpha2
    std::tuple<double, double, double, double> getParams() const;
    // Punkty
    QPair<QVector<double>, QVector<double>> getPoints() const;

private:
    double alpha1;
    double alpha2;
    double intercept1;
    double intercept2;

    QVector<double> log_n1, log_F_n1;
    QVector<double> log_n2, log_F_n2;

    const QVector<int> n1 = [] {
        QVector<int> temp;
        for (int i = 4; i <= 16; ++i) temp.append(i);
        return temp;
    }();

    const QVector<int> n2 = [] {
        QVector<int> temp;
        for (int i = 16; i <= 64; ++i) temp.append(i);
        return temp;
    }();

    double fluktuacja(const QVector<double>& y, int n);
    std::pair<double, double> polyfit(const QVector<double>& x, const QVector<double>& y);

};

#endif // HRV_DFA_H




