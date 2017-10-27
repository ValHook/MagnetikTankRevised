#ifndef LASER_FUNCTIONS_H_INCLUDED
#define LASER_FUNCTIONS_H_INCLUDED

#include "Structures.h"
#include "Laser_Functions.h"

void create_laser(MAP *room, int x, int y, int direction);
void make_lasers_expire(MAP *room);

#endif // LASER_FUNCTIONS_H_INCLUDED
