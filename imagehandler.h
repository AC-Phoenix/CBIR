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
private:
    // 保存图像的统一大小
    static QSize size;
};


#endif // IMAGEHANDLER_H
