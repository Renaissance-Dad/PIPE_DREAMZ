#include <genesis.h>
#include <resources.h>

#ifndef GAMEDATA
#define GAMEDATA
////
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

#define IS_BLANK (my_grid[selector_y][selector_x] == SEG_BLANK)
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

//ENUMS
enum direction{N,E,S,W};
enum states {
    GAME_INIT,
    GAME_LOOP,
    GAME_OVER,
    GAME_PAUSE,
    LEVEL_CLEARED,
    BONUS_MODE
};

//DECLARATION
extern struct level level_one;
extern struct level level_two;
extern struct level level_three;

extern const bool pipe_data[14][4];
extern struct speed game_speeds[5];

extern int my_grid[GRIDROWS][GRIDCOLUMNS];
extern u8 flooz_x; //in tiles
extern u8 flooz_y; //in tiles

extern enum direction flooz_direction;
extern enum direction inverse_direction_table[4];
extern u8 selector_x;
extern u8 selector_y;
extern u8 my_segment_goal;
extern u8 pipe_queue[5];
extern u8 head;
extern u8 tail;
extern u8 sfx_chute;
extern u8 sfx_explosion_frame;
extern u16 my_countdown;
extern u16 timer;
extern u8 flooz_length; //in tiles
extern u8 flooz_grid_x;
extern u8 flooz_grid_y;
extern enum states my_state;
extern int my_score;
extern struct level *my_levels_ptr[3]; 
extern struct speed *game_pace_ptr;
extern u8 my_pace;
extern u8 my_level;

#endif