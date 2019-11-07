#include "snake_game.h"
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>

/**
 *
 */
SnakeGame::SnakeGame()
{
    foodsToEat = 10;
    spawn_set = false;
}

/**
 * Libera a memoria alocada no initialize_game e fecha o arquivo de input.
 */
SnakeGame::~SnakeGame()
{
    file_input.close();
    delete m_snakeAI;

    if(state.foods != foodsToEat) { delete[] level; }
}

/**
 * retira de uma string dois inteiro
 * @param line = string que contem os interios separados por espacos
 */
void SnakeGame::get_RowsColumns(std::string & line)
{
    std::string aux_str;

    //Pega o numero de linhas
    aux_str = line.substr(0, line.find(' '));
    rows = std::stoul(aux_str);
    //Pega o numero de colunas
    aux_str = line.substr(line.find(' '));
    columns = std::stoul(aux_str);
}

/**
 * Carrega o primeiro level do jogo
 * @param file_name = nome do arquivo de entrada com as configuracoes
 */
void SnakeGame::initialize_game(std::string file_name)
{
    file_input.open(file_name);
    std::string line;

    getline(file_input, line);
    get_RowsColumns(line);

    if((rows < 1 or columns < 1) or (rows > 100 or columns > 100))
    {
        level = new char[1];
        failReadFile = true;
    }else
    {
        failReadFile = false;
        level = new char[rows*columns];
        for(auto i{0u}; i < rows; i++)
        {
            getline(file_input,line);
            for(auto j{0u}; j < columns; j++)
            {
                level[i*columns+j] = line[j];
                if(line[j] == '*')
                {
                    snake.i = i;
                    snake.j = j;
                    spawn.i = i;
                    spawn.j = j;
                    spawn_set = true;
                }
            }
        }
    }
    m_snakeAI = new Snake{level, rows, columns, snake};
}

/**
 * Gera a comida aleatoriamente em algum lugar do grid
 */
void SnakeGame::render_food()
{
    srand(time(NULL));

    do
    {
       food.i = rand() % rows;
       food.j = rand() % columns;

       if(level[food.i*columns+food.j] == ' '){
            if(not m_snakeAI->checks_body(food.i*columns+food.j))
            {
                if(check_sidesFood(food))
                {
                    level[food.i*columns+food.j] =  'f';
                    break;
                }
            }
        }
    
    } while(true);
    
}

/**
 * verifica se a comida tem um dos lados livres para que a snake chegue ate ela.
 * @param food = posicao da food no grid
 * @return true se um dos vizinhos da food esta livre, false caso contrario.
 */
bool SnakeGame::check_sidesFood(Position & food)
{
    if(level[(food.i+1)*columns+food.j] == ' ')
    {
        return true;
    }else if(level[(food.i)*columns+(food.j+1)] == ' ')
    {
        return true;
    }else if(level[(food.i-1)*columns+food.j] == ' ')
    {
        return true;
    }else if(level[food.i*columns+(food.j+1)] == ' ')
    {
        return true;
    }

    return false;
}

/**
 * mostra no terminal o grid atual mais os estatus atuais da simulacao.
 */
void SnakeGame::render_grid()
{
    level[0] = '#';
    std::cout << "Vidas: " << state.lives << std::endl;
    std::cout << "Comidas: " << state.foods << "/" << foodsToEat << std::endl;
    std::cout << "Level: " << state.level << std::endl;
    for(auto i{0u}; i < rows; i++)
    {
        for(auto j{0u}; j < columns; j++)
        {
            if(level[i*columns+j] == '.'){
                std::cout << " ";
            }else
            {
                std::cout << level[i*columns+j];
            }
        }
        std::cout << "\n";
    }
}

/**
 * move a snake no grid.
 */
void SnakeGame::snakeMovement()
{
    auto aux_body{m_snakeAI->get_snakeBody()};
    std::vector<unsigned> body_movement, path{m_snakeAI->get_shortestPath(food)};
    unsigned forword{0}, count{0};

    //Coloca a snake na posicao inicial
    while(not aux_body.empty())
    {
        body_movement.push_back(aux_body.front());
        level[aux_body.front()] = 'o';
        aux_body.pop();
        count++;
    }
    render_grid();
    //Faz o movimento da snake
    for(auto i{2u}; i < path.size(); i++)
    {
        //system("clear");
        level[body_movement[forword]] = ' ';
        forword++;
        body_movement.push_back(path[i]);
        level[path[i]] = 'o';
        if(i == (path.size()-1)) { state.foods++; }
        render_grid();
        std::this_thread::sleep_for(std::chrono::milliseconds(175));
    }
    //system("clear");
    //Limpa o caminho feito pela snake.
    for(auto i{forword}; i < body_movement.size(); i++)
    {
        level[body_movement[i]] = ' ';
    }
}

/**
 * move a snake na direcao da parede.
 */
void SnakeGame::snakeKamikazeMovement()
{
    auto aux_body{m_snakeAI->get_snakeBody()};
    std::vector<unsigned> body_movement, path{m_snakeAI->get_kamikazePath()};
    unsigned forword{0};

    //Coloca a snake na posicao inicial
    while(not aux_body.empty())
    {
        body_movement.push_back(aux_body.front());
        level[aux_body.front()] = 'o';
        aux_body.pop();
    }
    render_grid();
    //Faz o movimento da snake
    for(auto i{0u}; i < path.size(); i++)
    {
        //system("clear");
        level[body_movement[forword]] = ' ';
        forword++;
        body_movement.push_back(path[i]);
        level[path[i]] = 'o';
        render_grid();
        std::this_thread::sleep_for(std::chrono::milliseconds(175));
    }
    //system("clear");
    //Limpa o caminho feito pela snake.
    for(auto i{forword}; i < body_movement.size(); i++)
    {
        level[body_movement[i]] = ' ';
    }
}

