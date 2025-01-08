#ifndef ECG_ANALYZER_HILBERTTRANSFORM_H
#define ECG_ANALYZER_HILBERTTRANSFORM_H

#include <QVector>

class HilbertTransform
{
public:
    HilbertTransform() = default;

    /*
     * @brief Funkcja do znajdowania szczytów R z wykorzystaniem transformaty Hilberta
     * @param[in] electrocardiogram_signal: sygnał EKG
     * @param[in] fs: częstotliwość próbkowania sygnału EKG (domyślnie = 360 Hz)
     * @return Indeksy szczytów R dla danego sygnału
     */
    QVector<int> GetPeaks(QVector<float> electrocardiogram_signal, int fs = 360);

private:
    static constexpr int m_kSize = 1024; // rozmiar okna w transformacie Hilberta
    int m_fs; // częstotliwość próbkowania sygnału EKG

    /*
     * @brief Obliczenie transformaty Hilberta dla okna o rozmiarze HilbertTransform::m_kSize
     * @param[in] signal: sygnał EKG
     * @param[in] first: Pierwszy indeks do rozpoczęcia
     * @return Wynik transformaty Hilberta
     */
    QVector<float> ComputeHilbertTransform(QVector<float> signal, int first) const;

    /*
     * @brief Obliczenie pochodnej sygnału EKG
     * @param[in, out] signal: sygnał EKG
     * @return Pochodna sygnału
     */
    QVector<float> Derivative(QVector<float>& signal) const;

    /*
     * @brief Zastosowanie filtru pasmowo-przepustowego na sygnale EKG
     * @param[in] signal: sygnał EKG
     * @param[in] fc1: Dolna częstotliwość odcięcia
     * @param[in] fc2: Górna częstotliwość odcięcia
     * @return Przefiltrowany sygnał
     */
    QVector<float> Filter(const QVector<float>& signal, float fc1, float fc2) const;

    /*
     * @brief Obliczenie średniej odległości między szczytami
     * @param[in] peaks: Indeksy szczytów
     * @return Średnia odległość między szczytami
     */
    int CalcAverageDistance(QVector<int>& peaks) const;

    /*
     * @brief Obliczenie wartości RMS sygnału EKG
     * @param[in] signal: sygnał EKG
     * @return Wartość RMS sygnału
     */
    float CalcRMSValue(QVector<float>& signal) const;

    /*
     * @brief Normalizacja sygnału EKG
     * @param[in, out] v: sygnał EKG
     */
    void Normalize(QVector<float>& v) const;

    /*
     * @brief Wykonanie operacji konwolucji na dwóch sygnałach
     * @param[in] f: Pierwszy sygnał
     * @param[in] g: Drugi sygnał
     * @return Wynik konwolucji
     */
    template <typename T>
    QVector<T> Conv(const QVector<T>& f, const QVector<T>& g) const;

    /*
     * @brief Obliczenie silni liczby
     * @param[in] n: Liczba, dla której ma być obliczona silnia
     * @return Silnia liczby n
     */
    float Factorial(int n) const;
};

#endif // ECG_ANALYZER_HILBERTTRANSFORM_H
