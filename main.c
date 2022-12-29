#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "peage.h"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wint-to-pointer-cast" //pour les warnings de conversion de int en pointeur

int main(int argc, char **argv){
    if(argc != 3){// nb peage ouverts et nb voiture
        fprintf(stderr,"ERREUR ARGUMENT\n Veuillez essayer avec un nb de peage et un nb de voiture\n");
        exit(EXIT_FAILURE);
    }
    if(atoi(argv[1])>NB_PDP-1 ){
        fprintf(stderr,"ERREUR ARGUMENT\n Veuillez essayer avec un nb de peage inferieur ou egal a %d\n",NB_PDP-1);
        exit(EXIT_FAILURE);
    }
   initialiser_peages(atoi(argv[1]));
   printf("%d\n",state_peage[0]);
   printf("%d\n",atoi(argv[1]));
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



    for(int i=0;i<nb_voiture;i++){ // creation des threads voiture
        if(pthread_create(thread_voiture+i,0,fct_voiture,(void*)i) !=0){
            fprintf(stderr,"ERREUR CREATION THREAD PEAGE\n");
            exit(EXIT_FAILURE);  
        }  
    }
    printf("%f\n",moyenne_voit());

    for(int i=0;i<nb_voiture;i++){ // on attend la fin des threads voiture
        pthread_join(thread_voiture[i],NULL);
        }  

    stop_thread=true;

    for(int i=0;i<NB_PDP;i++){
        reveiller_peage(i);//on reveille les threads peage endormis
        pthread_join(thread_pdp[i],NULL); // on attend la fin des threads peage
        }
    printf("Bonne terminaison\n");
    return 0;


}
#pragma GCC diagnostic pop

