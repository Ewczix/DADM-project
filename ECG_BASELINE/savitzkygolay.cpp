#include "savitzkygolay.h"

#include <QVector>
#include <Eigen/Dense>

Eigen::MatrixXd pseudoinverse(const Eigen::MatrixXd& Y) {
    Eigen::JacobiSVD<Eigen::MatrixXd> svd(Y, Eigen::ComputeThinU | Eigen::ComputeThinV);
    double tolerance = 1e-6;
    Eigen::MatrixXd Sigma = svd.singularValues().asDiagonal();
    Eigen::MatrixXd SigmaInv = Sigma;

    for (int i = 0; i < Sigma.rows(); ++i) {
        if (Sigma(i, i) > tolerance) {
            SigmaInv(i, i) = 1.0 / Sigma(i, i);
        } else {
            SigmaInv(i, i) = 0.0;
        }
    }

    return svd.matrixV() * SigmaInv * svd.matrixU().transpose();
}

QVector<double> SavitzkyGolay::applyFilter(QVector<double> data) {

    QVector<double> coefficients = this->coefficients();
    QVector<double> smoothed(data.size(), 0.0);

    int half_window = window_size / 2;

    for (int i = half_window; i < data.size() - half_window; ++i) {
        double smoothed_value = 0.0;
        for (int j = -half_window; j <= half_window; ++j) {
            smoothed_value += coefficients[j + half_window] * data[i + j];
        }
        smoothed[i] = smoothed_value;
    }

    return smoothed;
}

QVector<double> SavitzkyGolay::coefficients() {
    if (window_size % 2 == 0 || window_size <= poly_order) {
        throw std::invalid_argument("Rozmiar okna musi być nieparzysty i większy niż stopień wielomianu.");
    }

    int half_window = window_size / 2;
    Eigen::MatrixXd Y(window_size, poly_order + 1);

    for (int i = -half_window; i <= half_window; ++i) {
        for (int j = 0; j <= poly_order; ++j) {
            Y(i + half_window, j) = pow(i, j);
        }
    }

    Eigen::MatrixXd Y_pseudo = pseudoinverse(Y);

    QVector<double> coefficients(window_size);
    for (int i = 0; i < window_size; ++i) {
        coefficients[i] = Y_pseudo(0, i);
    }

    return coefficients;
}
