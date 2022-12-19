#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "peage.h"
#include "voiture.h"
int main(){

    pthread_t thread_voiture[NB_VOITURE];
    pthread_t thread_pdp[NB_PDP];

    // creation des threads peages
    for(int i=0;i<NB_PDP;i++){
        if(pthread_create(thread_pdp+i,0,fct_peage,(void*)i) !=0){
            fprintf(stderr,"ERREUR CREATION THREAD PEAGE\n");
            exit(EXIT_FAILURE);  
        }    
    }

    vehicule_pdp ** vpdp;
    vpdp =(vehicule_pdp **)malloc(sizeof(vehicule_pdp)*NB_VOITURE);

    for(int i=0;i<NB_VOITURE;i++){

        vpdp[i] = malloc(sizeof(vehicule_pdp));

        int classe=rand()%4+1; // Génération aléatoire de la classe de la voiture
        vpdp[i]->v=creer_vehicule(classe);
        vpdp[i]->pdp_id=rand()%NB_PDP;

        if(pthread_create(thread_voiture+i,0,fct_voiture,vpdp[i]) !=0){
            fprintf(stderr,"ERREUR CREATION THREAD PEAGE\n");
            exit(EXIT_FAILURE);  
        }  
    }


    for(int i=0;i<NB_VOITURE;i++){

        pthread_join(thread_voiture[i],NULL);
        
        }  

    for(int i=0;i<NB_VOITURE;i++){

       free(vpdp[i]);
        
        }   
        free(vpdp);


        return 0;
}

