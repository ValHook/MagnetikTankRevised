//
//  Jeu3D.cpp
//  Certains sous programmes mélangent malheuresement la vue et le modèle dans ce fichier
//

#include "Jeu3D.h"



void commencerpartie(IrrlichtDevice *device, ISoundEngine* son, DetectionSouris *souris, RECT *ptrecran, int argc, char *argv[])
{
    /* Contrôlleur principal du jeu */
    int nbpartie = 0, isDead;
    Jeu3D* jeu = NULL;
    do {
        // Tout d'abord on crée l'environnement de jeu
        // Cela comporte la carte, l'éclairage, le tank, les ennemis, la caméra, les musiques et les animations
        if (jeu) delete jeu;
        jeu = new Jeu3D(device,son,souris,nbpartie++,ptrecran);
        // On continue à jouer tant qu'on est vivant
        do {
            jeu->afficher();
            isDead = jeu->mort();

        } while (!isDead || !strcmp(argv[argc - 1],"invincible"));
    // On affiche un menu pour savoir si on veut rejouer
    } while (!jeu->confirmationQuitter());

}

Jeu3D::Jeu3D(IrrlichtDevice *ptrdevice, ISoundEngine* ptrson, DetectionSouris *ptrsouris, int nbpartie, RECT* ptrecran)
{
    // Initialisation des variables
    int i,j;
    ecran = ptrecran;
    event = (bool *) calloc(14,sizeof(bool));
    mineactivee = (bool *) calloc(4,sizeof(bool));
    device = ptrdevice;
    son = ptrson;
    souris = ptrsouris,
    driver = device->getVideoDriver();
    smgr = device->getSceneManager();


    // On charge les musiques
    derezzed = lireMusique(device->getFileSystem()->createAndOpenFile("Derezzed.ogg"),son,"Soundtrack");
    tir = lireMusique(device->getFileSystem()->createAndOpenFile("tir.ogg"),son,"Tir");
    cri = lireMusique(device->getFileSystem()->createAndOpenFile("cri.ogg"),son,"Cri");
    harlemshake = lireMusique(device->getFileSystem()->createAndOpenFile("HarlemShake.ogg"),son,"Harlem Sake");
    spotsound = lireMusique(device->getFileSystem()->createAndOpenFile("spot.ogg"),son,"Spot");
    sabrelaser = lireMusique(device->getFileSystem()->createAndOpenFile("bzii.wav"),son,"Sabre Laser");
    ml606 = lireMusique(device->getFileSystem()->createAndOpenFile("ML-606.ogg"),son,"ML-606");
    canon = lireMusique(device->getFileSystem()->createAndOpenFile("canon.ogg"),son,"Canon");
    explosion = lireMusique(device->getFileSystem()->createAndOpenFile("explosion.wav"),son,"Explosion");
    endtitles = lireMusique(device->getFileSystem()->createAndOpenFile("EndTitles.ogg"),son,"End Titles");
    striii = lireMusique(device->getFileSystem()->createAndOpenFile("striii.ogg"),son,"Striii");
    hahaha = lireMusique(device->getFileSystem()->createAndOpenFile("hahaha.ogg"),son,"Ha Ha Ha");
    whysoserious = lireMusique(device->getFileSystem()->createAndOpenFile("WhySoSerious.ogg"),son,"Why So Serious");
    fall = lireMusique(device->getFileSystem()->createAndOpenFile("Fall.wav"),son,"Fall");
    whysoserious->setDefaultVolume(10.0f);
    fall->setDefaultVolume(10.0f);
    derezzed->setDefaultVolume(0.3f);
    ml606->setDefaultVolume(0.5f);
    endtitles->setDefaultVolume(0.5f);
    son->play2D(derezzed,true);



    // On charge la map
    // ARCHITECTURE
    mapmesh = smgr->getMesh("redo23.3ds");
    map = smgr->addMeshSceneNode(mapmesh);
    // LUMIERES
    lumiere[0] = smgr->addLightSceneNode(0,vector3df(vector3df(-850,50,-150)), SColorf(255,255,255), 2000.0f, 1 );
    lumiere[1] = smgr->addLightSceneNode(0,vector3df(vector3df(-8,1097,1755)), SColorf(255,255,255), 2000.0f, 1 );
    lumiere[2] = smgr->addLightSceneNode(0,vector3df(vector3df(-3328,-1300,1908)), SColorf(255,255,255), 2000.0f, 1 );
    lumiere[3] = smgr->addLightSceneNode(0,vector3df(vector3df(-2224,-1300,2374)), SColorf(255,255,255), 2000.0f, 1 );
    lumiere[4] = smgr->addLightSceneNode(0,vector3df(vector3df(0,8000,4000)), SColorf(255,255,255), 4000.0f, 1 );
    lumiere[5] = smgr->addLightSceneNode(0,vector3df(vector3df(-8000,16000,4000)), SColorf(255,255,255), 34000.0f, 1 );
    lumiere[6] = smgr->addLightSceneNode(0,vector3df(vector3df(0,16000,10000)), SColorf(255,255,255), 34000.0f, 1 );
    lumiere[7] = smgr->addLightSceneNode(0,vector3df(vector3df(0,-300,3400)), SColorf(200,255,200), 2000.0f, 1 );
    lumiere[8] = smgr->addLightSceneNode(0,vector3df(15581,30800,-1325),SColorf(255,255,255),2000.0f,1);
    map->setMaterialFlag(EMF_LIGHTING, true);
    map->setScale(vector3df(150,150,150));
    map->setMaterialTexture(0, driver->getTexture("map.jpg"));
    smgr->getMeshManipulator()->makePlanarTextureMapping(mapmesh->getMesh(0), 0.2f);
    // EAU
   	eaumesh = smgr->addHillPlaneMesh( "eau",
                                  dimension2d<f32>(85,85),
                                  dimension2d<u32>(40,40), 0, 0,
                                  dimension2d<f32>(0,0),
                                  dimension2d<f32>(10,10));
	eau = smgr->addWaterSurfaceSceneNode(eaumesh->getMesh(0), 3.0f, 300.0f, 30.0f);
	eau->setPosition(vector3df(0,7,0));
	eau->setMaterialTexture(0, driver->getTexture("map.jpg"));
	eau->setMaterialTexture(1, driver->getTexture("water.jpg"));
	eau->setMaterialType(EMT_REFLECTION_2_LAYER);
    eau->setPosition(vector3df(0,-500,4006));
    // LASERS
    lasermesh = smgr->getMesh("laser.3ds");
    laser = smgr->addMeshSceneNode(lasermesh);
    laser->setMaterialTexture(0, driver->getTexture("laser.png"));
    laser->setMaterialFlag(EMF_LIGHTING,false);
    laser->setScale(vector3df(150,150,150));
    laser->setMaterialFlag(EMF_BACK_FACE_CULLING, false);
    laser->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL);
    // PARTICULES
	ps = smgr->addParticleSystemSceneNode(false);
    IParticleEmitter* em = ps->createBoxEmitter(
		aabbox3d<f32>(-2000,0,-2000,2000,2500,2000), // taille
		vector3df(0.0f,0.06f,0.0f),   // direction
		2700,3000, // nombre de particules
		SColor(0,255,255,255),
		SColor(0,255,255,255),
		800,2000,0, // angles
		dimension2df(10.f,10.f),
		dimension2df(20.f,20.f));
	ps->setEmitter(em);
	em->drop();
    IParticleAffector* paf = ps->createFadeOutParticleAffector();
	ps->addAffector(paf);
	paf->drop();
	ps->setPosition(vector3df(-4409, -5869, 7089));
	ps->setScale(vector3df(2,2,2));
	ps->setMaterialFlag(EMF_LIGHTING, false);
	ps->setMaterialFlag(EMF_ZWRITE_ENABLE, false);
	ps->setMaterialTexture(0, driver->getTexture("fire.bmp"));
	ps->setMaterialType(EMT_TRANSPARENT_VERTEX_ALPHA);
    // CIEL
    skybox=smgr->addSkyBoxSceneNode(
                                    driver->getTexture("sup.png"),
                                    driver->getTexture("sdown.png"),
                                    driver->getTexture("sleft.png"),
                                    driver->getTexture("sright.png"),
                                    driver->getTexture("sface.png"),
                                    driver->getTexture("sback.png"));





    // Boutons gravité
    spotmesh = smgr->getMesh("button.3ds");

    spot[0] = smgr->addMeshSceneNode(spotmesh);
    spot[0]->setMaterialFlag(EMF_LIGHTING, true);
    spot[0]->setScale(vector3df(100,100,100));
    spot[0]->setPosition(vector3df(0,-1200,0));
    spot[0]->setMaterialTexture(0, driver->getTexture("spot.jpg"));
    infospot[0] = 1;

    spot[1] = smgr->addMeshSceneNode(spotmesh);
    spot[1]->setMaterialFlag(EMF_LIGHTING, true);
    spot[1]->setScale(vector3df(100,100,100));
    spot[1]->setPosition(vector3df(-8,1200,1305));
    spot[1]->setMaterialTexture(0, driver->getTexture("spot.jpg"));
    spot[1]->setRotation(vector3df(0,0,180));
    infospot[1] = -10;

    spot[2] = smgr->addMeshSceneNode(spotmesh);
    spot[2]->setMaterialFlag(EMF_LIGHTING, true);
    spot[2]->setScale(vector3df(100,100,100));
    spot[2]->setPosition(vector3df(-100,5000,3920));
    spot[2]->setMaterialTexture(0, driver->getTexture("spot.jpg"));
    infospot[2] = 1;

    for (i = 3; i <= 6; i++){
        spot[i] = smgr->addMeshSceneNode(spotmesh);
        spot[i]->setMaterialFlag(EMF_LIGHTING, true);
        spot[i]->setScale(vector3df(100,100,100));
        spot[i]->setMaterialTexture(0, driver->getTexture("spot.jpg"));
        spot[i]->setRotation(vector3df(0,0,180));
        infospot[i] = -10;
    }
    infospot[3] = -11;
    spot[3]->setPosition(vector3df(-6929.97,6979.37,5479.15));
    spot[4]->setPosition(vector3df(-14922.7,15436.29,13274.1));
    spot[5]->setPosition(vector3df(-1435.41,15436.29,6860.83));
    spot[6]->setPosition(vector3df(-11411.4,15436.29,720.586));

    spot[7] = smgr->addMeshSceneNode(spotmesh);
    spot[7]->setMaterialFlag(EMF_LIGHTING, true);
    spot[7]->setScale(vector3df(100,100,100));
    spot[7]->setPosition(vector3df(-4852.99, -55962.29, 7421.94));
    spot[7]->setMaterialTexture(0, driver->getTexture("spot.jpg"));
    infospot[7] = 1;
    lumierebleue = smgr->addLightSceneNode(0,vector3df(-4852.99, -5872.29, 7421.94),SColorf(0,180,255),2000.0f,1);
    spot[7]->setVisible(false);
    lumierebleue->setVisible(false);

    for (i = 8; i <= 11; i++){
        spot[i] = smgr->addMeshSceneNode(spotmesh);
        spot[i]->setMaterialFlag(EMF_LIGHTING, true);
        spot[i]->setScale(vector3df(100,100,100));
        spot[i]->setMaterialTexture(0, driver->getTexture("spot.jpg"));
        spot[i]->setRotation(vector3df(0,0,180));
        spot[i]->setVisible(false);
        infospot[i] = -1;
    }
    spot[8]->setPosition(vector3df(-4663.4,-50690,5699.13));
    spot[9]->setPosition(vector3df(-6664.47,-50690,7396.23));
    spot[10]->setPosition(vector3df(-4938.27,-50690,9374.91));
    spot[11]->setPosition(vector3df(-3013.62,-50690,7671.85));




    // Caméra
    speed = 0.010f;
    keyMap[0].Action = EKA_MOVE_FORWARD;
    keyMap[0].KeyCode = KEY_KEY_Z;
    keyMap[1].Action = EKA_MOVE_BACKWARD;
    keyMap[1].KeyCode = KEY_KEY_S;
    keyMap[2].Action = EKA_STRAFE_LEFT;
    keyMap[2].KeyCode = KEY_KEY_Q;
    keyMap[3].Action = EKA_STRAFE_RIGHT;
    keyMap[3].KeyCode = KEY_KEY_D;
    keyMap[4].Action = EKA_JUMP_UP;
    keyMap[4].KeyCode = KEY_SPACE;
    camera = smgr->addCameraSceneNodeFPS(0, 100.0f, speed, -1,keyMap, 5, true, 0.0f);
    camera->setPosition(vector3df(850,50,500/*-4938.27,-3500,9374.91*/));
    camera->setTarget(vector3df(0,0,0));
	camera->setFarValue(1000000.0f);
	camgravity = -10;
	trickcamera = smgr->addCameraSceneNode(camera);
	smgr->setActiveCamera(camera);




    // Tank
    tankmesh = smgr->getMesh("tank.obj");
    tank = smgr->addMeshSceneNode(tankmesh->getMesh(0));
    tank->setPosition(vector3df(0,40,0));
    tank->setMaterialFlag(EMF_LIGHTING, true);
    tank->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL);
    tank->setScale(vector3df(3,3,3));
    tank->setMaterialTexture(0, driver->getTexture("lobby.png"));
    smgr->getMeshManipulator()->makePlanarTextureMapping(tankmesh->getMesh(0), 0.1f);
    vitesse = vector3df(0,0,0);
    munitions = 100000000; // Finalement non utilisé

    // Zombies
    zombiemesh = smgr->getMesh("zombie.md2");

    zombie[0] = smgr->addAnimatedMeshSceneNode(zombiemesh);
    zombie[0]->setMaterialFlag(EMF_LIGHTING, true);
    zombie[0]->setMD2Animation(EMAT_STAND);
    zombie[0]->setMaterialTexture(0, driver->getTexture("zombie.jpg"));
    zombie[0]->setPosition(vector3df(0,-900,0));
    infozombie[0].vie = 100.0f;
    infozombie[0].gravity = -10;

    zombie[1] = smgr->addAnimatedMeshSceneNode(zombiemesh);
    zombie[1]->setMaterialFlag(EMF_LIGHTING, true);
    zombie[1]->setMD2Animation(EMAT_STAND);
    zombie[1]->setMaterialTexture(0, driver->getTexture("zombie.jpg"));
    zombie[1]->setPosition(vector3df(-550,200,550));
    infozombie[1].vie = 100.0f;
    infozombie[1].gravity = -10;

    zombie[2] = smgr->addAnimatedMeshSceneNode(zombiemesh);
    zombie[2]->setMaterialFlag(EMF_LIGHTING, true);
    zombie[2]->setMD2Animation(EMAT_STAND);
    zombie[2]->setMaterialTexture(0, driver->getTexture("zombie.jpg"));
    zombie[2]->setPosition(vector3df(-8,1050,1305));
    infozombie[2].vie = 100.0f;
    infozombie[2].gravity = 1;

    for (i = 3; i < 13; i++){
        zombie[i] = smgr->addAnimatedMeshSceneNode(zombiemesh);
        zombie[i]->setMaterialFlag(EMF_LIGHTING, true);
        zombie[i]->setMD2Animation(EMAT_STAND);
        zombie[i]->setMaterialTexture(0, driver->getTexture("zombievert.jpg"));
        zombie[i]->setFrameLoop(0,0);
        infozombie[i].gravity = -9;
        infozombie[i].vie = 99999999.0f;
    }
    zombie[3]->setPosition(vector3df(-1035,-300,4450));
    zombie[4]->setPosition(vector3df(-992,-300,4029));
    zombie[5]->setPosition(vector3df(-523,-300,4244));
    zombie[6]->setPosition(vector3df(-504,-300,3798));
    zombie[7]->setPosition(vector3df(-746,-300,3312));
    zombie[8]->setPosition(vector3df(253,-300,3766));
    zombie[9]->setPosition(vector3df(267,-300,4235));
    zombie[10]->setPosition(vector3df(776,-300,4489));
    zombie[11]->setPosition(vector3df(765,-300,4029));
    zombie[12]->setPosition(vector3df(510,-300,3303));


    // Golems
    golemesh = smgr->getMesh("golem.3ds");
    for(i = 13; i < 15; i++){
        zombie[i] = smgr->addAnimatedMeshSceneNode(golemesh);
        zombie[i]->setMaterialFlag(EMF_LIGHTING, true);
        zombie[i]->setMaterialTexture(0, driver->getTexture("golem.jpg"));
        zombie[i]->setScale(vector3df(10,10,10));
        infozombie[i].gravity = -9;
        infozombie[i].vie = 150.0f;
    }
    zombie[13]->setPosition(vector3df(-3344,-5972,6671));
    zombie[14]->setPosition(vector3df(-5583,-5972,9493));


    // Physics
    mapselector = smgr->createTriangleSelector(mapmesh,map);
    map->setTriangleSelector(mapselector);
    animcam = smgr->createCollisionResponseAnimator(mapselector, camera, vector3df(10,1,10),vector3df(0,-10,0),vector3df(0,102,0));
    camera->addAnimator(animcam);

    laserselector = smgr->createTriangleSelector(lasermesh,laser);
    laser->setTriangleSelector(laserselector);
    animlaser = smgr->createCollisionResponseAnimator(laserselector, trickcamera, vector3df(80,1,80),vector3df(0,0,0),vector3df(0,0,0));
    trickcamera->addAnimator(animlaser);

    animtank = smgr->createCollisionResponseAnimator(mapselector, tank, vector3df(10,1,10),vector3df(0,-10,0),vector3df(0,100,0));
    tank->addAnimator(animtank);

    float k;
    for (i=0; i<NBZOMBIES; i++){
        if (i <13){
            k = 1;
            if(infozombie[i].gravity == 1) k = -1;
            if (infozombie[i].vie == 99999999.0f) k = 0.86;
            anim = smgr->createCollisionResponseAnimator(mapselector, zombie[i], vector3df(1,1,1),vector3df(0,infozombie[i].gravity,0),vector3df(0,100*k,0));
            zombie[i]->addAnimator(anim);
            anim->drop();
        }
        zombieselector[i] = smgr->createTriangleSelector(zombiemesh, zombie[i]);
        zombie[i]->setTriangleSelector(zombieselector[i]);
    }


    // Curseur
    curseur = smgr->addBillboardSceneNode();
	curseur->setMaterialType(EMT_TRANSPARENT_ADD_COLOR );
	curseur->setMaterialTexture(0, driver->getTexture("particle.bmp"));
	curseur->setMaterialFlag(EMF_LIGHTING, false);
	curseur->setMaterialFlag(EMF_ZBUFFER, false);
	curseur->setSize(dimension2d<f32>(20.0f, 20.0f));

    // Dernières variables
    collMan = smgr->getSceneCollisionManager();
    sourisenfoncee = 0;
    zoomtexture = 0;
    b = 1;
    splashscreen = driver->getTexture("splashscreen.png");
}


























