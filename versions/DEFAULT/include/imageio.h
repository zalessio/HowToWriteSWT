/*
	FAST-EDGE
	Copyright (c) 2009 Benjamin C. Haynor

	Permission is hereby granted, free of charge, to any person
	obtaining a copy of this software and associated documentation
	files (the "Software"), to deal in the Software without
	restriction, including without limitation the rights to use,
	copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the
	Software is furnished to do so, subject to the following
	conditions:

	The above copyright notice and this permission notice shall be
	included in all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
	OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
	NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
	HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
	WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
	FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
	OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef IMAGEIO_H_
#define IMAGEIO_H_

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv/cxcore.h>
#include <assert.h>
#include <stdio.h>
// #include <math.h>


/* DEFINE STRUCTURES */
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
    int len;
    struct Point2d *points;
};

struct image {
    int width;
    int height;
    unsigned char * pixel_data;
};


//bool Point2dSort (const Point2d &lhs, const Point2d &rhs) {
//    return lhs.SWT < rhs.SWT;
//}


void convertToFloatImage ( IplImage * byteImage, IplImage * floatImage )
{
  cvConvertScale ( byteImage, floatImage, 1 / 255., 0 );
}


IplImage * loadByteImage ( const char * name )
{
  IplImage * image = cvLoadImage ( name , 1);

  if ( !image )
  {
    return 0;
  }
  cvCvtColor ( image, image, CV_BGR2RGB );
  return image;
}

void convertImg(IplImage * input, struct image *output){
    printf("CREATE IMAGES \n");
    IplImage * grayImage = cvCreateImage ( cvGetSize ( input ), IPL_DEPTH_8U, 1 );
    //convert to grayscale
    cvCvtColor ( input, grayImage, CV_RGB2GRAY );
    //create output
    output->width = input->width;
    output->height = input->height;
    output->pixel_data = (unsigned char*)grayImage->imageData;
}

// using System;
// using System.IO;


void save_img(char * s, struct image *input){
    IplImage * output = cvCreateImage( cvSize (input->width,input->height),IPL_DEPTH_8U, 1 );
    output->imageData = (char *)input->pixel_data;
    cvSaveImage (s,output,0);
    cvReleaseImage(&output);
    // FileStream fs = new FileStream(new FileStream(@s,Filemode.Append,FileAccess.Write));
    // fs.write(input->pixel_data);
    // fs.Close();
}


//void save_and_convert_img(char * s, struct image *input){
//    IplImage * output = cvCreateImage( cvSize (input->width,input->height),IPL_DEPTH_32F, 1 );
//    output->imageData = (char *)input->pixel_data;
//    
//    normalizeImage (output, output);
//    cvConvertScale(output, output, 255, 0);
//   
//    cvSaveImage(s, output);
//    cvReleaseImage(&output);
//}
    
    //void normalizeImage (IplImage * input, IplImage * output) {
    //    assert ( input->depth == IPL_DEPTH_32F );
    //    assert ( input->nChannels == 1 );
    //    assert ( output->depth == IPL_DEPTH_32F );
    //    assert ( output->nChannels == 1 );
    //    float maxVal = 0;
    //    float minVal = 1e100;
    //    for( int row = 0; row < input->height; row++ ){
    //        const float* ptr = (const float*)(input->imageData + row * input->widthStep);
    //        for ( int col = 0; col < input->width; col++ ){
    //            if (*ptr < 0) { }
    //            else {
    //                maxVal = std::max(*ptr, maxVal);
    //                minVal = std::min(*ptr, minVal);
    //            }
    //            ptr++;
    //        }
    //    }
    //    float difference = maxVal - minVal;
    //    for( int row = 0; row < input->height; row++ ){
    //        const float* ptrin = (const float*)(input->imageData + row * input->widthStep);\
    //        float* ptrout = (float*)(output->imageData + row * output->widthStep);\
    //        for ( int col = 0; col < input->width; col++ ){
    //            if (*ptrin < 0) {
    //                *ptrout = 1;
    //            } else {
    //                *ptrout = ((*ptrin) - minVal)/difference;
    //            }
    //            ptrout++;
    //            ptrin++;
    //        }
    //    }
    //}

//class FeatureError : public std::exception
//{
    //std::string message;
    //public:
    //FeatureError ( const std::string & msg, const std::string & file )
    //{
    //  std::stringstream ss;
    //
    //  ss << msg << " " << file;
    //  message = msg.c_str ();
    //}
    //~FeatureError () throw ( )
    //{
    //}
    //};
    
//IplImage * loadFloatImage ( const char * name )
//{
//  IplImage * image = cvLoadImage ( name );
//
//  if ( !image )
//  {
//    return 0;
//  }
//  cvCvtColor ( image, image, CV_BGR2RGB );
//  IplImage * floatingImage = cvCreateImage ( cvGetSize ( image ),
//                                             IPL_DEPTH_32F, 3 );
//  cvConvertScale ( image, floatingImage, 1 / 255., 0 );
//  cvReleaseImage ( &image );
//  return floatingImage;
//}


#endif
