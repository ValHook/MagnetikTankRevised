//
//  Jeu3D.h
//

#ifndef __Jeu3D__
#define __Jeu3D__
#define NBZOMBIES 15
#define NBSPOTS 12

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

#include "DetectionSouris.h"
#include "LectureMedia.h"


void commencerpartie(IrrlichtDevice *device, ISoundEngine* son, DetectionSouris *souris, RECT *ptrecran, int argc, char *argv[]);

class Jeu3D
{
public:
    Jeu3D(IrrlichtDevice *device, ISoundEngine* son, DetectionSouris *souris, int nbpartie, RECT *ptrecran);
    void afficher();
    void lancerNouveauEvenement();
    void shoot();
    void appliquerDegats();
    void deplacerzombies();
    void allumerMine();
    bool onGravitySpot();
    bool invertGravity();
    bool mort();
    bool confirmationQuitter();
    ~Jeu3D();


private:
    RECT *ecran;

    ISoundSource *derezzed, *tir, *cri, *spotsound, *harlemshake, *sabrelaser, *ml606, *canon, *explosion, *endtitles, *striii, *hahaha, *whysoserious, *fall;
    bool *event;
    u32 harlemshaketimer, harlemshakefin, finaletimer, splashtimer;


    DetectionSouris* souris;
    ISoundEngine* son;
    IrrlichtDevice *device;
    IVideoDriver* driver;
    ISceneManager* smgr;

    IParticleSystemSceneNode* ps;

    IAnimatedMesh* mapmesh, *eaumesh, *lasermesh, *torusmesh, *minemesh;
    IAnimatedMesh* tankmesh;
    IAnimatedMesh* zombiemesh, *golemesh, *jokermesh;
    IAnimatedMesh* spotmesh;

    ILightSceneNode *lumiere[9], *lumierecurseur, *lumierebleue;
    IVolumeLightSceneNode *lumiereviolette[2];

    ISceneNode* map, *eau, *laser, *torus, *mine[4];
    ISceneNode* tank;
    ISceneNode* spot[NBSPOTS];
    int infospot[NBSPOTS];
    IAnimatedMeshSceneNode* zombie[NBZOMBIES], *joker;

    struct SZombie
    {
        float vie;
        int gravity;
    };
    struct SZombie infozombie[NBZOMBIES];


    vector3df pos, newpos, vitesse;
    ICameraSceneNode* camera, *trickcamera, *cameraboss;
    int camgravity;

    ITriangleSelector* mapselector, *tankselector, *laserselector, *toruselector;
    ITriangleSelector* zombieselector[NBZOMBIES];
    ISceneNodeAnimator* anim, *animcam, *animtank;
    ISceneNodeAnimatorCollisionResponse *animlaser;

    IBillboardSceneNode* curseur;

    ISceneCollisionManager* collMan;
    line3d<f32> ray;
    vector3df intersection;
    triangle3df hitTriangle;
    ISceneNode* touche;

    struct SDegatsZombie
	{
		u32 tps;
		int idzombie;
	};
    core::array<SDegatsZombie> Degats;

    SKeyMap keyMap[5];
    float speed;
    int munitions;
    float zoomtexture;
    int b;
    bool sourisenfoncee;

    ISceneNode* skybox;

    vector3df poscamboss, oldposcamboss;
    u32 bossshootupdatetime;

    bool *mineactivee;
    ITexture *splashscreen;

    int vie;
};

#endif
