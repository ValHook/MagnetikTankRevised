#include <stdio.h>
#include <stdlib.h>
#include <allegro.h>
#include "Structures.h"
#include "Laser_Functions.h"

void create_laser(MAP *room, int x, int y, int direction)
{
    // Allocation mémoire
    room -> lasers[y][x] = (LASER *) malloc(sizeof(LASER));
    // Initialisation
    room -> lasers[y][x] -> x = 20*(x+1);
    room -> lasers[y][x] -> y = 20*(y+1);
    room -> lasers[y][x] -> z = 20;
    room -> lasers[y][x] -> direction = direction;
    room -> lasers[y][x] -> delai = 10;
    room -> lasers[y][x] -> delaiMax = 10;
}

void make_lasers_expire(MAP *room)
{
    // Déclaration des ressources
    int x, y;
    // Parcours des lasers pour les faire périmer
    for (y = 0; y < room -> y; y++)
    {
        for (x = 0; x < room -> x; x++)
        {
            if (room -> lasers[y][x] != NULL)
            {
                if (room -> lasers[y][x] -> delai == 0)
                {
                    free(room -> lasers[y][x]);
                    room -> lasers[y][x] = NULL;
                }
                else
                {
                    room -> lasers[y][x] -> delai--;
                }
            }
        }
    }
}


