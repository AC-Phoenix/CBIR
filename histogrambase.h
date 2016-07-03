#ifndef HISTOGRAMBASE_H
#define HISTOGRAMBASE_H

#include "imagehandler.h"

const int HISTOGRAM_DIMENSIONS = 361;
const int HISTOGRAM_OFFSET = 1;

class HistogramBase
{
public:
    HistogramBase();
    ~HistogramBase();

    int data[COLOR_DIMENSIONS][HISTOGRAM_DIMENSIONS];
};

#endif // HISTOGRAMBASE_H
