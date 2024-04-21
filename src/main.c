#include <genesis.h>
#include <resources.h>

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

#define BORDERTILES 9
#define FLOOZTILES 33
#define SPECIALTILES 90
#define SUPERTILES 18
#define REGULARTILES 63
#define TILEINDEXOFFSET_START 10+FLOOZTILES
#define TILEINDEXOFFSET_SUPERSPECIAL 10+FLOOZTILES+SPECIALTILES
#define TILEINDEXOFFSET_REGULAR 10+FLOOZTILES+SPECIALTILES+SUPERTILES

//FFWD DECLARATION OF OUR FUNCTIONS
void drawBorder(u8 x_column, u8 y_row, u8 width, u8 height);
void initGame();
void loadLevel(u8 lvl);
void drawSegment(u8 x_grid, u8 y_grid, u8 segment);
void myJoyEventCallbackGame(u16 joy, u16 changed, u16 state);
void drawSelector();
void loadDMA();
int calcRandomValue(u8 max);
void enQueue(u8 value); //writes a value and pushes the head
void advanceTailQueue(); //pushes the tail
void initQueue(); //calls enQueue() 5 times
void setQueueSprites();
void redrawSingleGridSegment(u8 x_grid, u8 y_grid);
void redrawQueueSprite();
void drawExplosion();
void cleanupExplosion();
void sfxQueueSpritesUpdate();
void drawCountdown();
void drawFlooz();
enum direction inverseDirection(enum direction windsock);
void checkNextSegment();
void drawScoreboard();
void doTimer();

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

const bool pipe_data[13][4] = {  //NORTH, EAST, SOUTH, WEST 
    //straigth down : row 0
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
    //end-segment south: row 7
	{FALSE,FALSE,TRUE,FALSE},
	//end-segment west: row 8
	{FALSE,FALSE,FALSE,TRUE},
	//end-segment north: row 9
	{TRUE,FALSE,FALSE,FALSE},
	//end-segment east: row 10
	{FALSE,TRUE,FALSE,FALSE},
    //horizontal reservoir: row 11
    {FALSE,TRUE,FALSE,TRUE},
    //vertical reservoir: row 12
    {TRUE,FALSE,TRUE,FALSE}
};

struct speed game_speeds[5] = {
		//SLOWEST
		{2,30,239,240},
		//SLOWER
		{6,10,79,80},
		//STANDARD
		{12,5,39,40},
		//FASTER
		{15,4,31,32},
		//FASTEST
		{20,3,23,24}
};

//GLOBAL SPRITE POINTERS
Sprite* selector_spr;
Sprite* queue_spr[5];
Sprite* explosion_spr;

//GLOBAL VARIABLES
int my_grid[GRIDROWS][GRIDCOLUMNS];
u8 flooz_x = NULL; //in tiles
u8 flooz_y = NULL; //in tiles
enum direction{N,E,S,W};
enum direction flooz_direction = NULL;
u8 selector_x;
u8 selector_y;
u8 my_segment_goal;
u8 pipe_queue[5];
u8 head = 0;
u8 tail = 0;
u8 sfx_chute = 0;
u8 sfx_explosion_frame = 0;
u16 my_countdown;
u16 timer = 0;
u8 flooz_length; //in tiles
u8 flooz_grid_x;
u8 flooz_grid_y;
enum states {
    GAME_INIT,
    GAME_LOOP,
    GAME_OVER,
    GAME_PAUSE,
    LEVEL_CLEARED,
    BONUS_MODE
};
enum states my_state = GAME_INIT;
int my_score = 0;
struct level *my_levels_ptr[3] = {&level_one, &level_two, &level_three};
struct speed *game_pace_ptr = game_speeds;
u8 my_pace;
u8 my_level;

