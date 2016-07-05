#ifndef CALCULATEFEATURE_H
#define CALCULATEFEATURE_H

#include "calculationrsp.h"

class CalculateFeature
{
public:
    CalculateFeature();
    ~CalculateFeature();

    static void calColorFeature(const char *path, ColorFeatureRsp &rsp);

    static void calShapeFeature(const char *path, ShapeFeatureRsp &rsp);
};

#endif // CALCULATEFEATURE_H
