#ifndef FREND_GEOM_HPP
#define FREND_GEOM_HPP

#include "frend.hpp"

namespace frend
{
    namespace goemetry
    {
        Geometry Sphere(unsigned int uSegments, unsigned int vSegments);
        Geometry Quad(const VEC3& p1, const VEC3& p2, const VEC3& p3, const VEC4& p4);
        Geometry Cube();
    }
}

#endif // FREND_GEOM_HPP