/**
 * verificar se a snake comeu todoas as comidas do level, se sim e a simulacao tiver mais de um level atualiza para o proximo.
 */
void SnakeGame::update()
{
    if((not failReadFile) and (state.foods == foodsToEat))
    {
        
        do
        {
            if(update_level())
            {
                state.foods = 0;
                m_snakeAI->reset(spawn, 0);
                m_snakeAI->update_grid(level, snake, rows, columns);
                if(spawn_set){ wait_user(1); }
                //system("clear");
            }
        }while(not spawn_set);
    }
}

/**
 * carrega o proximo level na memoria.
 * @return true se o proximo level foi carregado, false caso contrario.
 */

bool SnakeGame::update_level()
{
    delete[] level;
    std::string line;

    if(not file_input.eof())
    {
        getline(file_input, line);
        
        if(line != "")
        {
            state.level++;
            get_RowsColumns(line);

            if((rows < 1 or columns < 1) or (rows > 100 or columns > 100))
            {
                failReadFile = true;
                return false;
            }else
            {
                spawn_set = false;
                failReadFile = false;
                level = new char[rows*columns];
                for(auto i{0u}; i < rows; i++)
                {
                    getline(file_input,line);
                    for(auto j{0u}; j < columns; j++)
                    {
                        level[i*columns+j] = line[j];
                        if(line[j] == '*')
                        {
                            snake.i = i;
                            snake.j = j;
                            spawn.i = i;
                            spawn.j = j;
                            spawn_set = true;
                        }
                    }
                }
                return true;        
            }    

        }

        return false;
    }

    return false;
}

/**
 * processa os eventos do jogo <br>
 * verifica se existe um caminho ate a comida <br>
 * atualiza os status <br>
 * renderiza o movimento da snake
 */
void SnakeGame::process_events()
{
    render_food();
    render_grid();

    if(m_snakeAI->find_solution(snake, food))
    {
        snakeMovement();
        m_snakeAI->update_body(food);
        //state.foods++;
        if(state.foods != foodsToEat)
        {
            m_snakeAI->reset(spawn, 32000); //passando o spwan so por passar.(nao e usuado no reset)
            snake.i = food.i;
            snake.j = food.j;
        }
        
    }else
    {
        m_snakeAI->snake_kamikaze(snake);
        snakeKamikazeMovement();
        state.lives--;
        wait_user(2);
        if(state.lives != 0)
        {
            level[food.i*columns+food.j] = ' ';
            snake.i = spawn.i;
            snake.j = spawn.j;
            m_snakeAI->reset(spawn, 1);
        }
    }
}

/**
 * exibi no terminal uma menssagem correspondente ao evento que acontceu(passou o level ou bateu) e espera o usuario apertar enter.
 * @param event = qual evento aconteceu <br>
 * 1 = a snake limpou o level <br>
 * 2 = a snake bateu
 */
void SnakeGame::wait_user(short event)
{
    std::string user_input;
    //system("clear");
    switch(event)
    {
        case 1:
            std::cout << "=============================\n";
            std::cout << "| A snake completou o level |\n";
            std::cout << "=============================\n";
            std::cout << ">>> APERTER ENTER para ir para o proximo level da simulacao\n";
            break;
        case 2:
            std::cout << "=============================\n";
            std::cout << "|       A snake bateu!      |\n";
            std::cout << "=============================\n";
            std::cout << ">>> APERTER ENTER para continuar a simulacao\n";
            break;
        default:
            std::cout << "Nenhuma menssagem!\n";
    }
    getline(std::cin, user_input);
}

/**
 * verifica se  uma das condicoes de termino de jogo aconteceu
 * @return true se o snake comeu o numero de todas as comidas do level ou se a quantidade de vidas chegou a zero. false caso contrario.
 * @return true se houve falha ao ler o input.
 */
bool SnakeGame::gamer_over()
{
    if( state.lives == 0 )
    {
        return true;
    }

    if( state.foods == foodsToEat )
    {
        return true;
    }

    if(failReadFile){
        return true;
    }

    return false;
}

/**
 * Mostra na tela as informacoes inicias.
 * @return true se o usuario apertou enter, false caso contrario.
 */
bool SnakeGame::show_info()
{
    std::string user_input;

    std::cout << "==============================================\n";
    std::cout << "| Vidas da Snake: " << state.lives << " | Comidas para comer: " << foodsToEat << " |\n";
    std::cout << "==============================================\n";
    std::cout << ">>> Aperte <ENTER> para iniciar a simulacao!";
    getline(std::cin, user_input);

    if(user_input == "") { return true; }
    return false;
}

/**
 * exibi uma menssagem final no terminal
 */
void SnakeGame::end_messenge()
{
    if(not failReadFile)
    { 
        //system("clear"); 
    }else
    {
        std::cout << "Tamanho do grid invalido\n";
        state.foods = 0;
    }
    

    if(state.foods == foodsToEat)
    {
        std::cout << "=============================\n";
        std::cout << "| A snake zerou a simulacao |\n";
        std::cout << "=============================\n";
    }
    
    if(state.lives == 0)
    {
        std::cout << "=============================\n";
        std::cout << "|      A snake perdeu!      |\n";
        std::cout << "=============================\n";
    }
}
