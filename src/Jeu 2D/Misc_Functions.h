#ifndef MISC_FUNCTIONS_H_INCLUDED
#define MISC_FUNCTIONS_H_INCLUDED

#include <allegro.h>
#include "Structures.h"
#include "Misc_Functions.h"

void load_achievements(BITMAP *tab[4]);
void draw_achievement(BITMAP *renderScreen, BITMAP *tab[4], int x, int y, int nSprite);
void draw_message(BITMAP *renderScreen, const char *phrase, int x, int y);
void draw_messages_matching_room (BITMAP *renderScreen, int nSalle, int *cleBoss);


#endif // MISC_FUNCTIONS_H_INCLUDED
