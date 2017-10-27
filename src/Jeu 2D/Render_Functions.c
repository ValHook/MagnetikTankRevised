#include "Render_Functions.h"

void render_background(BITMAP *background, BITMAP *renderScreen)
{
    // Blindage des dimensions du fond
    if (background->w != renderScreen->w || background->h != renderScreen->h)
    {
        allegro_message("An error occured during the background render.\n\nProgram closing.");
        allegro_exit();
        exit(EXIT_FAILURE);
    }
    // Faire un rendu du fond d'écran
    blit(background, renderScreen, 0, 0, 0, 0, background->w, background->h);
    // Fin du sous-programme
}

void render_flat_map(BITMAP *renderScreen, BITMAP *flatMapSprites[], int x_origin, int y_origin, MAP *room, TANK *tank)
{
    // Déclaration des ressources
    int x, y;
    // Faire un rendu du sol plat
    for (y = 0; y < room->y*20; y++)
    {
        for (x = 0; x < room->x*20; x++)
        {
            if (x % 20 == 0 && y % 20 == 0)
            {
                if (room->ground[y/20][x/20] < 16)
                {
                    draw_sprite(renderScreen, flatMapSprites[room->ground[y/20][x/20]], 2*x-2*y-flatMapSprites[0]->w/2+x_origin, x+y+flatMapSprites[0]->h/2+y_origin);
                }
                else if (room->ground[y/20][x/20] < 20)
                {
                    render_elevator(renderScreen, flatMapSprites, x/20, y/20, x_origin, y_origin, room);
                }
            }
            if (tank->x == x && tank->y == y)
            {
                render_tank(renderScreen, tank, x_origin, y_origin+20);
            }
        }
    }
}

void render_walls(BITMAP *renderScreen, BITMAP *wallSprites[], int x_origin, int y_origin, MAP *room)
{
    // Déclaration des ressources
    int x, y, z;
    // Faire un rendu des briques complémentaires
    x = -1;
    y = -1;
    for (z = 0; z <= room->z; z++)
    {
        render_wall_block(renderScreen, wallSprites, x_origin, y_origin, room, x, y, z);
    }
    y = -1;
    z = 0;
    for (x = 0; x < room->x; x++)
    {
        render_wall_block(renderScreen, wallSprites, x_origin, y_origin, room, x, y, z);
    }
    x = -1;
    z = 0;
    for (y = 0; y < room->y; y++)
    {
        render_wall_block(renderScreen, wallSprites, x_origin, y_origin, room, x, y, z);
    }
    // Faire un rendu du mur nord
    y = -1;
    for (x = 0; x < room->x; x++)
    {
        for (z = 1; z <= room->z; z++)
        {
            render_wall_block(renderScreen, wallSprites, x_origin, y_origin, room, x, y, z);
        }
    }
    // Faire un rendu du mur ouest
    x = -1;
    for (y = 0; y < room->y; y++)
    {
        for (z = 1; z <= room->z; z++)
        {
            render_wall_block(renderScreen, wallSprites, x_origin, y_origin, room, x, y, z);
        }
    }
    // Fin du sous-programme
}

void render_tank(BITMAP *renderScreen, TANK *tank, int x_origin, int y_origin)
{
    // Dessiner le tank
    draw_sprite(renderScreen, tank->sprites[tank->direction], 2*tank->x-2*tank->y+x_origin-tank->sprites[0]->w/2, tank->x+tank->y+y_origin-2*tank->z-tank->sprites[0]->h/2-68);
    // Fin du sous-programme
}

void render_elevator(BITMAP *renderScreen, BITMAP *sprites[], int x, int y, int x_origin, int y_origin, MAP *room)
{
    // Déclaration des ressources
    int spr;
    ELEVATOR * elevator;

    y_origin -= 40;
    elevator = room->elevators[y][x];
    // Déterminer le sprite à utiliser
    if (elevator->up == 0)
    {
        spr = 16;
    }
    else
    {
        spr = 17;
    }
    // Faire un rendu de l'ascenseur
    draw_sprite(renderScreen, sprites[spr], 2*elevator->x-2*elevator->y-sprites[0]->w/2+x_origin, elevator->x+elevator->y-2*elevator->z+sprites[0]->h/2+y_origin);
    // Fin du sous-programme
}

