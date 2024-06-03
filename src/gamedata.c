

#ifndef GLOBALS
#define GLOBALS
//
#include <genesis.h>
#include <resources.h>
#include "gamedata.h"


//DATA
struct level level_one = {1, 16,
    {
        {0,0,0,0,0,0,0,0,0,0,0}, //row 0
        {0,0,0,0,0,1,0,0,0,0,0}, //row 1
        {0,0,0,0,0,0,0,0,0,0,0}, //row 2
        {0,0,0,0,0,0,0,0,0,0,0}, //row 3
        {0,0,0,0,0,0,0,0,0,0,0}, //row 4
        {0,0,0,0,0,0,0,0,0,0,0}, //row 5
        {0,0,0,0,0,0,0,0,0,0,0}  //row 6
    },
    1200,
    2 
};

struct level level_two = {2, 20, 
    {
        {0,0,0,0,0,0,0,0,0,0,0}, //row 0
        {0,0,0,0,9,0,0,0,0,0,0}, //row 1
        {0,0,0,0,0,0,0,0,0,0,0}, //row 2
        {0,0,4,0,0,0,0,0,8,0,0}, //row 3
        {0,0,0,0,0,0,0,0,0,0,0}, //row 4
        {0,0,0,0,0,9,0,0,0,0,0}, //row 5
        {0,0,0,0,0,0,0,0,0,0,0}  //row 6
    },
    300,
    2 
};

struct level level_three = {3, 20,
    {
        {0,0,0,0,0,17,10,14,0,0,0}, //row 0
        {0,0,0,0,9,11,0,11,0,0,0}, //row 1
        {0,0,0,0,0,12,0,12,0,0,0}, //row 2
        {0,0,0,0,0,3,0,11,0,0,0}, //row 3
        {0,0,0,0,0,0,10,15,0,0,0}, //row 4
        {0,0,0,0,0,9,0,0,0,0,0}, //row 5
        {0,0,0,0,0,0,0,0,0,0,0}  //row 6
    },
    400,
    3 
};

const bool pipe_data[14][4] = {  //NORTH, EAST, SOUTH, WEST 
    //straigth down : row 0  --> segment 12 (-12)
    {TRUE,FALSE,TRUE,FALSE},
    //straight flat : row 1
    {FALSE,TRUE,FALSE,TRUE},
    //SW-bend : row 2
    {FALSE,FALSE,TRUE,TRUE},
    //NW-bend : row 3
    {TRUE,FALSE,FALSE,TRUE},
    //NE-bend : row 4
    {TRUE,TRUE,FALSE,FALSE},
    //SE-bend : row 5
    {FALSE,TRUE,TRUE,FALSE},
    //cross: row 6
    {TRUE, TRUE, TRUE, TRUE},
    //cross semi filled: row 7
    {TRUE, TRUE, TRUE, TRUE},
    //end-segment south: row 8 --> segment 5 (+3)
    {FALSE,FALSE,TRUE,FALSE},
    //end-segment west: row 9
    {FALSE,FALSE,FALSE,TRUE},
    //end-segment north: row 10
    {TRUE,FALSE,FALSE,FALSE},
    //end-segment east: row 11
    {FALSE,TRUE,FALSE,FALSE},
    //horizontal reservoir: row 12 --> segment 10 (+2)
    {FALSE,TRUE,FALSE,TRUE},
    //vertical reservoir: row 13
    {TRUE,FALSE,TRUE,FALSE}
};

struct speed game_speeds[5] = {
		//SLOWEST 0
		{2,30,239,240},
		//SLOWER 1
		{6,10,79,80},
		//STANDARD 2
		{12,5,39,40},
		//FASTER 3
		{15,4,31,32},
		//FASTEST
		{20,3,23,24}
};

u8 flooz_x = NULL; //in tiles
u8 flooz_y = NULL; //in tiles
enum direction flooz_direction = NULL;
enum direction inverse_direction_table[4] = {S, W, N, E};
u8 head = 0;
u8 tail = 0;
u8 sfx_chute = 0;
u8 sfx_explosion_frame = 0;
u16 timer = 0;
enum states my_state = GAME_INIT;
int my_score = 0;
struct level *my_levels_ptr[3] = {&level_one, &level_two, &level_three};
struct speed *game_pace_ptr = game_speeds;






#endif