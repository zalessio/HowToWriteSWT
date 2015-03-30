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
#include "../include/imageio.h"
#include "../include/CannyEdge.h"
#include "../include/Swt.h"


void textDetection (IplImage * input, bool dark_on_light)
{   

    assert ( input->depth == IPL_DEPTH_8U );
    assert ( input->nChannels == 3 );
    printf("Running textDetection with dark_on_light %d\n", dark_on_light);
    
    int h,w;
    struct image grayImg,edgeImg,swtImg;
    convertImg(input, &grayImg);
    h = grayImg.height;
    w = grayImg.width;
    swtImg.width = w;
    swtImg.height = h;
    swtImg.pixel_data = (unsigned char *)malloc(w * h * sizeof(unsigned char));
    edgeImg.width = w;
    edgeImg.height = h;
    edgeImg.pixel_data = (unsigned char *)malloc(w * h * sizeof(unsigned char));

    //Canny Image
    printf("CANNY EDGE\n");
    canny_edge_detect(&grayImg, &edgeImg);
    save_img((char *)"imgs/DEFAULT_canny.png",&edgeImg);

    convertImg(input, &grayImg);
    //SWT 
    printf("SWT\n");
    strokeWidthTransform(&grayImg,&edgeImg,dark_on_light,&swtImg);

    save_img((char *)"imgs/DEFAULT_SWT.png",&swtImg);
    
    //free(grayImg.pixel_data);
    free(swtImg.pixel_data);
    free(edgeImg.pixel_data);
}

#endif
