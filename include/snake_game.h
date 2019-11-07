#ifndef SNAKEGAME_H
#define SNAKEGAME_H
#include <iostream>
#include <fstream>
#include "../include/snake.h"
#include "common.h"


/*! \brief
 * Gerencia a simulacao.
 */
class SnakeGame
{
    private:
        Snake *m_snakeAI;
        char *level;
        unsigned rows, columns, foodsToEat;
        Position snake, food, spawn;
        std::ifstream file_input;
        States state;
        bool spawn_set; //Se true, o spawn foi setado, false caso contrario.
        bool failReadFile; //Se true, o houve falha ao ler o arquivo. Caso cantrario false.
    public:
        SnakeGame();
        ~SnakeGame();
        void initialize_game(std::string file_name);
        void get_RowsColumns(std::string & line);
        bool update_level();
        void update();
        void render_food();
        bool check_sidesFood(Position & food);
        void render_grid();
        void snakeMovement();
        void process_events();
        bool gamer_over();
        void end_messenge();
        void snakeKamikazeMovement();
        void wait_user(short event);
        bool show_info();

};


#endif