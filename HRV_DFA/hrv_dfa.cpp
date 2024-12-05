#include "HRV_DFA.h"
#include <numeric>
#include <iostream>
#include <algorithm>
#include <cmath>

// Konstruktor, inicjalizacja stałych
HRV_DFA::HRV_DFA() : alpha1(0.0), alpha2(0.0){}//,
    // n1([] {
    //     QVector<int> temp;
    //     for (int i = 10; i <= 40; ++i) temp.append(i);
    //     return temp;
    // }()),
    // n2([] {
    //     QVector<int> temp;
    //     for (int i = 40; i <= 256; ++i) temp.append(i);
    //     return temp;
    // }()) {}

// Fluktuacja
double HRV_DFA::fluktuacja(const QVector<double>& y, int n) {
    int N = y.size();
    int num_windows = N / n;
    double F_n2 = 0.0;

    for (int i = 0; i < num_windows; ++i) {
        QVector<double> segment = y.mid(i * n, n);

        // polyfit
        QVector<double> x(n);
        for (int j = 0; j < n; ++j) {
            x[j] = j + 1;
        }
        auto coeffs = polyfit(x, segment);

        // Trend
        QVector<double> trend_fit(n);
        for (int j = 0; j < n; ++j) {
            trend_fit[j] = coeffs.first * x[j] + coeffs.second;
        }

        for (int j = 0; j < n; ++j) {
            F_n2 += std::pow(segment[j] - trend_fit[j], 2);
        }
    }

    return std::sqrt(F_n2 / num_windows);
}

// polyfit
std::pair<double, double> HRV_DFA::polyfit(const QVector<double>& x, const QVector<double>& y) {
    int N = x.size();
    double sum_x = std::accumulate(x.begin(), x.end(), 0.0);
    double sum_y = std::accumulate(y.begin(), y.end(), 0.0);
    double sum_xx = std::inner_product(x.begin(), x.end(), x.begin(), 0.0);
    double sum_xy = std::inner_product(x.begin(), x.end(), y.begin(), 0.0);

    double slope = (N * sum_xy - sum_x * sum_y) / (N * sum_xx - sum_x * sum_x);
    double intercept = (sum_y - slope * sum_x) / N;

    return {slope, intercept};
    }

// DFA
void HRV_DFA::analyze(const QVector<double>& rr_intervals){//, const QVector<int>& n1, const QVector<int>& n2) {

    double rr_mean = std::accumulate(rr_intervals.begin(), rr_intervals.end(), 0.0) / rr_intervals.size();

    // Cumsum
    QVector<double> y(rr_intervals.size());
    std::transform(rr_intervals.begin(), rr_intervals.end(), y.begin(), [rr_mean](double rr) { return rr - rr_mean; });
    std::partial_sum(y.begin(), y.end(), y.begin());

    // F(n)1
    QVector<double> F_n1(n1.size());
    for (int i = 0; i < n1.size(); ++i) {
        F_n1[i] = fluktuacja(y, n1[i]);
    }

    // F(n)2
    QVector<double> F_n2(n2.size());
    for (int i = 0; i < n2.size(); ++i) {
        F_n2[i] = fluktuacja(y, n2[i]);
    }

    QVector<double> log_n1(n1.size()), log_F_n1(n1.size());
    QVector<double> log_n2(n2.size()), log_F_n2(n2.size());

    std::transform(n1.begin(), n1.end(), log_n1.begin(), [](int n) { return std::log(n); });
    std::transform(F_n1.begin(), F_n1.end(), log_F_n1.begin(), [](double F) { return std::log(F); });

    std::transform(n2.begin(), n2.end(), log_n2.begin(), [](int n) { return std::log(n); });
    std::transform(F_n2.begin(), F_n2.end(), log_F_n2.begin(), [](double F) { return std::log(F); });

    // Alpha1 i alpha2 jako nachylenie linii regresji
    alpha1 = polyfit(log_n1, log_F_n1).first;
    alpha2 = polyfit(log_n2, log_F_n2).first;

    // Wyniki
    std::cout << "Alpha1: " << alpha1 << std::endl;
    std::cout << "Alpha2: " << alpha2 << std::endl;
}

// Pobieranie wyników (alpha1 i alpha2)
std::pair<double, double> HRV_DFA::getParams() const {
    return {alpha1, alpha2};
}




