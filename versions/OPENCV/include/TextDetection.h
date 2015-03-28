/*
    Copyright 2012 Andrew Perrault and Saurav Kumar.

    This file is part of DetectText.

    DetectText is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    DetectText is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with DetectText.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef TEXTDETECTION_H
#define TEXTDETECTION_H

#include <opencv/cv.h>

struct Point2d {
    int x;
    int y;
    float SWT;
};

struct Point2dFloat {
    float x;
    float y;
};

struct Ray {
        Point2d p;
        Point2d q;
        std::vector<Point2d> points;
};

struct Point3dFloat {
    float x;
    float y;
    float z;
};


struct Chain {
    int p;
    int q;
    float dist;
    bool merged;
    Point2dFloat direction;
    std::vector<int> components;
};

bool Point2dSort (Point2d const & lhs,
                  Point2d const & rhs);

void textDetection (IplImage *    float_input,
                          bool dark_on_light);

void strokeWidthTransform (IplImage * edgeImage,
                           IplImage * gradientX,
                           IplImage * gradientY,
                           bool dark_on_light,
                           IplImage * SWTImage,
                           std::vector<Ray> & rays);


#endif // TEXTDETECTION_H

