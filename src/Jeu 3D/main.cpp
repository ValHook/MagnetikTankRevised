#include <iostream>
using namespace std;

#include <wtypes.h>

#include <irrlicht.h>
using namespace irr;
using namespace core;
using namespace io;
using namespace video;
using namespace scene;
using namespace gui;

#include <irrKlang.h>
using namespace irrklang;

#include "LectureMedia.h"
#include "DetectionSouris.h"
#include "MenuPrincipal.h"
#include "Jeu3D.h"

int main(int argc, char *argv[])
{
    // Console bleue, juste pour le fun
    system("color 3b");

    // Si on a passé l'argument 2D on lance le Jeu Allegro
    if (!strcmp(argv[argc-1],"2D")){
        system("start /B /WAIT /D 2D MGT2D.exe");
        return 0;
    }

    // Récupération des dimensions de l'écran
    RECT ecran;
    const HWND hecran = GetDesktopWindow();
    GetWindowRect(hecran, &ecran);

    // Installation de la souris, de la sortie audio
    DetectionSouris souris;
    ISoundEngine* son = createIrrKlangDevice();

    // Vérification de la sortie audio
    if (!son){
        system("cls");
        cout << "Impossible de trouver une sortie audio. Ce jeu est beaucoup moins fun sans son." << endl;
        cout << "C'est pourquoi nous vous recommandons de brancher un casque \nou des enceintes audio." << endl;
        cin.get();
        return -1;
    }

    // Vérification des ressources
    if (signatureMedia() != 0x36B6C6A){
        system("cls");
        cout << "Le fichier de ressources media n\'existe pas ou est corrompu" << endl;
        cin.get();
        return -1;
    }

    // Création d'une fenêtre plein écran et initialisation du moteur OpenGL
    IrrlichtDevice *device = createDevice(EDT_OPENGL, dimension2d<u32>(ecran.right,ecran.bottom),32,true,true,false,&souris);
    device->getFileSystem()->addFileArchive("media",true,true,EFAT_UNKNOWN);


    // Instanciation du menu principal
    MenuPrincipal* menu = new MenuPrincipal(device,son,&souris);

    while (!menu->choix()) menu->afficher();

    // Validation de l'option
    switch(menu->choix())
    {
        case 1 : // Jouer en 3D
            // On se laisse tomber du menu
            menu->tomber();
            // On supprime notre scene
            device->getSceneManager()->clear();
            // On commence une nouvelle partie
            commencerpartie(device,son,&souris,&ecran,argc,argv);
            // Quand la partie est finie on quitte le jeu
            son->drop();
            device->closeDevice();
            break;


        case 2 : // Jouer en 2D
            son->drop();
            device->closeDevice();
            system("start /B /WAIT /D 2D MGT2D.exe");
            break;


        case 3 : // Quitter
            son->drop();
            device->closeDevice();
    }

    return 0;
}
