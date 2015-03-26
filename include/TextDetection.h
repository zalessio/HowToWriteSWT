#ifndef TEXTDETECTION_H_
#define TEXTDETECTION_H_
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


#include "../include/imageio.h"
#include "../include/CannyEdge.h"
#include "../include/Swt.h"


void textDetection (IplImage * input, bool dark_on_light)
{   

    assert ( input->depth == IPL_DEPTH_8U );
    assert ( input->nChannels == 3 );
    std::cout << "Running textDetection with dark_on_light " << dark_on_light << std::endl;
    

    int h,w;

    struct image grayImg,gaussianImg,edgeImg;
    
    convertImg(input, &grayImg);
    
    h = grayImg.height;
    w = grayImg.width;
    
    edgeImg.width = w;
    edgeImg.height = h;
    edgeImg.pixel_data = (unsigned char *)malloc(w * h * sizeof(unsigned char));

    // Create Canny Image
    std::cout << "CANNY EDGE " << std::endl;
    canny_edge_detect(&grayImg, &edgeImg);
    
    IplImage * edgeImage = cvCreateImage( cvGetSize (input),IPL_DEPTH_8U, 1 );
    edgeImage->imageData = (char *)edgeImg.pixel_data;
    cvSaveImage ( "imgs/canny.png", edgeImage);


    gaussianImg.width = w;
    gaussianImg.height = h;
    gaussianImg.pixel_data = (unsigned char *)malloc(w * h * sizeof(unsigned char));

    // Create gradient X, gradient Y
    std::cout << "GRADIENT " << std::endl;
     IplImage * grayImage = cvCreateImage ( cvGetSize ( input ), IPL_DEPTH_8U, 1 );
    cvCvtColor ( input, grayImage, CV_RGB2GRAY );
    grayImg.pixel_data = (unsigned char*)grayImage->imageData;
    IplImage * gradientX = cvCreateImage ( cvGetSize ( input ), IPL_DEPTH_32F, 1 );
    IplImage * gradientY = cvCreateImage ( cvGetSize ( input ), IPL_DEPTH_32F, 1 );
    
    //gaussian_noise_reduce(&grayImg, &gaussianImg);
    //gradient_sobel_x_y(&gaussianImg, (unsigned char*)gradientX->imageData , (unsigned char*)gradientY->imageData);

    IplImage * gaussianImage = cvCreateImage ( cvGetSize(input), IPL_DEPTH_32F, 1);
    cvConvertScale (grayImage, gaussianImage, 1./255., 0);
    cvSmooth( gaussianImage, gaussianImage, CV_GAUSSIAN, 5, 5);
   
    cvSobel(gaussianImage, gradientX , 1, 0, CV_SCHARR);
    cvSobel(gaussianImage, gradientY , 0, 1, CV_SCHARR);
    cvSmooth(gradientX, gradientX, 3, 3);
    cvSmooth(gradientY, gradientY, 3, 3);

    // Calculate SWT and return ray vectors
    std::cout << "SWT " << std::endl;
    std::vector<Ray> rays;
    IplImage * SWTImage = cvCreateImage ( cvGetSize ( input ), IPL_DEPTH_32F, 1 );
    for(int row = 0; row < h; row++ ){
        float* ptr = (float*)(SWTImage->imageData + row * SWTImage->widthStep);
        for ( int col = 0; col < w; col++ ){
            *ptr++ = -1;
        }
    }
    strokeWidthTransform ( edgeImage, gradientX, gradientY, dark_on_light, SWTImage, rays );
    SWTMedianFilter ( SWTImage, rays );


    IplImage * output2 = cvCreateImage ( cvGetSize ( input ), IPL_DEPTH_32F, 1 );
    normalizeImage (SWTImage, output2);
    IplImage * saveSWT = cvCreateImage ( cvGetSize ( input ), IPL_DEPTH_8U, 1 );
    cvConvertScale(output2, saveSWT, 255, 0);
    cvSaveImage ( "imgs/SWT.png", saveSWT);
    
    cvReleaseImage ( &output2 );
    cvReleaseImage( &saveSWT );
    cvReleaseImage ( &SWTImage );
    cvReleaseImage ( &grayImage );
    cvReleaseImage ( &edgeImage );

}

#endif
