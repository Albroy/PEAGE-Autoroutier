#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include<semaphore.h>

#include "voiture.h"
#define NB_PDP 5// nb de postes de péage
//sem_t semaphore[NB_PDP]; // tableau de sémaphores


