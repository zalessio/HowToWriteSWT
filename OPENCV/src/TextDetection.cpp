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

#include <cassert>
#include <cmath>
#include <iostream>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv/cxcore.h>
#include <math.h>
#include <time.h>
#include <utility>
#include <algorithm>
#include <vector>
#include "TextDetection.h"

#define PI 3.14159265

void normalizeImage (IplImage * input, IplImage * output) {
    assert ( input->depth == IPL_DEPTH_32F );
    assert ( input->nChannels == 1 );
    assert ( output->depth == IPL_DEPTH_32F );
    assert ( output->nChannels == 1 );
    float maxVal = 0;
    float minVal = 1e100;
    for( int row = 0; row < input->height; row++ ){
        const float* ptr = (const float*)(input->imageData + row * input->widthStep);
        for ( int col = 0; col < input->width; col++ ){
            if (*ptr < 0) { }
            else {
                maxVal = std::max(*ptr, maxVal);
                minVal = std::min(*ptr, minVal);
            }
            ptr++;
        }
    }

    float difference = maxVal - minVal;
    for( int row = 0; row < input->height; row++ ){
        const float* ptrin = (const float*)(input->imageData + row * input->widthStep);\
        float* ptrout = (float*)(output->imageData + row * output->widthStep);\
        for ( int col = 0; col < input->width; col++ ){
            if (*ptrin < 0) {
                *ptrout = 1;
            } else {
                *ptrout = ((*ptrin) - minVal)/difference;
            }
            ptrout++;
            ptrin++;
        }
    }
}

void textDetection (IplImage * input, bool dark_on_light)
{
    assert ( input->depth == IPL_DEPTH_8U );
    assert ( input->nChannels == 3 );
    std::cout << "Running textDetection with dark_on_light " << dark_on_light << std::endl;
    // Convert to grayscale
    std::cout << "Convert to grayscale " << std::endl;
    IplImage * grayImage =
            cvCreateImage ( cvGetSize ( input ), IPL_DEPTH_8U, 1 );
    cvCvtColor ( input, grayImage, CV_RGB2GRAY );
    // Create Canny Image
    std::cout << "Create Canny Image " << std::endl;
    double threshold_low = 175;
    double threshold_high = 320;
    IplImage * edgeImage =
            cvCreateImage( cvGetSize (input),IPL_DEPTH_8U, 1 );
    cvCanny(grayImage, edgeImage, threshold_low, threshold_high, 3) ;
    cvSaveImage ( "imgs/OPENCV_canny.png", edgeImage);

    // Create gradient X, gradient Y
    std::cout << "Create gradient X, gradient Y " << std::endl;
    IplImage * gaussianImage =
            cvCreateImage ( cvGetSize(input), IPL_DEPTH_32F, 1);
    cvConvertScale (grayImage, gaussianImage, 1./255., 0);
    cvSmooth( gaussianImage, gaussianImage, CV_GAUSSIAN, 5, 5);
    IplImage * gradientX =
            cvCreateImage ( cvGetSize ( input ), IPL_DEPTH_32F, 1 );
    IplImage * gradientY =
            cvCreateImage ( cvGetSize ( input ), IPL_DEPTH_32F, 1 );
    cvSobel(gaussianImage, gradientX , 1, 0, CV_SCHARR);
    cvSobel(gaussianImage, gradientY , 0, 1, CV_SCHARR);
    cvSmooth(gradientX, gradientX, 3, 3);
    cvSmooth(gradientY, gradientY, 3, 3);
    cvReleaseImage ( &gaussianImage );
    cvReleaseImage ( &grayImage );

    // Calculate SWT and return ray vectors
    std::cout << "SWT " << std::endl;
    std::vector<Ray> rays;
    IplImage * SWTImage =
            cvCreateImage ( cvGetSize ( input ), IPL_DEPTH_32F, 1 );
    for( int row = 0; row < input->height; row++ ){
        float* ptr = (float*)(SWTImage->imageData + row * SWTImage->widthStep);
        for ( int col = 0; col < input->width; col++ ){
            *ptr++ = -1;
        }
    }
    strokeWidthTransform ( edgeImage, gradientX, gradientY, dark_on_light, SWTImage, rays );

    IplImage * output2 =
            cvCreateImage ( cvGetSize ( input ), IPL_DEPTH_32F, 1 );
    normalizeImage (SWTImage, output2);
    IplImage * saveSWT =
            cvCreateImage ( cvGetSize ( input ), IPL_DEPTH_8U, 1 );
    cvConvertScale(output2, saveSWT, 255, 0);
    cvSaveImage ( "imgs/OPENCV_SWT.png", saveSWT);
    
    cvReleaseImage ( &output2 );
    cvReleaseImage( &saveSWT );
    cvReleaseImage ( &gradientX );
    cvReleaseImage ( &gradientY );
    cvReleaseImage ( &SWTImage );
    cvReleaseImage ( &edgeImage );
}

