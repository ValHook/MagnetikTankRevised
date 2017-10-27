#include <stdio.h>
#include <stdlib.h>
#include <allegro.h>
#include "Structures.h"
#include "Load_Functions.h"
#include "Render_Functions.h"
#include "Calculation_Functions.h"
#include "Main_Game_Functions.h"
#include "Elevator_Functions.h"
#include "Transition_Functions.h"
#include "Free_Functions.h"
#include "Turret_Functions.h"
#include "Laser_Functions.h"
#include "Misc_Functions.h"

void allegro_initialisation()
{
    // Initialisation Allegro
    allegro_init();
    install_keyboard();
    install_mouse();

    // Initialisation du mode graphique
    set_color_depth(desktop_color_depth());
    if (set_gfx_mode(GFX_AUTODETECT_WINDOWED,800,600,0,0)!=0)
    {
        allegro_message("prb gfx mode");
        allegro_exit();
        exit(EXIT_FAILURE);
    }
}

void gestionnaire_jeu()
{
    // Déclaration des ressources
    int salleSuivante = -1, sallePrecedente = -1, temp;
    int cleBoss = 0;
    // Initialisation Allegro
    allegro_initialisation();
    // Initialisation variables inter-salles

    while (salleSuivante != -2)
    {
        switch(salleSuivante)
        {
            case -1:
                sallePrecedente = menu_principal(salleSuivante, sallePrecedente, &cleBoss);
                break;
            default:
                sallePrecedente = jeu_principal(salleSuivante, sallePrecedente, &cleBoss);
                break;
        }
        temp = sallePrecedente;
        sallePrecedente = salleSuivante;
        salleSuivante = temp;
    }
}



