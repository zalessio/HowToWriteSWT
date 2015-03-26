#ifndef HYSTERESIS_H_
#define HYSTERESIS_H_

#define LOW_THRESHOLD_PERCENTAGE 0.98 // percentage of the high threshold value that the low threshold shall be set at
#define HIGH_THRESHOLD_PERCENTAGE 0.05 // percentage of pixels that meet the high threshold - for example 0.15 will ensure that at least 15% of edge pixels are considered to meet the high threshold

#include "imageio.h"	


int range(struct image * img, int x, int y)
{
	if ((x < 0) || (x >= img->width)) {
		return(0);
	}
	if ((y < 0) || (y >= img->height)) {
		return(0);
	}
	return(1);
}

int trace(int x, int y, int low, struct image * img_in, struct image * img_out)
{
	int y_off, x_off;//, flag;
	if (img_out->pixel_data[y * img_out->width + x] == 0)
	{
		img_out->pixel_data[y * img_out->width + x] = 255;

		for (y_off = -1; y_off <=1; y_off++)
		{
		    for(x_off = -1; x_off <= 1; x_off++)
		    {
				if (!(y == 0 && x_off == 0) && range(img_in, x + x_off, y + y_off) && img_in->pixel_data[(y + y_off) * img_out->width + x + x_off] >= low) {
					if (trace(x + x_off, y + y_off, low, img_in, img_out))
					{
					    return(1);
					}
				}
		    }
		}
		return(1);
	}
	return(0);
}

void hysteresis (int high, int low, struct image * img_in, struct image * img_out)
{
	int x, y, n, max,w,h;
	w = img_out->width;
	h = img_out->height;
	max =  w*h;

	for (n = 0; n < max; n++) {
		img_out->pixel_data[n] = 0x00;
	}

	for (y=0; y < h; y++) {
	  for (x=0; x < w; x++) {
			if (img_in->pixel_data[y * w + x] >= high) {
				trace (x, y, low, img_in, img_out);
			}
		}
	}
	
}

#endif
