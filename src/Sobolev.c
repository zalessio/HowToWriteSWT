#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "TextDetection.h"

/*
	CALC_GRADIENT_SOBEL
	calculates the result of the Sobel operator - http://en.wikipedia.org/wiki/Sobel_operator - and estimates edge direction angle
*/
/*void calc_gradient_sobel(struct image * img_in, int g_x[], int g_y[], int g[], int dir[]) {//float theta[]) {*/
void gradient_sobel_magnitudo_direction(struct image * img_in, int *g , int *dir) {
	int w, h, x, y, max_x, max_y, g_x, g_y;
	float g_div;
	w = img_in->width;
	h = img_in->height;
	max_x = w - 3;
	max_y = w * (h - 3);
	for (y = w * 3; y < max_y; y += w) {
		for (x = 3; x < max_x; x++) {
			g_x = (2 * img_in->pixel_data[x + y + 1] 
				+ img_in->pixel_data[x + y - w + 1]
				+ img_in->pixel_data[x + y + w + 1]
				- 2 * img_in->pixel_data[x + y - 1] 
				- img_in->pixel_data[x + y - w - 1]
				- img_in->pixel_data[x + y + w - 1]);
			g_y = 2 * img_in->pixel_data[x + y - w] 
				+ img_in->pixel_data[x + y - w + 1]
				+ img_in->pixel_data[x + y - w - 1]
				- 2 * img_in->pixel_data[x + y + w] 
				- img_in->pixel_data[x + y + w + 1]
				- img_in->pixel_data[x + y + w - 1];
			#ifndef ABS_APPROX
			g[x + y] = sqrt(g_x * g_x + g_y * g_y);
			#endif
			#ifdef ABS_APPROX
			g[x + y] = abs(g_x[x + y]) + abs(g_y[x + y]);
			#endif
			if (g_x == 0) {
				dir[x + y] = 2;
			} else {
				g_div = g_y / (float) g_x;
				/* the following commented-out code is slightly faster than the code that follows, but is a slightly worse approximation for determining the edge direction angle
				if (g_div < 0) {
					if (g_div < -1) {
						dir[n] = 0;
					} else {
						dir[n] = 1;
					}
				} else {
					if (g_div > 1) {
						dir[n] = 0;
					} else {
						dir[n] = 3;
					}
				}
				*/
				if (g_div < 0) {
					if (g_div < -2.41421356237) {
						dir[x + y] = 0;
					} else {
						if (g_div < -0.414213562373) {
							dir[x + y] = 1;
						} else {
							dir[x + y] = 2;
						}
					}
				} else {
					if (g_div > 2.41421356237) {
						dir[x + y] = 0;
					} else {
						if (g_div > 0.414213562373) {
							dir[x + y] = 3;
						} else {
							dir[x + y] = 2;
						}
					}
				}
			}
		}
		
	}	
}

void gradient_sobel_x_y(struct image * img_in, unsigned char * g_x , unsigned char *g_y) {
	int w, h, x, y, max_x, max_y;

	w = img_in->width;
	h = img_in->height;
	max_x = w - 3;
	max_y = w * (h - 3);
	for (y = w * 3; y < max_y; y += w) {
		for (x = 3; x < max_x; x++) {
			
			g_x[x + y]  = (2 * img_in->pixel_data[x + y + 1] 
				+ img_in->pixel_data[x + y - w + 1]
				+ img_in->pixel_data[x + y + w + 1]
				- 2 * img_in->pixel_data[x + y - 1] 
				- img_in->pixel_data[x + y - w - 1]
				- img_in->pixel_data[x + y + w - 1]);
			g_y[x + y]  = 2 * img_in->pixel_data[x + y - w] 
				+ img_in->pixel_data[x + y - w + 1]
				+ img_in->pixel_data[x + y - w - 1]
				- 2 * img_in->pixel_data[x + y + w] 
				- img_in->pixel_data[x + y + w + 1]
				- img_in->pixel_data[x + y + w - 1];

		}
		
	}	
}