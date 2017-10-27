#include "Load_Functions.h"

BITMAP * load_bitmap_shielded(char *fileName)
{
    // Déclaration des ressources
    BITMAP *loadedBitmap;
    // Chargement de la bitmap
    loadedBitmap = load_bitmap(fileName, NULL);
    // Blindage
    if (!loadedBitmap)
    {
        allegro_message("An error occured during a bitmap load.\n\nProgram closing.");
        allegro_exit();
        exit(EXIT_FAILURE);
    }
    // Fin du sous-programme
    return loadedBitmap;
}

void multiple_static_sprite_load_division(char *fileName, BITMAP *sprites[], int numberSprites, int x_sprite, int y_sprite)
{
    // Déclaration des ressources
    int i;
    BITMAP *bitmap;
    // Chargement de la bitmap
    bitmap = load_bitmap_shielded(fileName);
    // Blindage des dimensions des sprites
    if (bitmap->h < y_sprite || bitmap->w < x_sprite*numberSprites)
    {
        allegro_message("An error occured during a bitmap division.\n\nProgram closing.");
        allegro_exit();
        exit(EXIT_FAILURE);
    }
    // Division de la bitmap
    for (i = 0; i < numberSprites; i++)
    {
        // Création des sprites
        sprites[i] = create_bitmap_shielded(x_sprite, y_sprite);
        // Remplissage des sprites
        blit(bitmap, sprites[i], i*x_sprite, 0, 0, 0, x_sprite, y_sprite);
    }
    // Fin du sous-programme
    destroy_bitmap(bitmap);
}

