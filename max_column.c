// Written by peter.bassett@unsw.edu.au April 2017
// as a COMP1511 Assignment
// This code calculates the maximum number of black pixels in given row boundary

#include<stdio.h>
#include "captcha.h"

#define INCREASE 4

double get_max_column(int height, int width, int pixels[height][width], int starting_place) {

    int starting_point;
    int count = 0;
    double max_black_pixels = 0;

    if (starting_place == 1) { //For the upper proprtion of the bounding box
        starting_point = height-INCREASE;
    } else if (starting_place == 2) { //For the lower proportion of the bounding box
        starting_point = height/2-INCREASE/2;
    } else {  //For the botom proprtion of the bouding box
        starting_point = 0;
    }
    
    for (int row = starting_point; row < starting_point+INCREASE; row++) {
        count = 0;
        for (int column = 0; column < width; column++) {
            if (pixels[row][column] == 1) {
                count++;
            }
        }
        if (count > max_black_pixels) {
            max_black_pixels = count;
        }
    }
    
    return max_black_pixels;
}
