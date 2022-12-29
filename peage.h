#ifndef PEAGE_H
#define PEAGE_H
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include<semaphore.h>

#include "voiture.h"
#define NB_PDP 15// nb de postes de péage
#define NB_TEL NB_PDP/4// nb de télépeage

extern bool stop_thread;
extern bool state_peage[NB_PDP];

void reveiller_peage(int);
void initializer(int);
void  Peage(int);
void *fct_peage(void *);
void Voiture(int);
void *fct_voiture(void *);
float moyenne_voit();
int choix_pdp(vehicule);




#endif /* PEAGE_H */