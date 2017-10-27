#ifndef STRUCTURES_H_INCLUDED
#define STRUCTURES_H_INCLUDED

#include "Structures.h"

typedef struct elevator
{
    int x;
    int y;
    int z;
    int electrical;
    int on;
    int up;
    int delai;
    int delaiMax;
    int zMax;
    int zMin;
} ELEVATOR;

typedef struct turret
{
    int x;
    int y;
    int z;
    int zMin;
    int zMax;
    int direction;
    int on;
    int canMove;
    int canRotate;
    int delai;
    int delaiMax;
} TURRET;

typedef struct laser
{
    int x;
    int y;
    int z;
    int direction;
    int delai;
    int delaiMax;
} LASER;

typedef struct map
{
    int x;
    int y;
    int z;
    int ** ground;
    int ** northWall;
    int ** westWall;
    ELEVATOR *** elevators;
    TURRET *** turrets;
    LASER *** lasers;
} MAP;

typedef struct tank
{
    int userControlsMove;
    int userControlsRotate;
    int x;
    int y;
    int z;
    int vx;
    int vy;
    int vz;
    int direction;
    int nbSprites;
    BITMAP ** sprites;
    int alive;
} TANK;

typedef struct boss
{
    int nRound;
    int counter;
} BOSS;

#endif // STRUCTURES_H_INCLUDED
