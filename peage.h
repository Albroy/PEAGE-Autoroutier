#ifndef PEAGE_H
#define PEAGE_H
#include<stdbool.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include<semaphore.h>

#include "voiture.h"
#define NB_PDP 5// nb de postes de péage



void  Peage(int);
void *fct_peage(void *);
void voiture(int, int);
void *fct_voiture(void *);
int temps_passage(int);
#endif /* PEAGE_H */