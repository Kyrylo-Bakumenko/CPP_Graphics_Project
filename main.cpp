#include <iostream>
#include <iomanip>
#include <SDL.h>
#include "Screen.h"
#include "Swarm.h"
using namespace std;
using namespace methods;

int main(int argc, char* args[] )
{
    srand((unsigned int)time(NULL));
    unsigned char red;
    unsigned char green;
    unsigned char blue;
    Uint8 choice = Screen::menu();
    Uint8 demonstration = (choice >> 4);
    Uint8 gridSize = choice%16;
    unsigned int seed;
    unsigned int density;

    Screen screen;
    if(!screen.init()){
        cout<<"Error initialising"<<endl;
    }

    Swarm swarm;


    while(true) {

        // Update particles
        // Draw particles
        Uint32 elapsed = SDL_GetTicks();
        screen.updateColors(red, green, blue, elapsed);

        // solid color
        if (demonstration == 1) {// logic selection
            screen.solidColorScreen(red, green, blue);
        // grid
        }else if(demonstration==2){
            screen.colorGrid(red, green, blue, gridSize);
        // cursor
        }else if(demonstration==3) {
            screen.cursorLines(red, green, blue);
        // smoothed cursor
        }else if(demonstration==4){
            screen.cursorLinesFilled(red, green, blue);
        // GoL, random
        }else if(demonstration==5){
            screen.gameOfLifeRandom();
        // GoL, seeded
        }else if(demonstration==6) {
            if (!screen.isGolInitiated()) {
                cout << "Enter desired seed: " << flush;
                cin >> seed;
                cout << "Enter desired density: " << flush;
                cin >> density;
            }
            screen.gameOfLifeFromSeed(seed, density);
        // growth sim, random
        }else if(demonstration==7) {
            screen.growthRandom();
        // growth sim, seeded
        }else if(demonstration==8) {
            if (!screen.isGolInitiated()) {
                cout << "Enter desired seed: " << flush;
                cin >> seed;
                cout << "Enter desired density: " << flush;
                cin >> density;
            }
            screen.growthFromSeed(seed, density);
        // sky
        }else if(demonstration==9){
            const Particle * const pParticles = swarm.getParticles();
            for(int i=0; i<Swarm::NPARTICLES; i++){
                Particle particle = pParticles[i];

                int x = (particle.m_x+1) * Screen::SCREEN_WIDTH/2;
                int y = (particle.m_y+1) * Screen::SCREEN_HEIGHT/2;

                screen.setPixel(x, y, red, green, blue);
            }
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