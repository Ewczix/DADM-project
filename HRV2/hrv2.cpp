#include "hrv2.h"
#include "ui_hrv2.h"
#include <cmath>
#include <numeric>
#include <algorithm>
#include <QDebug>
using namespace std;


// wyznaczanie wektora czasu na podstawie wektora indeksow wykrytych zalamkow R
QVector<double> HRV2::calculateTimeVector(QVector<int> &peaks, int &sampling_rate) {
    QVector<double> time_vector;

    for(int i=0; i<peaks.size(); i++) {
        time_vector.push_back(peaks[i]*(1/static_cast<double>(sampling_rate)));
    }
    return time_vector;
}

// funkcja do obliczania czasu między kolejnymi zalamkami R
QVector<double> HRV2::calculateRRIntervalDuration(QVector<double> &time_vector) {
    QVector<double> rr_vector;

    for (int i=0; i<time_vector.size()-1; i++) {
        rr_vector.push_back((time_vector[i+1]-time_vector[i]));
    }
    return rr_vector;
}

// funkcja do wyznaczania sredniej arytmetycznej
double HRV2::calculateAverage(QVector<double> &vectValues) {
    if (vectValues.isEmpty()) {
        return 0.0;
    }
    double sum = 0.0;
    sum = accumulate(vectValues.begin(), vectValues.end(), 0.0);
    return sum / vectValues.size();
}

// funkcja do wyznaczania odchylenia standardowego
double HRV2::calculateStd(QVector<double> &vectValues) {
    if (vectValues.isEmpty()) {
        return 0.0;
    }
    double mean = calculateAverage(vectValues);
    double std = 0.0;
    for (int i=0; i<vectValues.size(); i++) {
        std += pow((vectValues[i] - mean), 2);
    }
    std = sqrt(std / (vectValues.size() - 1));
    return std;
}

// funkcja do wyznaczania wspolczynnikow SD1 i SD2 na podstawie punktow z wykresu Poincare
QPair<double, double> HRV2::calculateSD1SD2(QVector<double> &vectValues1, QVector<double> &vectValues2) {
    QPair<double, double> s1s2;
    QVector<double> diff;
    QVector<double> sum;
    if (vectValues1.size() != vectValues2.size()) {
        s1s2.first = 0.0;
        s1s2.second = 0.0;
        return s1s2;
    }

    for (int i=0; i<vectValues1.size(); i++) {
        diff.push_back(abs(vectValues1[i] - vectValues2[i]));
        sum.push_back(vectValues1[i] + vectValues2[i]);
    }

    double std1 = calculateStd(diff) / sqrt(2);
    double std2 = calculateStd(sum) / sqrt(2);
    s1s2.first = std1;
    s1s2.second = std2;
    return s1s2;
}

// funkcja wyznacza srodki przedzialow histogramu oraz liczbe wystapien odcinkow RR dla danego przedzialu
QPair<QVector<double>, QVector<double>> HRV2::getRRHistogram(QVector<double> &rrVectValues, double &binWidth) {
    QPair<QVector<double>, QVector<double>> hist_values;
    QVector<double> hist;
    QVector<double> binCentres;
    int num_intervals;

    // sortowanie wektora rosnaco
    sort(rrVectValues.begin(), rrVectValues.end());
    auto max = max_element(rrVectValues.begin(), rrVectValues.end());
    auto min = min_element(rrVectValues.begin(), rrVectValues.end());

    if (binWidth > 0) {
        num_intervals = floor(*max/binWidth);
        hist.resize(num_intervals, 0);
        binCentres.resize(num_intervals, 0);
        double leftEdge = 0.0;
        double rightEdge = binWidth;
        int vectorIdx = 0;

        for (int i=0; i<rrVectValues.size(); i++) {
            if(rrVectValues[i] < rightEdge && rrVectValues[i] >= leftEdge) {
                hist[vectorIdx]++;
            }

            if (rrVectValues[i] >= rightEdge) {
                leftEdge = rightEdge;
                rightEdge += binWidth;
                vectorIdx++;
            }
        }

        for (int j=0; j<num_intervals; j++) {
            binCentres[j] = *min + binWidth*(j + 0.5);
        }
    }

    hist_values.first = binCentres;
    hist_values.second = hist;
    return hist_values;
}

