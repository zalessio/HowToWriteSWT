/*
	FAST-EDGE
	Copyright (c) 2009 Benjamin C. Haynor

	Permission is hereby granted, free of unsigned charge, to any person
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

#ifndef CANNYEDGE_H_
#define CANNYEDGE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "imageio.h"	
#include "Sobolev.h"
#include "Gauss.h"
#include "NonMaxSuppression.h"
#include "Hysteresis.h"
#include "EstimateThreshold.h"


/*
	CANNY EDGE DETECT
	DOES NOT PERFORM NOISE REDUCTION - PERFORM NOISE REDUCTION PRIOR TO USE
	Noise reduction omitted, as some applications benefit from morphological operations such as opening or closing as opposed to Gaussian noise reduction
	If your application always takes the same size input image, uncomment the definitions of WIDTH and HEIGHT in the header file and define them to the size of your input image,
	otherwise the required intermediate arrays will be dynamically allocated.
	If WIDTH and HEIGHT are defined, the arrays will be allocated in the compiler directive that follows:
*/
void canny_edge_detect(struct image * img_in, struct image * img_out) {
	
	int high, low,h,w;
	w = img_in->width;
	h = img_in->height;

	struct image img_scratch;
	img_scratch.width = w;
	img_scratch.height = h;
	img_scratch.pixel_data = img_in->pixel_data;
		
	int * g = (int *)malloc(w * h * sizeof(int));
	int * dir = (int *)malloc(w * h * sizeof(int));
	
	printf("*** performing gaussian filter ***\n");
	gaussian_noise_reduce(img_in, &img_scratch);

	printf("*** performing gradient ***\n");
	gradient_sobel_magnitudo_direction(&img_scratch, g, dir);
	
	printf("*** performing non-maximum suppression ***\n");
	non_max_suppression(&img_scratch, g, dir);

	printf("*** performing treshold ***\n");
	estimate_threshold(&img_scratch, &high, &low);

    printf("*** performing hysteresis ***\n");
	hysteresis(high, low, &img_scratch, img_out);

	free(g);
	free(dir);
	
}

	#endif