int jeu_principal(int nSalle, int sallePrecedente, int *cleBoss)
{
    // Déclaration des ressources
    BITMAP *background, *buffer, *hud, *blockSprites[31], *achievements[4];
    MAP *room;
    TANK *tank;
    BOSS *boss;
    int prochaineSalle;
    char nomSalle[] = "Map Salle XX.txt";
    int hasWonFor = 0;
    int beenDeadFor = 0;


    // Traitement initial

    prochaineSalle = nSalle;
    // Création du buffer et du hud
    buffer = create_bitmap_shielded(800, 600);
    hud = create_bitmap_shielded(800, 600);
    // Récupérer les sprites de blocs
    multiple_static_sprite_load_division("Dalles 3D.bmp", blockSprites, 31, 80, 80);
    // Chargement de l'image de fond
    background = load_bitmap_shielded("Background.bmp");
    // Déterminer le nom de la map à charger
    sprintf(nomSalle, "Map Salle %02d.txt", nSalle);
    // Chargement map
    room = map_load(nomSalle);
    // Initialisation des ascenseurs principaux
    init_entrance_exit_elevators(room, nSalle, sallePrecedente);
    // Création et initialisation du tank
    tank = tank_load();
    init_tank(tank, room, nSalle, sallePrecedente);
    // Initialisation du boss
    boss = init_boss();
    // Chargement des succès
    load_achievements(achievements);

    // Boucle de traitement
    while (prochaineSalle == nSalle)
    {
        if (key[KEY_ESC])
            prochaineSalle = -2;

        // Calculer la vitesse du tank en fonction des commandes
        calculate_tank_speed_matching_commands(tank);
        // Calculer la vitesse du tank en fonction de la friction
        calculate_tank_speed_matching_friction(tank);
        // Calculer la vitesse du tank en fonction du magnétisme
        calculate_tank_speed_matching_magnetism(tank);
        // Prendre en compte les collisions
        calculate_tank_speed_matching_collision(tank, room);

        // Calculer la position du tank en fonction de la vitesse
        calculate_tank_position_matching_speed(tank);

        // Calculer la direction du tank en fonction des commandes
         calculate_tank_direction_matching_commands(tank);

        // Activer les ascenseurs pression
        activate_pressure_elevators(room, tank);
        // Calculer la hauteur des ascenseurs
        calculate_elevator_height(room);
        // Calculer la hauteur du tank en fonction de la hauteur de l'ascenseur
        calculate_tank_height_matching_elevator(room, tank);

        // Désactiver les tourelles pression
        deactivate_pressure_turrets(room, tank);
        // Calculer la hauteur des tourelles
        calculate_turret_height(room);
        // Calculer la hauteur du tank en fonction de la hauteur de la tourelle
        calculate_tank_height_matching_turret(room, tank);
        // Tourner les tourelles rotatives
        rotate_turrets(room);
        // Tirer sur la tank s'il passe devant une tourelle
        shoot_lasers_on_tank(room, tank);
        // Supprimer les lasers après un moment
        make_lasers_expire(room);
        // Shut down turrets when tank dead
        shut_down_turrets_when_tank_dead(room, tank);



        // Gestion du boss
        boss_room_management(hud, room, tank, boss, nSalle, cleBoss);


        // Affichage de l'image de fond
        render_background(background, buffer);
        // Affichage de la salle et du tank
        render_blocks_tank(buffer, blockSprites, tank, room, 400, 260);

        // Dessiner message en fonction de la salle
        draw_messages_matching_room (hud, nSalle, cleBoss);
        // Passage du HUD au buffer
        masked_blit(hud, buffer, 0, 0, 0, 0, 800, 600);
        clear_to_color(hud, makecol(255, 0, 255));

        // Assombrissement de la salle en fonction de la hauteur du tank (Transition inter-salle)
        darken_buffer_matching_tank_height(buffer, tank);
        // Vérifier si on a changé de salle
        change_room_matching_tank_height(&prochaineSalle, tank);
        // Animation de fin de jeu si le tank est mort
        if (tank -> alive == 0) beenDeadFor++;
        if (tank -> alive == 2) hasWonFor++;
        game_over_animation(buffer, tank, beenDeadFor, &prochaineSalle);
        game_won_animation(buffer, tank, hasWonFor, &prochaineSalle, achievements);
        // Passage du buffer à l'écran
        blit(buffer, screen, 0, 0, 0, 0, 800, 600);
        rest(40);
    }

    // Libération de mémoire allouée
    free_tank(tank);
    free_map(room);

    // Fin du sous-programme
    return prochaineSalle;
}

int menu_principal(int nSalle, int sallePrecedente, int *cleBoss)
{
    // Déclaration des ressources
    BITMAP *background, *titleEmpty, *titleFilling, *buffer;
    int i, j, c, stop;

    // Traitement initial
    background = load_bitmap_shielded("Menu Background.bmp");
    titleEmpty = load_bitmap_shielded("Titre Menu Vide.bmp");
    titleFilling = load_bitmap_shielded("Titre Menu Remplissage.bmp");
    buffer = create_bitmap_shielded(800, 600);
    stop = 0;
    c = 0;
    i = 0;
    j = 0;

    // Boucle de traitement
    while(stop == 0)
    {
        // Afficher image de fond
        render_background(background, buffer);
        // Afficher titre
        switch(c)
        {
            case 0:
                if (j <= 400)
                {
                    masked_blit(titleEmpty, buffer, 0, 0, 100, 100, j, 100);
                    j += 16;
                }
                else
                {
                    draw_sprite(buffer, titleEmpty, 100, 100);
                }
                if (i < 40)
                {
                    i++;
                }
                else
                {
                    masked_blit(titleFilling, buffer, 0, 0, 100, 100, 400, 100);
                    i = 0;
                    c++;
                }
                break;
            case 1:
                readkey();
                stop = 1;
                break;
        }
        // Afficher buffer à l'écran
        blit(buffer, screen, 0, 0, 0, 0, 800, 600);
        rest(40);
    }
    return 1;
}


