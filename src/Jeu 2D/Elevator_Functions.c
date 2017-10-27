#include "Elevator_Functions.h"


void activate_pressure_elevators(MAP *room, TANK *tank)
{
    // Déclaration des ressources
    int x, y;
    ELEVATOR * elevator;

    // Activer les ascenseurs pression
    for (y = 0; y < room -> y; y++)
    {
        for (x = 0; x < room -> x; x++)
        {
            // Trouver les cases qui contiennent des ascenseurs
            elevator = room -> elevators[y][x];
            if (elevator != NULL)
            {
                // Si ascenseur pression
                if (elevator->electrical == 0 && elevator->on == 1)
                {
                    // Si délai d'attente écoulé
                    if (elevator -> delai == 0)
                    {
                        // Si tank immobile sur l'ascenseur
                        if (tank->x == elevator->x && tank->y == elevator->y && tank->vx == 0 && tank->vy == 0)
                        {
                            // Activer l'ascenseur
                            elevator->up = 1 - elevator->up;
                            tank->userControlsMove = 0;
                            elevator->delai = elevator->delaiMax;
                        }
                    }
                }
            }
        }
    }
    // Fin du sous-programme
}

void calculate_elevator_height(MAP *room)
{
    // Déclaration des ressources
    int x, y;
    ELEVATOR *elevator;

    // Régler la hauteur des ascenseurs
    for (y = 0; y < room->y; y++)
    {
        for (x = 0; x < room->x; x++)
        {
            // Trouver les cases qui contiennent des ascenseurs
            elevator = room -> elevators[y][x];
            if (elevator != NULL)
            {
                // Monter ou descendre ceux qu'il faut
                if (elevator->up == 0 && elevator->z > elevator->zMin)
                {
                    elevator->z--;
                }
                else if (elevator->up == 1 && elevator->z < elevator->zMax)
                {
                    elevator->z++;
                }
                // Décrémenter le délai d'attente si ascenseur pression immobile
                else if (elevator->delai > 0 && elevator->electrical == 0)
                {
                    elevator->delai--;
                }
            }
        }
    }
}

void calculate_tank_height_matching_elevator(MAP *room, TANK *tank)
{
    // Déclaration des ressources
    int x, y;
    ELEVATOR * elevator;

    // Régler la hauteur du tank
    for (y = 0; y < room->y; y++)
    {
        for (x = 0; x < room->x; x++)
        {
            // Trouver les cases qui contiennent des ascenseurs
            elevator = room->elevators[y][x];
            if (elevator != NULL)
            {
                // Si tank sur ascenseur
                if (tank->x == elevator->x && tank->y == elevator->y)
                {
                    // Régler hauteur du tank
                    tank->z = elevator->z;
                    // Si ascenseur arrivé
                    if (elevator->z == elevator->zMin || elevator->z == elevator->zMax)
                    {
                        // Libérer le tank
                        tank->userControlsMove=1;
                    }
                }
            }
        }
    }
}

void init_entrance_exit_elevators(MAP *room, int salleActuelle, int sallePrecedente)
{
    // Déclaration des ressources
    int x, y;
    ELEVATOR *elevator;

    // Initialiser les ascenseurs d'entrée et sortie de salle
    for (y = 0; y < room -> y; y++)
    {
        for (x = 0; x < room -> x; x++)
        {
            // Ascenseur vers la salle du dessus
            if (room -> ground[y][x] == 18)
            {
                elevator = room -> elevators[y][x];
                if (sallePrecedente < salleActuelle)
                {
                    elevator -> z = elevator -> zMin;
                }
            }
            // Ascenseur vers la salle du dessous
            else if (room -> ground[y][x] == 19)
            {
                elevator = room -> elevators[y][x];
                if (sallePrecedente > salleActuelle)
                {
                    elevator -> z = elevator -> zMax;
                }
            }

        }
    }
}


