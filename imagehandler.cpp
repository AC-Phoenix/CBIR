#include "imagehandler.h"
#include <algorithm>
#include <iostream>

ImageHandler::ImageHandler()
{

}

ImageHandler::~ImageHandler()
{

}

// 定义 static 成员
QSize ImageHandler::size;

// 中值滤波
void ImageHandler::medianFilter(QImage &image, int D)
{
    // start行、列的负偏移，end行、列的正偏移
    int start = -(D >> 1);
    int end = (D >> 1);
    int width = image.width();
    int height = image.height();

    // tmp 临时保存结果
    QImage tmp = image.copy(0, 0, width, height);

    // 依次存放D*D矩阵中的rgb分量
    int **arr = new int* [3];
    for (int i = 0; i < 3; ++i)
    {
        arr[i] = new int[D*D];
    }

    for (int y = end; y < height-end; ++y) {
        for (int x = end; x < width-end; ++x) {
            int tot = 0;

            // 获取D*D矩阵中的rgb分量
            for (int dy = start; dy <= end; ++dy) {
                for (int dx = start; dx <= end; ++dx) {
                    QColor color = image.pixel(x+dx, y+dy);
                    arr[0][tot] = color.red();
                    arr[1][tot] = color.green();
                    arr[2][tot] = color.blue();
                    ++tot;
                }
            }

            // 依次计算rgb数组的中位数，结果写入tmp
            std::nth_element(arr[0], arr[0] + (tot>>1), arr[0]+tot);
            std::nth_element(arr[1], arr[1] + (tot>>1), arr[1]+tot);
            std::nth_element(arr[2], arr[2] + (tot>>1), arr[2]+tot);
            QRgb rgb = qRgb(arr[0][tot>>1], arr[1][tot>>1], arr[2][tot>>1]);
            tmp.setPixel(x, y, rgb);
        }
    }

    // 结果写回image
    image = tmp;
    for (int i = 0; i < 3; ++i)
    {
        delete[] arr[i];
    }
    delete arr;
}

// 高斯平滑
void ImageHandler::gaussianSmoothing(QImage &image)
{

}
