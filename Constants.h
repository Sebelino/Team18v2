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

extern int NR_ROWS;
extern int NR_COLUMNS;
extern int BOARD_SIZE;

#endif

