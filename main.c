#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "peage.h"
#include "voiture.h"


int main(int argc, char **argv){

    if(argc != 3){// nb peage ouverts et nb voiture
        fprintf(stderr,"ERREUR ARGUMENT\n Veuillez essayer avec un nb de peage et un nb de voiture\n");
        exit(EXIT_FAILURE);
    }
    if(atoi(argv[1])>NB_PDP){
        fprintf(stderr,"ERREUR ARGUMENT\n Veuillez essayer avec un nb de peage inferieur ou egal a %d\n",NB_PDP);
        exit(EXIT_FAILURE);
    }
   int nb_pdp=atoi(argv[1]);
   int nb_voiture=atoi(argv[2]);

    pthread_t thread_voiture[nb_voiture];
    pthread_t thread_pdp[NB_PDP];


    srand(time(NULL));


    // creation des threads peages
    for(int i=0;i<NB_PDP;i++){
        if(pthread_create(thread_pdp+i,0,fct_peage,(void*)i) !=0){
            fprintf(stderr,"ERREUR CREATION THREAD PEAGE\n");
            exit(EXIT_FAILURE);  
        }    
    }



    for(int i=0;i<nb_voiture;i++){


      

        if(pthread_create(thread_voiture+i,0,fct_voiture,(void*)i) !=0){
            fprintf(stderr,"ERREUR CREATION THREAD PEAGE\n");
            exit(EXIT_FAILURE);  
        }  
    }


    for(int i=0;i<nb_voiture;i++){

        pthread_join(thread_voiture[i],NULL);
        
        }  
        return 0;
}

