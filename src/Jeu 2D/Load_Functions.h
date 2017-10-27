#ifndef LOAD_FUNCTIONS_H_INCLUDED
#define LOAD_FUNCTIONS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <allegro.h>
#include "Structures.h"
#include "Load_Functions.h"

BITMAP * load_bitmap_shielded(char *fileName);
void multiple_static_sprite_load_division(char *fileName, BITMAP *sprites[], int numberSprites, int x_sprite, int y_sprite);
MAP * map_load(char *fileName);
TANK * tank_load();
void init_tank(TANK *tank, MAP *room, int salleActuelle, int sallePrecedente);
BITMAP * create_bitmap_shielded(int w, int h);
BOSS * init_boss();

#endif // LOAD_FUNCTIONS_H_INCLUDED
