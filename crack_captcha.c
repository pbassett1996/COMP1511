// written by andrewt@unsw.edu.au April 2017 and modified by peter.bassett@unsw.edu.au April 2017
// as COMP1511 sample solution and edited as COMP1511 Assignment
// identify digit in monochrome captcha in the pbm format 

#include <stdio.h>
#include "captcha.h"

void analyze_image(int height, int width, int pixels[height][width]);
int identify_digit(double horizontal_balance, double vertical_balance, double tallness, double density, int holes, double hole_fraction, double lower_density, double right_density, int which_hole, double middle_max, double upper_max, double lower_max, double symmetry);

// The main was written by Andrew Taylor. It finds the file name in the command line and then inputs it into read_pbm.c to output the captcha in an array called 'pixels'.
int main(int argc, char *argv[]) {
    int height, width;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <image-file>\n", argv[0]);
        return 1;
    }

    // first get image dimensions
    if (get_pbm_dimensions(argv[1], &height, &width) != 1) {
        return 1;
    }

    // now declare appropriate array
    int pixels[height][width];
    if (read_pbm(argv[1], height, width, pixels)) {
        analyze_image(height, width, pixels);
    }
    return 0;     
}

// This function uses a while loop to seperate the digits in the captcha, create a bounding box around them and then use a variety of attributes in order to determine the value of each individual digit. It will then ouput these digits.
void analyze_image(int height, int width, int pixels[height][width]) {
    int which_digit = 1;
    int end_column = 0;

    while (which_digit < 5) {
        
        //Intiatialising variables
        int start_row, start_column, box_width, box_height, start_column2, end_column2, which_hole;
        double hole_fraction;
        
        //Seperates the captcha into four digits
        seperate_digits(height, width, pixels, &start_column2, &end_column2, which_digit, end_column);
        
        //Creates a bounding box around the digits and outputs them into box_pixels
        get_bounding_box_captcha(height, width, pixels, &start_row, &start_column, &box_height, 
        &box_width, start_column2, end_column2);
        int box_pixels[box_height][box_width];
        copy_pixels(height, width, pixels, start_row, start_column, box_height, box_width, box_pixels);
        
        //Finds the horizontal balance of the digits
        double horizontal_balance = get_horizontal_balance(box_height, box_width, box_pixels);
        
        //Finds the density of black pixels for each digit
        double density = get_density(box_height, box_width, box_pixels);

        // Finds the density of black pixels in the lower proprtion of the array
        double lower_density = get_lower_density(box_height, box_width, box_pixels);
        
        //Finds the vertical balance of the digits
        double vertical_balance = get_vertical_balance(box_height, box_width, box_pixels);
        
        //Finds the number of holes (if any) of the digits
        int hole = get_holes(box_height, box_width, box_pixels, &hole_fraction, &which_hole);

        //Calcaultes the tallness of the array
        double tallness = get_tallness(box_height, box_width, box_pixels);

        //Calculates the density of black pixels in the right hand side of the array
        double right_density = get_right_density(box_height, box_width, box_pixels);
        
        //Determines the max number of pixels in one row for the upper proportion of the digit
        int starting_place = 1;
        double upper_max = get_max_column(box_height, box_width, box_pixels, starting_place);

        //Determines the max number of pixels in one row for the middle proportion of the digit
        starting_place = 2;
        double middle_max  = get_max_column(box_height, box_width, box_pixels, starting_place);

        //Determines the max number of pixels in one row for the lower proportion of the digit
        starting_place = 3;
        double lower_max  = get_max_column(box_height, box_width, box_pixels, starting_place);

        //Determines the symmetry of the digits
        double symmetry = get_symmetry(box_height, box_width, box_pixels);

        //Inputs all the atributes and then outputs a digit via the function 'identify_digit'
        int digit = identify_digit(horizontal_balance, vertical_balance, tallness,density,
        hole, hole_fraction, lower_density, right_density, which_hole, middle_max, upper_max, 
        lower_max, symmetry);
        
        //Outputs the digit into stdout
        printf("%c", digit);

        end_column = end_column2;
        which_digit++;
    }
    printf("\n");
}

// identify digit based on horizontal balance, vertical balance, tallness, density, number of holes, hole fraction, location of holes, lower density, right density, upper, middle and lower max rows and symmetry. The basis behind this function is to create a precise range in which the digits will fall into and then to allocate if statements in an order that achieves the highest accuracy.

