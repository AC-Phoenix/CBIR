#include "imagehandler.h"
#include <algorithm>
#include <iostream>

ImageHandler::ImageHandler()
{

}

ImageHandler::~ImageHandler()
{

}

QSize ImageHandler::size;

void ImageHandler::medianFilter(QImage &image, int D)
{
    int y, x;
    int dy, dx;
    int **arr;
    int _start, _end;
    _start = -(D >> 1);
    _end = (D >> 1);
    int width = image.width();
    int height = image.height();
    QImage tmp = image.copy(0, 0, width, height);

    std::cout << "zero" << std::endl;

    arr = new int* [3];
    for (int i = 0; i < 3; ++i)
    {
        arr[i] = new int[D*D];
    }

    std::cout << "first" << std::endl;

    for (y = _end; y < height-_end; ++y) {
        for (x = _end; x < width-_end; ++x) {
            int tot = 0;
            for (dy = _start; dy <= _end; ++dy) {
                for (dx = _start; dx <= _end; ++dx) {
                    QColor color = image.pixel(x+dx, y+dy);
                    arr[0][tot] = color.red();
                    arr[1][tot] = color.green();
                    arr[2][tot] = color.blue();
                    ++tot;
                }
            }
            std::nth_element(arr[0], arr[0] + (tot>>1), arr[0]+tot);
            std::nth_element(arr[1], arr[1] + (tot>>1), arr[1]+tot);
            std::nth_element(arr[2], arr[2] + (tot>>1), arr[2]+tot);
            QRgb rgb = qRgb(arr[0][tot>>1], arr[1][tot>>1], arr[2][tot>>1]);
            tmp.setPixel(x, y, rgb);
        }
    }

    std::cout << "second" << std::endl;

    image = tmp;
    for (int i = 0; i < 3; ++i)
    {
        delete[] arr[i];
    }
    delete arr;
}
