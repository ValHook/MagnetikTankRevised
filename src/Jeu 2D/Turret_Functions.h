#ifndef TURRET_FUNCTIONS_H_INCLUDED
#define TURRET_FUNCTIONS_H_INCLUDED

#include <allegro.h>
#include "Structures.h"
#include "Turret_Functions.h"

void deactivate_pressure_turrets(MAP *room, TANK *tank);
void calculate_turret_height(MAP *map);
void calculate_tank_height_matching_turret(MAP *room, TANK *tank);
void rotate_turrets(MAP *room);
void shoot_lasers_on_tank(MAP *room, TANK *tank);
void shut_down_turrets_when_tank_dead(MAP *room, TANK *tank);
void boss_room_management(BITMAP *hud, MAP *room, TANK *tank, BOSS *boss, int nSalle, int *cleBoss);


#endif // TURRET_FUNCTIONS_H_INCLUDED
