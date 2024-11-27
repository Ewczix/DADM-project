#include "hrv1.h"
#include <cmath>
#include <numeric>
#include <algorithm>

HRV1::HRV1():
    time_parameters(),
    freq_parameters()
{}

// Analiza w dziedzinie czasu
void HRV1::analyzeTimeDomain(const QVector<int>& peaks, int fs) {
    // Przeliczenie indeksów na odstępy RR w ms
    QVector<double> RR_intervals(peaks.size() - 1);
    for (int i = 1; i < peaks.size(); ++i) {
        RR_intervals[i - 1] = (peaks[i] - peaks[i - 1]) * (1000.0 / fs);
    }

    // Stworzenie wektora zawierającego różnice pomiędzy kolejnymi odstępami
    QVector<double> RR_diff(RR_intervals.size() - 1);
    for (int i = 1; i < RR_intervals.size(); ++i) {
        RR_diff[i - 1] = RR_intervals[i] - RR_intervals[i - 1];
    }

    // Obliczenie parametrów HRV w dziedzinie czasu
    double RR_mean = std::accumulate(RR_intervals.begin(), RR_intervals.end(), 0.0) / RR_intervals.size();
    double SDNN = std::sqrt(std::inner_product(RR_intervals.begin(), RR_intervals.end(), RR_intervals.begin(), 0.0) / RR_intervals.size() - RR_mean * RR_mean);
    double RMSSD = std::sqrt(std::accumulate(RR_diff.begin(), RR_diff.end(), 0.0, [](double acc, double x) { return acc + x * x; }) / RR_diff.size());
    int NN50 = std::count_if(RR_diff.begin(), RR_diff.end(), [](double diff) { return std::abs(diff) > 50; });
    double pNN50 = (static_cast<double>(NN50) / RR_diff.size()) * 100.0;

    // Zapis parametrów
    time_parameters["RR_mean"] = RR_mean;
    time_parameters["SDNN"] = SDNN;
    time_parameters["RMSSD"] = RMSSD;
    time_parameters["NN50"] = static_cast<double>(NN50);
    time_parameters["pNN50"] = pNN50;
}

// Analiza w dziedzinie częstotliwości
void HRV1::analyzeFreqDomain(const QVector<int>& peaks, int fs) {
    // Przeliczenie indeksów na odstępy RR w s
    QVector<double> RR_intervals(peaks.size() - 1);
    for (int i = 1; i < peaks.size(); ++i) {
        RR_intervals[i - 1] = (peaks[i] - peaks[i - 1]) / static_cast<double>(fs);
    }

    // Stworzenie wektora czasu
    QVector<double> t_k(RR_intervals.size());
    std::partial_sum(RR_intervals.begin(), RR_intervals.end(), t_k.begin());

    // Obliczenie liczby próbek
    int samples = t_k.size();

    double freq_res = samples /  t_k.last();

    QVector<double> freqs(samples);

    double step = (freq_res - 0.001) / (samples - 1);

    for (int i = 0; i < samples; ++i) {
        freqs[i] = 0.001 + i * step;
    }

    // Periodogram Lomb-Scargle
    double sum_num = 0.0, sum_den = 0.0;
    for (int i = 0; i < t_k.size(); ++i) {
        double num = std::sin(4 * M_PI * freqs[i] * t_k[i]);
        sum_num += num;
        double den = std::cos(4 * M_PI * freqs[i] * t_k[i]);
        sum_den += den;
    }

    QVector<double> P_LS(samples, 0.0);
    for (int i = 0; i < samples; ++i) {
        double f = freqs[i];

        double tau = 1.0 / (4 * M_PI * f) * std::atan(sum_num / sum_den);

        double sum_cos = 0.0, sum_sin = 0.0, sum_cos_sq = 0.0, sum_sin_sq = 0.0;
        for (int j = 0; j < t_k.size(); ++j) {
            double cos_term = std::cos(2 * M_PI * f * (t_k[j] - tau));
            double sin_term = std::sin(2 * M_PI * f * (t_k[j] - tau));
            sum_cos += RR_intervals[j] * cos_term;
            sum_sin += RR_intervals[j] * sin_term;
            sum_cos_sq += cos_term * cos_term;
            sum_sin_sq += sin_term * sin_term;
        }

        P_LS[i] = 0.5 * ((sum_cos * sum_cos) / sum_cos_sq + (sum_sin * sum_sin) / sum_sin_sq);
    }

    // Obliczenie parametrów HRV w dziedzinie częstotliwości
    double ULF = 0.0, VLF = 0.0, LF = 0.0, HF = 0.0;
    for (int i = 0; i < samples; ++i) {
        if (freqs[i] > 0.0 && freqs[i] <= 0.0033) ULF += P_LS[i];
        else if (freqs[i] > 0.0033 && freqs[i] <= 0.04) VLF += P_LS[i];
        else if (freqs[i] > 0.04 && freqs[i] <= 0.15) LF += P_LS[i];
        else if (freqs[i] > 0.15 && freqs[i] <= 0.4) HF += P_LS[i];
    }
    double TP = ULF + VLF + LF + HF;
    double LFHF = LF / HF;

    // Zapis parametrów
    freq_parameters["ULF"] = ULF;
    freq_parameters["VLF"] = VLF;
    freq_parameters["LF"] = LF;
    freq_parameters["HF"] = HF;
    freq_parameters["TP"] = TP;
    freq_parameters["LFHF"] = LFHF;
}

// Pobranie parametrów czasowych
QMap<QString, double> HRV1::getTimeParameters() const {
    return {
        time_parameters
    };
}

// Pobranie parametrów częstotliwościowych
QMap<QString, double> HRV1::getFreqParameters() const {
    return {
        freq_parameters
    };
}
