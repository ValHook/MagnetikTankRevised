#include <stdio.h>
#include <stdlib.h>
#include <allegro.h>
#include "Structures.h"
#include "Transition_Functions.h"
#include "Misc_Functions.h"

void darken_buffer_matching_tank_height(BITMAP *buffer, TANK *tank)
{
    // Déclaration des ressources
    int x, y;
    float f;
    int col;
    // Si tank très haut
    if (tank->z > 20 && tank -> alive == 1)
    {
        f = (50-tank->z)/30.0;
        for (x = 0; x < buffer->w; x++)
        {
            for (y = 0; y < buffer->h; y++)
            {
                col = getpixel(buffer, x, y);
                col = makecol(getr(col)*f, getg(col)*f, getb(col)*f);
                putpixel(buffer, x, y, col);
            }
        }
    }
    // Si tank très bas
    else if (tank->z < 0 && tank -> alive == 1)
    {
        f = (30+tank->z)/30.0;
        for (x = 0; x < buffer->w; x++)
        {
            for (y = 0; y < buffer->h; y++)
            {
                col = getpixel(buffer, x, y);
                col = makecol(getr(col)*f, getg(col)*f, getb(col)*f);
                putpixel(buffer, x, y, col);
            }
        }
    }
}

void change_room_matching_tank_height(int *prochaineSalle, TANK *tank)
{
    // Si tank à hauteur maximale
    if (tank->z == 50 && tank -> alive == 1)
    {
        (*prochaineSalle)++;
    }
    // Si tank à hauteur minimale
    else if (tank->z == -30 && tank -> alive == 1)
    {
        (*prochaineSalle)--;
    }
}

void game_over_animation(BITMAP *buffer, TANK *tank, int beenDeadFor, int *salleSuivante)
{
    // Déclaration des ressources
    int x, y;

    if (beenDeadFor <= 150)
    {
        // Fondue en rouge
        for (x = 0; x < 80; x++)
        {
            for (y = 0; y < 60; y++)
            {
                if (x + y <= beenDeadFor)
                {
                    circlefill(buffer, 10*x, 10*y, beenDeadFor-x-y, makecol(255*beenDeadFor/150, 0, 0));
                }
            }
        }
    }
    else if (beenDeadFor <= 300)
    {
        // Fondue en noir
        clear_to_color(buffer, makecol(255*(300-beenDeadFor)/150, 0, 0));
    }
    else
    {
        clear_to_color(buffer, makecol(0, 0, 0));
        // Recommencer le jeu
        *salleSuivante = -1;
    }
}

void game_won_animation(BITMAP *buffer, TANK *tank, int hasWonFor, int *salleSuivante, BITMAP *tab[4])
{
    // Déclaration des ressources
    int x, y;

    if (hasWonFor <= 150)
    {
        // Fondue en rouge
        for (x = 0; x < 80; x++)
        {
            for (y = 0; y < 60; y++)
            {
                if (x + y <= hasWonFor)
                {
                    circlefill(buffer, 10*x, 10*y, hasWonFor-x-y, makecol(0, 255*hasWonFor/150, 255*hasWonFor/150));
                }
            }
        }
    }
    else if (hasWonFor <= 300)
    {
        // Fondue en noir
        clear_to_color(buffer, makecol(255*(hasWonFor-150)/150, 255, 255));
    }
    else
    {
        clear_to_color(buffer, makecol(255, 255, 255));
        draw_achievement(buffer, tab, 300, 100, 0);
        draw_achievement(buffer, tab, 300, 200, 1);
        draw_achievement(buffer, tab, 300, 300, 2);
        blit(buffer, screen, 0, 0, 0, 0, 800, 600);

        // Recommencer le jeu
        while (!key[KEY_SPACE]);
        *salleSuivante = -1;
    }

}

