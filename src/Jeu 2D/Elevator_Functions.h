#ifndef ELEVATOR_FUNCTIONS_H_INCLUDED
#define ELEVATOR_FUNCTIONS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <allegro.h>
#include "Structures.h"
#include "Elevator_Functions.h"

void activate_pressure_elevators(MAP *room, TANK *tank);
void calculate_elevator_height(MAP *room);
void calculate_tank_height_matching_elevator(MAP *room, TANK *tank);
void init_entrance_exit_elevators(MAP *room, int salleActuelle, int sallePrecedente);

#endif // ELEVATOR_FUNCTIONS_H_INCLUDED