//actual callback function for the joypad
void myJoyEventCallbackGame(u16 joy, u16 changed, u16 state){
    if (joy == JOY_1){
        if(state & BUTTON_RIGHT){
            if (selector_x+1 < GRIDCOLUMNS){
                selector_x++;
            }
        }else if(state & BUTTON_LEFT){
            if (selector_x-1 >= 0){
                selector_x--;
            }
        }else if(state & BUTTON_UP){
            if (selector_y-1 >= 0){
                selector_y--;
            }
        }else if(state & BUTTON_DOWN){
            if (selector_y+1 < GRIDROWS){
                selector_y++;
            }
        }
        //A-button logic
        if (changed & BUTTON_A & state){
            if (my_grid[selector_y][selector_x] == 0 || my_grid[selector_y][selector_x] >= SEG_VERT){ 
                if(my_grid[selector_y][selector_x] >= SEG_VERT && sfx_chute == 0){drawExplosion();} 
                my_grid[selector_y][selector_x] = pipe_queue[tail]+12; //first 10+2 segments are special and/or super segments
                redrawSingleGridSegment(selector_x, selector_y);
                redrawQueueSprite();
                advanceTailQueue();
                sfx_chute = 24;
            }  
        }
        //START-button logic
		if (changed & BUTTON_START & state){
            if (my_state != GAME_PAUSE){my_state = GAME_PAUSE; VDP_drawText("PAUSE",1,26);}
            else if (my_state == GAME_PAUSE){my_state = GAME_LOOP; VDP_drawText("LOOP!",1,26);}
        }
        //B-button logic
	    if (changed & BUTTON_B & state){
            if (my_pace <4){my_pace++;}
            game_pace_ptr = &game_speeds[my_pace];
            char pace_str[4];
            sprintf(pace_str,"%d", game_pace_ptr->pixels_per_second);
            VDP_drawText("FASTER!",1,27);
            VDP_drawText(pace_str,9,27);
            VDP_drawText(" pixels per second", 11,27);		
        }
    }
}

//MAIN FUNCTION
int main(bool hard_reset)
{

    loadDMA();
    initGame();

    while(my_state != GAME_OVER)
    {
        drawSelector();
        sfxQueueSpritesUpdate();
        drawCountdown();
        drawFlooz();
        drawScoreboard();
        doTimer();
        SYS_doVBlankProcess();
    }
    return (0);
}

//drawBorder() function: draws borders with tiles on BG_A. Needs origin coords and width and height (all in tiles)
void drawBorder(u8 x_column, u8 y_row, u8 width, u8 height){
    // draw the xcolumn
    for (u8 i = 0; i < width; i++){
        VDP_setTileMapXY(BG_A,TILE_ATTR_FULL(PAL1,0,FALSE,FALSE,5), x_column + i, y_row);
        VDP_setTileMapXY(BG_A,TILE_ATTR_FULL(PAL1,0,FALSE,FALSE,6), x_column + i, y_row+height);		
    }
    // draw the yrow
    for (u8 i=0; i < height; i++){
        VDP_setTileMapXY(BG_A,TILE_ATTR_FULL(PAL1,0,FALSE,FALSE,8), x_column, y_row+i);
        VDP_setTileMapXY(BG_A,TILE_ATTR_FULL(PAL1,0,FALSE,FALSE,7), x_column+width, y_row+i);
    }
    // draw the corners
        VDP_setTileMapXY(BG_A,TILE_ATTR_FULL(PAL1,0,FALSE,FALSE,1), x_column, y_row);
        VDP_setTileMapXY(BG_A,TILE_ATTR_FULL(PAL1,0,FALSE,FALSE,3), x_column, y_row+height);
        VDP_setTileMapXY(BG_A,TILE_ATTR_FULL(PAL1,0,FALSE,FALSE,2), x_column+width, y_row);
        VDP_setTileMapXY(BG_A,TILE_ATTR_FULL(PAL1,0,FALSE,FALSE,4), x_column+width, y_row+height);   
}

//initGame() function which sets up the main processes (sprite engine and joypad callback). Draws the borders.
//sets up selector sprite.
void initGame(){
    drawBorder(0,0,4,16); //chute
    drawBorder(5,3,34,22); //grid
    drawBorder(5,0,34,2); //scoreboard 
    
    //initializing and setting up the joypad
    JOY_init();
    JOY_setEventHandler(&myJoyEventCallbackGame);
    
    //initializing the sprite engine
    SPR_init();
    selector_spr = SPR_addSprite(&selectorsprite, 100, 100, TILE_ATTR(PAL1, 0, FALSE, FALSE));
    explosion_spr = SPR_addSprite(&explosionsprite, -100, -100, TILE_ATTR(PAL3, 0, FALSE, FALSE));
    
    //loading the level data
    loadLevel(3);
}

