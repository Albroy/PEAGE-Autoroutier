#ifndef VOITURE_H
#define VOITURE_H

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#define NB_voiture 10

typedef  struct vehicules { // Structure de base d'un véhicule
	int siege;
	int passager;
	int classe;
    bool telepeage;
	char immatriculation[9];
	bool critair;
	bool taxi;
	int prix_ticket;
}vehicule;


vehicule creer_vehicule(int);
void afficher_vehicule(vehicule);
void creer_immatriculation(char *);

#endif /* VOITURE_H */