void Jeu3D::afficher()
{

    // Nouvel événemnt si l'état du jeu l'impose (Harlem Shake, Extinction des lumières, Changement de musique, Boss)
    lancerNouveauEvenement();

    pos = camera->getPosition();
    device->run();

    // Caméra et Tank
    tank->setRotation(camera->getRotation());
    tank->setPosition(camera->getPosition() + (camera->getTarget() - camera->getPosition()).normalize()*0.5f - vector3df(0,2,0));
    trickcamera->setPosition(camera->getPosition());

    // Curseur
    ray.start = camera->getPosition();
    ray.end = ray.start + (camera->getTarget() - ray.start).normalize() * 100000.0f;
    touche = collMan->getSceneNodeAndCollisionPointFromRay(ray,intersection,hitTriangle);
    if (touche)  curseur->setPosition(intersection);


    // Inversion gravité
    if (onGravitySpot())
        invertGravity();

    // Tir
    if ((munitions && touche && souris->GetMouseState().LeftButtonDown && !sourisenfoncee) || event[10]) {sourisenfoncee = true; shoot();}
    if (!souris->GetMouseState().LeftButtonDown) sourisenfoncee = false;
    appliquerDegats();

    // Déplacement zombies
    deplacerzombies();


    // Animations motifs salle du boss
    if (event[8]){
        zoomtexture += b*0.0001;
        if (zoomtexture < 0) b = 1;
        if (zoomtexture > 0.3) b = -1;
        smgr->getMeshManipulator()->makePlanarTextureMapping(torusmesh->getMesh(0), zoomtexture);
    }

    // Activation des mines si proche des mines
    if (event[10])
        allumerMine();

    // Swap buffer
    driver->beginScene(true, true, SColor ( 0, 0, 0, 0));
    smgr->drawAll();
    device->getGUIEnvironment()->drawAll();
    if (event[13] && device->getTimer()->getTime() > splashtimer)
        driver->draw2DImage(splashscreen, position2d<s32>((ecran->right - 1000)/2,(ecran->bottom - 625)/2 + 150), rect<s32>(0,0,1000,625), 0, SColor(255,255,255,255), true);
    driver->endScene();

    // Inertie (Après le swap du buffer pour bien différencier les états pos et newpos car le swap du buffer est lent)
    newpos = camera->getPosition();
    if(pos != newpos) vitesse += ((newpos - pos).normalize()*speed*500/driver->getFPS());
    else vitesse *= vector3df(0.950f,0.950,0.950f);
    camera->setPosition(newpos + vitesse);

    // On quitte le jeu si on clique sur la souris et que le jeu est fini
    if (event[13] && souris->GetMouseState().LeftButtonDown){
        device->drop();
        son->drop();
        exit(0);
    }

}

























