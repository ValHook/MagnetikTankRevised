#include <stdio.h>
#include <stdlib.h>
#include <allegro.h>
#include "Structures.h"
#include "Laser_Functions.h"
#include "Turret_Functions.h"
#include "Misc_Functions.h"

void deactivate_pressure_turrets(MAP *room, TANK *tank)
{
    // Déclaration des ressources
    int x, y;
    TURRET *turret;

    // Désactiver les tourelles si le joueur passe dessus
    for (y = 0; y < room -> y; y++)
    {
        for (x = 0; x < room -> x; x++)
        {
            turret = room->turrets[y][x];
            if (turret != NULL)
            {
                // Si tourelle activée
                if (turret -> on == 1)
                {
                    // Si tank immobile sur tourelle
                    if (tank->x == turret->x && tank->vx == 0 && tank->y == turret->y && tank->vy == 0)
                    {
                        turret -> on = 0;
                        tank -> userControlsMove = 0;
                    }
                }
            }
        }
    }
}

void calculate_turret_height(MAP *room)
{
    // Déclaration des ressources
    int x, y;
    TURRET *turret;

    // Monter ou descendre les tourelles qu'il faut
    for (y = 0; y < room -> y; y++)
    {
        for (x = 0; x < room -> x; x++)
        {
            // Trouver les cases où il y a une tourelle
            turret = room->turrets[y][x];
            if (turret != NULL)
            {
                // Monter ou descendre ce qu'il faut
                if (turret -> on == 1 && turret -> z < turret -> zMax)
                {
                    turret -> z++;
                }
                else if (turret -> on == 0 && turret -> z > turret -> zMin)
                {
                    turret -> z--;
                }
            }
        }
    }
}

void calculate_tank_height_matching_turret(MAP *room, TANK *tank)
{
    // Déclaration des ressources
    int x, y;
    TURRET *turret;

    // Régler la hauteur du tank
    for (y = 0; y < room->y; y++)
    {
        for (x = 0; x < room->x; x++)
        {
            // Trouver les cases qui contiennent des ascenseurs
            turret = room->turrets[y][x];
            if (turret != NULL)
            {
                // Si tank sur ascenseur
                if (tank->x == turret->x && tank->y == turret->y)
                {
                    // Régler hauteur du tank
                    tank->z = turret->z;
                    // Si ascenseur arrivé
                    if (turret->z == turret->zMin)
                    {
                        // Libérer le tank
                        tank->userControlsMove = 1;
                    }
                }
            }
        }
    }
}

void rotate_turrets(MAP *room)
{
    // Déclaration des ressources
    int x, y;
    TURRET *turret;

    // Tourner les tourelles qu'il faut
    for (y = 0; y < room -> y; y++)
    {
        for (x = 0; x < room -> x; x++)
        {
            // Trouver les cases qui contiennent des tourelles
            turret = room -> turrets[y][x];
            if (turret != NULL)
            {
                // Si tourelle rotative
                if (turret -> canRotate == 1)
                {
                    // Si tourelle allumée
                    if (turret -> on == 1)
                    {
                        // Si délai écoulé
                        if (turret -> delai == 0)
                        {
                            turret -> direction = (turret -> direction + 1) % 4;
                            turret -> delai = turret -> delaiMax;
                        }
                        else
                        {
                            turret -> delai--;
                        }
                    }
                }
            }
        }
    }
}

