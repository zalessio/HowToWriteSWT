#ifndef SWT_H_
#define SWT_H_

#include <cmath>
#include "Sobel.h"
#include "Gauss.h"
#include "../include/imageio.h"


#define PI 3.14159265

// void SWTMedianFilter (struct image * SWTImg, std::vector<Ray> & rays) {

//     int h = SWTImg->height;
//     int w = SWTImg->width;

//     for (std::vector<Ray>::iterator rit = rays.begin(); rit != rays.end(); rit++) {
//         for (std::vector<Point2d>::iterator pit = rit->points.begin(); pit != rit->points.end(); pit++) {
//             pit->SWT = SWTImg->pixel_data[pit->y+w*pit->x];
//         }
//         std::sort(rit->points.begin(), rit->points.end(), &Point2dSort);
//         float median = (rit->points[rit->points.size()/2]).SWT;
//         for (std::vector<Point2d>::iterator pit = rit->points.begin(); pit != rit->points.end(); pit++) {
//             SWTImg->pixel_data[pit->y+w*pit->x] = std::min(pit->SWT, median);
//         }
//     }

// }

void strokeWidthTransform (struct image * grayImg, struct image * edgeImg,
                           bool dark_on_light,
                           struct image * SWTImg) {

    int h = grayImg->height;
    int w = grayImg->width;

    int i;

    int rays_dim = w*h/200;
    int rays_it = 0;
    int points_dim = w*h/2;
    int points_it = 0;
    Ray *rays = (Ray *) malloc(rays_dim*sizeof(Ray));
    Point2d *points = (Point2d *) malloc(points_dim*sizeof(Point2d));

    struct image gaussianImg;
    gaussianImg.width = w;
    gaussianImg.height = h;
    gaussianImg.pixel_data = (unsigned char *)malloc(w * h * sizeof(unsigned char));
    float * gradientXtemp = (float *)malloc(w * h * sizeof(float));
    float * gradientYtemp = (float *)malloc(w * h * sizeof(float));
    float * gradientX = (float *)malloc(w * h * sizeof(float));
    float * gradientY = (float *)malloc(w * h * sizeof(float));

    printf("*** performing gradient ***\n");
    gaussian_noise_reduce(grayImg, &gaussianImg);
    gradient_sobel_x_y(&gaussianImg, gradientXtemp , gradientYtemp);
    gaussian_noise_reduce_float_5(gradientXtemp,gradientX,h,w);
    gaussian_noise_reduce_float_5(gradientYtemp,gradientY,h,w);
    
    printf("*** initializing SWT image ***\n");
    for(int y = 0; y < h; y++ ){
        for (int x = 0; x < w; x++ ){
            SWTImg->pixel_data[x+w*y] = 255;
        }
    }
    
    printf("*** finding rays ***\n");
    float prec = .05;
    for( int row = 0; row < h; row++ ){
        for ( int col = 0; col < w; col++ ){
            if (edgeImg->pixel_data[row*w + col] > 0) {

                if(points_it<points_dim) {
                    points[points_it].x = col; 
                    points[points_it].y = row; 
                    points_it++;
                } else {
                    // NOTICE: REALLOC NOT WORKING
                    points = (Point2d *) realloc(points,2*points_dim*sizeof(Point2d));
                    points[points_it].x = col; 
                    points[points_it].y = row; 
                    points_it++;
                }

                if(rays_it<rays_dim) {
                    rays[rays_it].len = 1;
                    rays[rays_it].points = &points[points_it-1];
                    rays_it++;
                } else {
                    // NOTICE: REALLOC NOT WORKING
                    rays = (Ray *) realloc(rays, 2*rays_dim*sizeof(Ray) );
                    rays[rays_it].len = 1;
                    rays[rays_it].points = &points[points_it-1];
                    rays_it++;
                }

                float curX = (float)col + 0.5;
                float curY = (float)row + 0.5;
                
                int curPixX = col;
                int curPixY = row;
                float G_x = gradientX[col+w*row];
                float G_y = gradientY[col+w*row];

                // normalize gradient
                float mag = sqrt( (G_x * G_x) + (G_y * G_y) );
                if (dark_on_light){
                    G_x = -G_x/mag;
                    G_y = -G_y/mag;
                } else {
                    G_x = G_x/mag;
                    G_y = G_y/mag;
                }

                while (true) {
                    curX += G_x*prec;
                    curY += G_y*prec;

                    if ((int)(floor(curX)) != curPixX || (int)(floor(curY)) != curPixY) {

                        curPixX = (int)(floor(curX));
                        curPixY = (int)(floor(curY));

                        // check if pixel is outside boundary of image
                        if (curPixX < 0 || (curPixX >= w) || curPixY < 0 || (curPixY >= h)) {
                            points_it = points_it - (rays[rays_it-1].len);
                            rays_it--;
                            break;
                        }

                        Point2d pnew;
                        pnew.x = curPixX;
                        pnew.y = curPixY;
                        rays[rays_it-1].len++;

                        if(points_it<points_dim) {
                            points[points_it] = pnew;
                            points_it++;
                        } else {
                            // NOTICE: NOT WORKING
                            points = (Point2d *) realloc(points,2*points_dim*sizeof(Point2d));
                            points[points_it] = pnew;
                            points_it++;
                        }

                        // Final point found -> enter following if-statement
                        if (edgeImg->pixel_data[curPixY*w+curPixX] > 0) {
                            // dot product
                            float G_xt = gradientX[curPixY*w+curPixX];
                            float G_yt = gradientY[curPixY*w+curPixX];
                            mag = sqrt( (G_xt * G_xt) + (G_yt * G_yt) );
                            if (dark_on_light){
                                G_xt = -G_xt/mag;
                                G_yt = -G_yt/mag;
                            } else {
                                G_xt = G_xt/mag;
                                G_yt = G_yt/mag;
                            }

                            if (acos(G_x * -G_xt + G_y * -G_yt) < PI/2.0) {
                                for(i=0; i<rays[rays_it-1].len; ++i){
                                    Point2d tmp = rays[rays_it-1].points[i];
                                    SWTImg->pixel_data[tmp.y*w+tmp.x] = 0;   
                                }
                            } else {
                                points_it = points_it - (rays[rays_it-1].len);
                                rays_it--;
                            }
                            break;
                        }
                    }
                }
            }
        }
    }


    free(rays);
    free(points);
    //SWTMedianFilter(SWTImg, rays );
    return;
}


#endif