bool Jeu3D::onGravitySpot()
{
    int i;
    for (i = 0; i < NBSPOTS;i++)
        if((tank->getPosition() - spot[i]->getPosition() /*- vector3df(0,tank->getPosition().Y - spot[i]->getPosition().Y,0)*/).getLength() <= 130.0f  && camgravity != infospot[i]){
                camgravity = infospot[i];
                return true;
        }
    return false;
}



bool Jeu3D::invertGravity()
{
    son->play2D(spotsound);
    int hcam = 102, htank = 100;
    if (camgravity == 1) { hcam = -100; htank = -104;}
    camera->removeAnimator(animcam);
    animcam->drop();
    animcam = smgr->createCollisionResponseAnimator(mapselector, camera, vector3df(30,15,30),vector3df(0,camgravity,0),vector3df(0,hcam,0));
    camera->addAnimator(animcam);

    tank->removeAnimator(animtank);
    animtank->drop();
    animtank = smgr->createCollisionResponseAnimator(mapselector, tank, vector3df(30,15,30),vector3df(0,camgravity,0),vector3df(0,htank,0));
    tank->addAnimator(animtank);

}

void Jeu3D::shoot()
{
    // Bruit de tir
    if (event[5] == false)
        son->play2D(tir,false);
    else
        if (!event[10])
        son->play2D(canon,false);

    // Création d'une balle
    munitions --;
    vector3df origine = vector3df(0,0,0) + ray.start + (camera->getTarget() - ray.start).normalize() * 50.0f;
    ISceneNode* bullet = NULL;
	bullet = smgr->addBillboardSceneNode(0, dimension2d<f32>(25,25), origine);
	bullet->setMaterialFlag(EMF_LIGHTING, false);
	bullet->setMaterialTexture(0, device->getVideoDriver()->getTexture("bullet.bmp"));
	bullet->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);


	// Mise en place de l'animation de tir
	if (event[10])
        intersection = cameraboss->getPosition();
    f32 distance = (f32)(intersection - origine).getLength();
	const f32 vitessetir = 1.2f;
	u32 temps = (u32)(distance / vitessetir);
    ISceneNodeAnimator* animator = NULL;
	animator = smgr->createFlyStraightAnimator(origine, intersection, temps);
	bullet->addAnimator(animator);
	animator->drop();

	animator = smgr->createDeleteAnimator(temps);
	bullet->addAnimator(animator);
	animator->drop();

	// On rajoute des lumières quand tire
	if (event[3] && !event[10])
        smgr->addLightSceneNode(bullet,vector3df(vector3df(0,0,0)), SColorf(255,170,0), 100.0f, 1 );

    // On programme l'infligeation des dégats
    int i, id = -1;
    for (i=0; i<NBZOMBIES; i++){
        if (touche == zombie[i])
            id = i;
    }
    if (id >= 0){
        SDegatsZombie deg;
        deg.tps = device->getTimer()->getTime() + temps - 100;
        deg.idzombie = id;
        Degats.push_back(deg);
    }

}



