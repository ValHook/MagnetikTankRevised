//
//  MenuPrincipal.h
//
#ifndef __MenuPrincipal__
#define __MenuPrincipal__

#include <iostream>
using namespace std;

#include <irrlicht.h>
using namespace irr;
using namespace core;
using namespace io;
using namespace video;
using namespace scene;
using namespace gui;

#include <irrKlang.h>
using namespace irrklang;

#include "DetectionSouris.h"


enum
{
	Imprenable = 0,
	Prenable = 1 << 0
};

class MenuPrincipal
{
public:
    MenuPrincipal(IrrlichtDevice *device, ISoundEngine* son, DetectionSouris *souris);
    int choix();
    void afficher();
    void ajouterLumieresRouges();
    void supprimerLumieresRouges();
    void tomber();
    ~MenuPrincipal();



private:
    float zoomtexture;
    float vitessetexture;
    int explosionbool;
    int optionchoisie;

    DetectionSouris* souris;
    ISoundEngine* son;
    IrrlichtDevice *device;

    ISoundSource* musiquefond;
    ISoundSource* explosion;

    IVideoDriver* driver;
    ISceneManager* smgr;

    IAnimatedMesh* mapmesh;
    IAnimatedMesh* jouer2Dmesh;
    IAnimatedMesh* jouer3Dmesh;
    IAnimatedMesh* creditsmesh;
    IAnimatedMesh* quittermesh;

    ISceneNode* map;
    ISceneNode* jouer2D;
    ISceneNode* jouer3D;
    ISceneNode* credits;
    ISceneNode* quitter;

    ICameraSceneNode* camera;

    ITriangleSelector* selector;
    ISceneNodeAnimator* anim;
    ISceneCollisionManager* collMan;

    ILightSceneNode** lumieresrouges;

    ISceneNode* skybox;
    ISceneNode* skyboxcredits;

    SKeyMap keyMap[5];

    line3d<f32> ray;
    vector3df intersection;
    triangle3df hitTriangle;
    ISceneNode* texteChoisi;

    IBillboardSceneNode* curseur;

    bool oncredits;

};
#endif
