#ifndef GAUSS_H_
#define GAUSS_H_

#include "ImageIO.h"


/*
	GAUSSIAN_NOISE_REDUCE
	apply 5x5 Gaussian convolution filter, shrinks the image by 4 pixels in each direction, using the classical Gaussian 5x5 kernel.
*/
void gaussian_noise_reduce(struct Image * img_in, struct Image * img_out)
{
	int w, h, x, y, max_x, max_y;
	w = img_in->width;
	h = img_in->height;
	img_out->width = w;
	img_out->height = h;
	max_x = w - 2;
	max_y = w * (h - 2);
	for (y = w * 2; y < max_y; y += w) {
		for (x = 2; x < max_x; x++) {
			img_out->pixel_data[x + y] = (2 * img_in->pixel_data[x + y - 2 - w - w] + 
			4 * img_in->pixel_data[x + y - 1 - w - w] + 
			5 * img_in->pixel_data[x + y - w - w] + 
			4 * img_in->pixel_data[x + y + 1 - w - w] + 
			2 * img_in->pixel_data[x + y + 2 - w - w] + 
			4 * img_in->pixel_data[x + y - 2 - w] + 
			9 * img_in->pixel_data[x + y - 1 - w] + 
			12 * img_in->pixel_data[x + y - w] + 
			9 * img_in->pixel_data[x + y + 1 - w] + 
			4 * img_in->pixel_data[x + y + 2 - w] + 
			5 * img_in->pixel_data[x + y - 2] + 
			12 * img_in->pixel_data[x + y - 1] + 
			15 * img_in->pixel_data[x + y] + 
			12 * img_in->pixel_data[x + y + 1] + 
			5 * img_in->pixel_data[x + y + 2] + 
			4 * img_in->pixel_data[x + y - 2 + w] + 
			9 * img_in->pixel_data[x + y - 1 + w] + 
			12 * img_in->pixel_data[x + y + w] + 
			9 * img_in->pixel_data[x + y + 1 + w] + 
			4 * img_in->pixel_data[x + y + 2 + w] + 
			2 * img_in->pixel_data[x + y - 2 + w + w] + 
			4 * img_in->pixel_data[x + y - 1 + w + w] + 
			5 * img_in->pixel_data[x + y + w + w] + 
			4 * img_in->pixel_data[x + y + 1 + w + w] + 
			2 * img_in->pixel_data[x + y + 2 + w + w]) / 159;
		}
	}
}

/*
	GAUSSIAN_NOISE_REDUCE_FLOAT_5
	apply 5x5 Gaussian convolution filter, shrinks the image by 4 pixels in each direction, using the classical Gaussian 5x5 kernel.
    The input, and output images are arrays of floats.
    h,w are the dimensions of the images
 */
void gaussian_noise_reduce_float_5(float * img_in, float * img_out, int h, int w)
{
	int x, y, max_x, max_y;
	max_x = w - 2;
	max_y = w * (h - 2);
	for (y = w * 2; y < max_y; y += w) {
		for (x = 2; x < max_x; x++) {
			img_out[x + y] = (2 * img_in[x + y - 2 - w - w] + 
			4 * img_in[x + y - 1 - w - w] + 
			5 * img_in[x + y - w - w] + 
			4 * img_in[x + y + 1 - w - w] + 
			2 * img_in[x + y + 2 - w - w] + 
			4 * img_in[x + y - 2 - w] + 
			9 * img_in[x + y - 1 - w] + 
			12 * img_in[x + y - w] + 
			9 * img_in[x + y + 1 - w] + 
			4 * img_in[x + y + 2 - w] + 
			5 * img_in[x + y - 2] + 
			12 * img_in[x + y - 1] + 
			15 * img_in[x + y] + 
			12 * img_in[x + y + 1] + 
			5 * img_in[x + y + 2] + 
			4 * img_in[x + y - 2 + w] + 
			9 * img_in[x + y - 1 + w] + 
			12 * img_in[x + y + w] + 
			9 * img_in[x + y + 1 + w] + 
			4 * img_in[x + y + 2 + w] + 
			2 * img_in[x + y - 2 + w + w] + 
			4 * img_in[x + y - 1 + w + w] + 
			5 * img_in[x + y + w + w] + 
			4 * img_in[x + y + 1 + w + w] + 
			2 * img_in[x + y + 2 + w + w]) / 159;
		}
	}
}

/*
	GAUSSIAN_NOISE_REDUCE_FLOAT_5
	apply 5x5 Gaussian convolution filter, shrinks the image by 4 pixels in each direction, using the classical Gaussian 3x3 kernel.
 The input, and output images are arrays of floats.
 h,w are the dimensions of the images
 */
void gaussian_noise_reduce_float_3(float * img_in, float * img_out, int h, int w)
{
	int x, y, max_x, max_y;
	max_x = w - 2;
	max_y = w * (h - 2);
	for (y = w * 2; y < max_y; y += w) {
		for (x = 2; x < max_x; x++) {
			img_out[x + y] = (
			1 * img_in[x + y - 1 - w] + 
			2 * img_in[x + y - w] + 
			1 * img_in[x + y + 1 - w] + 
			2 * img_in[x + y - 1] + 
			4 * img_in[x + y] + 
			2 * img_in[x + y + 1] + 
			1 * img_in[x + y - 1 + w] + 
			2 * img_in[x + y + w] + 
			1 * img_in[x + y + 1 + w] ) / 16;
		}
	}
}

#endif
