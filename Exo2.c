#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


typedef struct{
	int nom;
	float prix;
	float poids;
	int pris; // boolean
}objet;

typedef struct{
	int poidsMax;
	int nbObjet;
}parametre;

parametre* remplirTab(objet* objets, char* nomFic){

	parametre* param = malloc(sizeof(parametre));
	int poids,valeur,compteur = 0;
	FILE* f = fopen(nomFic,"r");

	char c;

	while(feof(f)){
		c=fgetc(f);
		if(c=='\n')compteur ++;
	}

	param->nbObjet = compteur-1;
	

	objets = malloc(sizeof(objet)*(compteur-1));
	rewind(f);


	fscanf(f,"%d",param->poidsMax);

	compteur = 0;
	while(feof(f)){
		fscanf(f,"%d %d",&poids,&valeur);
		objet* obj = malloc(sizeof(objet));
		obj->poids=poids;
		obj->prix=valeur;
		obj->nom=compteur+1;
		objets[compteur]=*obj;
		compteur ++;
	}

	fclose(f);
	return param;
}


//Fonctions de tri
 
void permute(int i, int j, objet* array) {
    objet tmp = array[i];
    array[i] = array[j];
    array[j] = tmp;
}

void placer_pivot(int g, int d, objet* array, int *p)
{
    int i;
    float pivot = array[g].prix/array[g].poids;

    *p = g;
    for (i = g + 1; i <= d; i++)
    {
        if (array[i].prix/array[i].poids < pivot) {
            (*p)++;
            if (i != *p) permute(i, *p, array);
        }
    }
    permute(*p, g, array);
}

void tri_rapide_back(int g, int d, objet* array)
{
    int p;
    if (g < d) {
        placer_pivot(g, d, array, &p);
        tri_rapide_back(g, p - 1, array);
        tri_rapide_back(p + 1, d, array);
    }
}

void tri_rapide(objet* array, int arraysize) {
    int debut = 0;
    tri_rapide_back(debut, arraysize - 1, array);
}

// fin fonction de tri

resoudreProbleme(objet* objets, float poidsMax, int nbObjet){
	float poidsRestant = poidsMax;
	int i = 0;

	for(i=0;i<nbObjet;i++){
		if(poidsRestant <=0) {break;}
		choisirObjet(&objets[i],&poidsRestant);
	}
}

choisirObjet(objet* obj, float* poidsDispo){
	if(obj->poids<=*poidsDispo){
		obj->pris = 1 ;
		*poidsDispo -= obj->poids;
	}
	else{
		obj->pris = 0;
	}
}

afficherSolution(objet* objets, int nbObjet){
	int i = 0;
	float prixVol = 0;
	printf("Butin :\n");
	for(i=0;i<nbObjet;i++){
		if(objets[i].pris==1){
			prixVol += objets[i].prix;
			printf("-%d\n", objets[i].nom );
		}
	}
	printf("Prix total : %f\n",prixVol);
}

int main(int argc, char *argv[])
{
	if(argc < 2){perror("T'es pas bon");exit(1);}


	parametre* param;
	objet* listeObjets;


	param = remplirTab (listeObjets, argv[1]); // remplir le tableau des objets

	tri_rapide (listeObjets,param->nbObjet); //trier le tableau en fonction des ratios

	resoudreProbleme (listeObjets, param->poidsMax, param->nbObjet); //choisir les objets pris

	afficherSolution (listeObjets, param->nbObjet); //afficher la solution
}