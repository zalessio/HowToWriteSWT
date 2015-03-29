#ifndef HYSTERESIS_H_
#define HYSTERESIS_H_

#include "imageio.h"	


/*
	RANGE
	check if the coordinates are inside the image
 */
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

/*
	TRACE
	find only the pixels with a gradient magnitudo higher then the low threshold (weak edges) that are in the neighborhood of strong edges
 */
int trace(int x, int y, int low, struct image * img_in, struct image * img_out)
{
	int y_off, x_off;
	if (img_out->pixel_data[y * img_out->width + x] == 0)
	{
		img_out->pixel_data[y * img_out->width + x] = 255;

		for (y_off = -1; y_off <=1; y_off++)
		{
		    for(x_off = -1; x_off <= 1; x_off++)
		    {
                //if the pixel is a weak edge then draw it and control if the neighbors are weak edges
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

/*
	HYSTERESIS
	compute the hysteresis keeping only the pixels with an high gradient magnitudo (strong edges) and then computing the weak edges that are in the neighborhood of these pixel (with the function trace)
 */
void hysteresis (int high, int low, struct image * img_in, struct image * img_out)
{
	int x, y, n, max,w,h;
	w = img_in->width;
	h = img_in->height;
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
