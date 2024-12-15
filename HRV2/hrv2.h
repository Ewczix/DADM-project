#ifndef HRV2_H
#define HRV2_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class HRV2;
}
QT_END_NAMESPACE

class HRV2 : public QMainWindow
{
    Q_OBJECT

public:
    HRV2(QWidget *parent = nullptr);
    ~HRV2();
    // tutaj deklaracja metod (bez implementacji)
    void analyzeGeometry(QVector<int> peaks, int sampling_rate);
    QMap<QString, double> getGeometryResults();
    QPair<QVector<double>, QVector<double>> getPoincarePlot();
    QPair<QVector<double>, QVector<double>> getHistogram();
    QPair<QVector<double>, QVector<double>> getTriangleVertices();
    QPair<QVector<double>, QVector<double>> getEllipse(const int &numPoints);
    QPair<QVector<double>, QVector<double>> getSd1Sd2Coordinates(const int &sdNum);

private:
    Ui::HRV2 *ui;

    // prywatne metody - funkcje pomocnicze
    QVector<double> calculateTimeVector(QVector<int> &peaks, int &sampling_rate);
    QVector<double> calculateRRIntervalDuration(QVector<double> &time_vector);
    double calculateAverage(QVector<double> &vectValues);
    double calculateStd(QVector<double> &vectValues);
    QPair<double, double> calculateSD1SD2(QVector<double> &vectValues1, QVector<double> &vectValues2);
    QPair<QVector<double>, QVector<double>> getRRHistogram(QVector<double> &rrVectValues, double &binWidth);
    double getTriangularIndex(QVector<double> &rrVectValues);
    double getTinn(QVector<double> &binCentres, QVector<double> &binCounts);
    void createRotatedEllipse(int numPoints, double angle);
    QPair<int, int> fitTriangle(QVector<double> &binCounts, QVector<double> &binCentres);

    // tutaj deklaracja atrybutow klasy
    QPair<QVector<double>, QVector<double>> rr_histogram;
    QPair<QVector<double>, QVector<double>> poincare_plot;
    double tinn;
    double triangular_index;
    double sd1;
    double sd2;
    // elipsa do wykresu poincare (juz obrocona o 45 stopni)
    QPair<QVector<double>, QVector<double>> ellipse;
    // wspolrzedne punktow do narysowania odcinkow sd1 i sd2 na elipsie (wykres Poincare)
    QPair<QVector<double>, QVector<double>> sd1Points;
    QPair<QVector<double>, QVector<double>> sd2Points;
    // wspolrzedne wierzcholkow dopasowanego trojkata
    QPair<QVector<double>, QVector<double>> triangle_vertices;
};
#endif // HRV2_H
