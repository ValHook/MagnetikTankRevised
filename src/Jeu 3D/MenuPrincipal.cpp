//
//  MenuPrincipal.cpp
//

#include "MenuPrincipal.h"
#include "LectureMedia.h"

MenuPrincipal::MenuPrincipal(IrrlichtDevice *ptrdevice, ISoundEngine* ptrson, DetectionSouris *ptrsouris)
{
    // Initialisation des variables
    device = ptrdevice;
    son = ptrson;
    souris = ptrsouris,
    driver = device->getVideoDriver();
    smgr = device->getSceneManager();

    // Lecture de la musique de fond
    musiquefond = lireMusique(device->getFileSystem()->createAndOpenFile("The Game Has Changed.ogg"),son,"musique-fond");
   // musiquefond->setDefaultVolume(0.7f);
    son->play2D(musiquefond,true);

    // Chargement en mémoire du bruit d'explosion
    explosion = lireMusique(device->getFileSystem()->createAndOpenFile("explosion.wav"),son,"explosion");

    // On charge la carte en 3D avec ses textures
    mapmesh = smgr->getMesh("lobby.3ds");
    map = smgr->addMeshSceneNode(mapmesh->getMesh(0),0,Imprenable);
    map->setScale(vector3df(3000.0,3000.0,3000.0));
    map->setMaterialTexture(0, driver->getTexture("lobby.png"));
    map->setMaterialFlag(EMF_LIGHTING, false);
    map->setMaterialFlag(EMF_BACK_FACE_CULLING, false);
    map->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL);

    // On charge les textes en 3D
    jouer2Dmesh = smgr->getMesh("jouer2D.3ds");
    jouer2D = smgr->addMeshSceneNode(jouer2Dmesh->getMesh(0),0,Prenable);
    jouer2D->setScale(vector3df(3000.02,3000.02,3000.02));
    jouer2D->setPosition(vector3df(0,0,-2000));
    selector = smgr->createTriangleSelectorFromBoundingBox(jouer2D);
    jouer2D->setTriangleSelector(selector);
    anim = smgr->createCollisionResponseAnimator(selector, jouer2D);
    selector->drop();

    jouer3Dmesh = smgr->getMesh("jouer3D.3ds");
    jouer3D = smgr->addMeshSceneNode(jouer3Dmesh->getMesh(0),0,Prenable);
    jouer3D->setScale(vector3df(3000.01,3000.01,3000.01));
    selector = smgr->createTriangleSelectorFromBoundingBox(jouer3D);
    jouer3D->setTriangleSelector(selector);
    anim = smgr->createCollisionResponseAnimator(selector, jouer3D);
    selector->drop();

    creditsmesh = smgr->getMesh("credits.3ds");
    credits = smgr->addMeshSceneNode(creditsmesh->getMesh(0),0,Prenable);
    credits->setScale(vector3df(3000.04,3000.04,3000.04));
    selector = smgr->createTriangleSelectorFromBoundingBox(credits);
    credits->setTriangleSelector(selector);
    anim = smgr->createCollisionResponseAnimator(selector, credits);
    selector->drop();

    quittermesh = smgr->getMesh("quitter.3ds");
    quitter = smgr->addMeshSceneNode(quittermesh->getMesh(0),0,Prenable);
    quitter->setScale(vector3df(3000.03,3000.03,3000.03));
    selector = smgr->createTriangleSelectorFromBoundingBox(quitter);
    quitter->setTriangleSelector(selector);
    anim = smgr->createCollisionResponseAnimator(selector, quitter);
    selector->drop();

    // On crée des éclairages
    smgr->addLightSceneNode( 0, vector3df(2540,5470,11235), SColorf(255,255,255), 10000.0f, 1 );
    smgr->addLightSceneNode( 0, vector3df(648,1514,1024), SColorf(255,255,255), 5000.0f, 1 );
    smgr->addLightSceneNode( 0, vector3df(3758,3814,-1920), SColorf(255,255,255), 5000.0f, 1 );
    smgr->addLightSceneNode( 0, vector3df(5888,4814,1703), SColorf(255,255,255), 5000.0f, 1 );
    smgr->addLightSceneNode( 0, vector3df(364,1514,642), SColorf(255,255,255), 5000.0f, 1 );
    smgr->addLightSceneNode( 0, vector3df(6300,4814,1403), SColorf(255,255,255), 5000.0f, 1 );
    lumieresrouges = NULL;



    // On crée une caméra
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
    camera= smgr->addCameraSceneNodeFPS(0, //parent
                                                          100.0f, //vitesse de rotation
                                                          0.7f, //vitesse de déplacement
                                                          -1, //id
                                                          keyMap, //keymap
                                                          5, //keymapsize
                                                          true, //mouvement vertical
                                                          6.5f //force de saut
                                                          );
    camera->setFarValue(2000000);
    camera->setPosition(vector3df(-29.0,2109.0,-927.0));
    camera->setTarget(vector3df(1750.0,0,-300.0));
    selector = smgr->createTriangleSelector(mapmesh,map);
    map->setTriangleSelector(selector);
    // On lui applique les lois physiques régies par la carte
    anim = smgr->createCollisionResponseAnimator(selector, camera, vector3df(80,160,80),vector3df(0,-10,0),vector3df(0,120,0));
    selector->drop();
    camera->addAnimator(anim);



    // On crée le ciel
    skyboxcredits=smgr->addSkyBoxSceneNode(
                                                       driver->getTexture("ece.png"),
                                                       driver->getTexture("ece.png"),
                                                       driver->getTexture("noir.png"),
                                                       driver->getTexture("noir.png"),
                                                       driver->getTexture("valentin.png"),
                                                       driver->getTexture("arthur.png"));
    skybox=smgr->addSkyBoxSceneNode(
                                                       driver->getTexture("up.png"),
                                                       driver->getTexture("down.png"),
                                                       driver->getTexture("left.png"),
                                                       driver->getTexture("right.png"),
                                                       driver->getTexture("face.png"),
                                                       driver->getTexture("back.png"));

    // Image du curseur de la souris
    curseur = smgr->addBillboardSceneNode();
	curseur->setMaterialType(EMT_TRANSPARENT_ADD_COLOR );
	curseur->setMaterialTexture(0, driver->getTexture("particle.bmp"));
	curseur->setMaterialFlag(EMF_LIGHTING, false);
	curseur->setMaterialFlag(EMF_ZBUFFER, false);
	curseur->setSize(dimension2d<f32>(40.0f, 40.0f));
	curseur->setID(Imprenable);


    // On déclare les dernières ressources nécéssaires
    collMan = smgr->getSceneCollisionManager();
    zoomtexture = 0.55f;
    vitessetexture = -1.0f;
    explosionbool = 0;
    optionchoisie = 0;
    oncredits = 0;
}