void shoot_lasers_on_tank(MAP *room, TANK *tank)
{
    // Déclaration des ressources
    int xc, yc;
    int x, y;
    int x2, y2;
    int peutVoirTank;
    int obstacle;
    int stop;
    TURRET *turret;

    // Calculer la case du tank
    xc = (tank -> x - 10) / 20;
    yc = (tank -> y - 10) / 20;
    // On parcours les cases
    for (y = 0; y < room -> y; y++)
    {
        for (x = 0; x < room -> x; x++)
        {
            // Si la case est une tourelle
            turret = room -> turrets[y][x];
            if (turret != NULL)
            {
                // Si la tourelle est allumée et entièrement sortie du sol et le tank est au sol et vivant
                if (turret -> on == 1 && turret -> z == turret -> zMax && tank -> z == 0 && tank -> alive == 1)
                {
                    // En fonction de l'orientation de la tourelle
                    switch (turret -> direction)
                    {
                        // Si la tourelle est tournée vers le nord
                        case 0:
                            // Si la tourelle est tournée vers le tank
                            if (x == xc && y > yc)
                            {
                                // Poser les coordonnées à parcourir
                                x2 = xc; y2 = y - 1;
                                // Parcourir les cases entre la tourelle et le tank
                                stop = 0; obstacle = 0; peutVoirTank = 0;
                                while (stop == 0)
                                {
                                    // Si on sort de la map
                                    if (y < 0)
                                    {
                                        stop = 1;
                                    }
                                    // Si on rentre dans un obstacle
                                    else if ((room -> elevators[y2][x2] != NULL && room -> elevators[y2][x2] -> z > 0)
                                             || (room -> turrets[y2][x2] != NULL && room -> turrets[y2][x2] -> z > 0))
                                    {
                                        obstacle = 1;
                                        stop = 1;
                                    }
                                    // Si on rentre dans le tank
                                    else if (x2 == xc && y2 == yc)
                                    {
                                        obstacle = 0;
                                        peutVoirTank = 1;
                                        stop = 1;
                                    }
                                    // Passer à la case suivante
                                    y2--;
                                }
                                // Si la tourelle peut voir le tank
                                if (peutVoirTank == 1)
                                {
                                    // Poser les coordonnées à parcourir
                                    x2 = xc; y2 = y - 1;
                                    // Parcourir les cases entre la tourelle et le tank
                                    stop = 0; obstacle = 0; peutVoirTank = 0;
                                    while (stop == 0)
                                    {
                                        // Si on rentre dans le tank
                                        if (x2 == xc && y2 == yc)
                                        {
                                            stop = 1;
                                        }
                                        // Créer un laser
                                        create_laser(room, x2, y2, turret->direction);
                                        // Passer à la case suivante
                                        y2--;
                                    }
                                    // Tuer le tank
                                    tank -> vx = 0;
                                    tank -> vy = 0;
                                    tank -> alive = 0;
                                    tank -> userControlsMove = 0;
                                    tank -> userControlsRotate = 0;
                                }

                            }

                            break;
                        // Si la tourelle est tournée vers l'est
                        case 1:
                            // Si la tourelle est tournée vers le tank
                            if (x < xc && y == yc)
                            {
                                // Poser les coordonnées à parcourir
                                x2 = x + 1; y2 = yc;
                                // Parcourir les cases entre la tourelle et le tank
                                stop = 0; obstacle = 0; peutVoirTank = 0;
                                while (stop == 0)
                                {
                                    // Si on sort de la map
                                    if (x >= room -> x)
                                    {
                                        stop = 1;
                                    }
                                    // Si on rentre dans un obstacle
                                    else if ((room -> elevators[y2][x2] != NULL && room -> elevators[y2][x2] -> z > 0)
                                             || (room -> turrets[y2][x2] != NULL && room -> turrets[y2][x2] -> z > 0))
                                    {
                                        obstacle = 1;
                                        stop = 1;
                                    }
                                    // Si on rentre dans le tank
                                    else if (x2 == xc && y2 == yc)
                                    {
                                        obstacle = 0;
                                        peutVoirTank = 1;
                                        stop = 1;
                                    }
                                    // Passer à la case suivante
                                    x2++;
                                }
                                // Si la tourelle peut voir le tank
                                if (peutVoirTank == 1)
                                {
                                    // Poser les coordonnées à parcourir
                                    x2 = x + 1; y2 = yc;
                                    // Parcourir les cases entre la tourelle et le tank
                                    stop = 0; obstacle = 0; peutVoirTank = 0;
                                    while (stop == 0)
                                    {
                                        // Si on rentre dans le tank
                                        if (x2 == xc && y2 == yc)
                                        {
                                            stop = 1;
                                        }
                                        // Créer un laser
                                        create_laser(room, x2, y2, turret->direction);
                                        // Passer à la case suivante
                                        x2++;
                                    }
                                    // Tuer le tank
                                    tank -> vx = 0;
                                    tank -> vy = 0;
                                    tank -> alive = 0;
                                    tank -> userControlsMove = 0;
                                    tank -> userControlsRotate = 0;
                                }

                            }

                            break;
                        // Si la tourelle est tournée vers le sud
                        case 2:
                            // Si la tourelle est tournée vers le tank
                            if (x == xc && y < yc)
                            {
                                // Poser les coordonnées à parcourir
                                x2 = xc; y2 = y + 1;
                                // Parcourir les cases entre la tourelle et le tank
                                stop = 0; obstacle = 0; peutVoirTank = 0;
                                while (stop == 0)
                                {
                                    // Si on sort de la map
                                    if (y >= room -> y)
                                    {
                                        stop = 1;
                                    }
                                    // Si on rentre dans un obstacle
                                    else if ((room -> elevators[y2][x2] != NULL && room -> elevators[y2][x2] -> z > 0)
                                             || (room -> turrets[y2][x2] != NULL && room -> turrets[y2][x2] -> z > 0))
                                    {
                                        obstacle = 1;
                                        stop = 1;
                                    }
                                    // Si on rentre dans le tank
                                    else if (x2 == xc && y2 == yc)
                                    {
                                        obstacle = 0;
                                        peutVoirTank = 1;
                                        stop = 1;
                                    }
                                    // Passer à la case suivante
                                    y2++;
                                }
                                // Si la tourelle peut voir le tank
                                if (peutVoirTank == 1)
                                {
                                    // Poser les coordonnées à parcourir
                                    x2 = xc; y2 = y + 1;
                                    // Parcourir les cases entre la tourelle et le tank
                                    stop = 0; obstacle = 0; peutVoirTank = 0;
                                    while (stop == 0)
                                    {
                                        // Si on rentre dans le tank
                                        if (x2 == xc && y2 == yc)
                                        {
                                            stop = 1;
                                        }
                                        // Créer un laser
                                        create_laser(room, x2, y2, turret->direction);
                                        // Passer à la case suivante
                                        y2++;
                                    }
                                    // Tuer le tank
                                    tank -> vx = 0;
                                    tank -> vy = 0;
                                    tank -> alive = 0;
                                    tank -> userControlsMove = 0;
                                    tank -> userControlsRotate = 0;
                                }

                            }

                            break;
                        // Si la tourelle est tournée vers l'ouest
                        case 3:
                            // Si la tourelle est tournée vers le tank
                            if (x > xc && y == yc)
                            {
                                // Poser les coordonnées à parcourir
                                x2 = x - 1; y2 = yc;
                                // Parcourir les cases entre la tourelle et le tank
                                stop = 0; obstacle = 0; peutVoirTank = 0;
                                while (stop == 0)
                                {
                                    // Si on sort de la map
                                    if (x < 0)
                                    {
                                        stop = 1;
                                    }
                                    // Si on rentre dans un obstacle
                                    else if ((room -> elevators[y2][x2] != NULL && room -> elevators[y2][x2] -> z > 0)
                                             || (room -> turrets[y2][x2] != NULL && room -> turrets[y2][x2] -> z > 0))
                                    {
                                        obstacle = 1;
                                        stop = 1;
                                    }
                                    // Si on rentre dans le tank
                                    else if (x2 == xc && y2 == yc)
                                    {
                                        obstacle = 0;
                                        peutVoirTank = 1;
                                        stop = 1;
                                    }
                                    // Passer à la case suivante
                                    x2--;
                                }
                                // Si la tourelle peut voir le tank
                                if (peutVoirTank == 1)
                                {
                                    // Poser les coordonnées à parcourir
                                    x2 = x - 1; y2 = yc;
                                    // Parcourir les cases entre la tourelle et le tank
                                    stop = 0; obstacle = 0; peutVoirTank = 0;
                                    while (stop == 0)
                                    {
                                        // Si on rentre dans le tank
                                        if (x2 == xc && y2 == yc)
                                        {
                                            stop = 1;
                                        }
                                        // Créer un laser
                                        create_laser(room, x2, y2, turret->direction);
                                        // Passer à la case suivante
                                        x2--;
                                    }
                                    // Tuer le tank
                                    tank -> vx = 0;
                                    tank -> vy = 0;
                                    tank -> alive = 0;
                                    tank -> userControlsMove = 0;
                                    tank -> userControlsRotate = 0;
                                }

                            }

                            break;

                    }
                }
            }
        }
    }

}

