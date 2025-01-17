#include "hrv_dfa.h"

#include <numeric>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <Eigen/Dense>

// Konstruktor, inicjalizacja stałych
HRV_DFA::HRV_DFA() : alpha1(0.0), alpha2(0.0), intercept1(0.0), intercept2(0.0){}

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
std::pair<double, double> HRV_DFA::polyfit(const QVector<double>& x,
                                           const QVector<double>& y) {
    int N = x.size();

    Eigen::VectorXd X(N), Y(N);
    for (int i = 0; i < N; ++i) {
        X[i] = x[i];
        Y[i] = y[i];
    }

    Eigen::MatrixXd A(N, 2);
    A.col(0) = X;
    A.col(1) = Eigen::VectorXd::Ones(N);

    Eigen::VectorXd coeffs = A.colPivHouseholderQr().solve(Y);

    double slope = coeffs[0];
    double intercept = coeffs[1];

    return {slope, intercept};
}

// DFA
void HRV_DFA::analyze(const QVector<double>& rr_intervals){

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

    log_n1.resize(n1.size());
    log_F_n1.resize(n1.size());
    log_n2.resize(n2.size());
    log_F_n2.resize(n2.size());

    std::transform(n1.begin(), n1.end(), log_n1.begin(), [](int n) { return std::log(n); });
    std::transform(F_n1.begin(), F_n1.end(), log_F_n1.begin(), [](double F) { return std::log(F); });

    std::transform(n2.begin(), n2.end(), log_n2.begin(), [](int n) { return std::log(n); });
    std::transform(F_n2.begin(), F_n2.end(), log_F_n2.begin(), [](double F) { return std::log(F); });

    alpha1 = polyfit(log_n1, log_F_n1).first;
    intercept1 = polyfit(log_n1, log_F_n1).second;

    alpha2 = polyfit(log_n2, log_F_n2).first;
    intercept2 = polyfit(log_n2, log_F_n2).second;

    std::cout << "Alpha1: " << alpha1 << std::endl;
    std::cout << "Alpha2: " << alpha2 << std::endl;
}

// Pobieranie wyników (alpha1, alpha2 i wyrazów wolnych)
std::tuple<double, double, double, double> HRV_DFA::getParams() const {
    return {alpha1, alpha2, intercept1, intercept2};
}

// Punkty
QPair<QVector<double>, QVector<double>> HRV_DFA::getPoints() const {
    QVector<double> log_n = log_n1 + log_n2;
    QVector<double> log_F = log_F_n1 + log_F_n2;
    return {log_n, log_F};
}