void render_wall_block(BITMAP *renderScreen, BITMAP *wallSprites[], int x_origin, int y_origin, MAP *room, int x, int y, int z)
{
    // Déclaration des ressources
    int nSprite = 0;
    // Déterminer quel sprite utiliser
    if (z == 0 || (x == -1 && y == -1))
    {
        nSprite = 0;
    }
    else if (x == -1)
    {
        nSprite = room -> westWall[y][z-1];
    }
    else if (y == -1)
    {
        nSprite = room -> northWall[x][z-1];
    }
    // Dessiner bloc
    draw_sprite(renderScreen, wallSprites[nSprite], 20*2*x-20*2*y-wallSprites[0]->w/2+x_origin, 20*x+20*y-20*2*z+wallSprites[0]->h/2+y_origin);
}




void render_blocks_tank(BITMAP *renderScreen, BITMAP *blockSprites[], TANK *tank, MAP *room, int x_origin, int y_origin)
{
    // Déclaration des ressources
    int x, y, z;
    int xc, yc;
    int a, b;
    // Calculer dans quelle case se trouve le coin Sud-Est du tank
    xc = (tank->x-10+8-1)/20;
    yc = (tank->y-10+8-1)/20;
    // Afficher les bases des murs
    x = -1; y = -1;

    for (z = 0; z <= room->z; z++)
    {
        render_block(renderScreen, blockSprites[15], x*20, y*20, z*20, x_origin, y_origin);
    }
    x = -1; z = 0;
    for (y = 0; y < room->y; y++)
    {
        render_block(renderScreen, blockSprites[15], x*20, y*20, z*20, x_origin, y_origin);
    }
    y = -1; z = 0;
    for (x = 0; x < room->x; x++)
    {
        render_block(renderScreen, blockSprites[15], x*20, y*20, z*20, x_origin, y_origin);
    }
    // Afficher le mur nord
    y = -1;
    for (x = 0; x < room->x; x++)
    {
        for (z = 0; z < room -> z; z++)
        {
            render_block(renderScreen, blockSprites[room->northWall[x][z]], 20*x, 20*y, 20*(z+1), x_origin, y_origin);
            if (x == xc && y == yc)
            {
                render_tank(renderScreen, tank, x_origin, y_origin);
            }
        }
    }
    // Afficher le mur ouest
    x = -1;
    for (y = 0; y < room->y; y++)
    {
        for (z = 0; z < room -> z; z++)
        {
            render_block(renderScreen, blockSprites[room->northWall[y][z]], 20*x, 20*y, 20*(z+1), x_origin, y_origin);
            if (x == xc && y == yc)
            {
                render_tank(renderScreen, tank, x_origin, y_origin);
            }
        }
    }
    // Affichage grosse case par grosse case (8*8)
    z = 0;
    for (a = 0; a < room->x; a++)
    {
        for (b = 0; b < a+1; b++)
        {
            x = b; y = a - b;
            // Dessiner les blocs neutres
            if (room -> ground[y][x] < 16)
            {
                render_block(renderScreen, blockSprites[room->ground[y][x]], 20*x, 20*y, 20*z, x_origin, y_origin);
            }
            // Dessiner les ascenseurs pressions
            else if (room -> ground[y][x] < 18)
            {
                render_block(renderScreen, blockSprites[16 + room->elevators[y][x]->up], 20*x, 20*y, room->elevators[y][x]->z, x_origin, y_origin);
            }
            // Dessiner l'ascenseur de montée
            else if (room -> ground[y][x] < 21)
            {
                render_block(renderScreen, blockSprites[room->ground[y][x]], 20*x, 20*y, room->elevators[y][x]->z, x_origin, y_origin);
            }
            // Dessiner les tourelles fixes et rotatives
            else if (room -> ground[y][x] < 29)
            {
                render_block(renderScreen, blockSprites[21+room->turrets[y][x]->direction], 20*x, 20*y, room->turrets[y][x]->z, x_origin, y_origin);
            }
            // Dessiner les lasers
            if (room -> lasers[y][x] != NULL)
            {
                render_block(renderScreen, blockSprites[25+room->lasers[y][x]->direction], 20*x, 20*y, room->lasers[y][x]->z, x_origin, y_origin);
            }
            // Dessiner le tank
            if (x == xc && y == yc)
            {
                render_tank(renderScreen, tank, x_origin, y_origin);
                if (xc > 0 && yc < room->y - 1)
                {
                    if (room -> ground[yc+1][xc-1] < 16)
                    {
                        render_block(renderScreen, blockSprites[room->ground[yc+1][xc-1]], 20*(xc-1), 20*(yc+1), 20*z, x_origin, y_origin);
                    }
                    else if (room -> ground[yc+1][xc-1] < 18)
                    {
                        render_block(renderScreen, blockSprites[16 + room->elevators[yc+1][xc-1]->up], 20*(xc-1), 20*(yc+1), room->elevators[yc+1][xc-1]->z, x_origin, y_origin);
                    }
                    else if (room -> ground[yc+1][xc-1] < 21)
                    {
                        render_block(renderScreen, blockSprites[room->ground[yc+1][xc-1]], 20*(xc-1), 20*(yc+1), room->elevators[yc+1][xc-1]->z, x_origin, y_origin);
                    }
                    else if (room -> ground[yc+1][xc-1] < 29)
                    {
                        render_block(renderScreen, blockSprites[21+room->turrets[yc+1][xc-1]->direction], 20*(xc-1), 20*(yc+1), room->turrets[yc+1][xc-1]->z, x_origin, y_origin);
                    }
                }
            }
        }
    }
    for (a = 1; a < room->x; a++)
    {
        for (b = room->x-1; b > a-1; b--)
        {
            x = a - b + 7; y = b;
            // Dessiner les blocs neutres
            if (room -> ground[y][x] < 16)
            {
                render_block(renderScreen, blockSprites[room->ground[y][x]], 20*x, 20*y, 20*z, x_origin, y_origin);
            }
            // Dessiner les ascenseurs pressions
            else if (room -> ground[y][x] < 18)
            {
                render_block(renderScreen, blockSprites[16 + room->elevators[y][x]->up], 20*x, 20*y, room->elevators[y][x]->z, x_origin, y_origin);
            }
            // Dessiner l'ascenseur de montée
            else if (room -> ground[y][x] < 21)
            {
                render_block(renderScreen, blockSprites[room->ground[y][x]], 20*x, 20*y, room->elevators[y][x]->z, x_origin, y_origin);
            }
            // Dessiner les tourelles fixes et rotatives
            else if (room -> ground[y][x] < 29)
            {
                render_block(renderScreen, blockSprites[21+room->turrets[y][x]->direction], 20*x, 20*y, room->turrets[y][x]->z, x_origin, y_origin);
            }
            // Dessiner les lasers
            if (room -> lasers[y][x] != NULL)
            {
                render_block(renderScreen, blockSprites[25+room->lasers[y][x]->direction], 20*x, 20*y, room->lasers[y][x]->z, x_origin, y_origin);
            }
            // Dessiner le tank
            if (x == xc && y == yc)
            {
                render_tank(renderScreen, tank, x_origin, y_origin);
                if (xc > 0 && yc < room->y - 1)
                {
                    if (room -> ground[yc+1][xc-1] < 16)
                    {
                        render_block(renderScreen, blockSprites[room->ground[yc+1][xc-1]], 20*(xc-1), 20*(yc+1), 20*z, x_origin, y_origin);
                    }
                    else if (room -> ground[yc+1][xc-1] < 18)
                    {
                        render_block(renderScreen, blockSprites[16 + room->elevators[yc+1][xc-1]->up], 20*(xc-1), 20*(yc+1), room->elevators[yc+1][xc-1]->z, x_origin, y_origin);
                    }
                    else if (room -> ground[yc+1][xc-1] < 21)
                    {
                        render_block(renderScreen, blockSprites[room->ground[yc+1][xc-1]], 20*(xc-1), 20*(yc+1), room->elevators[yc+1][xc-1]->z, x_origin, y_origin);
                    }
                    else if (room -> ground[yc+1][xc-1] < 29)
                    {
                        render_block(renderScreen, blockSprites[21+room->turrets[yc+1][xc-1]->direction], 20*(xc-1), 20*(yc+1), room->turrets[yc+1][xc-1]->z, x_origin, y_origin);
                    }
                }
            }
        }
    }
}

void render_block(BITMAP *renderScreen, BITMAP *sprite, int x, int y, int z, int x_origin, int y_origin)
{
    // Déclaration des ressources

    // Dessiner le bloc
    draw_sprite(renderScreen, sprite, 2*x-2*y-sprite->w/2+x_origin, x+y-2*z-sprite->h/2+y_origin);
    // Fin du sous-programme
}






