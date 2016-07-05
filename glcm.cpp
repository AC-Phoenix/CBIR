#include "glcm.h"
#include <iostream>
#include <cmath>
#include <QtCore/qmath.h>
#include <cstdio>
#include <cstdlib>
#include <float.h>

GLCM::GLCM()
{
    memset(data, 0, sizeof(data));
}

GLCM::~GLCM()
{

}

// 读取图像到灰度共生矩阵，并归一化
bool GLCM::readImage(QImage &image, int dx, int dy)
{
    if (false == image.allGray())
    {
        return false;
    }
    int height = image.height();
    int width  = image.width();
    int count = 0;
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            if (0 <= x+dx && x+dx < width && 0 <= y+dy && y+dy < height)
            {
                QColor color1   = image.pixel(x, y);
                QColor color2   = image.pixel(x+dx, y+dy);
                int grayLevel1  = color1.red();
                int grayLevel2  = color2.red();

                // 统计各gray对出现的次数。
                ++data[grayLevel1/GRAYLENGTH][grayLevel2/GRAYLENGTH];
                ++count;
            }
        }
    }
    // 归一化
    int segmentCount = GRAYCOUNT / GRAYLENGTH;
    for (int i = 0; i < segmentCount; ++i)
    {
        for (int j = 0; j < segmentCount; ++j)
        {
            data[i][j] /= count;
        }
    }

    return true;
}

// 计算灰度共生矩阵特征值
void GLCM::calFeature(GLCMFeature &res)
{
    int segmentCount = GRAYCOUNT / GRAYLENGTH;
    double u_i   = 0;
    double u_j   = 0;
    double s_i_2 = 0;
    double s_j_2 = 0;
    double s_i   = 0;
    double s_j   = 0;

    for (int i = 0; i < segmentCount; ++i)
    {
        for (int j = 0; j < segmentCount; ++j)
        {
            u_i += i*data[i][j];
            u_j += j*data[i][j];
        }
    }

    for (int i = 0; i < segmentCount; ++i)
    {
       for (int j = 0; j < segmentCount; ++j)
       {
           //s_i的平方
           s_i_2 += (i-u_i)*(i-u_i)*data[i][j];
           //s_j的平方
           s_j_2 += (j-u_j)*(j-u_j)*data[i][j];
       }
    }
    s_i = sqrt(s_i_2);
    s_j = sqrt(s_j_2);

    res.entropy     = 0;
    res.energy      = 0;
    res.contrast    = 0;
    res.IDM         = 0;
    res.correlation = 0;
    for (int i = 0; i < segmentCount; ++i)
    {
        for (int j = 0; j < segmentCount; ++j)
        {
            //熵
            if (data[i][j] > 0)
            {
                res.entropy -= data[i][j] * qLn(data[i][j])/qLn(10);
            }
            //能量
            res.energy += data[i][j] * data[i][j];
            //对比度
            res.contrast += (i-j) * (i-j) * data[i][j];
            //逆差矩
            res.IDM += data[i][j] / (1 + (i-j)*(i-j));
            //自相关性
            res.correlation += (i*j*data[i][j] - u_i*u_j) / (s_i*s_j);
        }
    }
}
