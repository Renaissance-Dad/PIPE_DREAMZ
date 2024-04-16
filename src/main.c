#include <genesis.h>
//#include <resources.h>

//FFWD DECLARATION OF OUR FUNCTIONS
void drawBorder(u8 x_column, u8 y_row, u8 width, u8 height);
void initGame();

//GLOBAL SPRITE POINTERS
Sprite* selector_spr;

int main(bool hard_reset)
{
    //loading the border tileset
    VDP_loadTileSet(bordertile.tileset,1,DMA);

    while(1)
    {
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
    //loadLevel(1);
}