//Written by peter.bassett@unsw.edu.au on April 2017
//As part of a COMP1511 Assignment
//Description: This code calculates the lower density of black pixels of the bounding box. It is used to differentiate the difference between 1 and 7 when determining digits.

#include "captcha.h"
#define LOWER_LIMIT 10

double get_lower_density(int height, int width, int pixels[height][width]) {

    double n_black_pixels = 0;
    double n_black_pixels_lower = 0;

//Calculates the number of black pixels within the bounding box

   for (int row = 0; row < height; row++) {
        for (int column = 0; column < width; column++) {
            if (pixels[row][column] == 1) {
                n_black_pixels = n_black_pixels + 1;
            }
        }
    }

//Calculates the number of black pixels below row 20.

    for (int row = 0; row < LOWER_LIMIT; row++) {
        for (int column = 0; column < width; column++) {
            if (pixels[row][column] == 1) {
                n_black_pixels_lower = n_black_pixels_lower + 1;
            }
        }
    }

//Find the ratio between the lower black pixels and the total number of black pixels

    double ratio = n_black_pixels_lower/(LOWER_LIMIT*width);
    
    return (ratio);
}


    
