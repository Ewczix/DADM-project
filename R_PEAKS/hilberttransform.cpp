#include "HilbertTransform.h"
#include <iostream>
#include <fstream>
#include <QDebug>
#include <QVector>
#include <vector>
#include <algorithm>
#include <gsl/gsl_fft_complex.h>
#include <cmath>
#include <gsl/gsl_complex_math.h>
#include <numeric>
#include <boost/math/special_functions.hpp>
#define _USE_MATH_DEFINES
#include <cmath>
using namespace std;

// Normalizacja wektora sygnału, skalująca wartości do przedziału [-1, 1]
void HilbertTransform::Normalize(QVector<float>& v) const
{
    float MAX = *std::max_element(v.begin(), v.end());
    float MIN = *std::min_element(v.begin(), v.end());
    float MAX_ABS = (abs(MAX) >= abs(MIN)) ? abs(MAX) : abs(MIN);

    std::for_each(v.begin(), v.end(), [MAX_ABS](float& element) {
        element = element / MAX_ABS;
    });
}

// Konwolucjw 2 sygnałów
template <typename T>
QVector<T> HilbertTransform::Conv(const QVector<T>& f, const QVector<T>& g) const
{
    int const nf = f.size();
    int const ng = g.size();
    int const n = nf + ng - 1;

    QVector<T> out(n, T());

    for (int i = 0; i < n; ++i)
    {
        int const jmn = (i >= ng - 1) ? i - (ng - 1) : 0;
        int const jmx = (i < nf - 1) ? i : nf - 1;

        for (int j = jmn; j <= jmx; ++j)
        {
            out[i] += (f[j] * g[i - j]);
        }
    }
    return out;
}

// Silnia
float HilbertTransform::Factorial(int n) const
{
    if ((n == 0) || (n == 1))
        return 1;
    else
        return n * Factorial(n - 1);
}

// Filtracja sygnału
QVector<float> HilbertTransform::Filter(const QVector<float>& signal, float fc1, float fc2) const
{
    const float M = 5.0;
    const float N = 2 * M + 1;
    QVector<float> n1;
    for (int i = -M; i <= M; i++)
        n1.push_back(i);

    float fc = fc1 / (m_fs / 2); // Normalizacja częstotliwości granicznej

    // Odpowiedź impulsowa dla filtra dolnoprzepustowego
    QVector<float> y1;
    for (float value : n1)
    {
        float result;
        if ((M_PI * value) != 0)
            result = sin(2 * M_PI * fc * value) / (M_PI * value);
        else
            result = 2 * fc;
        y1.push_back(result);
    }

    // Odpowiedź impulsowa dla filtra górnoprzepustowego
    QVector<float> y2;
    fc = fc2 / (m_fs / 2);
    for (float value : n1)
    {
        float result;
        if ((M_PI * value) != 0)
            result = -sin(2 * M_PI * fc * value) / (M_PI * value);
        else
            result = 1 - (2 * fc);
        y2.push_back(result);
    }

    QVector<float> n(N); // Tworzenie okna Kaisera
    std::iota(n.begin(), n.end(), 0);

    const float alpha = 3; // Parametr Kaisera wynoszący 3
    QVector<float> window;

    for (float n_value : n)
    {
        float ans = pow(2 * n_value / N - 1, 2);
        ans = M_PI * alpha * sqrt(1 - ans);
        ans = boost::math::cyl_bessel_i(0, ans) / boost::math::cyl_bessel_i(0, M_PI * alpha);
        window.push_back(ans);
    }

    // Mnożenie odpowiedzi impulsowych przez okno Kaisera
    for (int i = 0; i < y1.size(); i++)
    {
        y1[i] = window[i] * y1[i];
        y2[i] = window[i] * y2[i];
    }

    // Konwolucja z sygnałem wejściowym
    QVector<float> c1 = Conv(y1, signal);
    Normalize(c1);
    QVector<float> c2 = Conv(y2, c1);
    Normalize(c2);
    return c2;
}

// Obliczenie wartości RMS (Root Mean Square) dla danego sygnału
float HilbertTransform::CalcRMSValue(QVector<float>& signal) const
{
    float rms = 0.f;
    std::for_each(signal.begin(), signal.end(), [&rms](float x) {
        rms += x * x;
    });
    if (signal.size() == 0)
        return 0;
    return sqrt(rms / signal.size());
}

// Różniczkowanie sygnału
QVector<float> HilbertTransform::Derivative(QVector<float>& signal) const
{
    QVector<float> deriv;
    for (int i = 1; i < signal.size() - 1; i++)
    {
        float acc = 1. / (2 * m_fs) * (signal[i + 1] - signal[i]);
        deriv.push_back(acc);
    }
    return deriv;
}

