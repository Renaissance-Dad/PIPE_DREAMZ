/**
 * Hello World Example
 * Created With Genesis-Code extension for Visual Studio Code
 * Use "Genesis Code: Compile" command to compile this program.
 **/
#include <genesis.h>

int main(bool hard_reset)
{
    VDP_drawText("Hello Sega!!", 10,13);
    while(1)
    {
        SYS_doVBlankProcess();
    }
    return (0);
}
