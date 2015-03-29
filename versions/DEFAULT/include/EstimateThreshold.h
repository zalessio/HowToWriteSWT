#ifndef ESTIMATETHRESHOLD_H_
#define ESTIMATETHRESHOLD_H_

#include "imageio.h"

#define LOW_THRESHOLD_PERCENTAGE 0.98 // percentage of the high threshold value that the low threshold shall be set at
#define HIGH_THRESHOLD_PERCENTAGE 0.02 // percentage of pixels that meet the high threshold - for example 0.15 will ensure that at least 15% of edge pixels are considered to meet the high threshold


/*
	ESTIMATE_THRESHOLD
	estimates hysteresis threshold, assuming that the top X% (as defined by the HIGH_THRESHOLD_PERCENTAGE) of edge pixels with the greatest intesity are true edges
	and that the low threshold is equal to the quantity of the high threshold plus the total number of 0s at the low end of the histogram divided by 2
 */
void estimate_threshold(struct image * img, int * high, int * low) {
	
	int i, max, pixels, high_cutoff;
	int histogram[256];
	max = img->width * img->height;
	for (i = 0; i < 256; i++) {
		histogram[i] = 0;
	}
	for (i = 0; i < max; i++) {
		histogram[abs(img->pixel_data[i])]++;
	}
	pixels = (max - histogram[0]) * HIGH_THRESHOLD_PERCENTAGE;
	high_cutoff = 0;
	i = 255;
	while (high_cutoff < pixels) {
		high_cutoff += histogram[i];
		i--;
	}
	*high = i;

	i = 1;
	while (histogram[i] == 0) {
		i++;
	}
	*low = (*high + i) * LOW_THRESHOLD_PERCENTAGE;
	
}

#endif