void MenuPrincipal::ajouterLumieresRouges()
{
    int i;
    if (!lumieresrouges){
        map->setMaterialFlag(EMF_LIGHTING, true);
        lumieresrouges = (ILightSceneNode **) malloc(100*sizeof(ILightSceneNode*));
        for (i=0; i <5; i++)
            lumieresrouges[i] = smgr->addLightSceneNode(0,vector3df(500+1000*i,2000,500+1000*i),SColorf(255,0,0),1000000.0f,i);
    }
    // Lumière faible et figée si crédits activés
    if (oncredits)
        zoomtexture = 0.01;
}



void MenuPrincipal::supprimerLumieresRouges()
{
    int i;
    if (lumieresrouges){
        map->setMaterialFlag(EMF_LIGHTING, false);
        for (i=0; i <5; i++)
            lumieresrouges[i]->remove();
        free(lumieresrouges);
        lumieresrouges = NULL;
    }
}




void MenuPrincipal::afficher()
{
    // On donne la main au jeu
    device->run();
    // Si la fenêtre est active
    if (device->isWindowActive())
    {


        // Positionnement des lumières bleues
        if (zoomtexture>=4.0f)
            vitessetexture=-1.0f; explosionbool = 0;
        if(vitessetexture == -1.0f && zoomtexture <= 0.02f)
            vitessetexture = -0.03f;
        if(vitessetexture == -1.0f && zoomtexture <= 0.007f)
            vitessetexture = -0.0000005f;
        if (zoomtexture<=-0.05f)
            vitessetexture=2.0f;
        if (driver->getFPS() > 1)
            zoomtexture+= vitessetexture*0.0005f*110/driver->getFPS();



        // Applications des motifs bleus
        smgr->getMeshManipulator()->makePlanarTextureMapping(mapmesh->getMesh(0), -zoomtexture);



        // On joue un bruit d'explosion si les lumières sont en train de disparaître
        if (!explosionbool && vitessetexture == -1.0f && zoomtexture<= 0.02f){
            son->play2D(explosion,false);
            explosionbool = 1;
        }




        // On détermine si on pointe sur du texte avec un lancer de rayon
        ray.start = camera->getPosition();
        ray.end = ray.start + (camera->getTarget() - ray.start).normalize() * 100000.0f;
        texteChoisi = collMan->getSceneNodeAndCollisionPointFromRay(
                                                                                       ray,
                                                                                       intersection, // Position de la collision
                                                                                       hitTriangle, // Face visée
                                                                                       Prenable, // On s'assure que le rayon traverse la carte
                                                                                       0);


        // Animation lumière rouge si texte pointé ou credits activés
        if(texteChoisi || oncredits)
                ajouterLumieresRouges();
        else
                supprimerLumieresRouges();




        // Projection du curseur bleu de la souris
        if(!texteChoisi)
        collMan->getSceneNodeAndCollisionPointFromRay(
                                                      ray,
                                                      intersection, // Position de la collision
                                                      hitTriangle, // Face visée
                                                      Imprenable, // On s'assure que le rayon ne traverse pas la carte si aucun element n'est choisi
                                                      0);
        device->getCursorControl()->setVisible(false);
        curseur->setPosition(intersection);






        /// Détéction clic souris
        bool clic;
        if (texteChoisi && souris->GetMouseState().LeftButtonDown){


            // Option Jouer en 3D
            if (texteChoisi->getScale() == vector3df(3000.01,3000.01,3000.01))
                optionchoisie = 1;


            // Option Jouer en 2D
            if (texteChoisi->getScale() == vector3df(3000.02,3000.02,3000.02))
                optionchoisie = 2;


            // Option Crédits
            if (texteChoisi->getScale() == vector3df(3000.04,3000.04,3000.04) && !clic){
                if (skybox->isVisible()){
                    // On masque le ciel pour afficher les crédits cachés derrière
                    skybox->setVisible(false);
                    oncredits = 1;
                }
                else {
                    // On remet le ciel
                    skybox->setVisible(true);
                    oncredits = 0;
                }
            }


            // Option Quitter
            if (texteChoisi->getScale() == vector3df(3000.03,3000.03,3000.03))
                optionchoisie = 3;


            clic = 1;
        }
        else
            clic = 0;








        // On met le buffer à l'écran
        driver->beginScene(true, true, SColor(255,200,200,200));
        smgr->drawAll();
        driver->endScene();

    }

    // Sinon on réduit la consommation CPU du jeu
    else
        device->yield();

}



