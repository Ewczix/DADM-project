#ifndef ECG_DATA_H
#define ECG_DATA_H

#include <QMainWindow>
#include <QObject>
#include <QVector>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui {
class ECG_Data;
}
QT_END_NAMESPACE

class ECG_Data : public QMainWindow
{
    Q_OBJECT

public:
    explicit ECG_Data(QWidget *parent = nullptr); // Konstruktor dla GUI
    explicit ECG_Data(float samplingRate, QWidget *parent = nullptr); // Konstruktor z samplingRate

    ~ECG_Data();

    // Funkcja do wczytywania danych z pliku .dat
    void loadData(const QString &filepath);

    // Funkcje do przechowywania przetworzonych danych
    void store_processed_data(const QVector<double> &ch1, const QVector<double> &ch2);
    // Funkcje do wizualizacji danych
    void visualizeData(QWidget *parent = nullptr);

    // Funkcje do pobrania surowych danych
    QVector<double> getRawCh(bool numch) const;

    // Funkcje do pobrania przetworzonych danych
    QVector<double> getProcessedCh(bool numch) const;

private:
    Ui::ECG_Data *ui;

    QVector<double> raw_ch1;          // Surowe dane dla kanału 1
    QVector<double> raw_ch2;          // Surowe dane dla kanału 2
    QVector<double> processed_ch1;    // Przetworzone dane dla kanału 1
    QVector<double> processed_ch2;    // Przetworzone dane dla kanału 2

    float sampling_rate = 360.0;     // Częstotliwość próbkowania (domyślnie 360)
};

#endif // ECG_DATA_H