MAP * map_load(char *fileName)
{
    // Déclaration des ressources
    MAP *room;
    FILE *f_map;
    int x, y, z;
    // Ouverture du fichier de la map
    f_map = fopen (fileName, "r");
    // Blindage
    if (f_map == NULL)
    {
        allegro_message("An error occured during a map load.\n\nProgram closing.");
        allegro_exit();
        exit(EXIT_FAILURE);
    }

    // Lire dimensions de la map
    room = (MAP *) malloc(sizeof(MAP));
    fscanf(f_map, "%d %d %d", &room->x, &room->y, &room->z);
    // Allouer la map
    // Allocation du sol
    room->ground = (int **) malloc(room->y * sizeof(int *));
    if (room->ground == NULL)
    {
        allegro_message("An error occured during memory allocation.\n\nProgram closing.");
        allegro_exit();
        exit(EXIT_FAILURE);
    }
    for (y = 0; y < room->y; y++)
    {
        room->ground[y] = (int *) malloc(room->x * sizeof(int));
        if (room->ground[y] == NULL)
        {
            allegro_message("An error occured during memory allocation.\n\nProgram closing.");
            allegro_exit();
            exit(EXIT_FAILURE);
        }
    }
    // Allocation du mur nord
    room->northWall = (int **) malloc(room->x * sizeof(int *));
    if (room->northWall == NULL)
    {
        allegro_message("An error occured during memory allocation.\n\nProgram closing.");
        allegro_exit();
        exit(EXIT_FAILURE);
    }
    for (x = 0; x < room->x; x++)
    {
        room->northWall[x] = (int *) malloc(room->z * sizeof(int));
        if (room->northWall[x] == NULL)
        {
            allegro_message("An error occured during memory allocation.\n\nProgram closing.");
            allegro_exit();
            exit(EXIT_FAILURE);
        }
    }
    // Allocation du mur ouest
    room->westWall = (int **) malloc(room->y * sizeof(int *));
    if (room->westWall == NULL)
    {
        allegro_message("An error occured during memory allocation.\n\nProgram closing.");
        allegro_exit();
        exit(EXIT_FAILURE);
    }
    for (y = 0; y < room->y; y++)
    {
        room->westWall[y] = (int *) malloc(room->z * sizeof(int));
        if (room->westWall[y] == NULL)
        {
            allegro_message("An error occured during memory allocation.\n\nProgram closing.");
            allegro_exit();
            exit(EXIT_FAILURE);
        }
    }
    // Allocation des ascenseurs
    room -> elevators = (ELEVATOR ***) malloc(room -> y * sizeof(ELEVATOR **));
    if (room->elevators == NULL)
    {
        allegro_message("An error occured during memory allocation.\n\nProgram closing.");
        allegro_exit();
        exit(EXIT_FAILURE);
    }
    for (y = 0; y < room->y; y++)
    {
        room->elevators[y] = (ELEVATOR **) malloc(room -> x * sizeof(ELEVATOR *));
        if (room->elevators[y] == NULL)
        {
            allegro_message("An error occured during memory allocation.\n\nProgram closing.");
            allegro_exit();
            exit(EXIT_FAILURE);
        }
    }
    // Allocation des tourelles
    room -> turrets = (TURRET ***) malloc(room -> y * sizeof(TURRET **));
    if (room->turrets == NULL)
    {
        allegro_message("An error occured during memory allocation.\n\nProgram closing.");
        allegro_exit();
        exit(EXIT_FAILURE);
    }
    for (y = 0; y < room->y; y++)
    {
        room->turrets[y] = (TURRET **) malloc(room -> x * sizeof(TURRET *));
        if (room->turrets[y] == NULL)
        {
            allegro_message("An error occured during memory allocation.\n\nProgram closing.");
            allegro_exit();
            exit(EXIT_FAILURE);
        }
    }
    // Allocation des lasers
    room -> lasers = (LASER ***) malloc(room -> y * sizeof(LASER **));
    if (room->lasers == NULL)
    {
        allegro_message("An error occured during memory allocation.\n\nProgram closing.");
        allegro_exit();
        exit(EXIT_FAILURE);
    }
    for (y = 0; y < room->y; y++)
    {
        room->lasers[y] = (LASER **) malloc(room -> x * sizeof(LASER *));
        if (room->lasers[y] == NULL)
        {
            allegro_message("An error occured during memory allocation.\n\nProgram closing.");
            allegro_exit();
            exit(EXIT_FAILURE);
        }
    }

    // Lire la map
    /* Dalles
    NSWE -SWE N-WE NS-E NSW- --WE -S-E -SW- N--E N-W- NS-- ---E --W- -S-- N--- ----
    0000 0001 0002 0003 0004 0005 0006 0007 0008 0009 0010 0011 0012 0013 0014 0015
    Ascenseurs
    RED  GREE BLUP BLDW STAT
    0016 0017 0018 0019 0020
    Tourelles
    ST-N ST-S ST-E ST-W DY-N DY-S DY-E DY-W
    0021 0022 0023 0024 0025 0026 0027 0028
    */


    // Lire le sol
    for (y = 0; y < room->y; y++)
    {
        for (x = 0; x < room->x; x++)
        {
            // Code du block
            fscanf(f_map, "%d", &room->ground[y][x]);
            // Ascenseurs
            if (room -> ground[y][x] >= 16 && room -> ground[y][x] <= 20)
            {
                room->elevators[y][x] = (ELEVATOR *) malloc(sizeof(ELEVATOR));
                if (room->elevators[y][x] == NULL)
                {
                    allegro_message("An error occured during memory allocation.\n\nProgram closing.");
                    allegro_exit();
                    exit(EXIT_FAILURE);
                }
                room->elevators[y][x]->x = 20*(x+1);
                room->elevators[y][x]->y = 20*(y+1);
                switch(room->ground[y][x])
                {
                    case 16:
                        room->elevators[y][x]->z = 0;
                        room->elevators[y][x]->up = 0;
                        room->elevators[y][x]->electrical = 0;
                        room->elevators[y][x]->on = 1;
                        room->elevators[y][x]->zMin = 0;
                        room->elevators[y][x]->zMax = 20;
                        room->elevators[y][x]->delaiMax = 25;
                        room->elevators[y][x]->delai = 0;
                        break;
                    case 17:
                        room->elevators[y][x]->z = 20;
                        room->elevators[y][x]->up = 1;
                        room->elevators[y][x]->electrical = 0;
                        room->elevators[y][x]->on = 1;
                        room->elevators[y][x]->zMin = 0;
                        room->elevators[y][x]->zMax = 20;
                        room->elevators[y][x]->delaiMax = 25;
                        room->elevators[y][x]->delai = 0;
                        break;
                    case 18:
                        room->elevators[y][x]->z = 50;
                        room->elevators[y][x]->up = 0;
                        room->elevators[y][x]->electrical = 0;
                        room->elevators[y][x]->on = 1;
                        room->elevators[y][x]->zMin = 0;
                        room->elevators[y][x]->zMax = 50;
                        room->elevators[y][x]->delaiMax = 50;
                        room->elevators[y][x]->delai = 50;
                        break;
                    case 19:
                        room->elevators[y][x]->z = -30;
                        room->elevators[y][x]->up = 1;
                        room->elevators[y][x]->electrical = 0;
                        room->elevators[y][x]->on = 1;
                        room->elevators[y][x]->zMin = -30;
                        room->elevators[y][x]->zMax = 0;
                        room->elevators[y][x]->delaiMax = 50;
                        room->elevators[y][x]->delai = 50;
                        break;
                    case 20:
                        room->elevators[y][x]->z = 20;
                        room->elevators[y][x]->up = 1;
                        room->elevators[y][x]->electrical = 0;
                        room->elevators[y][x]->on = 0;
                        room->elevators[y][x]->zMin = 0;
                        room->elevators[y][x]->zMax = 20;
                        room->elevators[y][x]->delaiMax = 25;
                        room->elevators[y][x]->delai = 0;
                }
            }
            else
            {
                room->elevators[y][x] = NULL;
            }
            // Tourelles
            if (room -> ground[y][x] >= 21 && room -> ground[y][x] <= 28)
            {
                room->turrets[y][x] = (TURRET *) malloc(sizeof(TURRET));
                if (room->turrets[y][x] == NULL)
                {
                    allegro_message("An error occured during memory allocation.\n\nProgram closing.");
                    allegro_exit();
                    exit(EXIT_FAILURE);
                }
                room -> turrets[y][x] -> x = 20*(x+1);
                room -> turrets[y][x] -> y = 20*(y+1);
                switch(room -> ground[y][x])
                {
                    case 21:
                        room -> turrets[y][x] -> direction = 0;
                        room -> turrets[y][x] -> on = 1;
                        room -> turrets[y][x] -> canMove = 0;
                        room -> turrets[y][x] -> canRotate = 0;
                        room -> turrets[y][x] -> z = 0;
                        room -> turrets[y][x] -> zMin = 0;
                        room -> turrets[y][x] -> zMax = 20;
                        room -> turrets[y][x] -> delai = 100;
                        room -> turrets[y][x] -> delaiMax = 100;
                        break;
                    case 22:
                        room -> turrets[y][x] -> direction = 1;
                        room -> turrets[y][x] -> on = 1;
                        room -> turrets[y][x] -> canMove = 0;
                        room -> turrets[y][x] -> canRotate = 0;
                        room -> turrets[y][x] -> z = 0;
                        room -> turrets[y][x] -> zMin = 0;
                        room -> turrets[y][x] -> zMax = 20;
                        room -> turrets[y][x] -> delai = 100;
                        room -> turrets[y][x] -> delaiMax = 100;
                        break;
                    case 23:
                        room -> turrets[y][x] -> direction = 2;
                        room -> turrets[y][x] -> on = 1;
                        room -> turrets[y][x] -> canMove = 0;
                        room -> turrets[y][x] -> canRotate = 0;
                        room -> turrets[y][x] -> z = 0;
                        room -> turrets[y][x] -> zMin = 0;
                        room -> turrets[y][x] -> zMax = 20;
                        room -> turrets[y][x] -> delai = 100;
                        room -> turrets[y][x] -> delaiMax = 100;
                        break;
                    case 24:
                        room -> turrets[y][x] -> direction = 3;
                        room -> turrets[y][x] -> on = 1;
                        room -> turrets[y][x] -> canMove = 0;
                        room -> turrets[y][x] -> canRotate = 0;
                        room -> turrets[y][x] -> z = 0;
                        room -> turrets[y][x] -> zMin = 0;
                        room -> turrets[y][x] -> zMax = 20;
                        room -> turrets[y][x] -> delai = 100;
                        room -> turrets[y][x] -> delaiMax = 100;
                        break;
                    case 25:
                        room -> turrets[y][x] -> direction = 0;
                        room -> turrets[y][x] -> on = 1;
                        room -> turrets[y][x] -> canMove = 0;
                        room -> turrets[y][x] -> canRotate = 1;
                        room -> turrets[y][x] -> z = 0;
                        room -> turrets[y][x] -> zMin = 0;
                        room -> turrets[y][x] -> zMax = 20;
                        room -> turrets[y][x] -> delai = 100;
                        room -> turrets[y][x] -> delaiMax = 100;
                        break;
                    case 26:
                        room -> turrets[y][x] -> direction = 1;
                        room -> turrets[y][x] -> on = 1;
                        room -> turrets[y][x] -> canMove = 0;
                        room -> turrets[y][x] -> canRotate = 1;
                        room -> turrets[y][x] -> z = 0;
                        room -> turrets[y][x] -> zMin = 0;
                        room -> turrets[y][x] -> zMax = 20;
                        room -> turrets[y][x] -> delai = 100;
                        room -> turrets[y][x] -> delaiMax = 100;
                        break;
                    case 27:
                        room -> turrets[y][x] -> direction = 2;
                        room -> turrets[y][x] -> on = 1;
                        room -> turrets[y][x] -> canMove = 0;
                        room -> turrets[y][x] -> canRotate = 1;
                        room -> turrets[y][x] -> z = 0;
                        room -> turrets[y][x] -> zMin = 0;
                        room -> turrets[y][x] -> zMax = 20;
                        room -> turrets[y][x] -> delai = 100;
                        room -> turrets[y][x] -> delaiMax = 100;
                        break;
                    case 28:
                        room -> turrets[y][x] -> direction = 3;
                        room -> turrets[y][x] -> on = 1;
                        room -> turrets[y][x] -> canMove = 0;
                        room -> turrets[y][x] -> canRotate = 1;
                        room -> turrets[y][x] -> z = 0;
                        room -> turrets[y][x] -> zMin = 0;
                        room -> turrets[y][x] -> zMax = 20;
                        room -> turrets[y][x] -> delai = 100;
                        room -> turrets[y][x] -> delaiMax = 100;
                        break;
                }
            }
            else
            {
                room -> turrets[y][x] = NULL;
            }
            // Lasers
            room -> lasers[y][x] = NULL;
        }
    }
    // Lire le mur nord
    for (y = 0; y < room->y; y++)
    {
        for (z = 0; z < room->z; z++)
        {
            fscanf(f_map, "%d", &room->northWall[y][z]);
        }
    }
    // Lire le mur ouest
    for (x = 0; x < room->x; x++)
    {
        for (z = 0; z < room->z; z++)
        {
            fscanf(f_map, "%d", &room->westWall[x][z]);
        }
    }


    // Fermeture du fichier de la map
    fclose(f_map);
    // Fin du sous-programme
    return room;
}

