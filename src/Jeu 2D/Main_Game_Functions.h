#ifndef MAIN_GAME_FUNCTIONS_H_INCLUDED
#define MAIN_GAME_FUNCTIONS_H_INCLUDED

#include <allegro.h>
#include "Structures.h"
#include "Main_Game_Functions.h"

void allegro_initialisation();
void gestionnaire_jeu();
int jeu_principal(int nSalle, int sallePrecedente, int *cleBoss);
int menu_principal(int nSalle, int sallePrecedente, int *cleBoss);


#endif // MAIN_GAME_FUNCTIONS_H_INCLUDED
