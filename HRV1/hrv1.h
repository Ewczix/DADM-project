#ifndef HRV1_H
#define HRV1_H

#include <QVector>
#include <QMap>
#include <QString>

class HRV1 {
private:
    QMap<QString, double> time_parameters; // Parametry HRV w dziedzinie czasu
    QMap<QString, double> freq_parameters; // Parametry HRV w dziedzinie częstotliwości

public:
    HRV1();

    void analyzeTimeDomain(const QVector<int>& peaks, int fs);
    void analyzeFreqDomain(const QVector<int>& peaks, int fs);

    QMap<QString, double> getTimeParameters() const;
    QMap<QString, double> getFreqParameters() const;
};

#endif // HRV1_H