int MenuPrincipal::choix()
{
    return optionchoisie;
}

void MenuPrincipal::tomber()
{
    // Supression des collisions contre les murs
    camera->removeAnimator(anim);

    // Réapplication d'une nouvelle gravité
    anim = smgr->createCollisionResponseAnimator(smgr->createTriangleSelectorFromBoundingBox(jouer3D), camera, vector3df(80,160,80),vector3df(0,-3,0),vector3df(0,120,0));
    camera->addAnimator(anim);
    anim->drop();
    // On supprime le ciel
    skybox->remove();
    skyboxcredits->remove();

    // Suppression de la musique
    son->removeSoundSource(musiquefond);

    // On joue une musique d'intro
    ISoundSource *intro = lireMusique(device->getFileSystem()->createAndOpenFile("Intro.ogg"),son,"Intro");
    son->play2D(intro,false);

    float i = camera->getAbsolutePosition().Y - 14000.0;
    camera->setFarValue(-i -2000.0);

    // On effectue une chute de 17000.00 mètres
    while (camera->getAbsolutePosition().Y > i){
        device->run();
        if (camera->getAbsolutePosition().Y > 0)
            driver->beginScene(true, true, SColor(255,255,255,255));
        else
            driver->beginScene(true, true, SColor(255,255+camera->getAbsolutePosition().Y/(-i)*255,255+camera->getAbsolutePosition().Y/(-i)*255,255+camera->getAbsolutePosition().Y/(-i)*255));
        smgr->drawAll();
        driver->endScene();
    }
    son->play2D(explosion);
    son->removeSoundSource(intro);
}

MenuPrincipal::~MenuPrincipal()
{

}

