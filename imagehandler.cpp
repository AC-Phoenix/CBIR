#include "imagehandler.h"
#include "histogramrgb.h"
#include <algorithm>
#include <iostream>

ImageHandler::ImageHandler()
{

}

ImageHandler::~ImageHandler()
{

}

// 定义static成员
QSize ImageHandler::size;

// 中值滤波
void ImageHandler::medianFilter(QImage &image, int D)
{
    // start行、列的负偏移，end行、列的正偏移
    int start = -(D >> 1);
    int end = (D >> 1);
    int width = image.width();
    int height = image.height();

    // tmp临时保存结果
    QImage tmp = image.copy(0, 0, width, height);

    // 依次存放D*D矩阵中的rgb分量
    int **arr = new int* [COLOR_DIMENSIONS];
    for (int i = 0; i < COLOR_DIMENSIONS; ++i)
    {
        arr[i] = new int[D*D];
    }

    for (int y = end; y < height-end; ++y)
    {
        for (int x = end; x < width-end; ++x)
        {
            int tot = 0;

            // 获取D*D矩阵中的rgb分量
            for (int dy = start; dy <= end; ++dy)
            {
                for (int dx = start; dx <= end; ++dx)
                {
                    QColor color    = image.pixel(x+dx, y+dy);
                    arr[RED][tot]   = color.red();
                    arr[GREEN][tot] = color.green();
                    arr[BLUE][tot]  = color.blue();
                    ++tot;
                }
            }

            // 依次计算rgb数组的中位数，结果写入tmp
            for (int i = 0; i < COLOR_DIMENSIONS; ++i)
            {
                std::nth_element(arr[i], arr[i] + (tot>>1), arr[i]+tot);
            }
            QRgb rgb = qRgb(arr[RED][tot>>1], arr[GREEN][tot>>1], arr[BLUE][tot>>1]);
            tmp.setPixel(x, y, rgb);
        }
    }

    // 结果写回image
    image = tmp;
    for (int i = 0; i < COLOR_DIMENSIONS; ++i)
    {
        delete[] arr[i];
    }
    delete[] arr;
}

// 高斯平滑
void ImageHandler::gaussianSmoothing(QImage &image)
{
    // 高斯模板(7*7)，sigma = 0.84089642，归一化后得到
    static const double gaussianTemplate[7][7] =
    {
        {0.00000067, 0.00002292, 0.00019117, 0.00038771, 0.00019117, 0.00002292, 0.00000067},
        {0.00002292, 0.00078633, 0.00655965, 0.01330373, 0.00655965, 0.00078633, 0.00002292},
        {0.00019117, 0.00655965, 0.05472157, 0.11098164, 0.05472157, 0.00655965, 0.00019117},
        {0.00038771, 0.01330373, 0.11098164, 0.22508352, 0.11098164, 0.01330373, 0.00038771},
        {0.00019117, 0.00655965, 0.05472157, 0.11098164, 0.05472157, 0.00655965, 0.00019117},
        {0.00002292, 0.00078633, 0.00655965, 0.01330373, 0.00655965, 0.00078633, 0.00002292},
        {0.00000067, 0.00002292, 0.00019117, 0.00038771, 0.00019117, 0.00002292, 0.00000067}
    };

    // start行、列的负偏移，end行、列的正偏移。因为高斯模板固定，所以这里直接将矩阵边长写死
    int start = -(7 >> 1);
    int end = (7 >> 1);
    int width = image.width();
    int height = image.height();

    // tmp临时保存结果
    QImage tmp = image.copy(0, 0, width, height);

    for (int y = end; y < height-end; ++y)
    {
        for (int x = end; x < width-end; ++x)
        {
            int tot = 0;

            // 计算7*7矩阵中的rgb分量与高斯模板的卷积
            double tmpR = 0;
            double tmpG = 0;
            double tmpB = 0;
            for (int dy = start; dy <= end; ++dy)
            {
                for (int dx = start; dx <= end; ++dx)
                {
                    QColor color = image.pixel(x+dx, y+dy);
                    tmpR += color.red()*gaussianTemplate[dx+end][dy+end];
                    tmpG += color.green()*gaussianTemplate[dx+end][dy+end];
                    tmpB += color.blue()*gaussianTemplate[dx+end][dy+end];
                    ++tot;
                }
            }

            // 结果写入tmp
            QRgb rgb = qRgb(tmpR, tmpG, tmpB);
            tmp.setPixel(x, y, rgb);
        }
    }
    image = tmp;
}

// 直方图均衡化
void ImageHandler::histogramEqualization(QImage &image)
{
    HistogramRGB histogramRGB;
    histogramRGB.readImage(image);

    int equalizationRes[COLOR_DIMENSIONS][HISTOGRAM_DIMENSIONS] = {0};
    int height = image.height();
    int width  = image.width();
    histogramRGB.equalization(equalizationRes, height * width);


    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            QColor color   = image.pixel(x, y);
            int redLevel   = color.red();
            int greenLevel = color.green();
            int blueLevel  = color.blue();
            redLevel   = equalizationRes[RED][redLevel];
            greenLevel = equalizationRes[GREEN][greenLevel];
            blueLevel  = equalizationRes[BLUE][blueLevel];

            QRgb rgb = qRgb(redLevel, greenLevel, blueLevel);
            image.setPixel(x, y, rgb);
        }
    }
}

