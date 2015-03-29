#ifndef CANNYEDGE_H_
#define CANNYEDGE_H_

#include <stdio.h>
#include "imageio.h"
#include "Sobel.h"
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
	
	int high,low,h,w;
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