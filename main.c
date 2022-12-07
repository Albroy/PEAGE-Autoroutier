#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "peage.h"
#include "voiture.h"
int main(){

    pthread_t tid[NB_VOITURE+NB_PDP];
    int num;
    //printf("test");

    // creation des threads peages
    for(num=0;num<NB_PDP;num ++){
        pthread_create(tid+num,0,(void *(*)())fct_peage,(void*)num);    
    }
    vehicule_pdp * v;
    v =malloc(sizeof(vehicule_pdp));
    //creation des threads voitures
    //sleep(1);
    for(num=NB_PDP;num<NB_VOITURE+NB_PDP;num ++){
            v->num=num;
            v->id=rand()%NB_PDP;
        
        if(pthread_create(tid+num,0,fct_voiture,v) !=0){
            fprintf(stderr,"ERREUR CREATION THREAD\n");
            exit(EXIT_FAILURE);  
        }
    }

    //attend la fin de toutes les threads voitures
    for(num=0;num<NB_VOITURE+NB_PDP;num ++)
            pthread_join(tid[num],NULL);

    /* liberation des ressources");*/
        return 0;
}

