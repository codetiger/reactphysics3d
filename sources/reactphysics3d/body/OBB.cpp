/****************************************************************************
* Copyright (C) 2009      Daniel Chappuis                                  *
****************************************************************************
* This file is part of ReactPhysics3D.                                     *
*                                                                          *
* ReactPhysics3D is free software: you can redistribute it and/or modify   *
* it under the terms of the GNU Lesser General Public License as published *
* by the Free Software Foundation, either version 3 of the License, or     *
* (at your option) any later version.                                      *
*                                                                          *
* ReactPhysics3D is distributed in the hope that it will be useful,        *
* but WITHOUT ANY WARRANTY; without even the implied warranty of           *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the             *
* GNU Lesser General Public License for more details.                      *
*                                                                          *
* You should have received a copy of the GNU Lesser General Public License *
* along with ReactPhysics3D. If not, see <http://www.gnu.org/licenses/>.   *
***************************************************************************/

// Libraries
#include "OBB.h"
#include <vector>
#include <GL/freeglut.h>        // TODO : Remove this in the final version
#include <GL/gl.h>              // TODO : Remove this in the final version
#include <cassert>
#include <cstdlib>
#include <iostream>             // TODO : Delete this

// We want to use the ReactPhysics3D namespace
using namespace reactphysics3d;

// Constructor
OBB::OBB(const Vector3D& center, const Vector3D& axis1, const Vector3D& axis2,
            const Vector3D& axis3, double extent1, double extent2, double extent3) {
    this->center = center;

    oldAxis[0] = axis1;
    oldAxis[1] = axis2;
    oldAxis[2] = axis3;

    this->axis[0] = axis1;
    this->axis[1] = axis2;
    this->axis[2] = axis3;

    this->extent[0] = extent1;
    this->extent[1] = extent2;
    this->extent[2] = extent3;
}

// Copy-Constructor
OBB::OBB(const OBB& obb) {
    this->center = obb.center;

    oldAxis[0] = obb.oldAxis[0];
    oldAxis[1] = obb.oldAxis[1];
    oldAxis[2] = obb.oldAxis[2];

    this->axis[0] = obb.axis[0];
    this->axis[1] = obb.axis[1];
    this->axis[2] = obb.axis[2];

    this->extent[0] = obb.extent[0];
    this->extent[1] = obb.extent[1];
    this->extent[2] = obb.extent[2];
}

// Destructor
OBB::~OBB() {

}

// TODO : Remove this method in the final version
// Draw the OBB (only for testing purpose)
void OBB::draw() const {
    double e0 = extent[0];
    double e1 = extent[1];
    double e2 = extent[2];

    Vector3D s1 = center + (axis[0]*e0) + (axis[1]*e1) - (axis[2]*e2);
    Vector3D s2 = center + (axis[0]*e0) + (axis[1]*e1) + (axis[2]*e2);
    Vector3D s3 = center - (axis[0]*e0) + (axis[1]*e1) + (axis[2]*e2);
    Vector3D s4 = center - (axis[0]*e0) + (axis[1]*e1) - (axis[2]*e2);
    Vector3D s5 = center + (axis[0]*e0) - (axis[1]*e1) - (axis[2]*e2);
    Vector3D s6 = center + (axis[0]*e0) - (axis[1]*e1) + (axis[2]*e2);
    Vector3D s7 = center - (axis[0]*e0) - (axis[1]*e1) + (axis[2]*e2);
    Vector3D s8 = center - (axis[0]*e0) - (axis[1]*e1) - (axis[2]*e2);

    // Draw in red
    glColor3f(1.0, 0.0, 0.0);

    // Draw the OBB
    glBegin(GL_LINES);
        glVertex3f(s1.getX(), s1.getY(), s1.getZ());
        glVertex3f(s2.getX(), s2.getY(), s2.getZ());

        glVertex3f(s2.getX(), s2.getY(), s2.getZ());
        glVertex3f(s3.getX(), s3.getY(), s3.getZ());

        glVertex3f(s3.getX(), s3.getY(), s3.getZ());
        glVertex3f(s4.getX(), s4.getY(), s4.getZ());

        glVertex3f(s4.getX(), s4.getY(), s4.getZ());
        glVertex3f(s1.getX(), s1.getY(), s1.getZ());

        glVertex3f(s5.getX(), s5.getY(), s5.getZ());
        glVertex3f(s6.getX(), s6.getY(), s6.getZ());

        glVertex3f(s6.getX(), s6.getY(), s6.getZ());
        glVertex3f(s7.getX(), s7.getY(), s7.getZ());

        glVertex3f(s7.getX(), s7.getY(), s7.getZ());
        glVertex3f(s8.getX(), s8.getY(), s8.getZ());

        glVertex3f(s8.getX(), s8.getY(), s8.getZ());
        glVertex3f(s5.getX(), s5.getY(), s5.getZ());

        glVertex3f(s1.getX(), s1.getY(), s1.getZ());
        glVertex3f(s5.getX(), s5.getY(), s5.getZ());

        glVertex3f(s4.getX(), s4.getY(), s4.getZ());
        glVertex3f(s8.getX(), s8.getY(), s8.getZ());

        glVertex3f(s3.getX(), s3.getY(), s3.getZ());
        glVertex3f(s7.getX(), s7.getY(), s7.getZ());

        glVertex3f(s2.getX(), s2.getY(), s2.getZ());
        glVertex3f(s6.getX(), s6.getY(), s6.getZ());

        glVertex3f(center.getX(), center.getY(), center.getZ());
        glVertex3f(center.getX() + 8.0 * axis[1].getX(), center.getY() + 8.0 * axis[1].getY(), center.getZ() + 8.0 * axis[1].getZ());

    glEnd();
}