void strokeWidthTransform (IplImage * edgeImage,
                           IplImage * gradientX,
                           IplImage * gradientY,
                           bool dark_on_light,
                           IplImage * SWTImage,
                           std::vector<Ray> & rays) {
    // First pass
    float prec = .05;
    for( int row = 0; row < edgeImage->height; row++ ){
        const uchar* ptr = (const uchar*)(edgeImage->imageData + row * edgeImage->widthStep);
        for ( int col = 0; col < edgeImage->width; col++ ){
            if (*ptr > 0) {
                Ray r;

                Point2d p;
                p.x = col;
                p.y = row;
                r.p = p;
                std::vector<Point2d> points;
                points.push_back(p);

                float curX = (float)col + 0.5;
                float curY = (float)row + 0.5;
                int curPixX = col;
                int curPixY = row;
                float G_x = CV_IMAGE_ELEM ( gradientX, float, row, col);
                float G_y = CV_IMAGE_ELEM ( gradientY, float, row, col);
                // normalize gradient
                float mag = sqrt( (G_x * G_x) + (G_y * G_y) );
                if (dark_on_light){
                    G_x = -G_x/mag;
                    G_y = -G_y/mag;
                } else {
                    G_x = G_x/mag;
                    G_y = G_y/mag;

                }
                while (true) {
                    curX += G_x*prec;
                    curY += G_y*prec;
                    if ((int)(floor(curX)) != curPixX || (int)(floor(curY)) != curPixY) {
                        curPixX = (int)(floor(curX));
                        curPixY = (int)(floor(curY));
                        // check if pixel is outside boundary of image
                        if (curPixX < 0 || (curPixX >= SWTImage->width) || curPixY < 0 || (curPixY >= SWTImage->height)) {
                            break;
                        }
                        Point2d pnew;
                        pnew.x = curPixX;
                        pnew.y = curPixY;
                        points.push_back(pnew);

                        if (CV_IMAGE_ELEM ( edgeImage, uchar, curPixY, curPixX) > 0) {
                            r.q = pnew;
                            // dot product
                            float G_xt = CV_IMAGE_ELEM(gradientX,float,curPixY,curPixX);
                            float G_yt = CV_IMAGE_ELEM(gradientY,float,curPixY,curPixX);
                            mag = sqrt( (G_xt * G_xt) + (G_yt * G_yt) );
                            if (dark_on_light){
                                G_xt = -G_xt/mag;
                                G_yt = -G_yt/mag;
                            } else {
                                G_xt = G_xt/mag;
                                G_yt = G_yt/mag;

                            }

                            if (acos(G_x * -G_xt + G_y * -G_yt) < PI/2.0 ) {
                                float length = sqrt( ((float)r.q.x - (float)r.p.x)*((float)r.q.x - (float)r.p.x) + ((float)r.q.y - (float)r.p.y)*((float)r.q.y - (float)r.p.y));
                                for (std::vector<Point2d>::iterator pit = points.begin(); pit != points.end(); pit++) {
                                    if (CV_IMAGE_ELEM(SWTImage, float, pit->y, pit->x) < 0) {
                                        CV_IMAGE_ELEM(SWTImage, float, pit->y, pit->x) = length;
                                    } else {
                                        CV_IMAGE_ELEM(SWTImage, float, pit->y, pit->x) = std::min(length, CV_IMAGE_ELEM(SWTImage, float, pit->y, pit->x));
                                    }
                                }
                                r.points = points;
                                rays.push_back(r);
                            }
                            break;
                        }
                    }
                }
            }
            ptr++;
        }
    }

}
