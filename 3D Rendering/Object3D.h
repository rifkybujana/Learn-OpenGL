#pragma once

#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

#define _USE_MATH_DEFINES
#include <math.h>

class Object3D
{
public:
    int circleCorner;

    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    void GetCorner();

    Object3D();

private:

};