int identify_digit(double horizontal_balance, double vertical_balance, double tallness, double density, int hole, double hole_fraction, double lower_density, double right_density, int which_hole, double middle_max, double upper_max, double lower_max, double symmetry) {

    if (hole == 0) {
        if (horizontal_balance < 0.700 && vertical_balance < 1.479 && tallness < 2.766 && 
        density < 0.504 && lower_density < 0.472 && right_density < 4.032 && lower_max < 17.001 
        && middle_max < 15.001 && upper_max < 38.001 && symmetry < 1.590 && 
        upper_max/lower_max < 8.688 && upper_max/middle_max < 7.344 && lower_max/middle_max < 1.401 
        && horizontal_balance > 0.419 && vertical_balance > 0.523 && tallness > 0.913 && density > 
        0.202 && lower_density > 0.095 && right_density > 0.230 && lower_max > 2.999 && middle_max > 
        2.999 && upper_max > 4.999 && symmetry > 0.234 && upper_max/lower_max > 0.666 && 
        upper_max/middle_max > 0.599 && lower_max/middle_max > 0.555 ) {
            return '7';
        } else if (horizontal_balance < 0.699 && vertical_balance < 1.355 && tallness < 2.639 && 
        density < 0.555 && lower_density < 0.820 && right_density < 1.189 && lower_max < 37.001 && 
        middle_max < 13.001 && upper_max < 25.001&& symmetry < 2.001 && upper_max/lower_max < 1.924 && 
        upper_max/middle_max < 3.176 && lower_max/middle_max < 5.360 && horizontal_balance > 0.422 && 
        vertical_balance > 0.496 && tallness > 1.078 && density > 0.176 && lower_density > 0.195 && 
        right_density > 0.183 && lower_max > 4.999 && middle_max > 2.999 && upper_max > 4.999 && symmetry 
        > 0.136 && upper_max/lower_max > 0.191 && upper_max/middle_max > 0.454 && lower_max/middle_max > 
        0.544 ) {
            return '1';
        } else if (horizontal_balance < 0.628 && vertical_balance < 1.121 && tallness < 2.039 && density 
        < 0.557 && lower_density < 0.854 && right_density < 2.028 && lower_max < 40.001 && middle_max < 
        17.626 && upper_max < 27.501 && symmetry < 1.190 && upper_max/lower_max < 3.360 && upper_max/
        middle_max < 4.001 && lower_max/middle_max < 5.001 && horizontal_balance > 0.445 && 
        vertical_balance > 0.349 && tallness > 0.890 && density > 0.264 && lower_density > 0.241 && 
        right_density > 0.352 && lower_max > 4.999 && middle_max > 2.999 && upper_max > 9.999 && symmetry 
        > 0.362 && upper_max/lower_max > 0.374 && upper_max/middle_max > 0.761 && lower_max/middle_max > 
        0.428) {
            return '2';
        } else if (horizontal_balance < 0.658 && vertical_balance < 1.078 && tallness < 2.165 && density 
        < 0.589 && lower_density < 0.801 && right_density < 2.036 && lower_max < 31.626 && middle_max < 
        25.501 && upper_max < 28.501 && symmetry < 0.792 && upper_max/lower_max < 1.833 && upper_max/
        middle_max < 2.072 && lower_max/middle_max < 2.069 && horizontal_balance > 0.512 && 
        vertical_balance > 0.442 && tallness > 0.859 && density > 0.216 && lower_density > 0.256 && 
        right_density > 0.510 && lower_max > 8.999 && middle_max > 6.999 && upper_max > 8.499 && symmetry 
        > 0.239 && upper_max/lower_max > 0.319 && upper_max/middle_max > 0.363 && lower_max/middle_max > 
        0.454) {
            return '3';
        } else { 
            return '5';
        }
    }
    
    if (hole == 1) {
        if (which_hole == 1) {
            return '6';
        } else if (horizontal_balance < 0.540 && vertical_balance < 1.070 && tallness < 2.160 && density 
        < 0.696 && lower_density < 0.673 && right_density < 3.364 && lower_max < 25.001 && middle_max < 
        33.001 && upper_max < 26.501 && symmetry < 1.320 && upper_max/lower_max < 1.666 && upper_max/
        middle_max < 1.833 && lower_max/middle_max < 2.015 && horizontal_balance > 0.443 && 
        vertical_balance > 0.485 && tallness > 0.999 && density > 0.308 && lower_density > 0.341 && 
        right_density > 0.864 && lower_max > 10.999 && middle_max > 5.999 && upper_max > 9.999 && 
        symmetry > 0.752 && upper_max/lower_max > 0.549 && upper_max/middle_max > 0.484 && lower_max/
        middle_max > 0.499 && hole_fraction < 0.479 && hole_fraction > 0.116) {
            return '0';
        } else if (horizontal_balance < 0.633 && vertical_balance < 0.920 && tallness < 1.870 && density 
        < 0.600 && lower_density < 0.625 && right_density < 1.680 && lower_max < 24.501 && middle_max < 
        29.501 && upper_max < 19.501 && symmetry < 0.966 && upper_max/lower_max < 2.401 && upper_max/
        middle_max < 1.050 && lower_max/middle_max < 1.440 && horizontal_balance > 0.462 && 
        vertical_balance > 0.975 && tallness > 0.259 && density > 0.999 && lower_density > 0.130 && 
        right_density > 0.183 && lower_max > 3.999 && middle_max > 7.999 && upper_max > 3.999 && symmetry 
        > 0.333 && upper_max/lower_max > 0.303 && upper_max/middle_max > 0.307 && lower_max/middle_max > 
        0.299 && hole_fraction < 0.129 && hole_fraction > 0.021) {
            return '4';
        } else {
            return '9';
        }
    }
    
    if (hole == 2) {
        return '8';
    } else {
        return '0';
    }
}
        
        