void Jeu3D::appliquerDegats()
{
	u32 mtn = device->getTimer()->getTime();

	for (s32 i=0; i<(s32)Degats.size(); ++i)
		if (mtn > Degats[i].tps)
		{
            infozombie[Degats[i].idzombie].vie -= 10.0f;
            // Mort du zombie
            if(infozombie[Degats[i].idzombie].vie <= 0.0f && zombie[Degats[i].idzombie])
            {
                son->play2D(cri,false);
                zombie[Degats[i].idzombie]->setFrameLoop(240,300);
                zombie[Degats[i].idzombie]->removeAnimators();
                if (Degats[i].idzombie < 13)
                    zombie[Degats[i].idzombie]->addAnimator(smgr->createCollisionResponseAnimator(mapselector, zombie[Degats[i].idzombie], vector3df(1,1,1),vector3df(0,-3,0),vector3df(0,100,0)));
                else
                    zombie[Degats[i].idzombie]->addAnimator(smgr->createCollisionResponseAnimator(mapselector, zombie[Degats[i].idzombie], vector3df(1,1,1),vector3df(0,15,0),vector3df(0,100,0)));
                zombie[Degats[i].idzombie]->addAnimator(smgr->createDeleteAnimator(1500));
                zombie[Degats[i].idzombie] = NULL;
            }
			Degats.erase(i);
			i--;
		}



    if (event[10]){
        poscamboss = cameraboss->getPosition();

        // Dégats joueur
        if (device->getTimer()->getTime() > bossshootupdatetime){
            bossshootupdatetime = device->getTimer()->getTime() + 130;
            if (poscamboss == oldposcamboss || (joker->getPosition() - cameraboss->getPosition()).getLength() < 700) {
                vie--;
                IGUIEnvironment *guienv = device->getGUIEnvironment();
                IGUIInOutFader* fondu = guienv->addInOutFader();
                fondu->setColor(SColor(220,255,0,0));
                fondu->fadeIn(100);
            }
            // Verouillage par le boss
            oldposcamboss = poscamboss;
        }
    }

}




