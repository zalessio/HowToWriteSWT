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
#include <assert.h>
#include <stdbool.h>
#include "ImageIO.h"
#include "CannyEdge.h"
#include "Swt.h"


void textDetection (IplImage * inputImage, bool dark_on_light)
{
    assert ( inputImage->depth == IPL_DEPTH_8U );
    assert ( inputImage->nChannels == 3 );
    
    printf("Running textDetection with dark_on_light %d\n", dark_on_light);
    
    int h,w;
    struct Image grayImg,grayImg2,edgeImg,swtImg;
    
    printf("CREATE IMAGES \n");
    h = inputImage->height;
    w = inputImage->width;
    grayImg.width = w;
    grayImg.height = h;
    grayImg.pixel_data = (unsigned char *)malloc(w * h * sizeof(unsigned char));
    grayImg2.width = w;
    grayImg2.height = h;
    grayImg2.pixel_data = (unsigned char *)malloc(w * h * sizeof(unsigned char));
    swtImg.width = w;
    swtImg.height = h;
    swtImg.pixel_data = (unsigned char *)malloc(w * h * sizeof(unsigned char));
    edgeImg.width = w;
    edgeImg.height = h;
    edgeImg.pixel_data = (unsigned char *)malloc(w * h * sizeof(unsigned char));
    printf("CONVERT INPUT IMAGE \n");
    convertImg(inputImage, &grayImg);
    convertImg(inputImage, &grayImg2);

    //Canny Image
    printf("CANNY EDGE\n");
    canny_edge_detect(&grayImg, &edgeImg);
    //save
    save_img((char *)"imgs/DEFAULT_canny.png",&edgeImg);

    //SWT
    printf("SWT\n");
    strokeWidthTransform(&grayImg2,&edgeImg,dark_on_light,&swtImg);
    //save
    save_img((char *)"imgs/DEFAULT_SWT.png",&swtImg);
    
    free(grayImg.pixel_data);
    free(swtImg.pixel_data);
    free(edgeImg.pixel_data);
}

#endif