//loadLevel() function copies the level data from ROM into RAM
void loadLevel(u8 lvl){
    // copy the leveldata first
    memcpy(my_grid, my_levels_ptr[lvl - 1]->grid_data, 308); //int = 4bytes, times 77
    my_segment_goal = my_levels_ptr[lvl - 1]->target_pipenumber;
    my_countdown = my_levels_ptr[lvl - 1]->flooz_countdown;
    my_pace = my_levels_ptr[lvl - 1]->level_pace;
    game_pace_ptr = &game_speeds[my_pace];
    // draw the leveldata on the grid
    for(u8 i=0; i< GRIDROWS; i++){
        for(u8 j=0; j< GRIDCOLUMNS; j++){
            drawSegment(j, i, my_grid[i][j]);
            if (my_grid[i][j] >= SEG_START_S && my_grid[i][j] <= SEG_START_E){
                flooz_grid_x = j;
                flooz_grid_y = i; 
                flooz_x = (j*3)+ FLOOZOFFSETX; 
                flooz_y = (i*3)+ FLOOZOFFSETY; 
                switch (my_grid[i][j]){
                    case SEG_START_S: flooz_direction = S; break;
                    case SEG_START_W: flooz_direction = W; break;
                    case SEG_START_N: flooz_direction = N; break;
                    case SEG_START_E: flooz_direction = E; break;

                }
            }
        }     
    }
    //draw the scoreboard content
    char level_str[2];
    sprintf(level_str,"%d",lvl);
    VDP_drawText("LEVEL:",7,1);
    VDP_drawText(level_str,13,1);
    char pipes_str[3];
    sprintf(pipes_str,"%d",my_segment_goal);
    VDP_drawText("TARGET:",29,1);
    VDP_drawText(pipes_str,36,1);
    //initialize the queue
    initQueue();
    setQueueSprites();
    //reset the flooz length value
    flooz_length = 0;
    //reset level score
    my_score = 0;
    //setlevel
    my_level = lvl;	
}

// the drawSegment() function draws 3x3 tiles which represents a pipe segment. 
void drawSegment(u8 x_grid, u8 y_grid, u8 segment){
    VDP_fillTileMapRectInc(BG_A,TILE_ATTR_FULL(PAL1,0,FALSE,FALSE,(segment*9)+TILEINDEXOFFSET_START),(x_grid*3)+GRIDOFFSETX, (y_grid*3)+GRIDOFFSETY, 3, 3);
}

//drawSelector() function
void drawSelector(){
    SPR_setPosition(selector_spr,(selector_x*24)+SELECTOROFFSETX,(selector_y*24)+SELECTOROFFSETY);
    SPR_update();
}

//loadDMA() function, groups all our DMA routines together
void loadDMA(){
    //loading the border tileset
    VDP_loadTileSet(bordertile.tileset,1,DMA);
    // loading the flooz segments
    VDP_loadTileSet(flooztiles.tileset,FLOOZTILEINDEXSTART,DMA); //10
    //loading the special segment tileset
    VDP_loadTileSet(pipesspecialtile.tileset,TILEINDEXOFFSET_START,DMA); //43
    //loading the super special segment
    VDP_loadTileSet(pipessupertile.tileset,TILEINDEXOFFSET_SUPERSPECIAL,DMA); 
    //loading the regular segment tileset
    VDP_loadTileSet(pipesregulartile.tileset,TILEINDEXOFFSET_REGULAR,DMA); //133
    //loading the super special segment
    PAL_setPalette(PAL1, bordertile.palette->data, DMA);
    // load the colors of the explosion
    PAL_setPalette(PAL3, explosioncolors.palette->data, DMA);
}

//calcRandomValue() function, gives random value between 0 and max
int calcRandomValue(u8 max){
    u8 value = (random() % (max + 1));
    return value;
}

//enQueue() function adds value to the queue at the head position and wraps around at 5
void enQueue(u8 value){
    pipe_queue[head] = value;
    head++;
    head %= 5;
}

//advanceTailQueue() function, advances the tail and wraps around at 5
void advanceTailQueue(){
    tail++;
    tail %= 5;
}

