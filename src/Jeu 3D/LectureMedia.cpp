#include "LectureMedia.h"

int signatureMedia()
{
    int taille;
    FILE *fp = fopen("media","rb");
    if (!fp)
        return -1;
    fseek(fp,0,SEEK_END);
    taille = ftell(fp);
    fclose(fp);

    return taille;
}

ISoundSource* lireMusique(IReadFile *fichier,  ISoundEngine* son, char *id)
{

	void* contenu = malloc(fichier->getSize());
	fichier->read(contenu, fichier->getSize());
	 // recreation des donnees du son
	ISoundSource* tmp = son->addSoundSourceFromMemory( contenu, fichier->getSize(),id,true);
	free(contenu);
	tmp->setDefaultVolume(1.0);
	return tmp ;

};
