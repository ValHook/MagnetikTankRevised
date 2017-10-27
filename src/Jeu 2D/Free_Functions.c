#include <stdio.h>
#include <stdlib.h>
#include <allegro.h>
#include "Structures.h"
#include "Free_Functions.h"

void free_tank(TANK *tank)
{
    // Déclaration des ressources
    int i;
    // Libération des bitmaps
    for (i = 0; i < tank -> nbSprites; i++)
    {
        destroy_bitmap(tank -> sprites[i]);
    }
    // Libération du tank
    free(tank);
    // Fin du sous-programme
}

void free_map(MAP *map)
{
    int x, y;
    // Libération du mur nord
    for (x = 0; x < map->x; x++)
    {
        free(map->northWall[x]);
    }
    free(map->northWall);
    // Libération du mur ouest
    for (y = 0; y < map->y; y++)
    {
        free(map->westWall[y]);
    }
    free(map->westWall);
    // Libération du sol
    for (y = 0; y < map->y; y++)
    {
        free(map->ground[y]);
    }
    free(map->ground);
    // Libération des ascenseurs et tourelles
    for (y = 0; y < map->y; y++)
    {
        for (x = 0; x < map->x; x++)
        {
            if (map->elevators[y][x] != NULL)
            {
                free(map->elevators[y][x]);
            }
            if (map->turrets[y][x] != NULL)
            {
                free(map->turrets[y][x]);
            }
        }
        free(map->elevators[y]);
        free(map->turrets[y]);
    }
    free(map->elevators);
    free(map->turrets);
    // Fin du sous-programme
}