void shut_down_turrets_when_tank_dead(MAP *room, TANK *tank)
{
    // Déclaration des ressources
    int x, y;
    // Si tank mort
    if (tank -> alive == 0)
    {
        // Eteindre toutes les tourelles
        for (y = 0; y < room -> y; y++)
        {
            for (x = 0; x < room -> x; x++)
            {
                if (room -> turrets[y][x] != NULL)
                {
                    room -> turrets[y][x] -> on = 0;
                }
            }
        }
    }
}

void boss_room_management(BITMAP *hud, MAP *room, TANK *tank, BOSS *boss, int nSalle, int *cleBoss)
{
    // Déclaration des ressources
    int x, y;
    int resteTourelle;

    // Cheat
    if (key[KEY_O]&&key[KEY_P]) *cleBoss = 1;

    // Dans la salle 01
    if (nSalle == 1)
    {
        // Si pas dévérouillé la salle du boss
        if (*cleBoss == 0)
        {
            // Parcourir les cases
            for (y = 0; y < room -> y; y++)
            {
                for (x = 0; x < room -> x; x++)
                {
                    // Si il y a un ascenseur
                    if (room -> elevators[y][x] != NULL)
                    {
                        // Si ascenseur en train de descendre vers la salle du boss
                        if (room -> elevators[y][x] -> z < -1 && room -> elevators[y][x] -> up == 0)
                        {
                            // Le faire remonter
                            room -> elevators[y][x] -> up = 1;
                        }
                    }
                }
            }
        }
    }

    // Dans la salle 05
    if (nSalle == 5)
    {
        resteTourelle = 0;
        // Parcourir les cases
        for (y = 0; y < room -> y; y++)
        {
            for (x = 0; x < room -> x; x++)
            {
                // Si tourelle
                if (room -> turrets[y][x] != NULL)
                {
                    // Voir si dans le sol
                    resteTourelle += room -> turrets[y][x] -> z;
                }
            }
        }
        if (resteTourelle == 0)
        {
            *cleBoss = 1;
        }
    }

    // Dans la salle 00
    if (nSalle == 0)
    {
        switch (boss -> nRound)
        {
            case 0:
                // Parcourir les cases
                for (y = 0; y < room -> y; y++)
                {
                    for (x = 0; x < room -> x; x++)
                    {
                        // Si ascenseur qui ne transporte pas le tank
                        if (room -> elevators[y][x] != NULL && room -> elevators[y][x] -> z != tank -> z)
                        {
                            room -> elevators[y][x] -> up = 0;
                            room -> elevators[y][x] -> z = 0;
                        }
                        // Si tourelle
                        if (room -> turrets[y][x] != NULL)
                        {
                            room -> turrets[y][x] -> on = 0;
                            room -> turrets[y][x] -> z = 0;
                        }
                    }
                }
                boss ->nRound++;
                break;

            case 1:
                if (tank -> z == 0)
                {
                    boss -> nRound++;
                }
                break;

            case 2:
                tank -> userControlsMove = 0;
                tank -> userControlsRotate = 0;
                boss -> nRound++;
                break;

            case 3:
                tank -> direction = 2;
                tank -> y += 2;
                if (tank -> y == 140)
                {
                    boss -> nRound++;
                }
                break;

            case 4:
                room -> elevators[2][2] -> on = 1; room -> elevators[2][2] -> up = 1;
                room -> elevators[3][2] -> on = 1; room -> elevators[3][2] -> up = 1;
                room -> elevators[4][2] -> on = 1; room -> elevators[4][2] -> up = 1;
                room -> elevators[2][3] -> on = 1; room -> elevators[2][3] -> up = 1;
                room -> elevators[4][3] -> on = 1; room -> elevators[4][3] -> up = 1;
                room -> elevators[2][4] -> on = 1; room -> elevators[2][4] -> up = 1;
                room -> elevators[3][4] -> on = 1; room -> elevators[3][4] -> up = 1;
                room -> elevators[4][4] -> on = 1; room -> elevators[4][4] -> up = 1;
                if (room -> elevators[2][2] -> z == 20)
                {
                    boss -> nRound++;
                }
                break;

            case 5:
                tank -> direction = 0;
                room -> elevators[2][2] -> on = 0; room -> elevators[2][2] -> up = 0;
                room -> elevators[3][2] -> on = 0; room -> elevators[3][2] -> up = 0;
                room -> elevators[4][2] -> on = 0; room -> elevators[4][2] -> up = 0;
                room -> elevators[2][3] -> on = 0; room -> elevators[2][3] -> up = 0;
                room -> elevators[4][3] -> on = 0; room -> elevators[4][3] -> up = 0;
                room -> elevators[2][4] -> on = 0; room -> elevators[2][4] -> up = 0;
                room -> elevators[3][4] -> on = 0; room -> elevators[3][4] -> up = 0;
                room -> elevators[4][4] -> on = 0; room -> elevators[4][4] -> up = 0;
                room -> elevators[3][3] -> z = -20;
                if (room -> elevators[2][2] -> z == 0)
                {
                    boss -> nRound++;
                }
                break;

            case 6:
                room -> elevators[6][3] -> on = 1; room -> elevators[6][3] -> up = 1;
                draw_message(hud, "HA! JE TE TIENS!", 50, 500);
                if (room -> elevators[6][3] -> z == 20)
                {
                    boss -> nRound++;
                }
                break;

            case 7:
                tank -> userControlsMove = 0;
                tank -> userControlsRotate = 0;
                room -> elevators[6][3] -> on = 0;
                room -> turrets[5][3] -> on = 1;
                room -> turrets[6][4] -> on = 1;
                room -> turrets[7][3] -> on = 1;
                room -> turrets[6][2] -> on = 1;
                draw_message(hud, "HA! JE TE TIENS!", 50, 500);
                draw_message(hud, "TU ES FICHU!", 50, 520);
                if (room -> turrets[5][3] -> z == 15)
                {
                    boss -> nRound++;
                }
                break;

            case 8:
                if (boss -> counter % 3 == 0)
                {
                    room -> turrets[5][3] -> on = 1 - room -> turrets[5][3] -> on;
                    room -> turrets[6][4] -> on = 1 - room -> turrets[6][4] -> on;
                    room -> turrets[7][3] -> on = 1 - room -> turrets[7][3] -> on;
                    room -> turrets[6][2] -> on = 1 - room -> turrets[6][2] -> on;
                }
                draw_message(hud, "QUE- ???", 50, 500);
                boss -> counter++;
                if (boss -> counter == 31)
                {
                    boss -> counter = 0;
                    boss -> nRound++;
                }
                break;

            case 9:
                room -> turrets[5][3] -> on = 0;
                room -> turrets[6][4] -> on = 0;
                room -> turrets[7][3] -> on = 0;
                room -> turrets[6][2] -> on = 0;
                room -> elevators[6][3] -> up = 0; room -> elevators[6][3] -> on = 1;
                draw_message(hud, "QUE- ???", 50, 500);
                draw_message(hud, "NON!", 50, 520);
                if (room -> elevators[6][3] -> z == 0)
                {
                    boss -> nRound++;
                }
                break;

            case 10:
                tank -> userControlsMove = 1;
                tank -> userControlsRotate = 1;
                room -> elevators[3][3] -> up = 1;
                draw_message(hud, "C'EST IMPOSSIBLE!", 50, 500);
                if (room -> elevators[3][3] -> z == 0)
                {
                    boss -> nRound++;
                }
                break;

            case 11:
                room -> elevators[3][3] -> up = 0;
                if (boss -> counter == 100)
                {
                    for (y = 0; y < room -> y; y++)
                    {
                        room -> turrets[y][0] -> on = 1;
                        room -> turrets[y][7] -> on = 1;
                    }
                }
                draw_message(hud, "C'EST IMPOSSIBLE!", 50, 500);
                draw_message(hud, "TU NE T'ECHAPPERAS PAS!", 50, 520);
                boss -> counter++;
                if (tank -> z > 0)
                {
                    boss -> counter = 0;
                    boss -> nRound++;
                }

            case 12:
                draw_message(hud, "...", 50, 500);
                room -> elevators[3][3] -> zMax = 49;
                tank -> alive = 2;



        }
    }


}