void Jeu3D::deplacerzombies()
{
    int i;
    // On traite tous les zombies
    for (i = 0; i < NBZOMBIES; i++){
        // Si le zombie est encore vivant et sur le même front que le tank
        if (infozombie[i].vie > 0.0f && infozombie[i].gravity == camgravity){
            // On oriente le zombie  vers nous
            zombie[i]->setRotation((camera->getPosition() - zombie[i]->getPosition()).getHorizontalAngle() - vector3df(0,90,0));
            // On déplace le zombie vers nous
            float vit = 2.5f + 0.5f*i;
            if (i > 13)
                vit = 0.6*i;
            zombie[i]->setPosition(zombie[i]->getPosition() + (tank->getPosition() - zombie[i]->getPosition()).normalize() * (vit*(45.0/(float)driver->getFPS())));
        }
    }
}


bool Jeu3D::mort()
{
    int i;
    // On detecte si on est mort uniquement si le jeu n'est pas fini
    if (!event[11]){
        // Mort par zombie
        for (i = 0; i < NBZOMBIES; i++){
            if (infozombie[i].vie > 0.0f)
                if ((tank->getPosition() - zombie[i]->getPosition()).getLength() < 20.0f)
                    return true;
        }
        // Mort par laser
        if (animlaser->collisionOccurred())
            return true;

        // Mort par boss
        if (event[10] && vie <= 0)
            return true;

        // Mort si l'on tombe sous la carte
        if (event[10])
            if (cameraboss->getPosition().Y < -27000)
                return true;
    }

    return false;
}



