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
    QVector<double> binEdges;

    double max = *max_element(rrVectValues.begin(), rrVectValues.end());
    double min = *min_element(rrVectValues.begin(), rrVectValues.end());
    double range = max - min;

    // sortuje rrVectValues, zeby ulatwic tworzenie histogramu
    sort(rrVectValues.begin(), rrVectValues.end());

    // zdefiniowanie liczby binow
    int num_intervals = ceil(range / binWidth);
    hist.resize(num_intervals, 0);
    binCentres.resize(num_intervals, 0);

    // Srodki przedzialow histogramu
    binCentres[0] = min;
    for (int j = 1; j < num_intervals; j++) {
        binCentres[j] = binCentres[j-1] + binWidth;
    }

    // wyznaczanie krawedzi binow
    binEdges.resize(num_intervals+1, 0);
    binEdges[0] = min - binWidth/2;
    for (int j = 1; j < num_intervals+1; j++) {
        binEdges[j] = binEdges[j-1] + binWidth;
    }

    int iter = 0;

    // wyznaczanie wartosci w binach
    for (int i=0; i<rrVectValues.size()-1; i++) {
        double leftEdge = binEdges[iter];
        double rightEdge = binEdges[iter+1];
        double value = rrVectValues[i];
        if (value >= leftEdge && value < rightEdge) {
            hist[iter]++;
        } else {
            iter++;
            i--;
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

// funkcja wyznaczajaca wspolczynnik tinn (w sekundach)
double HRV2::getTinn(QVector<double> &binCentres, QVector<double> &binCounts) {
    QPair<int, int> indices = fitTriangle(binCounts, binCentres);
    double tin = binCentres[indices.second] - binCentres[indices.first];
    return tin;
}

// funkcja wykonujaca dopasowanie trojkata do histogramu
QPair<int, int> HRV2::fitTriangle(QVector<double> &binCounts, QVector<double> &binCentres) {

    // max wartosc dla binCounts i indeks
    double ymax = *max_element(binCounts.begin(), binCounts.end());
    int maxIdx = binCounts.indexOf(ymax);
    double xmax = binCentres[maxIdx];


    QVector<double> leftErrorVect;

    // wyznaczam indeks dla lewego wierzcholka podstawy
    for (int i=maxIdx; i>0; i--) {
        double leftError1 = 0;
        double leftError2 = 0;
        double M = binCentres[i];

        for (int j=maxIdx-1; j>=0; j--) {
            if (binCounts[j] == 0) continue;
            if (binCentres[j] < M) {
                leftError1 += pow(binCounts[j], 2);
            } else {
                double yFit = ymax * (binCentres[j] - xmax) / (xmax - M) + ymax;
                leftError2 += pow((binCounts[j] - yFit), 2);
            }
        }

        int leftVectSize = distance(binCentres.begin(), binCentres.begin() + maxIdx);
        leftErrorVect.append((leftError1 + leftError2) / leftVectSize);
    }

    // indeks m dotyczy pierwszej polowy histogramu, ale musze odwrocic kolejnosc zeby dopasowany wierzcholek to byl pierwszy
    // w kolejnosci od srodka histogramu, a nie od jego poczatku
    double m = *min_element(leftErrorVect.rbegin(), leftErrorVect.rend());
    int mIdx_rev = leftErrorVect.indexOf(m);
    int mIdx = leftErrorVect.size() - 1 - mIdx_rev;

    QVector<double> rightErrorVect;

    // wyznaczam indeks dla prawego wierzcholka podstawy
    for (int i=maxIdx; i<binCentres.size()-1; i++) {
        double rightError1 = 0;
        double rightError2 = 0;
        double N = binCentres[i];

        for (int j=maxIdx+1; j<=binCentres.size()-1; j++) {
            if (binCounts[j] == 0) continue;
            if (binCentres[j] > N) {
                rightError1 += pow(binCounts[j], 2);
            } else {
                double yFit = ymax * (binCentres[j] - xmax) / (xmax - N) + ymax;
                rightError2 += pow((binCounts[j] - yFit), 2);
            }
        }
        int rightVectSize = distance(binCentres.begin() + maxIdx, binCentres.end());
        rightErrorVect.append((rightError1 + rightError2) / rightVectSize);
    }

    // tutaj trzeba dodac do idxMax wartosc n (bo szukamy wartosci minimalnej dla wektora bedacego
    // prawa strona histogramu, ktory jest od niego prawdopodobnie mniejszy)
    double n = *min_element(rightErrorVect.begin(), rightErrorVect.end());
    int nIdx = rightErrorVect.indexOf(n);

    QPair<int, int> triangleBaseIndices;
    triangleBaseIndices.first = mIdx;
    triangleBaseIndices.second = maxIdx + nIdx;

    return triangleBaseIndices;
}


// tworzenie elipsy
// UWAGA UWAGA!!!: do rysowania elipsy polecam QCPCurveData i QCPCurve zamiast wbudowanej funkcji do rysowania elipsy (QCPItemEllipse),
// bo ta wbudowana przyjmuje gorny lewy rog i prawy dolny do rysowania elipsy, co jest niewygodne zwlaszcza jak jest obrocona o 45 stopni
void HRV2::createRotatedEllipse(int numPoints, double angle) {
    QVector<double> x(numPoints);
    QVector<double> y(numPoints);
    QVector<double> rotatedX(numPoints);
    QVector<double> rotatedY(numPoints);
    QVector<double> RRx = poincare_plot.first;
    QVector<double> RRy = poincare_plot.second;

    // srodek elipsy
    double ctrX = calculateAverage(RRx);
    double ctrY = calculateAverage(RRy);

    // zamiana na radiany
    double angleRad = angle * M_PI / 180.0;

    // generowanie elipsy
    for (int i = 0; i < numPoints; ++i) {
        // kat w biezacym punkcie (od 0 do 2*pi)
        double theta = 2.0 * M_PI * i / numPoints;

        x[i] = sd1 * cos(theta);
        y[i] = sd2 * sin(theta);

        // obrot o zadany kat (45 stopni)
        rotatedX[i] = x[i] * cos(angleRad) + y[i] * sin(angleRad) + ctrX;
        rotatedY[i] = -x[i] * sin(angleRad) + y[i] * cos(angleRad) + ctrY;
    }
    ellipse.first = rotatedX;
    ellipse.second = rotatedY;

    // zdefiniowanie odcinkow SD1 i SD2 na elipsie, zeby moc je narysowac
    // poniewaz sa to fragmenty linii prostych ktore sie przecinaja pod katem prostym i sa obrocone pod katem 45 stopni, to
    // przesuniecie w osi x i y punktow lezacych na prostej prostopadlej do linii identycznosci mozna wyznaczyc z zaleznosci
    // w trojkacie prostokatnym rownoramiennym:
    double x_sd1 = ctrX - sd1/sqrt(2);
    double y_sd1 = ctrY + sd1/sqrt(2);
    double x_sd2 = ctrX + sd2/sqrt(2);
    double y_sd2 = ctrY + sd2/sqrt(2);
    QVector<double> sd1_vectX = {x_sd1, ctrX};
    QVector<double> sd1_vectY = {y_sd1, ctrY};
    QVector<double> sd2_vectX = {ctrX, x_sd2};
    QVector<double> sd2_vectY = {ctrY, y_sd2};
    sd1Points = QPair<QVector<double>, QVector<double>>(sd1_vectX, sd1_vectY);
    sd2Points = QPair<QVector<double>, QVector<double>>(sd2_vectX, sd2_vectY);
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

    /// czesc pierwsza: obliczenia do wykresu Poincare
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

    /// czesc druga: obliczenia do histogramu (zakladam, ze dane sa w sekundach a nie ms)
    double binWd = 1.0/128.0;

    QVector<double> binCounts;
    QVector<double> binCentres;
    QPair<QVector<double>, QVector<double>> hist_params = getRRHistogram(rr_vect, binWd);
    rr_histogram = hist_params;

    triangular_index = getTriangularIndex(hist_params.second);
    binCentres = hist_params.first;
    binCounts = hist_params.second;

    tinn = getTinn(binCentres, binCounts);

    QVector<double> xCoords;
    QVector<double> yCoords;
    QPair<QVector<double>, QVector<double>> coords;
    double max = *max_element(binCounts.begin(), binCounts.end());
    int maxIdx = binCounts.indexOf(max);

    QPair<int, int> params = fitTriangle(binCounts, binCentres);
    tinn = binCentres[params.second] - binCentres[params.first];
    xCoords = {binCentres[params.first], binCentres[maxIdx], binCentres[params.second]};
    yCoords = {0.0, max, 0.0};
    coords.first = xCoords;
    coords.second = yCoords;
    triangle_vertices = coords;
}

// getter do parametrow typu double (wiekszosc wskaznikow z HRV2)
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

// getter do elipsy obroconej o 45 stopni
QPair<QVector<double>, QVector<double>> HRV2::getEllipse(const int &numPoints = 100) {
    createRotatedEllipse(numPoints, 45.0);
    return ellipse;
};

// getter do punktow definiujacych odcinki sd1 i sd2 na wykresie Poincare
// dla argumentu sdNum = 1 zwracane sa wspolrzedne dla sd1, dla argumentu sdNum = 2 zwracane sa wspolrzedne dla sd2
QPair<QVector<double>, QVector<double>> HRV2::getSd1Sd2Coordinates(const int &sdNum = 1) {
    if (sdNum == 1) {
        return sd1Points;
    }
    if (sdNum == 2) {
        return sd2Points;
    }
    // domyslnie zwraca zerowe wektory, jesli nie zostanie podane 1 lub 2
    return QPair<QVector<double>, QVector<double>>({0.0}, {0.0});
}

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
