#ifndef WAVES_H
#define WAVES_H

#include <QtCore/QVector>
#include <QtCore/QPair>
#include <QtCore/QSharedPointer>
#include <QString>

class Waves {
public:
    explicit Waves(float fs);

    QVector<int> getPeaks(QSharedPointer<const QVector<float>> electrocardiogram_signal, float fs);
    QVector<int> getQRSOnsets(const QVector<float>& signal, const QVector<int>& r_peaks);
    QVector<int> getQRSOffsets(const QVector<float>& signal, const QVector<int>& r_peaks);
    QVector<int> getPonsets(const QVector<float>& signal, const QVector<int>& r_peaks);
    QVector<QPair<int, int>> getTandTends(const QVector<float>& signal, const QVector<int>& r_peaks);

private:
    float m_fs;
};

QVector<float> loadECGFromFile(const QString& fileName);

#endif // WAVES_H
