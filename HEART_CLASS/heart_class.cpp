#include "heart_class.h"
#include "./ui_heart_class.h"
#include <QVector>
#include <QtMath>
#include <numeric>
#include <tuple>
#include <limits>

HEART_CLASS::HEART_CLASS(double samplingFrequency, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::HEART_CLASS)
    , samplingRate(samplingFrequency)
{
    ui->setupUi(this);
}

HEART_CLASS::~HEART_CLASS()
{
    delete ui;
}

// Oblicza długość początkową na podstawie częstotliwości próbkowania
unsigned int HEART_CLASS::calculateStartingLength(double samplingFrequency) {
    double restingHeartRate = 60.0;
    double cardiacCycle = 60.0 / restingHeartRate;
    double systole = cardiacCycle / 3.0;
    return qRound(samplingFrequency * systole);
}

// Ekstrakcja podciągów o długości `m` z sygnału
QVector<QVector<double>> HEART_CLASS::extractSubsequences(const QVector<double>& signal, int length) {
    QVector<QVector<double>> subsequences;
    unsigned int n = signal.size();
    unsigned int subsequencesNum = (n-(n%length))/length;
    for (int i = 0; i < subsequencesNum; ++i) {
        subsequences.append(QVector<double>(signal.mid(i*length, length)));
    }
    return subsequences;
}

// Normalizacja Z wymagana do przeprowadzenia poprawnej analizy
QVector<QVector<double>> HEART_CLASS::zNormalize(const QVector<QVector<double>>& subsequences) {
    QVector<QVector<double>> normalized;
    for (const auto& seq : subsequences) {
        double mean = std::accumulate(seq.begin(), seq.end(), 0.0) / seq.size();
        double sqSum = std::inner_product(seq.begin(), seq.end(), seq.begin(), 0.0);
        double stdDev = qSqrt(sqSum / seq.size() - mean * mean);
        QVector<double> normSeq;
        for (double val : seq) {
            normSeq.append((val - mean) / stdDev);
        }
        normalized.append(normSeq);
    }
    return normalized;
}

// Rozpoznawanie dwóch najbardziej reprezentatywnych wycinków z całego sygnału
std::tuple<QVector<double>, QVector<double>, double> HEART_CLASS::findMostSimilarPair(const QVector<QVector<double>>& subsequences) {
    double minDist = std::numeric_limits<double>::max();
    QVector<double> sequenceA, sequenceB;
    for (int i = 0; i < subsequences.size()-2; ++i) {
        for (int j = i + 1; j < subsequences.size()-1; ++j) {
            double dist = 0.0;
            for (int k = 0; k < subsequences[i].size()-1; ++k) {
                dist += qPow(subsequences[i][k] - subsequences[j][k], 2);
            }
            dist = qSqrt(dist);
            if (dist < minDist) {
                minDist = dist;
                sequenceA = subsequences[i];
                sequenceB = subsequences[j];
            }
        }
    }
    return {sequenceA, sequenceB, minDist};
}

// Obliczanie bitsave
double HEART_CLASS::calculateBitsave(const QVector<double>& A, const QVector<double>& B, double dist) {
    return 1.0 / (1.0 + dist);
}

// Znajdowanie kolejnego sąsiada o najbliższych parametrach
QVector<double> HEART_CLASS::findNextNeighbor(const QVector<double>& signal, const QVector<double>& center, int length) {
    auto subsequences = extractSubsequences(signal, length);
    double minDist = std::numeric_limits<double>::max();
    QVector<double> closest;
    for (const auto& seq : subsequences) {
        double dist = 0.0;
        for (int i = 0; i < seq.size(); ++i) {
            dist += qPow(seq[i] - center[i], 2);
        }
        dist = qSqrt(dist);
        if (dist < minDist) {
            minDist = dist;
            closest = seq;
        }
    }
    return closest;
}

// Dodanie wykrytego podciągu do grupy
HEART_CLASS::Motif HEART_CLASS::addToGroup(const HEART_CLASS::Motif& group, const QVector<double>& nextSequence) {
    HEART_CLASS::Motif updatedGroup = group;
    updatedGroup.bitSave += calculateBitsave(group.center, nextSequence, qSqrt(std::inner_product(
                                                                             group.center.begin(), group.center.end(), nextSequence.begin(), 0.0)));
    for (int i = 0; i < group.center.size()-1; ++i) {
        updatedGroup.center[i] = (group.center[i] + nextSequence[i]) / 2.0;
    }
    return updatedGroup;
}

// Algorytm odkrywania motywu zgodnie z publikacją https://onlinelibrary.wiley.com/doi/10.1155/2015/453214
HEART_CLASS::Motif HEART_CLASS::motifDiscoveryAlgorithm(const QVector<double>& signal, double samplingFrequency) {
    int signalLength = signal.size();
    int startingLength = calculateStartingLength(samplingFrequency);
    double maxBitSave = -std::numeric_limits<double>::infinity();
    Motif motif;

    for (int length = startingLength; length <= signalLength / 2; ++length) {
        auto subsequences = extractSubsequences(signal, length);
        auto normalized = zNormalize(subsequences);
        auto [A, B, dist] = findMostSimilarPair(normalized);

        Motif group;
        group.bitSave = calculateBitsave(A, B, dist);
        group.center = QVector<double>(A.size());
        for (int i = 0; i < A.size(); ++i) {
            group.center[i] = (A[i] + B[i]) / 2.0;
        }

        if (group.bitSave > maxBitSave) {
            maxBitSave = group.bitSave;
            motif = group;
        }

        // while (!A.isEmpty() && !B.isEmpty()) {
        //     auto next = findNextNeighbor(signal, group.center, length);
        //     if (next.isEmpty()) break;

        //     double bs = calculateBitsave(group.center, next, dist);
        //     if (bs > group.bitSave) {
        //         group = addToGroup(group, next);
        //     } else {
        //         break;
        //     }
        // }


    }
    return motif;
}

