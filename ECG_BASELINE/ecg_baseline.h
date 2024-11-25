#ifndef ECG_BASELINE_H
#define ECG_BASELINE_H

#include "filter.h"
//#include "ecg_data.h" <--- bez zakomentowania jest błąd i plik .cpp nie załącza .h

#include <QObject>

class ECG_Baseline
{
    Q_OBJECT

    Filter* filter;

public:
    ECG_Baseline();

    void setFilter(Filter* choice);
    //void ProcessData(ECG_Data full_data); <------------
    void ProcessData(); // do usunięcia
};

#endif // ECG_BASELINE_H
