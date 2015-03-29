#ifndef NONMAXSUPPRESSION_H_
#define NONMAXSUPPRESSION_H_

#include "imageio.h"	


/*
	NON_MAX_SUPPRESSION
	using the estimates of the Gx and Gy image gradients and the edge direction angle determines whether the magnitude of the gradient assumes a local  maximum in the gradient direction
	if the rounded edge direction angle is 0 degrees, checks the north and south directions
	if the rounded edge direction angle is 45 degrees, checks the northwest and southeast directions
	if the rounded edge direction angle is 90 degrees, checks the east and west directions
	if the rounded edge direction angle is 135 degrees, checks the northeast and southwest directions
*/
void non_max_suppression(struct image * img, int *g, int *dir) {
	
	int w, h, x, y, max_x, max_y;
	w = img->width;
	h = img->height;
	
	max_x = w;
	max_y = w * h;
	for (y = 0; y < max_y; y += w) {
		for (x = 0; x < max_x; x++) {
			switch (dir[x + y]) {
				case 0:
					if (g[x + y] > g[x + y - w] && g[x + y] > g[x + y + w]) {
						if (g[x + y] > 255) {
						img->pixel_data[x + y] = 0xFF;
						} else {
							img->pixel_data[x + y] = g[x + y];
						}
					} else {
						img->pixel_data[x + y] = 0x00;
					}
					break;
				case 1:
					if (g[x + y] > g[x + y - w - 1] && g[x + y] > g[x + y + w + 1]) {
						if (g[x + y] > 255) {
						img->pixel_data[x + y] = 0xFF;
						} else {
							img->pixel_data[x + y] = g[x + y];
						}
					} else {
						img->pixel_data[x + y] = 0x00;
					}
					break;
				case 2:
					if (g[x + y] > g[x + y - 1] && g[x + y] > g[x + y + 1]) {
						if (g[x + y] > 255) {
						img->pixel_data[x + y] = 0xFF;
						} else {
							img->pixel_data[x + y] = g[x + y];
						}
					} else { 
						img->pixel_data[x + y] = 0x00;
					}
					break;
				case 3:
					if (g[x + y] > g[x + y - w + 1] && g[x + y] > g[x + y + w - 1]) {
						if (g[x + y] > 255) {
						img->pixel_data[x + y] = 0xFF;
						} else {
							img->pixel_data[x + y] = g[x + y];
						}
					} else {
						img->pixel_data[x + y] = 0x00;
					}
					break;
				default:
					printf("ERROR - direction outside range 0 to 3");
					break;
			}
		}
	}

}

#endif