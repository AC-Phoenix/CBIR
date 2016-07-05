#ifndef CALCULATEFEATURE_H
#define CALCULATEFEATURE_H

#include "calculationrsp.h"
#include <QImage>

class CalculateFeature
{
public:
    CalculateFeature();
    ~CalculateFeature();

    static void calColorFeature(const char *path, ColorFeatureRsp &rsp);

    static void calTextureFeature(const char *path, TextureFeatureRsp &rsp);

    static void calShapeFeature(const char *path, ShapeFeatureRsp &rsp);

private:
    static void calShapeOneDirection(const QImage &image, ShapeFeature &ret);
};

#endif // CALCULATEFEATURE_H
