#include "ecg_baseline.h"

#include <QVector>

ECG_Baseline::ECG_Baseline() : filter(nullptr){}

void ECG_Baseline::setFilter(Filter* choice) {
    filter = choice;
}

void ECG_Baseline::ProcessData(ECG_Data full_data) {

    if (!filter) {
        qWarning("Nie wybrano filtra");
        return;
    }

    QVector<double> data = full_data.raw_data;
    full_data.processed_data = filter->applyFilter(data);
}