void Jeu3D::allumerMine()
{
    int i;
    for (i = 0; i < 4 ; i++){
        if ((cameraboss->getPosition() - mine[i]->getPosition()).getLength() < 50.0 && !mineactivee[i]){
            son->play2D(spotsound, false);
            mine[i]->setMaterialTexture(0, driver->getTexture("mineverte.png"));
            mineactivee[i] = true;
        }
    }
}








































void Jeu3D::lancerNouveauEvenement()
{
    // Évenement Musique Harlem Shake
    if (!event[0] && camera->getPosition().Z >= 3100){
        harlemshaketimer = device->getTimer()->getTime() + 15600;
        harlemshakefin = device->getTimer()->getTime() + 28500;
        son->play2D(harlemshake, false);
        int i;
        for (i = 3; i < 13; i++){
            zombie[i]->setFrameLoop(150, 240);
            infozombie[i].vie = 60.0f;
        }
        event[0] = true;
    }

    // Évenement Animation Harlem Shake
    if (event[0] && !event[1] && device->getTimer()->getTime() > harlemshaketimer){
        int i;
        for (i = 3; i < 13; i++){
            if (infozombie[i].vie > 0.0f){
                infozombie[i].gravity = -10;
            }
        }
        event[1] = true;
    }

    // Évenement Fin Harlem Shake
    if (event[0] && !event[2] && device->getTimer()->getTime() > harlemshakefin){
        int i;
        for (i = 3; i < 13; i++){
            if (infozombie[i].vie > 0.0f){
                infozombie[i].vie = 0.0f;
                son->play2D(cri,false);
                zombie[i]->setFrameLoop(240,300);
                zombie[i]->removeAnimators();
                zombie[i]->addAnimator(smgr->createDeleteAnimator(1500));
                zombie[i] = NULL;
            }
        }
        spot[2]->addAnimator(smgr->createCollisionResponseAnimator(mapselector, spot[2], vector3df(1,1,1),vector3df(0,-2,0),vector3df(0,15,0)));
        event[2] = true;
    }

    // Bruit sabre laser
    if (!event[3] && camera->getPosition().Y > 2000.0f){
        int i = 0;
        for(; i<9; i++){
            lumiere[i]->remove();
            lumiere[i] = NULL;
        }
        //lumierecurseur = smgr->addLightSceneNode(curseur,vector3df(vector3df(0,0,0)), SColorf(255,200,0), 500.0f, 1 );
        son->play2D(sabrelaser,false);
        event[3] = true;
    }

    // Lumière rouge
    if (!event[4] && camera->getPosition().Y > 6000.0f){
        smgr->addLightSceneNode(0,vector3df(-6871,774,5621),SColorf(255,55,0),1000.0f,1);
        event[4] = true;
    }

    // Dernière salle
    if (!event[5] && camgravity == -11){
        son->setAllSoundsPaused();
        son->play2D(explosion,false);
        son->play2D(ml606,false);
        event[5] = true;
    }
    if (!event[6] && camera->getPosition().Y < -4900){
        vitesse = vector3df(0,0,0);
        camgravity = -9;
        event[6] = true;
    }

    // Révélations derniers spots bleus quand les golems meurent
    if (!event[7] && !zombie[13] && !zombie[14]){
        int i;
        for (i=7;i<12;i++){
            spot[i]->setPosition(spot[i]->getPosition() + vector3df(0,50000,0));
            spot[i]->setVisible(true);
        }
        lumierebleue->setVisible(true);
        event[7] = true;
    }

    // Salle du boss
    if (!event[8] && camera->getPosition().Y < -8000){
        // OPTIMISATION
        map->setVisible(false);
        laser->setVisible(false);
        eau->setVisible(false);
        int i;
        for (i = 0; i <NBSPOTS; i++)
            spot[i]->setVisible(false);
        for (i = 0; i <NBZOMBIES; i++)
            if (zombie[i])
            zombie[i]->setVisible(false);
        ps->remove();
        // ARCHITECTURE
        torusmesh = smgr->getMesh("torus.3ds");
        torus = smgr->addMeshSceneNode(torusmesh);
        torus->setMaterialTexture(0, driver->getTexture("torus.png"));
        torus->setScale(vector3df(150,150,150));
        torus->setMaterialFlag(EMF_BACK_FACE_CULLING, false);
        torus->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL);
        torus->setPosition(vector3df(-4938.27,-25000,9374.91));
        torus->setMaterialFlag(EMF_LIGHTING, false);
        toruselector = smgr->createTriangleSelector(torusmesh,torus);
        torus->setTriangleSelector(toruselector);
        // MINES
        minemesh = smgr->getMesh("mine.3ds");
        for (i = 0; i < 4; i++){
            mine[i] = smgr->addMeshSceneNode(minemesh);
            mine[i]->setMaterialTexture(0, driver->getTexture("minerouge.png"));
            mine[i]->setScale(vector3df(10,10,10));
            mine[i]->setMaterialFlag(EMF_LIGHTING, false);
        }
        mine[0]->setPosition(vector3df(-4938.27 - 500, -25000, 9374.91));
        mine[1]->setPosition(vector3df(-4938.27 + 500, -25000, 9374.91));
        mine[2]->setPosition(vector3df(-4938.27, -25000, 9374.91 + 500));
        mine[3]->setPosition(vector3df(-4938.27, -25000, 9374.91 - 500));
        // LUMIERES DE LA SALLE DU BOSS
        lumiereviolette[0] = smgr->addVolumeLightSceneNode(0, -1, 64, 64, SColor(0, 255, 255, 255), SColor(0, 0, 0, 0));
        lumiereviolette[0]->setScale(vector3df(6000,1000,6000));
        lumiereviolette[0]->setPosition(vector3df(-4938.27,-25400,9374.91));
        core::array<ITexture*> textures;
        s32 g=7;
        for (; g > 0; --g){
            stringc temp;
            temp = "portal";
            temp += g;
            temp += ".bmp";
            ITexture* texture = driver->getTexture( temp.c_str() );
            textures.push_back(texture);
        }
        ISceneNodeAnimator* glow = smgr->createTextureAnimator(textures, 150);
        lumiereviolette[0]->addAnimator(glow);
        glow->drop();

        // Particules
        ps = NULL;
        ps = smgr->addParticleSystemSceneNode(false);
        IParticleEmitter* em = ps->createBoxEmitter(
                                                    aabbox3d<f32>(-1000,0,-1000,1000,700,1000), // taille
                                                    vector3df(0.0f,0.06f,0.0f),   // direction
                                                    2700,3000, // nombre de particules
                                                    SColor(0,255,255,255),
                                                    SColor(0,255,255,255),
                                                    800,2000,0, // angles
                                                    dimension2df(10.f,10.f),
                                                    dimension2df(20.f,20.f));
        ps->setEmitter(em);
        em->drop();
        IParticleAffector* paf = ps->createFadeOutParticleAffector();
        ps->addAffector(paf);
        paf->drop();
        ps->setPosition(vector3df(-4938.27,-25500,9374.91));
        ps->setScale(vector3df(2,2,2));
        ps->setMaterialFlag(EMF_LIGHTING, false);
        ps->setMaterialFlag(EMF_ZWRITE_ENABLE, false);
        ps->setMaterialTexture(0, driver->getTexture("fire.bmp"));
        ps->setMaterialType(EMT_TRANSPARENT_VERTEX_ALPHA);
        // Mise à jour des collisions
        camera->removeAnimator(animcam);
        animcam->drop();
        animcam = smgr->createCollisionResponseAnimator(toruselector, camera, vector3df(100,100,100),vector3df(0,camgravity,0),vector3df(0,0,0));
        camera->addAnimator(animcam);

        tank->removeAnimator(animtank);
        animtank->drop();

        // Changement de musique
        son->play2D(endtitles,false);
        // Recadrage de la caméra pour éviter de tomber hors de la map à l'arrivée
        camera->setPosition(vector3df(-4938.27,camera->getPosition().Y,9374.91));
        vitesse = vector3df(0,vitesse.Y,0);
        event[8] = true;
    }

    // Étourdissement à l'arrivée dans la salle du boss + apparition boss (le nom est joker mais c'est un golem)
    if (!event[9] && camera->getPosition().Y < -24500){
        joker = smgr->addAnimatedMeshSceneNode(golemesh);
        joker->addAnimator(smgr->createCollisionResponseAnimator(toruselector, joker, vector3df(10,10,10),vector3df(0,-10,0),vector3df(0,50,0)));
        joker->setPosition(vector3df(-4938.27,-19500,9374.91));
        joker->setScale(vector3df(3,2,3));
        IGUIInOutFader* fondu = device->getGUIEnvironment()->addInOutFader();
        fondu->setColor(SColor(0,255,255,255));
        fondu->fadeIn(8000);
        cameraboss = smgr->addCameraSceneNodeFPS(0, 100.0f, 0.5f, -1,keyMap, 5, true, 2.0f);
        cameraboss->setFarValue(100000.0f);
        cameraboss->setPosition(camera->getPosition() + vector3df(800,0,0));
        animcam->drop();
        animcam = smgr->createCollisionResponseAnimator(toruselector, cameraboss, vector3df(50,40,50),vector3df(0,-10,0),vector3df(0,0,0));
        cameraboss->addAnimator(animcam);
        cameraboss->setTarget(camera->getTarget());
        tank->setScale(vector3df(70,70,70));
        camera->setTarget(vector3df(-4938.27,-25500,19374.91));
        camera->setPosition(vector3df(-4938.27,-24800,9374.91));
        camera->removeAnimators();
        vitesse = vector3df(0,0,0);
        son->play2D(striii,false);
        son->play2D(hahaha, false);
        son->removeSoundSource(ml606);
        ml606 = NULL;
        event[9] = true;
    }


    if (event[9])
        if (joker->getPosition().Y > -24900)
            cameraboss->setTarget(joker->getPosition());
    if (event[9])
    {
        camera->setTarget(cameraboss->getPosition());
        joker->setRotation(camera->getRotation() - vector3df(0,90,0));
    }
    // Début du combat contre le boss
    if (!event[10] && event[9]){
        if (joker->getPosition().Y < -24900){
            son->play2D(whysoserious, false);
            IGUIInOutFader* fondu = device->getGUIEnvironment()->addInOutFader();
            fondu->setColor(SColor(200,255,0,255));
            fondu->fadeOut(360000);
            bossshootupdatetime = 0;
            vie = 55;
            oldposcamboss = cameraboss->getPosition();
            event[10] = true;
        }
    }
    // Activation des mines et fondu bleu
    if (!event[11] && mineactivee[0] && mineactivee[1] && mineactivee[2] && mineactivee[3]){
            finaletimer = device->getTimer()->getTime() + 14500;
            lumiereviolette[0]->setVisible(false);
            ps->remove();
            torus->addAnimator(smgr->createRotationAnimator(vector3df(0.005f, 0.1f, 0.003f)));
            IGUIInOutFader* fondu = device->getGUIEnvironment()->addInOutFader();
            fondu->setColor(SColor(0,0,0,255));
            fondu->fadeOut(15000);
            son->play2D(explosion, false);
            son->play2D(fall, false);
            son->removeSoundSource(whysoserious);
            whysoserious = NULL;
            event[11] = true;
    }
    // Fondu blanc
    if (!event[12] && event[11] && device->getTimer()->getTime() > finaletimer - 5000){
            IGUIInOutFader* fondu = device->getGUIEnvironment()->addInOutFader();
            fondu->setColor(SColor(0,255,255,255));
            fondu->fadeOut(5000);
            event[12] = true;
    }

    // Fin du jeu (Explosion des mines)
    if (!event[13] && event[11] && device->getTimer()->getTime() > finaletimer){
        son->setAllSoundsPaused();
        son->play2D(explosion, false);
        son->play2D(lireMusique(device->getFileSystem()->createAndOpenFile("EndTitles.ogg"),son,"End Titles FIN"));
        splashtimer = device->getTimer()->getTime() + 3600;
        event[13] = true;
    }

}









