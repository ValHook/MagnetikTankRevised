#ifndef CALCULATION_FUNCTIONS_H_INCLUDED
#define CALCULATION_FUNCTIONS_H_INCLUDED

#include <allegro.h>
#include <math.h>
#include "Structures.h"
#include "Calculation_Functions.h"

void calculate_tank_speed_matching_commands(TANK *tank);
void calculate_tank_position_matching_speed(TANK *tank);
void calculate_tank_direction_matching_commands(TANK *tank);
void calculate_tank_direction_matching_speed(TANK *tank);
void calculate_tank_speed_matching_friction(TANK *tank);
void calculate_tank_speed_matching_magnetism(TANK *tank);
void calculate_tank_speed_matching_collision(TANK *tank, MAP *room);

#endif // CALCULATION_FUNCTIONS_H_INCLUDED
