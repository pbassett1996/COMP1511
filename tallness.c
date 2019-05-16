// Written by peter.bassett@unsw.edu.au April 2017
// as a COMP1511 Assignment
// This code calculates the tallness of a digit

#include "captcha.h"

double get_tallness(int height, int width, int pixels[height][width]) {

    double tallness = height/(double)width;
    return tallness;
}

