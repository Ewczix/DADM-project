#include <QApplication>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QScatterSeries>
#include <QtCore/QTextStream>
#include <QtCore/QFile>
#include <QtMath>
#include <QtGlobal>
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
QT_USE_NAMESPACE

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

Waves::Waves(float fs) : m_fs(fs) {}

QVector<int> Waves::getPeaks(QSharedPointer<const QVector<float>> ecg, float fs) {
    QVector<int> r_peaks;
    const float threshold = 0.2f;
    const int minDistance = static_cast<int>(0.2f * fs);
    int lastPeakIndex = -minDistance;

    for (int i = 1; i < ecg->size() - 1; ++i) {
        float left = (*ecg)[i - 1];
        float current = (*ecg)[i];
        float right = (*ecg)[i + 1];

        if (current > left && current > right && current > threshold) {
            if (i - lastPeakIndex >= minDistance) {
                r_peaks.push_back(i);
                lastPeakIndex = i;
            }
        }
    }
    return r_peaks;
}

QVector<int> Waves::getQRSOnsets(const QVector<float>& signal, const QVector<int>& r_peaks) {
    QVector<int> qrs_onsets;
    qrs_onsets.reserve(r_peaks.size());

    for (int r_idx : r_peaks) {
        int start_idx = std::max(0, r_idx - static_cast<int>(0.15f * m_fs));
        int end_idx = r_idx;

        int onset_idx = end_idx;
        float max_negative_slope = 0.0f;

        for (int i = end_idx - 1; i > start_idx; --i) {
            float slope = signal[i] - signal[i - 1];
            if (slope < max_negative_slope) {
                max_negative_slope = slope;
                onset_idx = i;
            }
        }

        qrs_onsets.push_back(onset_idx);
    }

    return qrs_onsets;
}

QVector<int> Waves::getQRSOffsets(const QVector<float>& signal, const QVector<int>& r_peaks) {
    QVector<int> qrs_offsets;
    qrs_offsets.reserve(r_peaks.size());

    for (int r_idx : r_peaks) {
        int start_idx = std::min<int>(r_idx + static_cast<int>(0.2f * m_fs), static_cast<int>(signal.size()) - 1);
        int end_idx = std::min<int>(r_idx + static_cast<int>(0.4f * m_fs), static_cast<int>(signal.size()) - 1);

        auto max_it = std::max_element(signal.begin() + start_idx, signal.begin() + end_idx);
        qrs_offsets.push_back(std::distance(signal.begin(), max_it));
    }
    return qrs_offsets;
}

QVector<int> Waves::getPonsets(const QVector<float>& signal, const QVector<int>& r_peaks) {
    QVector<int> p_onsets;
    p_onsets.reserve(r_peaks.size());

    for (int r_idx : r_peaks) {
        int window_end = std::max(0, r_idx - static_cast<int>(0.08f * m_fs));
        int window_start = std::max(0, r_idx - static_cast<int>(0.3f * m_fs));

        int onset_idx = window_end;
        float threshold_slope = 0.01f;
        bool found_onset = false;

        for (int i = window_end - 1; i > window_start; --i) {
            float slope = signal[i] - signal[i - 1];
            if (slope < threshold_slope) {
                onset_idx = i;
                found_onset = true;
            } else if (found_onset) {
                onset_idx = i;
                break;
            }
        }

        p_onsets.push_back(onset_idx);
    }

    return p_onsets;
}

QVector<QPair<int, int>> Waves::getTandTends(const QVector<float>& signal, const QVector<int>& r_peaks) {
    QVector<QPair<int, int>> t_locs_and_tends;
    t_locs_and_tends.reserve(r_peaks.size());

    for (int r_idx : r_peaks) {
        int start_idx = r_idx + static_cast<int>(0.2f * m_fs);
        int end_idx = std::min(r_idx + static_cast<int>(0.9f * m_fs), static_cast<int>(signal.size() - 1));

        auto max_it = std::max_element(signal.begin() + start_idx, signal.begin() + end_idx);
        int t_peak_idx = std::distance(signal.begin(), max_it);

        int tend_idx = t_peak_idx;
        for (int i = t_peak_idx; i < signal.size() - 1; ++i) {
            if (signal[i] < signal[tend_idx]) {
                tend_idx = i;
            }
            if (signal[i] < signal[i + 1]) {
                break;
            }
        }

        t_locs_and_tends.push_back(qMakePair(t_peak_idx, tend_idx));
    }

    return t_locs_and_tends;
}

