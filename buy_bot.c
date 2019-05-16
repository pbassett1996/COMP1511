//Name: Peter Bassett
//Student Id: z5060611
//Date: 01.06.2017
//Description: This code aims to identify the buy_sell combination that will generate the greatest profit

#include <stdio.h>
#include "trader_bot.h"
#include <string.h>
#include "my_functions.h"
#include <stdlib.h>

void buy_bot(struct bot *b, int *buy_moves, int *quantity, char *comm[20], int profit_ratio) {
    
    //Extract data from the bot b
    int curr_cash = b->cash; //Current cash of bot
    int mcv = b->maximum_cargo_volume; // The maximum volume the bot can contain
    int mcw = b->maximum_cargo_weight; // The maximum weight the bot can contain
    int max_move = b->maximum_move; //Max moves the bot can perform
    int curr_fuel = b->fuel; //Bots current fuel
    
    //Initialise variables
    int temp_quantity = 0;
    int temp_quantity_1 = 0;
    int profit = 0;
    int no_moves_forward = 0;
    int no_moves_backward = 0;
    int quant_forward = 0;
    int quant_backward = 0;
    int i = 0;
    int j = 0;
    char *temp_comm, *temp_comm_2;
    
    //Find the best buy/sell combination that will generate the highest profit. 
    //Output the number of moves and the quantity to be sold by going forwards through the nodes
    
    struct location *n = b->location;
    char *start = n->name; //This is the name of the starting location
    
    while (i == 0 || strcmp(start,n->name) != 0 ) {
        if (n->type == LOCATION_SELLER) {
            
            //Extract data from the location
            char *curr_com_b = n->commodity->name; //Current commodity
            int price_buy = n->price; //Price of commodity
            int stock_b = n->quantity; //Amount of stock available
            int w_b = n->commodity->weight; //Weight of commodity
            int v_b = n->commodity->volume; //Volume of commodity
            
            //Find out how much of the commodity can be bought. 
            //It can not exceed the max weight of the volume
            int q_by_weight = mcw/w_b; // Quantity that can be bought limited by weight
            int q_by_volume = mcv/v_b; // Quantity that can be bought limited by volume
            int q_by_cash = curr_cash/price_buy; //Quantity that can be bought limited by cash
            
            //Find the limiting factor for the quantity to be bought
            
            temp_quantity_1 = 0;
            if (q_by_weight > q_by_volume) {
                temp_quantity_1 = q_by_volume;
            } else {
                temp_quantity_1 = q_by_weight;
            }
            
            if (temp_quantity_1 > stock_b) {
                temp_quantity_1 = stock_b;
            }
            
            if (temp_quantity_1 > q_by_cash) {
                temp_quantity_1 = q_by_cash;
            }
            
            //Find a seller of the given commodity and then determine the profit generated
            struct location *p = n;
            char *new_start = n->name;
            
            j = 0; //reset j
            
            while (j == 0 || strcmp(new_start,p->name) != 0) {
                
                if(p->type == LOCATION_BUYER && strcmp(curr_com_b, p->commodity->name) == 0) {
                    
                    //Extract data from location
                    int price_sell = p->price; //Price of seller
                    int stock_s = p->quantity - 1; //Stock available of seller
                    
                    //Find the limitng factor for the quantity to be bought
                    
                    temp_quantity = temp_quantity_1;
                    
                    if (temp_quantity > stock_s) {
                        temp_quantity = stock_s;
                    }
                    
                    //Find out the no_moves that geneartes the best profit
                    
                    int revenue = temp_quantity*price_sell;
                    int cost = temp_quantity*price_buy;
                    
                    if (revenue - cost > profit_ratio*profit) {
                        profit = revenue-cost;
                        quant_forward = temp_quantity;
                        no_moves_forward = i;
                        temp_comm = n->commodity->name;
                    }
                    
                }
                j++;
                p = p->next;
            }
        }
        
        i++;
        n = n->next;
    }
    
    //Find the best buy/sell combination that will generate the highest profit. 
    //Output the number of moves and the quantity to be sold by going backwards through the nodes
    
    struct location *t = b->location;
    
    //Reset variables
    i = 0;
    j = 0;
    profit = 0;
    
    while (i == 0 || strcmp(start,t->name) != 0 ) {
    
        
        if (t->type == LOCATION_SELLER) {
            
            //Extract data from the location
            char *curr_com_b = t->commodity->name; //Current commodity
            int price_buy = t->price; //Current price
            int stock_b = t->quantity; //Amount of stock availble
            int w_b = t->commodity->weight; //Weight of commodity
            int v_b = t->commodity->volume; //Volume of commodity
            
            //Find out how much of the commodity can be bought. 
            //It can not exceed the max weight of the volume
            
            int q_by_weight = mcw/w_b; // Quantity that can be bought limited by weight
            int q_by_volume = mcv/v_b; // Quantity that can be bought limited by volume
            int q_by_cash = curr_cash/price_buy; // Quantity that can be bough limited by cash
            
            //Find the limiting factor for the quantity to be bought
            temp_quantity_1 = 0;
            
            if (q_by_weight > q_by_volume) {
                temp_quantity_1 = q_by_volume;
            } else {
                temp_quantity_1 = q_by_weight;
            }
            
            if (temp_quantity_1 > stock_b) {
                temp_quantity_1 = stock_b;
            }
            
            if (temp_quantity_1 > q_by_cash) {
                temp_quantity_1 = q_by_cash;
            }
            
            //Find a seller of the given commodity and then determine the profit generated
            struct location *s = t;
            char *new_start = t->name;
            j = 0;
   
            while (j == 0 || strcmp(new_start,s->name) != 0) {
                
                if(s->type == LOCATION_BUYER && strcmp(curr_com_b, s->commodity->name) == 0) {
                    
                    //Extract data from location
                    int price_sell = s->price; //Price of seller
                    int stock_s = s->quantity - 1; //Stock availble of seller
                    
                    //Find the limitng factor for the quantity to be bought
                    temp_quantity = temp_quantity_1;
                    
                    if (temp_quantity > stock_s) {
                        temp_quantity = stock_s;
                    }
                    
                    //Find out the no_moves that geneartes the best profit
                    int revenue = temp_quantity*price_sell;
                    int cost = temp_quantity*price_buy;
                    
                    if (revenue - cost > profit_ratio*profit) {
                        profit = revenue-cost;
                        quant_backward = temp_quantity;
                        no_moves_backward = -i;
                        temp_comm_2 = t->commodity->name; 
                    }
                    
                }
                j++;
                s = s->previous;
            }
        }
        
        i++;
        t = t->previous;
    }
    
    
    //Determine the shortest distance (forward or backwards) to maximum profit
    if (no_moves_forward > abs(no_moves_backward) && quant_backward != 0) {
        *buy_moves = no_moves_backward;
        *quantity = quant_backward;
        *comm = temp_comm_2;
    } else if (quant_forward != 0) {
        *buy_moves = no_moves_forward;
        *quantity = quant_forward;
        *comm = temp_comm;
    } 
    
}
