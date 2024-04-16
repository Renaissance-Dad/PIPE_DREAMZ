#include <genesis.h>
//#include <resources.h>

//DEFINES
#define GRIDROWS 7
#define GRIDCOLUMNS 11
#define TILEINDEXOFFSET 10
#define AMOUNTOFSPECIALTILES 90
#define GRIDOFFSETX 6
#define GRIDOFFSETY 4
#define FLOOZOFFSETX 7
#define FLOOZOFFSETY 5
#define SELECTOROFFSETX 47
#define SELECTOROFFSETY 31

//FFWD DECLARATION OF OUR FUNCTIONS
void drawBorder(u8 x_column, u8 y_row, u8 width, u8 height);
void initGame();
void loadLevel(u8 lvl);
void drawSegment(u8 x_grid, u8 y_grid, u8 segment);
void myJoyEventCallbackGame(u16 joy, u16 changed, u16 state);
void drawSelector();

//STRUCTS
struct level{
   u8 level_number;
   u8 target_pipenumber;
   int grid_data[GRIDROWS][GRIDCOLUMNS];
   // we can add additional data fields later
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
    }
};

//GLOBAL SPRITE POINTERS
Sprite* selector_spr;

//GLOBAL VARIABLES
int my_grid[GRIDROWS][GRIDCOLUMNS];
u8 flooz_x = NULL; //in tiles
u8 flooz_y = NULL; //in tiles
enum direction{N,E,S,W};
enum direction flooz_direction = NULL;
u8 selector_x;
u8 selector_y;

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
    }
}

//MAIN FUNCTION
int main(bool hard_reset)
{
    //loading the border tileset
    VDP_loadTileSet(bordertile.tileset,1,DMA);
    //loading the starting segment tileset
    VDP_loadTileSet(pipesspecialtile.tileset,TILEINDEXOFFSET,DMA);
    //loading the regular segment tileset
    VDP_loadTileSet(pipesregulartile.tileset,(TILEINDEXOFFSET + AMOUNTOFSPECIALTILES),DMA);

    initGame();

    while(1)
    {
        drawSelector();
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
    
    //loading the level data
    loadLevel(1);
}

//loadLevel() function copies the level data from ROM into RAM
void loadLevel(u8 lvl){
    // copy the leveldata first
    if (lvl == 1){
        memcpy(my_grid, level_one.grid_data, 308); //int = 4bytes, times 77
    }
    // draw the leveldata on the grid
    for(u8 i=0; i< GRIDROWS; i++){
        for(u8 j=0; j< GRIDCOLUMNS; j++){
            if (my_grid[i][j] == 0){
                drawSegment(j, i, 0);
            } else if (my_grid[i][j] >= 1 && my_grid[i][j] <= 4) {
                drawSegment(j, i, my_grid[i][j]);
                flooz_x = (j*3)+ FLOOZOFFSETX; 
                flooz_y = (i*3)+ FLOOZOFFSETY; 
                switch (my_grid[i][j]){
                    case 1: flooz_direction = S; break;
                    case 2: flooz_direction = E; break;
                    case 3: flooz_direction = N; break;
                    case 4: flooz_direction = W; break;
                }
            }
        }     
    }
}

// the drawSegment() function draws 3x3 tiles which represents a pipe segment. 
void drawSegment(u8 x_grid, u8 y_grid, u8 segment){
    VDP_fillTileMapRectInc(BG_A,TILE_ATTR_FULL(PAL1,0,FALSE,FALSE,(segment*9)+TILEINDEXOFFSET),(x_grid*3)+GRIDOFFSETX, (y_grid*3)+GRIDOFFSETY, 3, 3);
}

//drawSelector() function
void drawSelector(){
    SPR_setPosition(selector_spr,(selector_x*24)+SELECTOROFFSETX,(selector_y*24)+SELECTOROFFSETY);
    SPR_update();
}