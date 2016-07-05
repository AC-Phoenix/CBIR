#ifndef IMAGEHANDLER_H
#define IMAGEHANDLER_H

#include <QImage>
#include <QSize>
#include <QRgb>
#include <QColor>

const int COLOR_DIMENSIONS = 3;

enum RGB
{
    RED = 0,
    GREEN = 1,
    BLUE = 2,
};

class ImageHandler
{
public:
    ImageHandler();
    ~ImageHandler();

    static void init()
    {
        // size的值要改为在文件中读取
        size.scale(800, 600, Qt::IgnoreAspectRatio);
    }

    static void scaled(QImage &image)
    {
        image = image.scaled(size);
    }

    // 中值滤波
    static void medianFilter(QImage &image, int D = 3);

    // 高斯平滑
    static void gaussianSmoothing(QImage &image);

    // 直方图均衡化
    static void histogramEqualization(QImage &image);

    // 图像灰度化Gray=0.30*R+0.59*G+0.11*B
    static void toGray(QImage &image);

    // 将位图，微分算子，以及微分算子对应矩阵的阶 当做参数，实现图像的锐化
    static void sharpening(QImage &image, int H[3][3], int D);

    // 根据otsu算法原理确定阈值大小
    static int findOtsuThreshold(QImage &image);

    // 使用otsu算法实现图像分割
    static void otsu(QImage &image);
private:
    // 保存图像的统一大小
    static QSize size;
};


#endif // IMAGEHANDLER_H