//initializes the Queue and fills it with 5 values between 0 and 6, representing 7 different (regular) pipe segments
void initQueue(){
    for (u8 i = 0; i<5; i++){
        enQueue(calcRandomValue(6));
    }
}

//setQueueSprites() function
void setQueueSprites(){
    for (u8 i = 0; i<5; i++){   
        queue_spr[i] = SPR_addSprite(&queuesprites, 8, 104-(i*24),TILE_ATTR_FULL(PAL1, 0, FALSE, FALSE, 0));
        SPR_setAnim(queue_spr[i], pipe_queue[i]);
    }
}


//redrawSingleGridSegment() function; this is a wrapper function
void redrawSingleGridSegment(u8 x_grid, u8 y_grid){
    drawSegment(x_grid, y_grid, my_grid[y_grid][x_grid]);
}

//redrawQueueSprite() function
void redrawQueueSprite(){
    SPR_setPosition(queue_spr[tail], 8, SPR_getPositionY(queue_spr[tail])-120);
    enQueue(calcRandomValue(6));
    SPR_setAnim(queue_spr[tail], pipe_queue[tail]);
    SPR_update();
}

// the drawExplosion() function which draws the explosion sprite on the screen
void drawExplosion(){
    SPR_setPosition(explosion_spr,(selector_x*24)+48,(selector_y*24)+32);
    SPR_setAnim(explosion_spr, 0);
    void (*sfx_ptr)(); //function pointer
    sfx_ptr = cleanupExplosion;
    SPR_setFrameChangeCallback(explosion_spr, sfx_ptr);
    SPR_update(); 
}

//the cleanupExplosion() callback function
void cleanupExplosion(){
    /* SGDK 2.0 has a new method but it's not working atm
    if (!SPR_getAnimationDone(explosion_spr)) {
        SPR_setPosition(explosion_spr, -100,-100);
        SPR_update();
    }
    */
    sfx_explosion_frame++;
    if (sfx_explosion_frame == 11) {
        SPR_setPosition(explosion_spr, -100,-100);
        sfx_explosion_frame = 0;
        SPR_update();
    }
}

//sfxQueueSpritesUpdate() function which animates the movement of the Queue
void sfxQueueSpritesUpdate(){
    if (sfx_chute != 0){
        for (u8 i = 0; i<5; i++){
            SPR_setPosition(queue_spr[i], 8, SPR_getPositionY(queue_spr[i])+1);
        }
        sfx_chute--;
    }   
    SPR_update();
}

//function that draws the depleting bar at the beginning of the game. Both numeric and visually.
void drawCountdown(){
    int delta_timer = (my_countdown - timer)/5;
    int my_whole_tiles = delta_timer /8;
    if (timer%5 == 0 && delta_timer >= 0 && delta_timer <= 240){
        int my_modulo = delta_timer %8;
        // draw the whole segments
        for (int i = 0; i < my_whole_tiles; i++){
            VDP_setTileMapXY(BG_A,TILE_ATTR_FULL(PAL1,0,FALSE,FALSE,FLOOZTILEINDEXSTART), 35-i, 26);
        }
        // draw the partial segments 
        VDP_setTileMapXY(BG_A,TILE_ATTR_FULL(PAL1,0,FALSE,FALSE,FLOOZTILEINDEXVERTEMPTY-my_modulo), 35 - my_whole_tiles, 26);
    }
    //printing my_countdown 
    if (timer%60 == 0 && delta_timer > 0){
        char t[4];
        sprintf(t, "%d", delta_timer/12);
        VDP_drawText("      ", 37 , 26);
        VDP_drawText(t, 37 , 26);
    } else if (delta_timer == 0) {
        VDP_drawText("0 ", 37, 26);
    }
}

