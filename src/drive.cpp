#include <iostream>
#include "snake_game.h"
#include "snake.h"

int main(int argc, char *argv[])
{
    SnakeGame maneger;
    maneger.initialize_game(argv[1]);

    if(maneger.show_info())
    {
        while(not maneger.gamer_over())
        {
            maneger.process_events();
            maneger.update();
        }
        maneger.end_messenge();
    }
        
    return 0;
}