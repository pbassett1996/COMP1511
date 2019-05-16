//Name: Peter Bassett
//Student Id: z5060611
//Date: 01.06.2017
//Description: This is the main of my trader bot. It compiles all the other functions together to output an action and a number n

#include <stdio.h>
#include "trader_bot.h"
#include "my_functions.h"
#include <stdlib.h>
#include <string.h>

char *get_bot_name(void) {
    return "Final Pete Bot 2";
}

void get_action(struct bot *b, int *action, int *n) {
    
    //Initialise variables
    int quant_buy = 0;
    int between_fuel = 0;
    int quant_between_f = 0;
    int between_buy = 0; 
    int quant_between_b = 0;
    int sell_between_f = 0; 
    int sell_between_b = 0; 
    int quant_buy_2 = 0; 
    int quant_buy_3 = 0;
    int dist_buy = 0;
    int dist_fuel = 0;
    int dist_buy_2 = 0;
    int dist_buy_3 = 0;
    
    char *buy_commodity, *comm_fuel, *comm_buy, 
        *buy_commodity_2, *buy_commodity_3;
    
    //Extract data from the bot
    int curr_cash = b->cash;
    double max_move = b->maximum_move;
    int max_fuel = b->fuel_tank_capacity;
    int turns_left = b->turns_left;
    int current_fuel = b->fuel;
    

    //Determine the number of moves to the optimal buyer
    buy_bot(b, &dist_buy, &quant_buy, &buy_commodity, 2);
    
    //Determine the number of moves to a profitable seller that is close
    buy_bot(b, &dist_buy_2, &quant_buy_2, &buy_commodity_2, 4);
    
    //Determine the number of moves to a profitable seller that is very close
    buy_bot(b, &dist_buy_3, &quant_buy_3, &buy_commodity_3, 6);    
    
    //Determine the distance to the nearest fuel station
    dist_fuel = fuel_bot(b, 0);
    
    //Determine the distance to the nearest fuel station after buying a 
//                                                              commodity
    int dist_fuel_future = fuel_bot(b, dist_buy);   
    int dist_fuel_future_2 = fuel_bot(b, dist_buy_2);   
    int dist_fuel_future_3 = fuel_bot(b, dist_buy_3);

    //Determine if there are any buy-sell opportunities whilst moving 
    //                                          towards a petrol station
    buy_between(b, dist_fuel, &between_fuel, &quant_between_f, 
                                        &sell_between_f, &comm_fuel);
    //Determine if there are any buy-sell opportunities whilst moving
    //                                            towards a optimal seller
    buy_between(b, dist_buy, &between_buy, &quant_between_b, 
                                        &sell_between_b, &comm_buy);  
    
    
    struct location *p = b->location;
    struct cargo *q = b->cargo;
    
    //The following if statements uses a set of criteria to determine the bots ouput.
    //If the bot is not carrying any cargo it will attempt to buy
    //If the bot is carrying cargo it will attempt to sell
    
    
    if (q == NULL && turns_left > 2) { //If the bot is not carrying any cargo 
        
        if (dist_buy == 0 && p->type == LOCATION_SELLER) {
            *action = ACTION_BUY;
            *n = quant_buy;
        } else if (dist_buy == 0 && p->type != LOCATION_SELLER) {
            *action = ACTION_MOVE;
            *n = 0;
        } else if (between_buy == 0 && p->type == LOCATION_SELLER 
                   && strcmp(comm_buy, p->commodity->name) == 0) {
            *action = ACTION_BUY;
            *n = quant_between_b;
        } else if (dist_buy_2 == 0 && p->type == LOCATION_SELLER) {
            *action = ACTION_BUY;
            *n = quant_buy_2;
        } else if (dist_buy_3 == 0 && p->type == LOCATION_SELLER) {
            *action = ACTION_BUY;
            *n = quant_buy_3;
        } else if (p->type == LOCATION_PETROL_STATION 
                   && current_fuel < max_fuel && p->quantity > 2) {
            *action = ACTION_BUY;
            *n = max_fuel-current_fuel;
        } else if (between_fuel == 0 && p->type == LOCATION_SELLER 
                   && strcmp(comm_fuel, p->commodity->name) == 0) {
            *action = ACTION_BUY;
            *n = quant_between_f;
        } else if (abs(dist_fuel_future) < current_fuel 
                   && abs(dist_fuel_future)/max_move < turns_left) {
            if (between_buy != 0) {
                *action = ACTION_MOVE;
                *n = between_buy;
            } else {
                *action = ACTION_MOVE;
                *n = dist_buy;
            }
        } else if (abs(dist_fuel_future_2) < current_fuel 
                    && abs(dist_fuel_future_2)/max_move < turns_left) {
            *action = ACTION_MOVE;
            *n = dist_buy_2;
        } else if (abs(dist_fuel_future_3) < current_fuel 
                    && abs(dist_fuel_future_3)/max_move < turns_left) {
            *action = ACTION_MOVE;
            *n = dist_buy_3;
        } else if (abs(dist_fuel_future) > current_fuel 
                   && abs(dist_fuel) < current_fuel 
                            && abs(dist_fuel)/max_move < turns_left 
                                        && abs(dist_fuel) != 0) {
            if (between_fuel != 0) {
                *action = ACTION_MOVE;
                *n = between_fuel;
            } else {
                *action = ACTION_MOVE;
                *n = dist_fuel;
            }
        } else {
            *action = ACTION_MOVE;
            *n = 1;
        }
    } else if (q == NULL) { //If there isn't enough turns left to make a move
        *action = ACTION_MOVE;
        *n = 0;
    }
        
    
    if (q > 0) { //If the bot is currently carrying cargo
        
        //Extract data
        struct commodity *d = q->commodity;
        char *comm = d->name;
        int q_cargo = q->quantity;
        
        //Determine the number of moves to optimal seller
        //The higher the sale ratio, the shorter the distance
        // to the seller will be
        int no_moves_sell = sell_bot(b, comm, q_cargo ,1, 0 );
        int moves_sell_2 = sell_bot(b, comm, q_cargo, 2, 0);
        int moves_sell_3 = sell_bot(b, comm, q_cargo, 6, 0);
        
        //Determine the number of moves to nearest seller
        int no_moves_sell_now = sell_now(b, comm, q_cargo);

        //Determine the number of moves to petrol station after sell move
        int dist_fuel_sell = fuel_bot(b, no_moves_sell);
        int dist_fuel_sell_2 = fuel_bot(b, moves_sell_2);
        int dist_fuel_sell_3 = fuel_bot(b, moves_sell_3);
        int dist_fuel_sell_now = fuel_bot(b, no_moves_sell_now);
        
            if (dist_fuel > current_fuel && dist_fuel_sell > current_fuel) {
                *action = ACTION_MOVE;
                *n = dist_fuel_sell_now;
            }
            
            if (no_moves_sell == 0 && p->type == LOCATION_BUYER 
                        && p->quantity >= q_cargo) {
                *action = ACTION_SELL;
                *n = q_cargo;
            } else if (moves_sell_2 == 0 && p->type == LOCATION_BUYER 
                        && p->quantity >= q_cargo) {
                *action = ACTION_SELL;
                *n = q_cargo;
            } else if (moves_sell_3 == 0 && p->type == LOCATION_BUYER 
                        && p->quantity >= q_cargo) {
                *action = ACTION_SELL;
                *n = q_cargo;
            } else if (no_moves_sell_now == 0 && p->type == LOCATION_BUYER 
                        && p->quantity >= q_cargo) {
                *action = ACTION_SELL;
                *n = q_cargo;
            } else if (p->type == LOCATION_PETROL_STATION 
                       && current_fuel < max_fuel && p->quantity > 0 
                            && curr_cash > (p->price * max_fuel-current_fuel)/2 ) {
                *action = ACTION_BUY;
                *n = max_fuel - current_fuel;
            } else if (abs(dist_fuel_sell) < current_fuel 
                       && abs(dist_fuel_sell)/max_move < turns_left 
                        && no_moves_sell != 0) {
                *action = ACTION_MOVE;
                *n = no_moves_sell;
            } else if (abs(dist_fuel_sell_2) < current_fuel 
                        && abs(dist_fuel_sell_2)/max_move < turns_left 
                            && moves_sell_2 != 0) {
                *action = ACTION_MOVE;
                *n = moves_sell_2;
            } else if (abs(dist_fuel_sell_3) < current_fuel 
                        && abs(dist_fuel_sell_3)/max_move < turns_left 
                            && moves_sell_3 != 0) {
                *action = ACTION_MOVE;
                *n = moves_sell_3;
            } else if (abs(dist_fuel_sell_now) < current_fuel 
                       && no_moves_sell != 0) {
                *action = ACTION_MOVE;
                *n = no_moves_sell_now;
            } else if (abs(dist_fuel_sell) > current_fuel 
                       && abs(dist_fuel) < current_fuel 
                            && abs(dist_fuel)/max_move < turns_left) {
                *action = ACTION_MOVE;
                *n = dist_fuel;
            } else if (abs(dist_fuel_sell_now) > current_fuel 
                       && abs(dist_fuel) < current_fuel 
                            && abs(dist_fuel)/max_move < turns_left) {
                *action = ACTION_MOVE;
                *n = dist_fuel;
            } else if (abs(no_moves_sell_now)/max_move < turns_left) {
                *action = ACTION_MOVE;
                *n = no_moves_sell_now;
            } else {
                *action = ACTION_MOVE;
                *n = 1;
            } 
       
    }
}
