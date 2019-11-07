#ifndef COMMON_H
#define COMMON_H

/*! \brief
 * Tipo composto de dois indices i e j.
 */
struct Position
{
    unsigned i, j;
};

/*! \brief
 * Tipo usado para guarda os status do jogo.
 */
struct States
{
    unsigned lives{5};
    unsigned foods{0};
    unsigned level{1};
};

#endif