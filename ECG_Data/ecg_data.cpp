#include "ecg_data.h"
#include "ui_ecg_data.h"
#include <QFile>
#include <QDataStream>
#include <QDebug>
#include <QException>
#include <limits>
#include <cmath>

// Konstruktor z QWidget
ECG_Data::ECG_Data(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ECG_Data)
{
    ui->setupUi(this);
}

// Konstruktor z samplingRate
ECG_Data::ECG_Data(float samplingRate, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::ECG_Data), sampling_rate(samplingRate)
{
    ui->setupUi(this);
}

// Destruktor
ECG_Data::~ECG_Data()
{
    delete ui;
}

// Funkcja do wczytywania danych z pliku .dat
void ECG_Data::loadData(const QString &filepath) {
    QFile file(filepath);

    if (!file.open(QIODevice::ReadOnly)) {
        qCritical() << "Nie można otworzyć pliku:" << filepath;
        throw QException();
    }

    raw_ch1.clear();
    raw_ch2.clear();

    QByteArray fileData = file.readAll();
    const char *data = fileData.constData();

    int sampleCount = fileData.size() / 3; // Każda próbka to 3 bajty (2 próbki 12-bitowe w 3 bajtach)

    // Skala przeskalowania (10mV / 2^11)
    double scale = 10.0 / 2048.0;

    for (int i = 0; i < sampleCount; ++i) {
        int byteIndex = i * 3;

        // Odczyt 12-bitowych danych (kanał 1 i kanał 2)
        int sample1 = ((data[byteIndex] & 0xFF) | ((data[byteIndex + 1] & 0x0F) << 8));
        int sample2 = (((data[byteIndex + 1] & 0xF0) >> 4) | ((data[byteIndex + 2] & 0xFF) << 4));

        // Obsługa wartości ujemnych (12-bit signed data)
        if (sample1 & 0x800) sample1 -= 0x1000;
        if (sample2 & 0x800) sample2 -= 0x1000;

        // Przeskalowanie danych na mV
        raw_ch1.append(sample1 * scale);  // Skalowanie na miliwolty
        raw_ch2.append(sample2 * scale);  // Skalowanie na miliwolty
    }

    file.close();

    qDebug() << "Wczytano dane: Kanał 1 =" << raw_ch1.size() << "próbek, Kanał 2 =" << raw_ch2.size() << "próbek.";
}


// Funkcja do przechowywania przetworzonych danych
void ECG_Data::store_processed_data(const QVector<double> &ch1, const QVector<double> &ch2) {
    processed_ch1 = ch1;
    processed_ch2 = ch2;
    qDebug() << "Przetworzone dane zapisane: Kanał 1 =" << processed_ch1.size() << "próbek, Kanał 2 =" << processed_ch2.size() << "próbek.";
}

// Funkcja do pobrania surowych danych dla 2 kanałów (True/1 - pierwszy kanał, False/0 - drugi kanał)
QVector<double> ECG_Data::getRawCh(bool numch) const {

    if (numch){
        return raw_ch1;
    }
    else{
       return raw_ch2;
    }
}

// Funkcja do pobrania przetworzonych danych dla 2 kanałów (True/1 - pierwszy kanał, False/0 - drugi kanał)
QVector<double> ECG_Data::getProcessedCh(bool numch) const {

    if (numch){
        return processed_ch1;
    }
    else{
        return processed_ch2;
    }
}




