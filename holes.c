//Written by peter.bassett@unsw.edu.au April 2017
//As part of the COMP1511 Assignment 1
//This function determines the number of holes (if any) in a digit and also outputs the hole_fraction and position of the holes.

#include <stdio.h>
#include "captcha.h"

#define HEIGHT 74
#define WIDTH 200
#define NOT_ASSIGNED -1


int get_holes(int height, int width, int pixels[height][width], double *hole_fraction, int *which_hole) {

    int row, column, row2, column2, replace_with, replace, copy[HEIGHT][WIDTH];
    int no_holes = 0; //Number of holes in the digit
    int new_label = 0;
        
//Create a new array with an increased bounding box_height

    int bounding_increase[HEIGHT][WIDTH];
    int bound_i_height = height + 2;
    int bound_i_width = width + 2;
    
    for (row = 0; row < bound_i_height; row++) {
        bounding_increase[row][0] = 0;
        bounding_increase[row][width+1] = 0;
    }
    
    for (column = 0; column < bound_i_width; column++) {
        bounding_increase[0][column] = 0;
        bounding_increase[height+1][column] = 0;
    }
    
    for (row = 0; row < height; row++) {
        for (column = 0; column < width; column++) {
            bounding_increase[row+1][column+1] = pixels[row][column];
        }
    }
            

//Creating an array of pixels

    for (row = 0; row < bound_i_height; row++) {
        for (column = 0; column < bound_i_width; column++) {
            copy[row][column] = NOT_ASSIGNED;
        }
    }

//Computing connected component labeling algorithm was used to determine the holes. It involves labelling connected components and comparing them to their surrounding pixels. The algorithm was sourced from wikipedia https://en.wikipedia.org/wiki/Connected-component_labeling . The technique used was 'One component at a time'.

    for (row = 0; row < bound_i_height; row++) {
        for (column = 0; column < bound_i_width; column++) {
            if(row > 0 && column > 0 && copy[row-1][column] != NOT_ASSIGNED && copy[row][column-1] != 
            NOT_ASSIGNED  && copy[row-1][column] != copy[row][column-1] && bounding_increase[row][column] 
            == bounding_increase[row-1][column] && bounding_increase[row][column] == bounding_increase
            [row][column-1]) { 
            //if the pixels before (column-1) and below (row-1) are equal to current pixel then proceed
            //if the pixel before is greater than the pixel below then store pixels in the following order
                if (copy[row][column-1] > copy[row-1][column]) {
                    copy[row][column] = copy[row-1][column];
                    replace = copy[row][column-1];
                    replace_with = copy[row-1][column];
                } else {
                    copy[row][column] = copy[row][column-1];
                    replace = copy[row-1][column];
                    replace_with = copy[row][column-1];
                }
                //Conencted pixels (i.e. same value) are replaced with eachother to be assigned to one value

                for (column2 = 0; column2 < bound_i_width; column2++) {
                    for (row2 = row; row2 > 0; row2--) {
                        if (copy[row2][column2] == replace) {
                            copy[row2][column2] = replace_with;
                        }
                    }
                }
                no_holes--;

            //If the pixel is only connected to the below or before pixel then make it the same value
            } else if (row > 0 && bounding_increase[row][column] == bounding_increase[row-1][column]) {
                copy[row][column] = copy[row-1][column];
            } else if (column > 0 && bounding_increase[row][column] == bounding_increase[row][column-1]) {
                copy[row][column] = copy[row][column-1];                
            } else { //If the pixel is not connected to either pixels then make a new label
                copy[row][column] = new_label; 
                new_label++;
                no_holes++;
            }
            
        }
        

    }
    
   int holes = no_holes - 2;
   int max_row = 0;
   int max_column = 0;
   int max_number = copy[0][0];
   double count = 0;
   
//Find middle of 0s && irregular occurances

    for(row = 0; row < bound_i_height; row++) {
       for(column = 0; column < width+1; column++){
          if(copy[row][column] > max_number && holes == 2) {
            max_number = copy[row][column];
            max_row = row;
            max_column = column;
          }
          if (copy[row][column] == 1) {
            count++;
         }
          
        }
    }
 
    if(bounding_increase[max_row][max_column] == 1) { //Determines whether a dot lies in the middle of a 0
        holes--;
    }
    
    if (count < 2) { //Determines any irregular occuranceso on the digits
        holes--;
    }
    
//Determines the hole_fraction of the digit_num

  
    count = 0;
    
    for (row = 0; row < height+1; row++) {
        for (column=0; column < width+2; column++) {
            if (copy[row][column] > 1) {
                count = count+1;
            }
        }
    }
    
    *hole_fraction = count/(height*width);
    
//Determines the position of the holes
    max_row = 0;
    max_column = 0;
    for (row = height+1; row>=0; row--) {
        for (column=0; column < width+2; column++) {
            if (copy[row][column] > 1) {
                max_row = row;
                max_column = column;
                break;
            }
        }
        if(max_row > 0) {
            break;
        }
    }
    
    
    if (max_row > 2*height/3) {
        *which_hole = 2; //upper hole 
    } else {
        *which_hole = 1; //lower hole 
    }
    
        
    return holes; 
}
