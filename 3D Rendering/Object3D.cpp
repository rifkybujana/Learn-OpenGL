#include "Object3D.h"
#include <cmath> // Include cmath for mathematical functions

Object3D::Object3D() : circleCorner(4) {
    // Constructor initializes circleCorner to 4
}

void Object3D::GetCorner() {
    // Clear the vertices and indices vectors
    vertices.clear();
    indices.clear();

    // Constants for the circle calculation
    const int cornerTotal = circleCorner;
    const float radius = 0.5f;
    const float angleIncrement = 2.0f * static_cast<float>(M_PI) / cornerTotal;

    // Reserve space to improve performance by minimizing reallocations
    vertices.reserve((cornerTotal + 1) * 3); // center vertex + corner vertices
    indices.reserve(cornerTotal * 3);        // number of triangles * 3 indices per triangle

    // Add center vertex at (0, 0, 0)
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);

    // Generate vertices around the circle
    for (int i = 0; i < cornerTotal; ++i) {
        float angle = i * angleIncrement;
        float x = radius * cosf(angle);
        float y = radius * sinf(angle);

        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(0.0f); // z-coordinate
    }

    // Generate indices for triangle fan
    for (int i = 1; i <= cornerTotal; ++i) {
        indices.push_back(0); // Center vertex index
        indices.push_back(i); // Current vertex index

        int nextIndex = (i % cornerTotal) + 1; // Wrap around to the first vertex
        indices.push_back(nextIndex);
    }
}
