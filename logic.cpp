#include "game_logic.h"
#include "skin_detection.h"
#include <random>
#include <fstream>
#include <sstream>


using namespace std;

struct player player1, player2;

choice setPlayerChoice( int index, choice ch ){

	if ( index==1 ) player1.ch=ch;
	else player2.ch=ch;

	return ch;
}
choice setPlayerChoice( int index , int defects ) {

	choice temp = unknown;

	if(defects==0){
				
		temp = stone;
	}

	else if(defects<=2){

		temp = scissors;
	}

	else if(defects<=4){

		temp = paper;

	}

	switch ( index ) {

	case 1:
		player1.ch=temp;
	break;
	
	case 2:
		player2.ch=temp;

	}
	return temp;
}
void startGame(){

	player1.wins=0;
	player1.ch=unknown;

	player2.wins=0;
	player2.ch=unknown;

	srand( ( unsigned int )time( NULL ) );

}

void battle(choice pl1,choice pl2){

	if ( !( (pl1==unknown) || (pl2==unknown) || (pl1==pl2) ) ){

		switch ( pl1 ) {

		case stone:

			switch ( pl2 ){

			case scissors:

				player1.wins++;
				break;

			case paper:

				player2.wins++;
				break;
			}
			break;

		case paper:

			switch ( pl2 ){

			case stone:

				player1.wins++;
				break;

			case scissors:

				player2.wins++;
				break;
			}
			break;

		case scissors:

			switch ( pl2 ){

			case stone:

				player2.wins++;
				break;

			case paper:

				player1.wins++;
				break;
			}
			break;
		}

		
	}
}
choice getPlayerChoice( int index ){

	if( index == 1 ) return player1.ch;
	else return player2.ch;

}

char* getScore(){

	char* buf=new char[10];
	sprintf(buf," %d : %d ", player1.wins, player2.wins);

	return buf;
}
choice aiChoice(){

	int tmp = rand() % 3;
	switch ( tmp ) {

	case 0:
		return stone;
	case 1:
		return paper;
	case 2:
		return scissors;
	default:
		return unknown;
	}
}
void saveCallSettings(){

	fstream set;
	
	set.open("setting.txt",ios::out);
	set.clear();
	for(int i=0;i<4;i++){
		if(i<2){
			ostringstream temp;
			temp<<vectorMs[i];
			set<<temp.str();
			temp.unsetf(C1_BLANK);
			set<<"\n";
		}
		ostringstream temp;
		temp<<covarianceMatrix[i];
		set<<temp.str();
		temp.unsetf(C1_BLANK);
		set<<"\n";
	}

	set.close();
	//fputs
}

void loadCallSettings(){

	fstream set;
	
	set.open("setting.txt",ios::in);
	
	string tmp;
	for(int i=0;i<4;i++){
		if(i<2){
			getline(set,tmp);
			istringstream temp(tmp);
			temp>>vectorMs[i];
			//vectorMs[i]<<temp;
			temp.clear();
		}
		getline(set,tmp);
		istringstream temp(tmp);
		temp>>covarianceMatrix[i];
			//vectorMs[i]<<temp;
		temp.clear();
	}

	set.close();
	//fputs
}

