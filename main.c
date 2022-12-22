#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "peage.h"
#include "voiture.h"
int main(){

    pthread_t thread_voiture[NB_VOITURE];
    pthread_t thread_pdp[NB_PDP];


    srand(time(NULL));


    // creation des threads peages
    for(int i=0;i<NB_PDP;i++){
        if(pthread_create(thread_pdp+i,0,fct_peage,(void*)i) !=0){
            fprintf(stderr,"ERREUR CREATION THREAD PEAGE\n");
            exit(EXIT_FAILURE);  
        }    
    }



    for(int i=0;i<NB_VOITURE;i++){


      

        if(pthread_create(thread_voiture+i,0,fct_voiture,(void*)i) !=0){
            fprintf(stderr,"ERREUR CREATION THREAD PEAGE\n");
            exit(EXIT_FAILURE);  
        }  
    }


    for(int i=0;i<NB_VOITURE;i++){

        pthread_join(thread_voiture[i],NULL);
        
        }  




        return 0;
}

