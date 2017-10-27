#include <allegro.h>
#include "Structures.h"
#include "Load_Functions.h"
#include "Misc_Functions.h"

void load_achievements(BITMAP *tab[4])
{
    // Chargement des images
    tab[0] = load_bitmap_shielded("I'm on a block!.bmp");
    tab[1] = load_bitmap_shielded("I'm on a turret!.bmp");
    tab[2] = load_bitmap_shielded("MagnetiK TanK Achievement.bmp");
    tab[3] = NULL;
    // Fin du sous-programme
}

void draw_achievement(BITMAP *renderScreen, BITMAP *tab[4], int x, int y, int nSprite)
{
    draw_sprite(renderScreen, tab[nSprite], x, y);
}

void draw_message(BITMAP *renderScreen, const char *phrase, int x, int y)
{
    textout_ex(renderScreen, font, phrase, x, y, makecol(0, 0, 0), -1);
}

void draw_messages_matching_room (BITMAP *renderScreen, int nSalle, int *cleBoss)
{
    switch(nSalle)
    {
        case 1:
            draw_message(renderScreen, "Bienvenue!", 30, 500);
            draw_message(renderScreen, "Deplacez-vous avec les fleches.", 30, 530);
            draw_message(renderScreen, "Votre tank est influence par", 30, 540);
            draw_message(renderScreen, "la friction avec le sol et par", 30, 550);
            draw_message(renderScreen, "l'attraction magnetique avec les cases.", 30, 560);
            break;
        case 2:
            draw_message(renderScreen, "Il existe plusieurs ascenseurs.", 30, 530);
            draw_message(renderScreen, "Arretez-vous sur un ascenseur", 30, 550);
            draw_message(renderScreen, "pour l'activer / le desactiver.", 30, 560);
            break;
        case 3:
            draw_message(renderScreen, "Faites attention aux tourelles!", 30, 30);
            draw_message(renderScreen, "Si jamais elles vous voient,", 30, 50);
            draw_message(renderScreen, "elles vous tireront dessus.", 30, 60);
            draw_message(renderScreen, "Essayez pour voir!", 30, 80);
            break;
        case 4:
            draw_message(renderScreen, "Faites attention aux tourelles!", 30, 40);
            draw_message(renderScreen, "Certaines tourelles peuvent", 30, 60);
            draw_message(renderScreen, "tourner sur elles-memes.", 30, 70);
            break;
        case 5:
            draw_message(renderScreen, "Desactivez toutes ces tourelles", 30, 50);
            draw_message(renderScreen, "afin de deverouiller la salle", 30, 60);
            draw_message(renderScreen, "du boss au rez-de-chaussee.", 30, 70);
            if (*cleBoss == 1)
                draw_message(renderScreen, "Salle deverouillee!", 30, 90);
            break;
    }
}
