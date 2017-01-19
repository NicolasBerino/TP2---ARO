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

parametre* initialiserParam(char* nomFic){
	int compteur = 0;
	FILE* f = fopen(nomFic,"r");
	parametre* param = malloc(sizeof(parametre));
	char c;
	int poidsMax = 0;

	fscanf(f,"%d",&poidsMax);
	do{
		c=fgetc(f);
		if(c=='\n')compteur ++;
	}while(c!=EOF);

	param->nbObjet = compteur-2;
	param->poidsMax = 0;

	
	fclose(f);

	param->poidsMax = poidsMax;
	return param;
}

objet* remplirTab(char* nomFic,int nbObjet){

	FILE* f = fopen(nomFic,"r");
	objet* objets = malloc(sizeof(objet)*(nbObjet));
	
	int poidsMax = 0;
	fscanf(f,"%d",&poidsMax);

	int compteur = 0;
	
	while(!feof(f)){
		float poids,valeur;
		fscanf(f,"%f %f",&poids,&valeur);
		objet* obj = malloc(sizeof(objet));
		obj->poids=poids;
		obj->prix=valeur;
		obj->nom=compteur;
		obj->pris = 0;
		objets[compteur]=*obj;
		compteur ++;
	}

	fclose(f);
	return objets;
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
float choisirObjet(objet* obj, float poidsRestant){ // AJOUTER LA POSSSIBILITE DE COUPER LES OBJETS
	float poidsDispo = poidsRestant;
	if(obj->poids<=poidsDispo){
		obj->pris = 1 ;
		poidsDispo = poidsDispo-obj->poids;
	}
	else{
		obj->pris = 0;
	}
	return poidsDispo;
}


resoudreProbleme(objet* objets, float poidsMax, int nbObjet){
	float poidsRestant = poidsMax;
	int i = 0;

	for(i=0;i<nbObjet;i++){
		if(poidsRestant <=0) {break;}
		poidsRestant=choisirObjet(&objets[i],poidsRestant);
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

	param = initialiserParam(argv[1]);

	listeObjets = remplirTab (argv[1], param->nbObjet); // remplir le tableau des objets	

	int i;
	for(i=0;i<param->nbObjet;i++){

		printf("%f %f\n",listeObjets[i].poids,listeObjets[i].prix);
	}
	printf("\n");

	tri_rapide (listeObjets,param->nbObjet); //trier le tableau en fonction des ratios

	for(i=0;i<param->nbObjet;i++){

		printf("%f %f %f\n",listeObjets[i].poids,listeObjets[i].prix,listeObjets[i].poids/listeObjets[i].prix );
	}


	resoudreProbleme (listeObjets, param->poidsMax, param->nbObjet); //choisir les objets pris*/

	afficherSolution (listeObjets, param->nbObjet); //afficher la solution

}