// Implementacja transformaty Hilberta za pomocą biblioteki GSL FFT
QVector<float> HilbertTransform::ComputeHilbertTransform(QVector<float> signal, int first) const
{
#define REAL(z, i) ((z)[2*(i)])
#define IMAG(z, i) ((z)[2*(i)+1])

    double data[2 * m_kSize];

    // Przygotowanie danych wejściowych do FFT
    for (int i = 0; i < m_kSize; i++)
    {
        REAL(data, i) = signal[first + i];
        IMAG(data, i) = 0.0;
    }

    // Wykonanie szybkiej transformaty Fouriera
    gsl_fft_complex_radix2_forward(data, 1, m_kSize);

    // Ustawienie składowej stałej na zero
    REAL(data, 0) = 0.0;
    IMAG(data, 0) = 0.0;

    // Modyfikacja transformaty w celu uzyskania transformaty Hilberta
    for (int i = 0; i < m_kSize / 2; i++)
    {
        gsl_complex a, b;
        GSL_REAL(a) = REAL(data, i);
        GSL_IMAG(a) = IMAG(data, i);
        GSL_REAL(b) = 0.0;
        GSL_IMAG(b) = -1.0;

        gsl_complex c = gsl_complex_mul(a, b);
        REAL(data, i) = GSL_REAL(c);
        IMAG(data, i) = GSL_IMAG(c);
    }

    for (int i = m_kSize / 2; i < m_kSize; i++)
    {
        gsl_complex a, b;
        GSL_REAL(a) = REAL(data, i);
        GSL_IMAG(a) = IMAG(data, i);
        GSL_REAL(b) = 0.0;
        GSL_IMAG(b) = 1.0;

        gsl_complex c = gsl_complex_mul(a, b);
        REAL(data, i) = GSL_REAL(c);
        IMAG(data, i) = GSL_IMAG(c);
    }

    // Odwrócenie transformaty Fouriera
    gsl_fft_complex_radix2_inverse(data, 1, m_kSize);

    QVector<float> h;
    for (int i = 0; i < m_kSize; i++)
        h.push_back(REAL(data, i));

#undef REAL
#undef IMAG
    return h;
}

// Obliczenie średniej odległości pomiędzy wykrytymi pikami
int HilbertTransform::CalcAverageDistance(QVector<int>& peaks) const
{
    float distance = 0.0;
    for (int i = 1; i < peaks.size(); i++)
        distance += (peaks[i] - peaks[i - 1]);
    return distance / (peaks.size() - 1);
}

// Wykrywanie pików w sygnale EKG
// Parametry wejściowe to sygnał oraz częstotliwość próbkowania (fs)
QVector<int> HilbertTransform::GetPeaks(QVector<float> electrocardiogram_signal, int fs)
{
    m_fs = fs;

    // Filtrowanie sygnału EKG w celu usunięcia szumów
    QVector<float> signal = Filter(electrocardiogram_signal, 8, 20);
    signal.erase(signal.begin(), signal.begin() + 9); // Usunięcie przesunięcia spowodowanego filtrem
    signal = Derivative(signal);

    int i = 0;
    float max_amplitude_old = 0.0;
    QVector<int> peaks;
    QVector<int> windows;

    while ((i + m_kSize) < signal.size())
    {
        windows.push_back(i);
        QVector<float> signal_hilb = ComputeHilbertTransform(signal, i);
        float rms_value = CalcRMSValue(signal_hilb);
        auto max_amplitude = std::max_element(signal_hilb.begin(), signal_hilb.end());
        float threshold;

        // Ustalanie progu detekcji
        if (rms_value >= 0.18 * *max_amplitude)
        {
            threshold = 0.39 * *max_amplitude;
            if (*max_amplitude > 2 * max_amplitude_old)
                threshold = 0.0039 * max_amplitude_old;
        }
        else
        {
            threshold = 1.6 * rms_value;
        }

        // Sprawdzenie, czy wartość maksymalnej amplitudy przekracza ustalony próg
        if (*max_amplitude < threshold)
        {
            std::cout << "Peak was not found" << std::endl;
            i += m_kSize;
            continue;
        }

        float maximum = 0.0;
        float maximum_val = 0.0;

        // Szukanie lokalnego maksimum w transformacie Hilberta
        for (int j = 0; j < signal_hilb.size(); j++)
        {
            if (signal_hilb[j] > threshold && signal_hilb[j] > maximum_val)
            {
                maximum = j;
                maximum_val = signal_hilb[j];
            }
            if (signal_hilb[j] < maximum_val && maximum_val > 0)
                break;
        }

        int peak_current = i + maximum;

        // Weryfikacja odległości między pikami
        if (peaks.size() >= 3 && peak_current - peaks.back() < 0.2 * m_fs)
        {
            int peak_last = peaks.back();
            peaks.pop_back();

            QVector<int> peaksVector = peaks;
            int average_distance = CalcAverageDistance(peaksVector);
            int peak_second_to_last = peaks.back();

            int dist1 = abs(average_distance - (peak_second_to_last - peak_last));
            int dist2 = abs(average_distance - (peak_second_to_last - peak_current));

            if (dist1 <= dist2)
            {
                peaks.push_back(peak_last);
            }
            else
            {
                peaks.push_back(peak_current);
            }
        }
        else
        {
            peaks.push_back(peak_current);
        }

        i = peak_current + 1;
        max_amplitude_old = *max_amplitude;
    }

    qDebug() << "Peaks:" << peaks;
    return peaks;
}