// Detekcja anomalii na podstawie odległości od rozpoznanego motywu
HEART_CLASS::AnomalyDetectionResult HEART_CLASS::detectAnomalies(const QVector<double>& ecgSignal) {
    HEART_CLASS::AnomalyDetectionResult result;
    HEART_CLASS::Motif motif = HEART_CLASS::motifDiscoveryAlgorithm(ecgSignal, HEART_CLASS::samplingRate);
    result.center = motif.center;
    int motifLength = motif.center.size();
    int numSegments = ecgSignal.size() / motifLength;

    QVector<double> distances;
    for (int i = 0; i < numSegments; ++i) {
        int startIdx = i * motifLength;
        QVector<double> segment(ecgSignal.mid(startIdx, motifLength));

        double distance = 0.0;
        for (int j = 0; j < motif.center.size(); ++j) {
            distance += qPow(segment[j] - motif.center[j], 2);
        }
        distance = qSqrt(distance);
        distances.append(distance);
    }

    double meanDist = std::accumulate(distances.begin(), distances.end(), 0.0) / distances.size();
    double stdDev = qSqrt(std::accumulate(distances.begin(), distances.end(), 0.0,
                                          [meanDist](double acc, double d) {
                                              return acc + qPow(d - meanDist, 2);
                                          }) / distances.size());

    for (int i = 0; i < distances.size(); ++i) {
        if (distances[i] > stdDev + meanDist) {
            int startIdx = i * motifLength;
            QVector<double> segment(ecgSignal.mid(startIdx, motifLength));
            result.anomalies.append(segment);
            result.anomalyIndices.append(startIdx);
        }
    }
    return result;
}

// Generacja wektora czasu
static QVector<double> generateTimeVector(double samplingRate, int n) {
    QVector<double> timeVector(n);
    double step = 1/samplingRate;

    for (int i = 0; i < n; ++i) {
        timeVector[i] = 0 + i * step;
    }
    return timeVector;
}

// Ostateczna klasyfikacja zespołu QRS po usunięciu wykrytych anomalii
void HEART_CLASS::classifyQRS(const QVector<double>& ecgSignal, QVector<int> qrs_onset_idx, QVector<int> qrs_end_idx) {
    int numQrs = qrs_onset_idx.size();
    if (qrs_onset_idx.size() != qrs_end_idx.size()) {
        throw std::invalid_argument("The lengths of qrsStartIndices and qrsEndIndices must match.");
        return;
    }
    QVector<QString> detectionVector(numQrs, "Undefined");
    QVector<double> timeVector = generateTimeVector(HEART_CLASS::samplingRate, ecgSignal.size());
    HEART_CLASS::AnomalyDetectionResult detectedAnomalies = HEART_CLASS::detectAnomalies(ecgSignal);

    for (int i = 0; i < numQrs-1; ++i) {
        bool continueLoop = false;
        for (int k = 0; k < detectedAnomalies.anomalyIndices.size()-1; k++){
            if (qrs_onset_idx[i] < detectedAnomalies.anomalyIndices[k] && qrs_onset_idx[i+1] > detectedAnomalies.anomalyIndices[k]){
                detectionVector[i] = "Anomaly";
                continueLoop = true;
            }
        }
        if (continueLoop) continue;

        // Rozróżnienie po pulsie
        double bpm = 60.0 / (timeVector[qrs_onset_idx[i + 1]] - timeVector[qrs_onset_idx[i]]);
        if (bpm < 100.0) {
            detectionVector[i] = "Undefined";
            continue;
        }

        // Rozróżnienie po czasie trwania QRS
        double qrsDuration = timeVector[qrs_end_idx[i]] - timeVector[qrs_onset_idx[i]];
        if (qrsDuration < 0.12) {
            detectionVector[i] = "Undefined";
            continue;
        }

        // Wyznaczanie amplitudy Vi
        int startIdx = qrs_onset_idx[i];
        int endIdx = startIdx + static_cast<int>(std::round(0.04 / (timeVector[1] - timeVector[0])));
        double baselineVoltageStart = ecgSignal[startIdx];
        double minVoltageStart = *std::min_element(ecgSignal.begin() + startIdx, ecgSignal.begin() + endIdx + 1);
        double maxVoltageStart = *std::max_element(ecgSignal.begin() + startIdx, ecgSignal.begin() + endIdx + 1);

        double Vi = std::abs(baselineVoltageStart - minVoltageStart) + std::abs(minVoltageStart - maxVoltageStart);

        // Wyznaczanie amplitudy Vt
        endIdx = qrs_end_idx[i];
        startIdx = endIdx - static_cast<int>(std::round(0.04 / (timeVector[1] - timeVector[0])));
        double baselineVoltageEnd = ecgSignal[endIdx];
        double minVoltageEnd = *std::min_element(ecgSignal.begin() + startIdx, ecgSignal.begin() + endIdx + 1);
        double maxVoltageEnd = *std::max_element(ecgSignal.begin() + startIdx, ecgSignal.begin() + endIdx + 1);

        double Vt = std::abs(baselineVoltageEnd - minVoltageEnd) + std::abs(minVoltageEnd - maxVoltageEnd);

        // Rozróżnienie w opraciu o amplitudy Vi i Vt
        if (Vi < Vt) {
            detectionVector[i] = "VT";
        } else {
            detectionVector[i] = "SVT";
        }
    }

    HEART_CLASS::classificationResults = detectionVector;
    return;
}

// Zwracanie wektoru klasyfikacji
QVector<QString> HEART_CLASS::getClassifications(){
    return HEART_CLASS::classificationResults;
}
