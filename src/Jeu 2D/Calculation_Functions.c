#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "Calculation_Functions.h"


void calculate_tank_speed_matching_commands(TANK *tank)
{
    if (tank -> userControlsMove == 1)
    {
        // Si flèche du haut appuyée
        if (key[KEY_UP])
        {
            // Décrémenter vitesse verticale
            tank -> vy -= 2;
        }
        // Si flèche du bas appuyée
        if (key[KEY_DOWN])
        {
            // Incrémenter vitesse verticale
            tank -> vy += 2;
        }
        // Si flèche de gauche appuyée
        if (key[KEY_LEFT])
        {
            // Décrémenter vitesse horizontale
            tank -> vx -= 2;
        }
        // Si flèche de droite appuyée
        if (key[KEY_RIGHT])
        {
            // Incrémenter vitesse horizontale
            tank -> vx += 2;
        }
    }
    // Fin du sous-programme
}

void calculate_tank_position_matching_speed(TANK *tank)
{
    tank -> x += tank -> vx;
    tank -> y += tank -> vy;
    tank -> z += tank -> vz;
}

void calculate_tank_direction_matching_commands(TANK *tank)
{
    if (tank -> userControlsRotate == 1)
    {
        // Si flèche du haut appuyée
        if (key[KEY_UP])
        {
            // Tourner tank vers le Nord
            tank -> direction = 0;
        }
        // Si flèche du bas appuyée
        if (key[KEY_DOWN])
        {
            // Tourner tank vers le Sud
            tank -> direction = 2;
        }
        // Si flèche de gauche appuyée
        if (key[KEY_LEFT])
        {
            // Tourner tank vers l'Ouest
            tank -> direction = 3;
        }
        // Si flèche de droite appuyée
        if (key[KEY_RIGHT])
        {
            // Tourner tank vers l'Est
            tank -> direction = 1;
        }
    }
    // Fin du sous-programme
}

void calculate_tank_direction_matching_speed(TANK *tank)
{
    // Si tank immobile
    if (tank -> vx == 0 && tank -> vy == 0)
    {
        // Ne pas changer orientation
        tank -> direction = tank -> direction;
    }
    // Si tank se déplace verticalement
    else if (tank -> vx == 0)
    {
        // Si vitesse verticale positive
        if (tank -> vy > 0)
        {
            // Orienter tank vers le Sud
            tank -> direction = 2;
        }
        // Si vitesse verticale négative
        else
        {
            // Orienter tank vers le Nors
            tank -> direction = 0;
        }
    }
    // Si tank se déplace horizontalement
    else if (tank -> vy == 0)
    {
        // Si vitesse horizontale positive
        if (tank -> vx > 0)
        {
            // Orienter tank vers l'Est
            tank -> direction = 1;
        }
        // Si vitesse horizontale négative
        else
        {
            // Orienter tank vers l'Ouest
            tank -> direction = 3;
        }
    }
    // Si vitesse horizontale plus importante que vitesse verticale
    else if (tank -> vy / tank -> vx > -1 && tank -> vy / tank -> vx < 1)
    {
        // Si vitesse horizontale positive
        if (tank -> vx > 0)
        {
            // Orienter tank vers l'Est
            tank -> direction = 1;
        }
        // Si vitesse horizontale négative
        else
        {
            // Orienter tank vers l'Ouest
            tank -> direction = 3;
        }
    }
    // Si vitesse verticale plus importante que vitesse horizontale
    else if (tank -> vx / tank -> vy > -1 && tank -> vx / tank -> vy < 1)
    {
        // Si vitesse verticale positive
        if (tank -> vy > 0)
        {
            // Orienter tank vers le Sud
            tank -> direction = 2;
        }
        // Si vitesse verticale négative
        else
        {
            // Orienter tank vers le Nord
            tank -> direction = 0;
        }
    }
    // Si vitesse verticale de même ordre que vitesse horizontale
    else if (tank -> vx == tank -> vy || tank -> vx == - tank -> vy)
    {
        // Ne pas changer orientation
        tank -> direction = tank -> direction;
    }
    // Fin du sous-programme
}

