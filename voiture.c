#include <stdlib.h>
#include <stdio.h>
#include <time.h>


#include "voiture.h"




vehicule creer_vehicule(int classe){
	if(classe<1 || classe>5){
		printf("Erreur de classe");
		exit(-1);
	}
	vehicule v;
	v.classe=classe;
	switch (classe){
		case 1://véhicule léger
			v.siege=4;
			break;
		case 2://véhicule intermédiaire
			v.siege=7;
			break;
		case 3://poids lourds, autocars et autres véhicules à 2 essieux
			v.siege=10;
			break;
		case 4://poids lourds et autres véhicules à 3 essieux et plus
			v.siege=15;
			break;
		case 5://motos, sidecars et trikes
			v.siege=2;
			break;
	}
	srand(time(NULL));
	v.passager=rand()%v.siege; // Génération aléatoire du nombre de passagers
	return v;
}
void afficher_vehicule(vehicule v){
	printf("Nombre de passagers : %d\n",v.passager);
	printf("Nombre de sièges : %d\n",v.siege);
	printf("Classe : %d\n",v.classe);
}