//drawFlooz() function which draws the flooz, using tiles. 
void drawFlooz(){
    int game_timer = timer - my_countdown;
    int flooz_counter = game_timer/game_pace_ptr->draw_frame;
    flooz_counter %= 8;

    //draw the flooz
    if ((game_timer %(game_pace_ptr->draw_frame) == 0) && (game_timer >= 0)) {
        //bend pipes 1/3rd of flooz drawn
        if ((SEG_BEND_SW <= abs(my_grid[flooz_grid_y][flooz_grid_x])) && (abs(my_grid[flooz_grid_y][flooz_grid_x]) <= SEG_BEND_SE) && ((flooz_length-2) %3 == 1)){
			switch (abs(my_grid[flooz_grid_y][flooz_grid_x])){
                case SEG_BEND_SW: //SW pipe
		            if (flooz_direction == E) {
                        VDP_setTileMapXY(BG_B,TILE_ATTR_FULL(PAL1,0,TRUE,TRUE,FLOOZTILEINDEXCWSTART+flooz_counter), flooz_x, flooz_y);
                        if (flooz_counter == 7){ flooz_direction = S;}
                    } else if (flooz_direction == N) {
                        VDP_setTileMapXY(BG_B,TILE_ATTR_FULL(PAL1,0,TRUE,TRUE,FLOOZTILEINDEXCCWSTART+flooz_counter), flooz_x, flooz_y);
                        if (flooz_counter == 7){ flooz_direction = W;}
                    }              
                break; 
                case SEG_BEND_NW:  //NW pipe
                    if (flooz_direction == E) {
                        VDP_setTileMapXY(BG_B,TILE_ATTR_FULL(PAL1,0,FALSE,TRUE,FLOOZTILEINDEXCWSTART+flooz_counter), flooz_x, flooz_y);
                        if (flooz_counter == 7){ flooz_direction = N;}
                    } else if (flooz_direction == S) {
                        VDP_setTileMapXY(BG_B,TILE_ATTR_FULL(PAL1,0,FALSE,TRUE,FLOOZTILEINDEXCCWSTART+flooz_counter), flooz_x, flooz_y);
                        if (flooz_counter == 7){ flooz_direction = W;}
                    }   
                break;
                case SEG_BEND_NE:  //NE pipe
                    if (flooz_direction == S) {
                        VDP_setTileMapXY(BG_B,TILE_ATTR_FULL(PAL1,0,FALSE,FALSE,FLOOZTILEINDEXCCWSTART+flooz_counter), flooz_x, flooz_y);
                        if (flooz_counter == 7){ flooz_direction = E;}
                    } else if (flooz_direction == W) {
                        VDP_setTileMapXY(BG_B,TILE_ATTR_FULL(PAL1,0,FALSE,FALSE,FLOOZTILEINDEXCWSTART+flooz_counter), flooz_x, flooz_y);
                        if (flooz_counter == 7){ flooz_direction = N;}
                    }  
                break;
                case SEG_BEND_SE:  //SE pipe
                    if (flooz_direction == W) {
                        VDP_setTileMapXY(BG_B,TILE_ATTR_FULL(PAL1,0,TRUE,FALSE,FLOOZTILEINDEXCWSTART+flooz_counter), flooz_x, flooz_y);
                        if (flooz_counter == 7){ flooz_direction = S;}
                    } else if (flooz_direction == N) {
                        VDP_setTileMapXY(BG_B,TILE_ATTR_FULL(PAL1,0,TRUE,FALSE,FLOOZTILEINDEXCCWSTART+flooz_counter), flooz_x, flooz_y);
                        if (flooz_counter == 7){ flooz_direction = E;}
                    }
                break;
            }
        //END OF BEND PIPES
        //BONUS SCORE FOR COMPLETING A LOOP
        } else if ((my_grid[flooz_grid_y][flooz_grid_x] == -SEG_CROSS) && ((flooz_length-2) %3 == 1)){
            if (flooz_counter == 7){ my_score += 100;}
        //START OF NORMAL PIPE SEGMENTS  
		} else {
            //when in a straight tile based on the direction, we draw the flooz
            switch (flooz_direction) {
                case N: VDP_setTileMapXY(BG_B,TILE_ATTR_FULL(PAL1,0,TRUE,FALSE,FLOOZTILEINDEXVERTSTART -flooz_counter), flooz_x, flooz_y); break;
                case E: VDP_setTileMapXY(BG_B,TILE_ATTR_FULL(PAL1,0,FALSE,TRUE,FLOOZTILEINDEXHORZSTART -flooz_counter), flooz_x, flooz_y); break;
                case S: VDP_setTileMapXY(BG_B,TILE_ATTR_FULL(PAL1,0,FALSE,FALSE,FLOOZTILEINDEXVERTSTART -flooz_counter), flooz_x, flooz_y); break;
                case W: VDP_setTileMapXY(BG_B,TILE_ATTR_FULL(PAL1,0,FALSE,FALSE,FLOOZTILEINDEXHORZSTART -flooz_counter), flooz_x, flooz_y); break;
            }
            //reservoir segments
            if ((abs(my_grid[flooz_grid_y][flooz_grid_x]) == SEG_RESV_H) || (abs(my_grid[flooz_grid_y][flooz_grid_x]) == SEG_RESV_V)){
                if ((flooz_length-2) %3 == 0){
                    if ((my_pace > 0) && (flooz_counter == 0)){my_pace = 0;}
                }
                game_pace_ptr = &game_speeds[my_pace];
                switch (flooz_direction) {
                    case N: 
                        VDP_setTileMapXY(BG_B,TILE_ATTR_FULL(PAL1,0,TRUE,FALSE,FLOOZTILEINDEXVERTSTART -flooz_counter), flooz_x+1, flooz_y); 
                        VDP_setTileMapXY(BG_B,TILE_ATTR_FULL(PAL1,0,TRUE,FALSE,FLOOZTILEINDEXVERTSTART -flooz_counter), flooz_x-1, flooz_y); 
                    break;
                    case E: 
                        VDP_setTileMapXY(BG_B,TILE_ATTR_FULL(PAL1,0,FALSE,TRUE,FLOOZTILEINDEXHORZSTART -flooz_counter), flooz_x, flooz_y+1); 
                        VDP_setTileMapXY(BG_B,TILE_ATTR_FULL(PAL1,0,FALSE,TRUE,FLOOZTILEINDEXHORZSTART -flooz_counter), flooz_x, flooz_y-1);
                    break;
                    case S: 
                        VDP_setTileMapXY(BG_B,TILE_ATTR_FULL(PAL1,0,FALSE,FALSE,FLOOZTILEINDEXVERTSTART -flooz_counter), flooz_x+1, flooz_y); 
                        VDP_setTileMapXY(BG_B,TILE_ATTR_FULL(PAL1,0,FALSE,FALSE,FLOOZTILEINDEXVERTSTART -flooz_counter), flooz_x-1, flooz_y);
                    break;
                    case W: 
                        VDP_setTileMapXY(BG_B,TILE_ATTR_FULL(PAL1,0,FALSE,FALSE,FLOOZTILEINDEXHORZSTART -flooz_counter), flooz_x, flooz_y+1);
                        VDP_setTileMapXY(BG_B,TILE_ATTR_FULL(PAL1,0,FALSE,FALSE,FLOOZTILEINDEXHORZSTART -flooz_counter), flooz_x, flooz_y-1); 
                    break;
                }
                if (flooz_counter == 7){ my_score += 100;}
            }
            //switch the sign after filling a segment
            if ((my_grid[flooz_grid_y][flooz_grid_x] > 0) && ((flooz_length-2)%3 == 0)){
		         my_grid[flooz_grid_y][flooz_grid_x] = my_grid[flooz_grid_y][flooz_grid_x] * -1;
            }
            //end pipes
            if ((SEG_END_S <= my_grid[flooz_grid_y][flooz_grid_x] && my_grid[flooz_grid_y][flooz_grid_x] <= SEG_END_W) && ((flooz_length-2) %3 == 0) && flooz_counter == 7){ my_state = LEVEL_CLEARED;}
        }
    } 
    
    // move flooz_x and flooz_y at the end as we need correct coordinates before we draw
    if ((game_timer%game_pace_ptr->full_tile_frame == game_pace_ptr->logic_frame) && (flooz_counter == 7)){
        switch (flooz_direction) {
            case N: flooz_y--; break;
            case E: flooz_x++; break;
            case S: flooz_y++; break;
            case W: flooz_x--; break;
        }
        flooz_length++;
        //change flooz_grid variables based on the flooz_length
        if ((flooz_length-2) %3 == 0){
            switch (flooz_direction){
                case N: flooz_grid_y--; break;
                case E: flooz_grid_x++; break;
                case S: flooz_grid_y++; break;
                case W: flooz_grid_x--; break;
            }
            my_score += 20;
            if (my_state == BONUS_MODE){
                my_score += 20;
            }
            my_segment_goal--;
            if (my_segment_goal == 0){
                my_state = BONUS_MODE; 
                VDP_drawText("BONUS MODE", 10, 24); 
            }    
            checkNextSegment();
            my_pace = my_levels_ptr[my_level - 1]->level_pace;
            game_pace_ptr = &game_speeds[my_pace];
        }  
    }   
}

