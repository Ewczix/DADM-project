#include "lms_filter.h"

#include <QVector>

QVector<double> LMS_filter::applyFilter(QVector<double> data) {

    QVector<double> processed(data.size()), weights(window_size, 0.0), buffer(window_size, 0.0);

    for (int n = 0; n < data.size(); ++n) {
        for (int i = window_size - 1; i > 0; --i) {
            buffer[i] = buffer[i - 1];
        }
        buffer[0] = data[n];
        std::tie(processed[n], weights) = this->step(buffer, weights);
        processed[n] = data[n] - processed[n];
    }

    return processed;
}

std::pair<double, QVector<double>> LMS_filter::step(QVector<double> input, QVector<double> weights) {
    // int compare = 1;
    double step_value = 0;

    for (int i=0; i<window_size; i++) step_value += weights[i] * input[i];

    double error = 1 - step_value;

    for (int i=0; i<window_size; i++) weights[i] += mu * error * input[i];

    return {step_value, weights};
}
