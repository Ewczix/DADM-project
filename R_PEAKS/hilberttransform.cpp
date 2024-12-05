#include "HilbertTransform.h"
#include <QtMath>
#include <QDebug>
#include <algorithm>
#include <hilberttransform.h>
#include <fftw3.h> // Wymaga zainstalowanej biblioteki FFTW

using namespace std;

// Normalizacja sygnału
void HilbertTransform::normalize(QVector<float>& v) const {
    float max = *std::max_element(v.begin(), v.end());
    float min = *std::min_element(v.begin(), v.end());
    float maxAbs = qMax(qAbs(max), qAbs(min));

    for (float& element : v)
        element /= maxAbs;
}

// Konwolucja sygnałów
template<typename T>
QVector<T> HilbertTransform::conv(const QVector<T>& f, const QVector<T>& g) const {
    int nf = f.size();
    int ng = g.size();
    int n = nf + ng - 1;

    QVector<T> out(n, T());
    for (int i = 0; i < n; ++i) {
        int jmin = (i >= ng - 1) ? i - (ng - 1) : 0;
        int jmax = (i < nf - 1) ? i : nf - 1;

        for (int j = jmin; j <= jmax; ++j)
            out[i] += f[j] * g[i - j];
    }

    return out;
}

// Filtracja sygnału
QVector<float> HilbertTransform::filter(const QVector<float>& signal, float fc1, float fc2) const {
    const float M = 5.0;
    const int N = 2 * M + 1;

    QVector<float> n1;
    for (int i = -M; i <= M; ++i)
        n1.append(i);

    float fc = fc1 / (m_fs / 2);
    QVector<float> y1, y2;

    for (float value : n1) {
        float result = (qFuzzyCompare(value, 0)) ? 2 * fc : qSin(2 * M_PI * fc * value) / (M_PI * value);
        y1.append(result);
    }

    fc = fc2 / (m_fs / 2);
    for (float value : n1) {
        float result = (qFuzzyCompare(value, 0)) ? 1 - (2 * fc) : -qSin(2 * M_PI * fc * value) / (M_PI * value);
        y2.append(result);
    }

    QVector<float> window(N);
    for (int i = 0; i < N; ++i)
        window[i] = 0.54 - 0.46 * qCos(2 * M_PI * i / (N - 1));

    for (int i = 0; i < y1.size(); ++i) {
        y1[i] *= window[i];
        y2[i] *= window[i];
    }

    QVector<float> c1 = conv(y1, signal);
    normalize(c1);

    QVector<float> c2 = conv(y2, c1);
    normalize(c2);

    return c2;
}

// (Root Mean Square) do analizy amplitudy
// Bardziej precyzyjne wyodrębnienie rzeczywistych pików R z sygnału EKG, ignorując przy tym niższe wartości, które mogą pochodzić z szumów
float HilbertTransform::calcRMSValue(const QVector<float>& signal) const {
    float rms = 0.0;
    for (float x : signal)
        rms += x * x;

    return qSqrt(rms / signal.size());
}

// Różniczkowanie
QVector<float> HilbertTransform::derivative(const QVector<float>& signal) const {
    QVector<float> deriv;
    for (int i = 1; i < signal.size() - 1; ++i)
        deriv.append(1.0 / (2 * m_fs) * (signal[i + 1] - signal[i]));

    return deriv;
}

// Transformata Hilberta przy pomocy biblioteki FFTW
QVector<float> HilbertTransform::computeHilbertTransform(const QVector<float>& signal, int first) const {
    int N = signal.size();
    QVector<float> hilbert(N);

    // Przygotowanie danych
    fftwf_complex *in = (fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex) * N);
    fftwf_complex *out = (fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex) * N);
    fftwf_plan plan = fftwf_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

    // Wypełnianie danych wejściowych
    for (int i = 0; i < N; ++i) {
        in[i][0] = signal[i]; // Część rzeczywista
        in[i][1] = 0.0;       // Część urojona
    }

    // FFT
    fftwf_execute(plan);

    // Modyfikacja wyników FFT zgodnie z transformacją Hilberta
    for (int i = 0; i < N; ++i) {
        if (i == 0 || i == N / 2) {
            out[i][1] = 0;
        } else if (i < N / 2) {
            out[i][1] = out[i][1];
        } else {
            out[i][1] = -out[i][1];
        }
        hilbert[i] = out[i][0];
    }

    fftwf_destroy_plan(plan);
    fftwf_free(in);
    fftwf_free(out);

    return hilbert;
}

// Średnia odległość między wykrytymi szczytami
int HilbertTransform::calcAverageDistance(const QVector<int>& peaks) const {
    float distance = 0.0;
    for (int i = 1; i < peaks.size(); ++i)
        distance += (peaks[i] - peaks[i - 1]);

    return distance / (peaks.size() - 1);
}


QVector<int> HilbertTransform::getPeaks(std::shared_ptr<const QVector<float>> electrocardiogram_signal, int fs) {
    m_fs = fs;
    // Filtrowanie sygnału (interesujace nas czestotliwosci)
    QVector<float> signal = filter(*electrocardiogram_signal, 8, 20);
    signal.remove(0, 9); // przesunięcie spowodowane filtracją

    // Różniczkowanie
    signal = this->derivative(signal);

    int i = 0;
    float max_amplitude_old = 0.0;
    QVector<int> peaks;
    QVector<int> windows;

    // Wykrywanie pików
    while ((i + m_kSize) < signal.size()) {
        windows.push_back(i);
        QVector<float> signal_hilb = computeHilbertTransform(signal, i);
        float rms_value = calcRMSValue(signal_hilb);
        auto max_amplitude = *std::max_element(signal_hilb.begin(), signal_hilb.end());
        float threshold;

        // Próg detekcji
        if (rms_value >= 0.18 * max_amplitude) {
            threshold = 0.39 * max_amplitude;
            if (max_amplitude > 2 * max_amplitude_old)
                threshold = 0.0039 * max_amplitude_old;
        } else {
            // rms_value < 0.18 * rms_value
            threshold = 1.6 * rms_value;
        }

        // Sprawdzenie, czy wartość maksymalnej amplitudy przekracza próg
        if (max_amplitude < threshold) {
            qDebug() << "Peak was not found";
            continue;
        }

        float maximum = 0.0;
        float maximum_val = 0.0;

        // Szukanie lokalnych maksimów w sygnale po TH
        for (int j = 0; j < signal_hilb.size(); j++) {
            if (signal_hilb[j] > threshold && signal_hilb[j] > maximum_val) {
                maximum = j;
                maximum_val = signal_hilb[j];
            }
            if (signal_hilb[j] < maximum_val && maximum_val > 0)
                break;
        }

        int peak_current = i + maximum;

        // Odległości między pikami
        if (peaks.size() >= 3 && peak_current - peaks.back() < 0.2 * m_fs) {
            int peak_last = peaks.back();
            peaks.pop_back();
            int average_distance = calcAverageDistance(peaks);
            int peak_second_to_last = peaks.back();

            int dist1 = qAbs(average_distance - (peak_second_to_last - peak_last));
            int dist2 = qAbs(average_distance - (peak_second_to_last - peak_current));
            if (dist1 <= dist2) {
                peaks.push_back(peak_last);
            } else {
                peaks.push_back(peak_current);
            }
        } else {
            peaks.push_back(peak_current);
        }

        i = peak_current + 1;
        max_amplitude_old = max_amplitude;
    }

    // Zwracanie wektora z indeksami wykrytych pików
    return std::move(peaks);
}
