#include "pantompkins.h"
#include <QtMath>
#include <QVector>
#include <QDebug>
#include <algorithm>

QVector<float> PanTompkins::normalize(QVector<float>& v) const
{
    // Maksymalna i minimalna wartość wektora
    float max = *std::max_element(v.begin(), v.end());
    float min = *std::min_element(v.begin(), v.end());

    // Maksymalna wartość bezwzględna
    float maxAbs = qMax(qAbs(max), qAbs(min));
    for (float& element : v)
        element /= maxAbs;
    qDebug()<<"normalize wywolane";
    return v;
}

// Konwolucja sygnałów
template<typename T>
QVector<T> PanTompkins::conv(const QVector<T>& f, const QVector<T>& g) const
{
    int nf = f.size();
    int ng = g.size();
    int n = nf + ng - 1;

    QVector<T> out(n, T());

    // Konwolucja
    for (int i = 0; i < n; ++i)
    {
        // Określenie granic indeksów w obrębie wektorów wejściowych
        int jmin = (i >= ng - 1) ? i - (ng - 1) : 0;
        int jmax = (i < nf - 1) ? i : nf - 1;

        // Obliczanie wartości w danym indeksie wynikowym
        for (int j = jmin; j <= jmax; ++j)
            out[i] += f[j] * g[i - j];
    }

    return out;
}

// Filtrowanie sygnału
QVector<float> PanTompkins::filter(const QVector<float>& signal, float fc1, float fc2) const
{
    const float M = 5.0; // Parametr określający rozmiar okna filtra
    const int N = 2 * M + 1; // Długość okna

    // Generowanie wektora próbek
    QVector<float> n1;
    for (int i = -M; i <= M; ++i)
        n1.append(i);

    // Filtr dolnoprzepustowy
    float fc = fc1 / (m_fs / 2);
    QVector<float> y1;
    for (float value : n1)
    {

        float result = (qFuzzyCompare(value, 0)) ? 2 * fc : qSin(2 * M_PI * fc * value) / (M_PI * value);
        y1.append(result);
    }

    // Filtr górnoprzepustowy
    fc = fc2 / (m_fs / 2);
    QVector<float> y2;
    for (float value : n1)
    {

        float result = (qFuzzyCompare(value, 0)) ? 1 - (2 * fc) : -qSin(2 * M_PI * fc * value) / (M_PI * value);
        y2.append(result);
    }

    // Okno Hamminga, aby wygładzić filtry
    QVector<float> window(N);
    for (int i = 0; i < N; ++i)
        window[i] = 0.54 - 0.46 * qCos(2 * M_PI * i / (N - 1));


    for (int i = 0; i < y1.size(); ++i)
    {
        y1[i] *= window[i];
        y2[i] *= window[i];
    }

    // Filtracja sygnału
    QVector<float> c1 = conv(y1, signal);
    normalize(c1);
    c1.remove(0, 6);

    QVector<float> c2 = conv(y2, c1);
    normalize(c2);
    c2.remove(0, 16);

    return c2;
}

QVector<int> PanTompkins::getPeaks(QSharedPointer<const QVector<float>> electrocardiogram_signal, int fs)
{
    m_fs = fs;

    // Filtracja sygnału
    QVector<float> signal1 = filter(*electrocardiogram_signal, 5, 15);

    // Różniczkowanie
    QVector<float> signal2(signal1.size());
    for (int i = 2; i < signal1.size() - 2; ++i)
    {
        signal2[i - 2] = 1. / 8 * (-signal1[i - 2] - 2 * signal1[i - 1] + 2 * signal1[i + 1] + signal1[i + 2]);
    }
    normalize(signal2);

    // Podniesienie do kwadratu
    for (float& value : signal2)
        value = qPow(value, 2);
    normalize(signal2);

    // Wygładzanie za pomocą uśrednienia
    int C = 0.15 * m_fs; // Rozmiar okna wygładzania (proporcjonalny do fs)
    QVector<float> window(C, 1. / C); // Tworzenie okna wygładzania
    QVector<float> signal3 = conv(window, signal2);
    normalize(signal3); // Normalizacja wygładzonego

    // Detekcja szczytów
    QVector<int> peaks;
    QVector<int> false_peaks;
    const int halfWindow = 360;
    const int fullWindow = 2 * halfWindow + 1;
    float signal_level = *std::max_element(signal3.begin(), signal3.begin() + fullWindow) * 1. / 3;
    float noise_level = 0.5 * signal_level;
    int i = 0;
    int step = 0.2 * m_fs;
    float local_max = 0.0;
    float local_max_val = 0.0;

    while (i + step < signal3.size()) {
        float threshold = noise_level + 0.25 * (signal_level - noise_level);

        local_max_val = 0;
        while (signal3[i + 1] > signal3[i]) {
            local_max = i;
            local_max_val = signal3[i];
            i += 1;
        }

        if (local_max_val >= threshold) {
            peaks.push_back(local_max);
            signal_level = 0.125 * local_max_val + 0.875 * signal_level;
            i = local_max + step;
        } else {
            false_peaks.push_back(local_max);
            noise_level = 0.125 * local_max_val + 0.875 * noise_level;
        }

        i += 1;
    }
    qDebug()<<"getPeaks called";

    QVector<int> peaks_ekg;
    std::for_each(peaks.begin(), peaks.end(), [electrocardiogram_signal, &peaks_ekg, fs](int peak) {
        auto local = std::max_element(electrocardiogram_signal->begin() + peak - int(0.150 * fs), electrocardiogram_signal->begin() + peak + int(0.150 * fs));
        int local_idx = std::distance(electrocardiogram_signal->begin(), local);
        peaks_ekg.push_back(local_idx);
    });

    return peaks_ekg;
}
