#ifndef HILBERTTRANSFORM_H
#define HILBERTTRANSFORM_H

#include <QVector>
#include <QSharedPointer>
#include <QMainWindow>
#include <QObject>
#include <QVector>
#include <QString>

class HilbertTransform
{
public:
    HilbertTransform() = default;
    /**
     * @brief Funkcja do wykrywania szczytów R metodą Transformaty Hilberta
     * @param - sygnał EKG
     * @param fs - częstotliwość próbkowania
     * @return Indeksy szczytów R
     */
    QVector<int> getPeaks(std::shared_ptr<const QVector<float>> electrocardiogram_signal, int fs = 360);

private:
    static constexpr int m_kSize = 1024; // rozmiar okna w transformacie Hilberta
    int m_fs; // częstotliwość próbkowania

    QVector<float> computeHilbertTransform(const QVector<float>& signal, int first) const;
    QVector<float> derivative(const QVector<float>& signal) const;
    QVector<float> filter(const QVector<float>& signal, float fc1, float fc2) const;

    int calcAverageDistance(const QVector<int>& peaks) const;
    float calcRMSValue(const QVector<float>& signal) const;

    void normalize(QVector<float>& v) const;

    template<typename T>
    QVector<T> conv(const QVector<T>& f, const QVector<T>& g) const;

    float factorial(int n) const;
};

#endif // HILBERTTRANSFORM_H