// funkcja do wyznaczania indeksu trojkatnego na podstawie wartosci z histogramu
double HRV2::getTriangularIndex(QVector<double> &rrVectValues) {
    double tri_idx = 0.0;

    auto max = max_element(rrVectValues.begin(), rrVectValues.end());
    if (*max != 0) {
        tri_idx = static_cast<double>(rrVectValues.size()) / *max;
    }
    return tri_idx;
}


// metoda najmniejszych kwadratow do dopasowania prostych oraz utworzenia trojkata
int HRV2::minimiseDistanceError(QVector<double> pointsX, QVector<double> &pointsY) {
    int minDistIdx;
    QVector<double> yFitted;
    QVector<double> distanceError;
    double sumXY = 0.0;
    double sumX = 0.0;
    double sumY = 0.0;
    double sumX2 = 0.0;
    double a;
    double b;

    if (pointsX.length() != pointsY.length()) {
        return -1;
    }
    int l = pointsX.length();

    for (int i=0; i<l; i++) {
        sumXY += pointsX[i]*pointsY[i];
        sumX += pointsX[i];
        sumY += pointsY[i];
        sumX2 += pow(pointsX[i], 2);
    }

    a = (l*sumXY - sumX*sumY)/(l*sumX2 - pow(sumX, 2));
    b = (sumX2*sumY - sumX*sumXY)/(l*sumX2 - pow(sumX, 2));

    for (int i=0; i<l; i++) {
        yFitted.push_back(a*pointsX[i] + b);
    }

    // kara dla zerowych binow, zeby mialy mniejszy wplyw na dopasowanie prostych
    double zeroBinPenalty = 200.0;
    for (int i = 0; i < l; i++) {
        double penalty = (pointsY[i] == 0) ? zeroBinPenalty : 1.0;
        distanceError.push_back(penalty * abs(yFitted[i] - pointsY[i]));
    }

    double min = *min_element(distanceError.begin(), distanceError.end());
    minDistIdx = distanceError.indexOf(min);
    return minDistIdx;
}

// wyznacza wspolczynnik tinn
QPair<double, QPair<QVector<double>, QVector<double>>> HRV2::getTinn(QVector<double> &binCounts, QVector<double> &binCentres) {
    double max = *max_element(binCounts.begin(), binCounts.end());
    int maxIdx = binCounts.indexOf(max);
    int beginIdxA = *binCounts.begin();
    int endIdxB = *binCounts.end();
    int idxA;
    int idxB;
    double tinnn;
    QVector<double> xCoords;
    QVector<double> yCoords;
    QPair<QVector<double>, QVector<double>> coords;
    QPair<double, QPair<QVector<double>, QVector<double>>> result;
    // szukamy pierwszego wierzcholka podstawy od poczatku zakresu wartosci podstawy histogramu do indeksu
    // wartosci maksymalnej
    QVector<double> binCentresA = binCentres.mid(beginIdxA, (maxIdx-beginIdxA)+1);
    QVector<double> binCountsA = binCounts.mid(beginIdxA, (maxIdx-beginIdxA)+1);
    QVector<double> binCentresB = binCentres.mid(maxIdx, (endIdxB-maxIdx)+1);
    QVector<double> binCountsB = binCounts.mid(maxIdx, (endIdxB-maxIdx)+1);
    idxA = minimiseDistanceError(binCentresA, binCountsA);
    idxB = minimiseDistanceError(binCentresB, binCountsB);

    tinnn = binCentres[idxB] - binCentres[idxA];
    xCoords = {binCentres[idxA], binCentres[maxIdx], binCentres[idxB]};
    yCoords = {0.0, max, 0.0};
    coords.first = xCoords;
    coords.second = yCoords;
    result.first = tinnn;
    result.second = coords;
    return result;
}