TANK * tank_load()
{
    // Déclaration des ressources
    TANK *tank;
    // Allouer tank
    tank = (TANK *) malloc(sizeof(TANK));
    if (tank == NULL)
    {
        allegro_message("An error occured during memory allocation.\n\nProgram closing.");
        allegro_exit();
        exit(EXIT_FAILURE);
    }
    // Allouer sprites du tank
    tank->nbSprites = 4;
    tank->sprites = (BITMAP **) malloc(tank->nbSprites * sizeof(BITMAP *));
    if (tank->sprites == NULL)
    {
        allegro_message("An error occured during memory allocation.\n\nProgram closing.");
        allegro_exit();
        exit(EXIT_FAILURE);
    }
    // Charger sprites du tank
    multiple_static_sprite_load_division("Tank.bmp", tank->sprites, tank->nbSprites, 80, 44);
    // Fin du sous-programme
    return tank;
}

void init_tank(TANK *tank, MAP *room, int salleActuelle, int sallePrecedente)
{
    // Déclaration des ressources
    int x, y, r;

    tank -> direction = -1;
    tank -> alive = 1;
    // Sur quel ascenseur le tank devra-t-il apparaître
    if (salleActuelle >= sallePrecedente)
    {
        r = 19;
    }
    else
    {
        r = 18;
    }
    // Parcours des cases
    for (y = 0; y < room -> y; y++)
    {
        for (x = 0; x < room -> x; x++)
        {
            // Trouver la case où sera le tank au départ
            if (room -> ground[y][x] == r)
            {
                tank -> x = 20*(x+1);
                tank -> y = 20*(y+1);
                tank -> z = room -> elevators[y][x] -> z;
                tank -> direction = 0;
                tank -> userControlsMove = 0;
                tank -> userControlsRotate = 1;
                tank -> vx = 0;
                tank -> vy = 0;
                tank -> vz = 0;
            }
        }
    }

    // Fin du sous-programme
}

BITMAP * create_bitmap_shielded(int w, int h)
{
    // Déclaration des ressources
    BITMAP *createdBitmap;
    // Création de la bitmap
    createdBitmap = create_bitmap(w, h);
    // Blindage
    if (!createdBitmap)
    {
        allegro_message("An error occured during a bitmap creation.\n\nProgram closing.");
        allegro_exit();
        exit(EXIT_FAILURE);
    }
    // Fin du sous-programme
    return createdBitmap;
}

BOSS * init_boss()
{
    // Déclaration des ressources
    BOSS *boss;
    // Allocation mémoire
    boss = (BOSS *) malloc(sizeof(BOSS));
    if (boss == NULL)
    {
        allegro_message("An error occured during memory allocation.\n\nProgram closing.");
        allegro_exit();
        exit(EXIT_FAILURE);
    }
    // Initialisation
    boss -> nRound = 0;
    boss -> counter = 0;
    // Fin du sous-programme
    return boss;
}