void calculate_tank_speed_matching_friction(TANK *tank)
{
    tank -> vx *= 8;
    tank -> vx /= 10;
    tank -> vy *= 8;
    tank -> vy /= 10;
}

void calculate_tank_speed_matching_magnetism(TANK *tank)
{
    // Magnétisme horizontale
    if (tank -> vx == 0 && tank -> x % 10 != 0) tank -> vx += ((tank->x)%20-10)/(abs((tank->x)%20-10));
    if (tank -> vx == 0 && abs(tank -> x % 20) == 10) tank -> vx -= 1;
    // Magnétisme verticale
    if (tank -> vy == 0 && tank -> y % 10 != 0) tank -> vy += ((tank->y)%20-10)/(abs((tank->y)%20-10));
    if (tank -> vy == 0 && abs(tank -> y % 20) == 10) tank -> vy -= 1;
    // Fin du sous-programme
}

void calculate_tank_speed_matching_collision(TANK *tank, MAP *room)
{
    // Déclaration des ressources
    int xc, yc, xc1, xc2, yc1, yc2, zc1, zc2;
    int i, d, xb = 0, yb = 0;

    // Collision avec des murs
    // North wall
    if (tank->y + tank->vy < 18)
    {
        tank -> vy = 18 - tank -> y;
    }
    // West wall
    if (tank->x + tank->vx < 18)
    {
        tank -> vx = 18 - tank -> x;
    }
    // South wall
    if (tank->y + tank->vy > 20*room->y+2)
    {
        tank -> vy = 20 * room->y - tank -> y + 2;
    }
    // East wall
    if (tank->x + tank->vx > 20*room->x+2)
    {
        tank -> vx = 20 * room->x - tank -> x + 2;
    }

    // Collision avec des obstacles
    // Collision vers l'Est
    xc = 0; yc = 0; d = 0;
    if (tank->vx > 0 && !(tank->x + tank->vx == 20*room->x+2))
    {
        if (tank->y % 20 == 2)
        {
            d = -1;
        }
        xc = (tank->x-10+8)/20;
        xc1 = (tank->x+tank->vx-10+8)/20;
        xc2 = xc1;
        yc1 = (tank->y-10+8+d)/20;
        yc2 = (tank->y-10-8+d)/20;
        if (yc1 == 8)
        {
            yc1 = 7;
        }
        if (room->elevators[yc1][xc1] == NULL && room->turrets[yc1][xc1] == NULL)
        {
            zc1 = 0;
        }
        else if (room->turrets[yc1][xc1] == NULL)
        {
            zc1 = room->elevators[yc1][xc1] -> z;
        }
        else
        {
            zc1 = room->turrets[yc1][xc1] -> z;
        }
        if (room->elevators[yc2][xc2] == NULL && room->turrets[yc2][xc2] == NULL)
        {
            zc2 = 0;
        }
        else if (room->turrets[yc2][xc2] == NULL)
        {
            zc2 = room->elevators[yc2][xc2] -> z;
        }
        else
        {
            zc2 = room->turrets[yc2][xc2] -> z;
        }

        i = 0;
        while ((tank->x + 8 + i) % 20 != 10)
        {
            i++;
        }
        xb = tank->x + 8 + i;
        if (tank->z != zc1 || tank->z != zc2)
        {
            tank -> vx = xb - tank->x - 8;
        }
    }
    // Collision vers l'Ouest
    xc = 0; yc = 0; d = 0;
    if (tank->vx < 0 && !(tank->x == 18))
    {
        if (tank->y % 20 == 2)
        {
            d = -1;
        }
        xc = (tank->x-10-8)/20;
        xc1 = (tank->x+tank->vx-10-8)/20;
        xc2 = xc1;
        yc1 = (tank->y-10+8+d)/20;
        yc2 = (tank->y-10-8+d)/20;
        if (yc1 == 8)
        {
            yc1 = 7;
        }
        if (room->elevators[yc1][xc1] == NULL && room->turrets[yc1][xc1] == NULL)
        {
            zc1 = 0;
        }
        else if (room->turrets[yc1][xc1] == NULL)
        {
            zc1 = room->elevators[yc1][xc1] -> z;
        }
        else
        {
            zc1 = room->turrets[yc1][xc1] -> z;
        }
        if (room->elevators[yc2][xc2] == NULL && room->turrets[yc2][xc2] == NULL)
        {
            zc2 = 0;
        }
        else if (room->turrets[yc2][xc2] == NULL)
        {
            zc2 = room->elevators[yc2][xc2] -> z;
        }
        else
        {
            zc2 = room->turrets[yc2][xc1] -> z;
        }

        i = 0;
        while ((tank->x - 8 - i) % 20 != 10)
        {
            i++;
        }
        xb = tank->x - 8 - i;
        if (tank->z != zc1 || tank->z != zc2)
        {
            tank -> vx = xb - tank->x + 8;
        }
    }
    // Collision vers le Sud
    xc = 0; yc = 0; d = 0;
    if (tank->vy > 0 && !(tank->y + tank->vy == 20*room->y+2))
    {
        if (tank->x % 20 == 2)
        {
            d = -1;
        }
        yc = (tank->y-10+8)/20;
        yc1 = (tank->y+tank->vy-10+8)/20;
        yc2 = yc1;
        xc1 = (tank->x-10+8+d)/20;
        xc2 = (tank->x-10-8+d)/20;
        if (xc1 == 8)
        {
            xc1 = 7;
        }
        if (room->elevators[yc1][xc1] == NULL && room->turrets[yc1][xc1] == NULL)
        {
            zc1 = 0;
        }
        else if (room->turrets[yc1][xc1] == NULL)
        {
            zc1 = room->elevators[yc1][xc1] -> z;
        }
        else
        {
            zc1 = room->turrets[yc1][xc1] -> z;
        }
        if (room->elevators[yc2][xc2] == NULL && room->turrets[yc2][xc2] == NULL)
        {
            zc2 = 0;
        }
        else if (room->turrets[yc2][xc2] == NULL)
        {
            zc2 = room->elevators[yc2][xc2] -> z;
        }
        else
        {
            zc2 = room->turrets[yc2][xc2] -> z;
        }

        i = 0;
        while ((tank->y + 8 + i) % 20 != 10)
        {
            i++;
        }
        yb = tank->y + 8 + i;
        if (tank->z != zc1 || tank->z != zc2)
        {
            tank -> vy = yb - tank->y - 8;
        }
    }
    // Collision vers le Nord
    xc = 0; yc = 0; d = 0;
    if (tank->vy < 0 && !(tank->y == 18))
    {
        if (tank->x % 20 == 2)
        {
            d = -1;
        }
        yc = (tank->y-10-8)/20;
        yc1 = (tank->y+tank->vy-10-8)/20;
        yc2 = yc1;
        xc1 = (tank->x-10+8+d)/20;
        xc2 = (tank->x-10-8+d)/20;
        if (xc1 == 8)
        {
            xc1 = 7;
        }
        if (room->elevators[yc1][xc1] == NULL && room->turrets[yc1][xc1] == NULL)
        {
            zc1 = 0;
        }
        else if (room->turrets[yc1][xc1] == NULL)
        {
            zc1 = room->elevators[yc1][xc1] -> z;
        }
        else
        {
            zc1 = room->turrets[yc1][xc1] -> z;
        }
        if (room->elevators[yc2][xc2] == NULL && room->turrets[yc2][xc2] == NULL)
        {
            zc2 = 0;
        }
        else if (room->turrets[yc2][xc2] == NULL)
        {
            zc2 = room->elevators[yc2][xc2] -> z;
        }
        else
        {
            zc2 = room->turrets[yc2][xc2] -> z;
        }

        i = 0;
        while ((tank->y - 8 - i) % 20 != 10)
        {
            i++;
        }
        yb = tank->y - 8 - i;
        if (tank->z != zc1 || tank->z != zc2)
        {
            tank -> vy = yb - tank->y + 8;
        }
    }

    // Fin du sous-programme
}




