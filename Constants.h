#ifndef _CONSTANTS_H__
#define _CONSTANTS_H__

#define FREE (' ')
#define WALL ('#')
#define GOAL ('.')
#define PLAYER ('@')
#define PLAYER_ON_GOAL ('+')
#define BOX ('$')
#define BOX_ON_GOAL ('*')
#define DEADLOCK ('X')
#define FIRSTBIT ( 2147483648 )
#define ROW_SIZE ( NR_COLUMNS )
#define COLUMN_SIZE ( NR_ROWS )
#define WORD_SIZE ( 8*sizeof(unsigned int) )

#include "Structs.h"

extern int NR_ROWS;
extern int NR_COLUMNS;
extern int BOARD_SIZE;
extern int NR_GOALS;
extern int NR_BOXES;
extern bitString WALLS;
extern bitString GOALS;
extern bitString DEADLOCKS;
extern bitString BOXES;
extern std::vector<pos> VEC_GOALS;
extern std::vector<std::vector<char> > BOARD;
extern pos PLAYER_START;


#endif

