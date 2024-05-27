#ifndef GAMEDATA
#define GAMEDATA

//DEFINES
#define GRIDROWS 7
#define GRIDCOLUMNS 11
#define GRIDOFFSETX 6
#define GRIDOFFSETY 4
#define FLOOZOFFSETX 7
#define FLOOZOFFSETY 5
#define SELECTOROFFSETX 47
#define SELECTOROFFSETY 31
#define FLOOZTILEINDEXSTART 10
#define FLOOZTILEINDEXVERTEMPTY 18
#define FLOOZTILEINDEXVERTSTART 26
#define FLOOZTILEINDEXHORZSTART 17
#define PIPEDATAOFFSET 10
#define FLOOZTILEINDEXCWSTART 35
#define FLOOZTILEINDEXCCWSTART 27

#define SEG_BLANK 0
#define SEG_START_S 1
#define SEG_START_W 2 
#define SEG_START_N 3 
#define SEG_START_E 4
#define SEG_END_S 5 
#define SEG_END_E 6
#define SEG_END_N 7
#define SEG_END_W 8 
#define SEG_DEAD 9 
#define SEG_RESV_H 10 
#define SEG_RESV_V 11
#define SEG_VERT 12
#define SEG_HORZ 13
#define SEG_BEND_SW 14
#define SEG_BEND_NW 15 
#define SEG_BEND_NE 16 
#define SEG_BEND_SE 17 
#define SEG_CROSS 18
#define SEG_CRISSCROSS SEG_CROSS+1

#define BORDERTILES 9
#define FLOOZTILES 33
#define SPECIALTILES 90
#define SUPERTILES 18
#define REGULARTILES 63
#define TILEINDEXOFFSET_START 10+FLOOZTILES
#define TILEINDEXOFFSET_SUPERSPECIAL 10+FLOOZTILES+SPECIALTILES
#define TILEINDEXOFFSET_REGULAR 10+FLOOZTILES+SPECIALTILES+SUPERTILES

#define IS_BLANK (my_grid[selector_y][selector_x] == 0)
#define IS_DETONATABLE (my_grid[selector_y][selector_x] >= SEG_VERT)
#define IS_BEND ((SEG_BEND_SW <= abs(my_grid[flooz_grid_y][flooz_grid_x])) && (abs(my_grid[flooz_grid_y][flooz_grid_x]) <= SEG_BEND_SE))
#define IS_ENDPIPE ((SEG_END_S <= my_grid[flooz_grid_y][flooz_grid_x] && my_grid[flooz_grid_y][flooz_grid_x] <= SEG_END_W))
#define FLOOZ_1OF3 ((flooz_length-2) %3 == 0)
#define FLOOZ_2OF3 ((flooz_length-2) %3 == 1)
#define FLOOZ_3OF3 ((flooz_length-2) %3 == 2)
#define IS_CROSS ((my_grid[flooz_grid_y][flooz_grid_x] == SEG_CROSS))
#define IS_CRISSCROSS ((my_grid[flooz_grid_y][flooz_grid_x] == ((SEG_CROSS + 1) * -1)))
#define IS_RESERVOIR ((abs(my_grid[flooz_grid_y][flooz_grid_x]) == SEG_RESV_H) || (abs(my_grid[flooz_grid_y][flooz_grid_x]) == SEG_RESV_V))
#define IS_START ((SEG_START_S <= my_grid[flooz_grid_y][flooz_grid_x] && my_grid[flooz_grid_y][flooz_grid_x] <= SEG_START_W))

//STRUCTS
struct level{
   u8 level_number;
   u8 target_pipenumber;
   int grid_data[GRIDROWS][GRIDCOLUMNS];
   // we can add additional data fields later
   u16 flooz_countdown; //in frames (60 frames = 1 sec)
   u8 level_pace;
};

struct speed{
   u8 pixels_per_second;
   u8 draw_frame;
   u8 logic_frame;
   u8 full_tile_frame;
};

//DECLARATION
extern struct level level_one;
extern struct level level_two;
extern struct level level_three;

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















#endif