//simple function inverseDirection() that inverses the enum NESW
enum direction inverseDirection(enum direction windsock){
    switch (windsock){
        case N: windsock = S; break;
        case E: windsock = W; break;
        case S: windsock = N; break;
        case W: windsock = E; break;
    }
    return windsock;
}

//the checkNextPipe() function which groups all the game-state logic when flooz hits a new grid position
void checkNextSegment(){
	//check if pipe is valid and update gamestate
    if (my_grid[flooz_grid_y][flooz_grid_x] == SEG_BLANK){ 
        if (my_state == BONUS_MODE){
            VDP_drawText("LEVEL CLEARED", 10, 24);
            my_state = LEVEL_CLEARED;
        } else {         
            VDP_drawText("OOOPS NO PIPE", 10, 24);
            my_state = GAME_OVER;
        }
    } else if (SEG_START_S <= my_grid[flooz_grid_y][flooz_grid_x] && my_grid[flooz_grid_y][flooz_grid_x] <= SEG_START_W){
        if (my_state == BONUS_MODE){
            VDP_drawText("LEVEL CLEARED", 10, 24);
            my_state = LEVEL_CLEARED;
        } else {         
            VDP_drawText("WRONG PIPE", 10, 24);
            my_state = GAME_OVER;
        }
    } else if (SEG_END_S <= my_grid[flooz_grid_y][flooz_grid_x] && my_grid[flooz_grid_y][flooz_grid_x] <= SEG_END_W){
        if (pipe_data[my_grid[flooz_grid_y][flooz_grid_x]+2][inverseDirection(flooz_direction)] == TRUE) {    //DOUBLE CHECK THIS NUMBER
            VDP_drawText("WRONG PIPE", 10, 24);
            my_state = GAME_OVER;
        } else {
            VDP_drawText("PIPE COMPLETED", 10, 24);
        }
    } else if ((my_grid[flooz_grid_y][flooz_grid_x] == SEG_RESV_H) || (my_grid[flooz_grid_y][flooz_grid_x] == SEG_RESV_V)){
        if (pipe_data[my_grid[flooz_grid_y][flooz_grid_x]][inverseDirection(flooz_direction)] == FALSE){
            VDP_drawText("RESERVOIR DOGS", 10, 24);
        }
    } else if (pipe_data[abs(my_grid[flooz_grid_y][flooz_grid_x])-12][inverseDirection(flooz_direction)] == FALSE) {
        if (my_state == BONUS_MODE){
            VDP_drawText("LEVEL CLEARED", 10, 24);
            my_state = LEVEL_CLEARED;
        } else {         
            VDP_drawText("WRONG PIPE", 10, 24);
            my_state = GAME_OVER;
        }
    } 
}

//the actual drawScoreboard function
void drawScoreboard(){
    char score_str[5];
    sprintf(score_str,"%d",my_score);
    VDP_drawText("SCORE:",17,1);
    VDP_drawText(score_str,23,1);
    char pipes_str[3];
    sprintf(pipes_str,"%d",my_segment_goal);
    VDP_drawText("TARGET:",29,1);
    VDP_drawText(pipes_str,36,1);
    if(my_segment_goal <= 9){VDP_drawText(" ",37,1);}
    if(my_segment_goal == 0){VDP_drawText("0",37,1);}
}

//wrapper function around our timer
void doTimer(){
		if(my_state != GAME_PAUSE){
				timer++;
		}
}

/*
//debug function that draws the current 
void debugTimer(){
    char pace_str[5];
    sprintf(pace_str,"%d",my_pace);
    VDP_drawText("PACE:",1,25);
    VDP_drawText(pace_str,6,25);
    VDP_drawText("DRAWF:");
}
*/
