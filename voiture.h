#include <stdlib.h>
#include <stdio.h>
#include <time.h>

typedef  struct vehicule { // Structure de base d'un véhicule
	int siege;
	int passager;
	int classe;
}vehicule;

vehicule creer_vehicule(int);
void afficher_vehicule(vehicule);