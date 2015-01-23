#ifndef LOGIC_H
#define LOGIC_H

#include <stdio.h>

enum choice{ unknown=0,stone=1,paper=2,scissors=3};
typedef struct player{

	int wins;
	choice ch;
}player;

extern void saveCallSettings();
extern void loadCallSettings();
extern void startGame();
extern choice setPlayerChoice( int index , int defects );
extern choice setPlayerChoice( int index , choice ch );
extern void battle(choice pl1, choice pl2);
extern char* getScore();
extern choice aiChoice();
choice getPlayerChoice( int index );


#endif