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

    // 读取图像到直方图
    virtual bool readImage(QImage &image) = 0;

    // 直方图均衡化
    virtual bool equalization(int (&equalizationRes)[COLOR_DIMENSIONS][HISTOGRAM_DIMENSIONS], int tot) = 0;
protected:
    // 直方图
    int data[COLOR_DIMENSIONS][HISTOGRAM_DIMENSIONS];

    // 直方图中第一个（从左往右）非零的列。在readImage()中被赋值
    int left[COLOR_DIMENSIONS];
};

#endif // HISTOGRAMBASE_H
