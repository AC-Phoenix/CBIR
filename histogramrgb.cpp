#include "histogramrgb.h"
#include <cstdlib>

HistogramRGB::HistogramRGB()
{
    memset(data, 0, sizeof(data));
}

HistogramRGB::~HistogramRGB()
{

}

// 读取图像到直方图
bool HistogramRGB::readImage(QImage &image)
{
    int height = image.height();
    int width  = image.width();
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            QColor color   = image.pixel(x, y);
            int redLevel   = color.red();
            int greenLevel = color.green();
            int blueLevel  = color.blue();

            // 统计rgb的各等级出现的次数。
            ++data[RED][redLevel];
            ++data[GREEN][greenLevel];
            ++data[BLUE][blueLevel];
        }
    }

    // 计算left[COLOR_DIMENSIONS]的值
    left[RED]   = -1;
    left[GREEN] = -1;
    left[BLUE]  = -1;
    for (int i = 0; i < COLOR_DIMENSIONS; ++i)
    {
        if (-1 == left[RED] && data[RED][i])
        {
            left[RED] = i;
        }
        if (-1 == left[GREEN] && data[GREEN][i])
        {
            left[GREEN] = i;
        }
        if (-1 == left[BLUE] && data[BLUE][i])
        {
            left[BLUE] = i;
        }
    }

    return true;
}

// 直方图均衡化
bool HistogramRGB::equalization(int (&equalizationRes)[COLOR_DIMENSIONS][HISTOGRAM_DIMENSIONS], int tot)
{
    int sum[COLOR_DIMENSIONS] = {0};
    int L = (1 << 8);
    for (int i = 0; i < HISTOGRAM_DIMENSIONS; ++i)
    {
        if (i > left[RED])
        {
            sum[RED] += data[RED][i];
            int denominator = tot-data[RED][left[RED]];
            equalizationRes[RED][i] = (0 == denominator) ? L-1 : (double)sum[RED] / denominator * (L-1);
        }
        if (i > left[GREEN])
        {
            sum[GREEN] += data[GREEN][i];
            int denominator = tot-data[GREEN][left[GREEN]];
            equalizationRes[GREEN][i] = (0 == denominator) ? L-1 : (double)sum[GREEN] / denominator * (L-1);
        }
        if (i > left[BLUE])
        {
            sum[BLUE] += data[BLUE][i];
            int denominator = tot-data[BLUE][left[BLUE]];
            equalizationRes[BLUE][i] = (0 == denominator) ? L-1 : (double)sum[BLUE] / denominator * (L-1);
        }
    }
    return true;
}

