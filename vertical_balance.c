// written by andrewt@unsw.edu.au April 2017 and edited by peter.bassett@unsw.edu.au April 2017
//As part of the COMP1511 Assignment
//Description: This code calculates the vertical balance (centre of gravity in vertical difrection) of the image.

#include "captcha.h"

// find vertical horizontal balance point (centre of gravity) of image

double get_vertical_balance(int height, int width, int pixels[height][width]) {
    double row_sum = 0;
    int n_black_pixels = 0;
        for (int column = 0; column < width; column++) {
           for (int row = 0; row < height; row++) {
            if (pixels[row][column] == 1) {
                row_sum = row_sum + row;
                n_black_pixels = n_black_pixels + 1;
            }
        }
    }
    return (row_sum/n_black_pixels + 0.5)/width;
}

