#include <iostream>
#include <iomanip>
#include <SDL.h>
#include "Screen.h"
using namespace std;
using namespace methods;

int main(int argc, char* args[] )
{
    unsigned char red;
    unsigned char green;
    unsigned char blue;
    Uint8 choice = Screen::menu();
    Uint8 demonstration = (choice >> 4);
    Uint8 gridSize = choice%16;

    Screen screen;
    if(!screen.init()){
        cout<<"Error initialising"<<endl;
    }



    while(true) {

        // Update particles
        // Draw particles
        Uint32 elapsed = SDL_GetTicks();
        screen.updateColors(red, green, blue, elapsed);

        if (demonstration == 1) {// logic selection
            screen.solidColorScreen(red, green, blue);
        }else if(demonstration==2){
            screen.colorGrid(red, green, blue, gridSize);
        }else if(demonstration==3){
            screen.cursorLines(red, green, blue);
        }else if(demonstration==4){
            screen.gameOfLifeRandom();
        }
        // Draw screen
        screen.update();
        // Check for messages/events

        if(!screen.processEvents()){
            break;
        }
    }

    screen.close();

    return 0;
}