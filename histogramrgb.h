#ifndef HISTOGRAMRGB_H
#define HISTOGRAMRGB_H

#include "histogrambase.h"

class HistogramRGB : public HistogramBase
{
public:
    HistogramRGB();
    ~HistogramRGB();

    // 读取信息到直方图
    bool readImage(QImage &image);

    // 直方图均衡化
    bool equalization(int (&equalizationRes)[COLOR_DIMENSIONS][HISTOGRAM_DIMENSIONS], int tot);
};

#endif // HISTOGRAMRGB_H
