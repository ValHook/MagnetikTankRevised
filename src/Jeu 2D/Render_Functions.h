#ifndef RENDER_FUNCTIONS_H_INCLUDED
#define RENDER_FUNCTIONS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <allegro.h>
#include "Structures.h"
#include "Render_Functions.h"

void render_background(BITMAP *background, BITMAP *renderScreen);
void render_flat_map(BITMAP *renderScreen, BITMAP *flatMapSprites[], int x_origin, int y_origin, MAP *room, TANK *tank);
void render_walls(BITMAP *renderScreen, BITMAP *wallSprites[], int x_origin, int y_origin, MAP *room);
void render_tank(BITMAP *renderScreen, TANK *tank, int x_origin, int y_origin);
void render_elevator(BITMAP *renderScreen, BITMAP *sprites[], int x, int y, int x_origin, int y_origin, MAP *room);
void render_wall_block(BITMAP *renderScreen, BITMAP *wallSprites[], int x_origin, int y_origin, MAP *room, int x, int y, int z);
void render_blocks_tank(BITMAP *renderScreen, BITMAP *blockSprites[], TANK *tank, MAP *room, int x_origin, int y_origin);
void render_block(BITMAP *renderScreen, BITMAP *sprite, int x, int y, int z, int x_origin, int y_origin);

#endif // RENDER_FUNCTIONS_H_INCLUDED