// Return all the vertices that are projected at the extreme of the projection of the bouding volume on the axis.
// The function returns the number of extreme vertices and a set that contains thoses vertices.
int OBB::getExtremeVertices(const Vector3D projectionAxis, std::vector<Vector3D>& extremeVertices) const {
    assert(extremeVertices.size() == 0);
    assert(projectionAxis.length() != 0);

    double maxProjectionLength = 0.0;     // Longest projection length of a vertex onto the projection axis

    // Compute the vertices of the OBB
    double e0 = extent[0];
    double e1 = extent[1];
    double e2 = extent[2];
    Vector3D vertices[8];
    vertices[0] = center + (axis[0]*e0) + (axis[1]*e1) - (axis[2]*e2);
    vertices[1] = center + (axis[0]*e0) + (axis[1]*e1) + (axis[2]*e2);
    vertices[2] = center - (axis[0]*e0) + (axis[1]*e1) + (axis[2]*e2);
    vertices[3] = center - (axis[0]*e0) + (axis[1]*e1) - (axis[2]*e2);
    vertices[4] = center + (axis[0]*e0) - (axis[1]*e1) - (axis[2]*e2);
    vertices[5] = center + (axis[0]*e0) - (axis[1]*e1) + (axis[2]*e2);
    vertices[6] = center - (axis[0]*e0) - (axis[1]*e1) + (axis[2]*e2);
    vertices[7] = center - (axis[0]*e0) - (axis[1]*e1) - (axis[2]*e2);

    for (int i=0; i<8; ++i) {
        // Compute the projection length of the current vertex onto the projection axis
        double projectionLength = projectionAxis.scalarProduct(vertices[i]-center) / projectionAxis.length();

        //std::cout << "Projection length : " << projectionLength << std::endl;
        //std::cout << "Max Projection length : " << maxProjectionLength << std::endl;

        // If we found a bigger projection length
        if (projectionLength > maxProjectionLength + EPSILON) {
            maxProjectionLength = projectionLength;
            extremeVertices.clear();
            extremeVertices.push_back(vertices[i]);
            //std::cout << "PRINT 1" << std::endl;
        }
        else if (equal(projectionLength, maxProjectionLength)) {
            extremeVertices.push_back(vertices[i]);
            //std::cout << "PRINT 2" << std::endl;
        }
    }

    // An extreme should be a unique vertex, an edge or a face
    assert(extremeVertices.size() == 1 || extremeVertices.size() == 2 || extremeVertices.size() == 4);

    // Return the number of extreme vertices
    return extremeVertices.size();
}