bool Jeu3D::confirmationQuitter()
{
    // On tombe si la gravité était inversée
    if (camgravity == 1){
        camera->removeAnimator(animcam);
        animcam->drop();
        animcam = smgr->createCollisionResponseAnimator(mapselector, camera, vector3df(100,1,100),vector3df(0,-1,0),vector3df(0,-100,0));
        camera->addAnimator(animcam);
    }
    if (event[10]){
        cameraboss->removeAnimators();
        camera->addAnimator(smgr->createCollisionResponseAnimator(0, cameraboss, vector3df(0,0,0),vector3df(0,-1,0),vector3df(0,0,0)));
    }
    son->play2D(hahaha,false);
    /// Fondu couleur rouge
    IGUIEnvironment *guienv = device->getGUIEnvironment();
    IGUIInOutFader* fondu = guienv->addInOutFader();
    fondu->setColor(SColor(0,255,0,0));
    fondu->fadeOut(4000);
    u32 fin = device->getTimer()->getTime() + 4000;
    // Préparation des boutons Rejouer et Quitter
    int w = driver->getScreenSize().Width, h = driver->getScreenSize().Height;
    IGUIButton *rejouer = guienv->addButton(rect<int>(3*w/4 - 300,h/2 - 50,3*w/4,h/2 + 50),0,-1,L"Rejouer");
    rejouer->setVisible(false);
    IGUIButton *quitter = guienv->addButton(rect<int>(w/4,h/2 - 50,w/4 + 300,h/2 + 50),0,-1,L"Quitter");
    quitter->setVisible(false);

    while (device->getTimer()->getTime() < fin){
        device->run();
        driver->beginScene(true, true, SColor ( 0, 255,255,255 ));
        smgr->drawAll();
        guienv->drawAll();
        driver->endScene();

        // On affiche les boutons vers la fin
        if (device->getTimer()->getTime() > fin - 1000){
            quitter->setVisible(true);
            rejouer->setVisible(true);
        }
    }


    /// Attente du choix de l'utilisateur
    camera->remove();
    camera = NULL;
    device->getCursorControl()->setVisible(true);
    int choix = -1;
    while (choix == -1){
        if (rejouer->isPressed())
            choix = 0;
        if (quitter->isPressed())
            choix = 1;

        device->run();
        driver->beginScene(true, true, SColor ( 0, 255,255,255 ));
        guienv->drawAll();
        driver->endScene();
    }


    // Destruction des donnees du jeu
    smgr->clear();
    guienv->clear();
    son->removeSoundSource(derezzed);
    son->removeSoundSource(cri);
    son->removeSoundSource(tir);
    son->removeSoundSource(harlemshake);
    son->removeSoundSource(spotsound);
    son->removeSoundSource(sabrelaser);
    if (ml606)
        son->removeSoundSource(ml606);
    son->removeSoundSource(canon);
    son->removeSoundSource(explosion);
    son->removeSoundSource(endtitles);
    son->removeSoundSource(striii);
    son->removeSoundSource(hahaha);
    if (whysoserious)
        son->removeSoundSource(whysoserious);
    son->removeSoundSource(fall);
    device->getCursorControl()->setVisible(false);
    return (bool) choix;
}





Jeu3D::~Jeu3D()
{

}
