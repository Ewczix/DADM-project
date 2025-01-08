#ifndef PANTOMPKINS_H
#define PANTOMPKINS_H
#include <QObject>
#include <QDebug>
#include <QVector>
#include <QSharedPointer>
class PanTompkins
{
public:
    PanTompkins()=default;
    /** @brief Funkcja do wykrywania szczytów R metodą Pana Tompkinsa.
     *  @param electrocardiogram_signal - sygnał EKG
     *  @param fs - częstotliwość próbkowania
     *  @return Indeksy szczytów R
     */
    QVector<int> getPeaks(QVector<float> electrocardiogram_signal, int fs = 360);
    QVector<float> normalize(QVector<float>& v) const;

private:
    int m_fs; // Częstotliwość próbkowania

    QVector<float> filter(const QVector<float>& signal, float fc1, float fc2) const;


    template<typename T>
    QVector<T> conv(const QVector<T>& f, const QVector<T>& g) const;
};

#endif // PANTOMPKINS_H
