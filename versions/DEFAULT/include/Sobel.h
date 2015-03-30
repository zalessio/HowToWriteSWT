#ifndef SOBEL_H_
#define SOBEL_H_

#include "ImageIO.h"


/*
	GRADIENT_SOBEL_MAGNITUDO_DIRECTION
	calculates the result of the Sobel operators (applying the Sobel kernels in x and y directions) and estimates gradient magnitudo (g) and direction angle (dir)
*/
void gradient_sobel_magnitudo_direction(struct Image * img_in, int *g , int *dir) {
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

/*
	GRADIENT_SOBEL_MAGNITUDO_DIRECTION
	calculates the result of the Sobel operators (applying the Sobel kernels in x and y directions) and estimates the two gradients x and y
 */
void gradient_sobel_x_y(struct Image * img_in, float *g_x, float *g_y) {
	int w, h, x, y, max_x, max_y;
	w = img_in->width;
	h = img_in->height;
	max_x = w - 3;
	max_y = w * (h - 3);
    
	for (y = w * 3; y < max_y; y += w) {
		for (x = 3; x < max_x; x++) {
			g_x[x + y]  = 2 * img_in->pixel_data[x + y + 1] 
				+ img_in->pixel_data[x + y - w + 1]
				+ img_in->pixel_data[x + y + w + 1]
				- 2 * img_in->pixel_data[x + y - 1] 
				- img_in->pixel_data[x + y - w - 1]
				- img_in->pixel_data[x + y + w - 1];

			g_y[x + y]  = - 2 * img_in->pixel_data[x + y - w] 
				- img_in->pixel_data[x + y - w + 1]
				- img_in->pixel_data[x + y - w - 1]
				+ 2 * img_in->pixel_data[x + y + w] 
				+ img_in->pixel_data[x + y + w + 1]
				+ img_in->pixel_data[x + y + w - 1];
		}
	}	
}

#endif