// 图像灰度化Gray=0.30*R+0.59*G+0.11*B
void ImageHandler::toGray(QImage &image)
{
    int height = image.height();
    int width  = image.width();
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            QColor color = image.pixel(x, y);
            int gray = color.red()*0.3 + color.green()*0.59 + color.blue()*0.11;
            QRgb rgb = qRgb(gray, gray, gray);
            image.setPixel(x, y, rgb);
        }
    }
}

// 将位图，微分算子，以及微分算子对应矩阵的阶 当做参数，实现图像的锐化
void ImageHandler::sharpening(QImage &image, int H[3][3], int D)
{
    int *tmp;
    int height, width, step;
    height = image.height();
    width  = image.width();
    step   = width*3;

    tmp = (int*) malloc(sizeof(int)*height*width*3);
    memset(tmp, 0, sizeof(int)*height*width*3);
    switch(D)
    {
    case 2:
        for (int i = 0; i < height-1; ++i) {
            for (int j = 0; j < width-1; ++j) {
                for (int _i = 0; _i < D; ++_i) {
                    for (int _j = 0; _j < D; ++_j) {
                        QColor color = image.pixel(j+_j, i+_i);
                        int redLevel   = color.red();
                        int greenLevel = color.green();
                        int blueLevel  = color.blue();

                        tmp[i*step + j*3 + 0] += redLevel * H[_i][_j];
                        tmp[i*step + j*3 + 1] += greenLevel * H[_i][_j];
                        tmp[i*step + j*3 + 2] += blueLevel * H[_i][_j];
                    }
                }

            }
        }
        for (int i = 0; i < height-1; ++i) {
            for (int j = 0; j < width-1; ++j) {
                int redLevel   = tmp[i*step + j*3 + 0];
                int greenLevel = tmp[i*step + j*3 + 1];
                int blueLevel  = tmp[i*step + j*3 + 2];

                QRgb rgb = qRgb(redLevel, greenLevel, blueLevel);
                image.setPixel(j, i, rgb);

            }
        }
        break;
    case 3:
        for (int i = 1; i < height-1; ++i) {
            for (int j = 1; j < width-1; ++j) {
                for (int _i = 0; _i < D; ++_i) {
                    for (int _j = 0; _j < D; ++_j) {
                        QColor color = image.pixel(j+_j-1, i+_i-1);
                        int redLevel   = color.red();
                        int greenLevel = color.green();
                        int blueLevel  = color.blue();

                        tmp[i*step + j*3 + 0] += redLevel * H[_i][_j];
                        tmp[i*step + j*3 + 1] += greenLevel * H[_i][_j];
                        tmp[i*step + j*3 + 2] += blueLevel * H[_i][_j];
                    }
                }

            }
        }
        for (int i = 1; i < height-1; ++i) {
            for (int j = 1; j < width-1; ++j) {
                int redLevel   = tmp[i*step + j*3 + 0];
                int greenLevel = tmp[i*step + j*3 + 1];
                int blueLevel  = tmp[i*step + j*3 + 2];

                QRgb rgb = qRgb(redLevel, greenLevel, blueLevel);
                image.setPixel(j, i, rgb);

            }
        }
        break;
    }
    free(tmp);
}

// 根据otsu算法原理确定阈值大小
int ImageHandler::findOtsuThreshold(QImage &image)
{
    if (false == image.allGray())
    {
        return -1;
    }
    int cnt_PixelValue[256];
    int height, width;
    height = image.height(), width = image.width();
    long long pixelValueSum, pixelCntSum;
    pixelValueSum = 0, pixelCntSum = height*width;

    memset(cnt_PixelValue, 0, sizeof(cnt_PixelValue));
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            QColor color = image.pixel(x, y);
            int grayLevel = color.red();
            ++cnt_PixelValue[grayLevel];
            pixelValueSum += grayLevel;
        }
    }
    long long cntSum1 = 0, cntSum2;
    long long valSum1 = 0, valSum2;
    double maxVariance = -1, nowVariance;
    double meanVal1, meanVal2;
    int ret = -1;

    for (int i = 0; i <= 255; ++i)
    {
        cntSum1 += cnt_PixelValue[i];
        if (0 == cntSum1)
        {
            continue;
        }
        cntSum2 = pixelCntSum - cntSum1;
        if (0 == cntSum2)
        {
            break;
        }

        valSum1 += (long long)i * cnt_PixelValue[i];
        valSum2 = pixelValueSum - valSum1;

        meanVal1 = (double)valSum1 / cntSum1;
        meanVal2 = (double)valSum2 / cntSum2;
        nowVariance = cntSum1*cntSum2*(meanVal1 - meanVal2)*(meanVal1 - meanVal2);
        if (maxVariance < nowVariance)
        {
            maxVariance = nowVariance;
            ret = i;
        }
    }
    return ret;
}

// 使用otsu算法实现图像分割
void ImageHandler::otsu(QImage &image)
{
    int threshold;
    int height, width;
    height = image.height(), width = image.width();
    if ((threshold = findOtsuThreshold(image)) == -1)
    {
        std::cerr << "findOtsuThreshold error." << std::endl;
    }
    std::cout << "threshold == " << threshold << std::endl;
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            QColor color = image.pixel(x, y);
            int grayLevel = color.red();
            if (grayLevel > threshold)
            {
                grayLevel = 255;
            }
            else
            {
                grayLevel = 0;
            }
            QRgb rgb = qRgb(grayLevel, grayLevel, grayLevel);
            image.setPixel(x, y, rgb);
        }
    }
}
