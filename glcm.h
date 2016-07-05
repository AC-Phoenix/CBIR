#ifndef GLCM_H
#define GLCM_H

#include <QImage>
#include <QColor>

const int GRAYCOUNT  = 256;
const int GRAYLENGTH = 32;

struct GLCMFeature
{
    // 熵
    double entropy;
    // 能量
    double energy;
    // 对比度
    double contrast;
    // 逆差矩
    double IDM;
    // 自相关性
    double correlation;
};

class GLCM
{
public:
    GLCM();
    ~GLCM();
    // 读取图像到灰度共生矩阵，并归一化
    bool readImage(QImage &image, int dx, int dy);

    // 计算灰度共生矩阵特征值
    void calFeature(GLCMFeature &res);

private:
    double data[GRAYCOUNT/GRAYLENGTH][GRAYCOUNT/GRAYLENGTH];
};

#endif // GLCM_H