HRV2::HRV2(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::HRV2)
{
    ui->setupUi(this);  
}

HRV2::~HRV2()
{
    delete ui;
}

void HRV2::analyzeGeometry(QVector<int> peaks, int sampling_rate) {

    /// czesc pierwsza: obliczenia do wykresu Poincare - brakuje wyznaczenia elipsy
    QVector<double> time_vect = calculateTimeVector(peaks, sampling_rate);;
    QVector<double> rr_vect = calculateRRIntervalDuration(time_vect);
    QVector<double> rr1 = rr_vect.mid(0, rr_vect.size()-1);
    QVector<double> rr2 = rr_vect.mid(1, rr_vect.size());
    QPair<double, double> ss12 = calculateSD1SD2(rr1, rr2);

    // przypisanie obliczonych wartosci atrybutom klasy
    sd1 = ss12.first;
    sd2 = ss12.second;
    poincare_plot.first = rr1;
    poincare_plot.second = rr2;

    /// czesc druga: obliczenia do histogramu
    double binWd = 0.0078125;
    QVector<double> binCounts;
    QVector<double> binCentres;
    QPair<QVector<double>, QVector<double>> hist_params = getRRHistogram(rr_vect, binWd);
    rr_histogram = hist_params;

    triangular_index = getTriangularIndex(hist_params.second);
    binCentres = hist_params.first;
    binCounts = hist_params.second;

    QPair<double, QPair<QVector<double>, QVector<double>>> tinn_params = getTinn(binCounts, binCentres);
    tinn = tinn_params.first;
    triangle_vertices = tinn_params.second;
}

// getter do parametrow typu double (wiekszosc wskaznikow)
QMap<QString, double> HRV2::getGeometryResults() {
    QMap<QString, double> geometryResults;
    geometryResults["TINN"] = tinn;
    geometryResults["Triangular_index"] = triangular_index;
    geometryResults["SD1"] = sd1;
    geometryResults["SD2"] = sd2;
    return geometryResults;
}

// getter do danych do wykresu Poincare
QPair<QVector<double>, QVector<double>> HRV2::getPoincarePlot() {
    return poincare_plot;
}

void HRV2::createEllipse(int numPoints) {
    QVector<double> x(numPoints);
    QVector<double> y(numPoints);
    QVector<double> RRx = poincare_plot.first;
    QVector<double> RRy = poincare_plot.second;

    // srodek elipsy
    double ctrX = calculateAverage(RRx);
    double ctrY = calculateAverage(RRy);
    for (int i = 0; i < numPoints; ++i) {
        double theta = 2 * M_PI * i / (numPoints - 1);
        x[i] = ctrX + sd1 * cos(theta);
        y[i] = ctrY + sd2 * sin(theta);
    }
    ellipse.first = x;
    ellipse.second = y;
}

QPair<QVector<double>, QVector<double>> HRV2::getEllipse(int numPoints) {
    createEllipse(numPoints);
    return ellipse;
};

// getter do histogramu
QPair<QVector<double>, QVector<double>> HRV2::getHistogram() {
    // rr_histogram.first: binCentres, rr_histogram.second: binCounts
    return rr_histogram;
}

// getter do wierzcholkow dopasowanego trojkata
QPair<QVector<double>, QVector<double>> HRV2::getTriangleVertices() {
    // wierzcholki w kolejnosci: a, m, b, gdzie a - lewy wierzcholek podstawy (poczatek histogramu),
    // b - prawy wierzcholek podstawy (koniec histogramu), m - wierzcholek odpowiadajacy wartosci maksymalnej histogramu
    // first - wspolrzedne X, second - wspolrzedne Y
    return triangle_vertices;
}
