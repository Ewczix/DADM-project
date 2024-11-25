#include "movingaverage.h"

#include <QVector>
#include <numeric>

QVector<double> MovingAverage::applyFilter(QVector<double> data) {

    int N = 0;
    if (window_size%2 == 0) {
        N = window_size/2;
        window_size++;
    }
    else {
        N = (window_size-1)/2;
    }

    int L = data.length();
    QVector<double> smoothed;

    int temp;
    for (int i=0; i<L; i++) {
        if (i>=N && i<=(L-N)) {
            temp = std::accumulate(smoothed[i-N], smoothed[i+N], 0.0) / window_size;
            smoothed.append(temp);
        }
    }

    return smoothed;
}