QVector<float> loadECGFromFile(const QString& fileName) {
    QVector<float> ecg_signal;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Nie można otworzyć pliku:" << fileName;
        return ecg_signal;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList values = line.split(",");
        for (const QString& value : values) {
            bool ok;
            float val = value.toFloat(&ok);
            if (ok) {
                ecg_signal.append(val);
            }
        }
    }
    file.close();

    return ecg_signal;
}

void displayChart(const QVector<float>& ecg_signal,
                  const QVector<int>& qrs_onsets,
                  const QVector<int>& qrs_offsets,
                  const QVector<int>& p_onsets,
                  const QVector<int>& t_locs,
                  const QVector<int>& t_ends) {
    QChart* chart = new QChart();
    QLineSeries* ecg_series = new QLineSeries();

    // Dodaj sygnał EKG jako linię
    for (int i = 0; i < ecg_signal.size(); ++i) {
        ecg_series->append(i, ecg_signal[i]);
    }
    chart->addSeries(ecg_series);
    chart->createDefaultAxes();
    chart->setTitle("Analiza sygnału EKG");

    // Dodajemy funkcję pomocniczą do wykreślania punktów
    auto addScatterSeries = [&](const QVector<int>& indices, const QString& name, QColor color, double size) {
        QScatterSeries* scatter_series = new QScatterSeries();
        scatter_series->setName(name);
        scatter_series->setColor(color);
        scatter_series->setMarkerSize(size);
        for (int idx : indices) {
            scatter_series->append(idx, ecg_signal[idx]);
        }
        chart->addSeries(scatter_series);
        scatter_series->attachAxis(chart->axisX());
        scatter_series->attachAxis(chart->axisY());
    };

    // Dodajemy oznaczenia
    addScatterSeries(qrs_onsets, "QRS Onsets", Qt::blue, 10.0);
    addScatterSeries(qrs_offsets, "QRS Offsets", Qt::magenta, 10.0);
    addScatterSeries(p_onsets, "P Onsets", Qt::green, 8.0);
    addScatterSeries(t_locs, "T Peaks", Qt::yellow, 8.0);
    addScatterSeries(t_ends, "T Ends", Qt::red, 8.0);

    QChartView* chart_view = new QChartView(chart);
    chart_view->setRenderHint(QPainter::Antialiasing);
    chart_view->show();
}

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    QVector<float> ecg_signal = loadECGFromFile("ecg.csv");
    if (ecg_signal.isEmpty()) {
        qWarning() << "Nie można załadować sygnału EKG.";
        return -1;
    }

    float fs = 500.0f;
    Waves waves(fs);

    QSharedPointer<const QVector<float>> signalPtr = QSharedPointer<const QVector<float>>::create(ecg_signal);
    QVector<int> r_peaks = waves.getPeaks(signalPtr, fs);
    QVector<int> qrs_onsets = waves.getQRSOnsets(ecg_signal, r_peaks);
    QVector<int> qrs_offsets = waves.getQRSOffsets(ecg_signal, r_peaks);
    QVector<int> p_onsets = waves.getPonsets(ecg_signal, r_peaks);
    QVector<QPair<int, int>> t_locs_and_tends = waves.getTandTends(ecg_signal, r_peaks);

    QVector<int> t_locs, t_ends;
    for (const auto& pair : t_locs_and_tends) {
        t_locs.push_back(pair.first);
        t_ends.push_back(pair.second);
    }

    displayChart(ecg_signal, qrs_onsets, qrs_offsets, p_onsets, t_locs, t_ends);

    return app.exec();
}
