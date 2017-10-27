#ifndef __Lecture_Media_
#define __Lecture_Media_

#include <iostream>
using namespace std;

#include <irrlicht.h>
using namespace irr;
using namespace core;
using namespace io;
using namespace video;
using namespace scene;

#include <irrKlang.h>
using namespace irrklang;

int signatureMedia();
ISoundSource* lireMusique(IReadFile *fichier,  ISoundEngine* son, char *id);
#endif
