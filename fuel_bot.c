//Name: Peter Bassett
//Student Id: z5060611
//Date: 01.06.2017
//Description: This code aims to determine the shortest distance between the bot and a fuel station. Some of the code was taken from the week 10 lab solutions and therefore should be credited to Andrew Taylor.

#include <stdio.h>
#include "trader_bot.h"
#include <math.h>
#include <stdlib.h>
#include "my_functions.h"

int fuel_bot(struct bot *b, int moves) {

    //Initialise variables
    int i = 0;
    int j = 0;
    int distance = 0;
    
    //Extract the maximum fuel the bot can hold
    int max_fuel = b->fuel_tank_capacity; //Max amount of fuel
    int current_fuel = b->fuel; //Current fuel of bot
    
    struct location *start = b->location;
    
    //This part of the code is used to 'look ahead a move'. 
    //An integer is passed through moves.
    //This determines whether it will be able to reach a fuel station after undertaking a buy/sell move.
    if (moves > 0) {
        for (i = 0; i < abs(moves); i++) {
            start = start->next;
        }
    } else if (moves < 0) {
        for (i = 0; i < abs(moves); i++) {
            start = start->previous;
        }
    }
    
    //This iterates throughout the circular world
    //It moves a pointer forward and backwards and stops once a PETROL_STATION is reached
    struct location *forwards = start;
    struct location *backwards = start;
    
    while (distance == 0 || forwards != start) {
        if (forwards->type == LOCATION_PETROL_STATION && forwards->quantity > 2) {
            return distance + abs(moves);
        }
        if (backwards->type == LOCATION_PETROL_STATION && backwards->quantity > 2) {
            return -distance - abs(moves);
        }
        forwards = forwards->next;
        backwards = backwards->next;
        distance++;
    }
    
    return 0; //only occurs when all petrol stations are out of fuel
}
