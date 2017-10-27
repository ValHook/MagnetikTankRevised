#ifndef TRANSITION_FUNCTIONS_H_INCLUDED
#define TRANSITION_FUNCTIONS_H_INCLUDED

#include "Transition_Functions.h"

void darken_buffer_matching_tank_height(BITMAP *buffer, TANK *tank);
void change_room_matching_tank_height(int *prochaineSalle, TANK *tank);
void game_over_animation(BITMAP *buffer, TANK *tank, int beenDeadFor, int *salleSuivante);
void game_won_animation(BITMAP *buffer, TANK *tank, int hasWonFor, int *salleSuivante, BITMAP *tab[4]);

#endif // TRANSITION_FUNCTIONS_H_INCLUDED
