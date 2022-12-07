#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#include "peage.h"
#include "voiture.h"

pthread_mutex_t mutex[NB_PDP];
pthread_cond_t attente_peage[NB_PDP], attente_voiture[NB_PDP];

int nb_voiture_attente[NB_PDP];



void Peage(int id){
    pthread_mutex_lock(&mutex[id]);
    printf("%d -> %d\n",nb_voiture_attente[id],id);
    if(nb_voiture_attente[id]>0){//si il y a des voitures en attente
        printf("Le peage %d dit a une voiture de venir\n",id);
        pthread_cond_signal(&attente_voiture[id]);// on reveille la prochaine voiture
        printf("La voiture passe au peage %d\n",id);
        sleep(1);
    }
    else{
        printf("Le peage %d attend une voiture\n",id);
        pthread_cond_wait(&attente_peage[id],&mutex[id]);//on attend une voiture 
    }
    pthread_mutex_unlock(&mutex[id]);
}

void Voiture(int id, int num_voiture){
    int classe=rand()%5; // Génération aléatoire de la classe de la voiture
    vehicule v=creer_vehicule(classe);
    //afficher_vehicule(v);

    pthread_mutex_lock(&mutex[id]);
    nb_voiture_attente[id]++;
    pthread_cond_signal(&attente_peage[id]);//on reveille le peage
    printf("La voiture %d de classe %d attend au peage %d\n",num_voiture, v.classe,id);
    pthread_cond_wait(&attente_voiture[id],&mutex[id]);//on attend le peage
    printf("La voiture %d passe au peage %d\n",num_voiture,id);
    nb_voiture_attente[id]--;
    sleep(1);
    pthread_mutex_unlock(&mutex[id]);
}



void *fct_peage(void * id){
    nb_voiture_attente[(int)id]=0;
    while(1){
        Peage((int)id); 
        sleep(1);
    }    
}
void *fct_voiture(void * arg){
    vehicule_pdp * vpdp = (vehicule_pdp *) arg;
    int num = vpdp->num;
    int id = vpdp->id;
    Voiture(id,num);
    sleep (2);
}

