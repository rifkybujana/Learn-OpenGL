#include "Object3D.h"

Object3D::Object3D() {
    circleCorner = 4;
}

void Object3D::GetCorner() {
    if (!vertices.empty()) {
        vertices.clear();
    }

    if (!indices.empty()) {
        indices.clear();
    }

    int cornerTotal = circleCorner;

    vertices.push_back(0);
    vertices.push_back(0);
    vertices.push_back(0);

    double defaultAngle = 360.0 / cornerTotal;

    for (int i = 0; i < cornerTotal * 3; i += 3)
    {
        int index = i + 3;

        double deg = i / 3 * defaultAngle;
        double rad = deg * M_PI / 180.0;

        vertices.push_back(0.5 * cos(rad));
        vertices.push_back(0.5 * sin(rad));
        vertices.push_back(0);
    }

    int tempIndex = 1;

    for (int i = 0; i < cornerTotal * 3; i += 3) {
        indices.push_back(0);
        indices.push_back(tempIndex);

        if (i == (cornerTotal - 1) * 3) {
            indices.push_back(1);
        }
        else {
            indices.push_back(tempIndex + 1);
        }

        tempIndex++;
    }
}