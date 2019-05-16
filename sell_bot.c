//Name: Peter Bassett
//Student Id: z5060611
//Date: 01.06.2017
//Description: This code aims to identify the moves to the best sale

#include <stdio.h>
#include "trader_bot.h"
#include <string.h>
#include "my_functions.h"
#include <stdlib.h>

int sell_bot(struct bot *b, char *comm, int quantity, int sale_ratio, int moves) {


    
    //initalise variables
    int i = 0;
    int j = 0;
    int revenue = 0;
    int sale = 0;
    int no_moves_forward = 0;
    int no_moves_backward = 0;
    
    //This part of the code is used to 'look ahead a move'. 
    //An integer is passed through moves.
    //This determines whether it will be able to reach a fuel station after undertaking a buy/sell move.
    
    struct location *n = b->location;
    struct location *p = b->location;
    
    if (moves > 0) {
        for (i = 0; i < abs(moves); i++) {
            n = n->next;
            p = p->next;
        }
    } else if (moves < 0) {
        for (i = 0; i < abs(moves); i++) {
            n = n->previous;
            p = p->previous;
        }
    }
    
    i = 0;
    
    //Extract data from bot
    int max_move = b->maximum_move; //Max move of bot
    
    char *start = n->name;
    
    //This loop iterates forward to find the closest best sale
    while (i == 0 || strcmp(start, n->name) != 0) {
       
       if(n->type == LOCATION_BUYER 
            && strcmp(comm, n->commodity->name) == 0 
                && n->quantity >= quantity) {
            
            //Extract data
            int price = n->price;
            
            //Generate revenue
            revenue = price*quantity;
            
            if (revenue > sale_ratio*sale) {
                no_moves_forward = i;
                sale = revenue;
            }
       }
       i++;
       n = n->next;
   }
   

   sale = 0;
   
    //This loop itereates backwards to find the closest best sale
    while (j == 0 || strcmp(start, p->name) != 0) {

       if(p->type == LOCATION_BUYER 
            && strcmp(comm, p->commodity->name) == 0 
                && p->quantity >= quantity) {

            //Extract data
            int price = p->price;

            //Generate revenue
            revenue = price*quantity;

            if (revenue > sale_ratio*sale) {
                no_moves_backward = -j;
                sale = revenue;
            }
        }
        j++;
        p = p->previous;
    }
   
    //Determine the closest no. of moves to best seller
    if (no_moves_forward > abs(no_moves_backward)) {
        return no_moves_backward;
    } else {
        return no_moves_forward;
    }
}
   
       
