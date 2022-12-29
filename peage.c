#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include "peage.h"
#include "voiture.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpointer-to-int-cast" //pour les warnings de conversion de pointeur en int

pthread_mutex_t mutex[NB_PDP];
pthread_cond_t attente_peage[NB_PDP], attente_voiture[NB_PDP];

int nb_voiture_attente[NB_PDP];
bool stop_thread=false;
bool state_peage[NB_PDP];


//fonction peage
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

//fonction pour reveiller le peage dans le cas ou il n'y a plus de voiture
void reveiller_peage(int id){
    if(stop_thread){
        pthread_cond_signal(&attente_peage[id]);
    }
}

void initialiser_peages(int N) {
  // Initialiser tous les booléens à true
  for (int i = 0; i < NB_PDP; i++) {
    state_peage[i] = true;
  }
    int index;
  // Générer N entiers aléatoires compris entre 0 et NB_PDP-1
  while(N!=0){
    index = rand() % NB_PDP;
    if(state_peage[index]){
        state_peage[index] = false;
        N--;
    }
  }
  for (int i=0;i<NB_PDP;i++){printf("%d ,",state_peage[i]);}
} 

//fonction voiture
void Voiture(int idVehicule){
    vehicule v=creer_vehicule(rand()%4+1);
    int pdp_id=choix_pdp(v);
    //afficher_vehicule(v);
    pthread_mutex_lock(&mutex[pdp_id]);
    nb_voiture_attente[pdp_id]++;
    pthread_cond_signal(&attente_peage[pdp_id]);//on reveille le peage
    printf("La voiture %s de classe %d attend au peage %d\n",v.immatriculation, v.classe,pdp_id);
    pthread_cond_wait(&attente_voiture[pdp_id],&mutex[pdp_id]);//on attend le peage
    printf("La voiture %s passe au peage %d\n",v.immatriculation,pdp_id);
    nb_voiture_attente[pdp_id]--;
    sleep(1);
    pthread_mutex_unlock(&mutex[pdp_id]);
}


//fonction thread peage
void *fct_peage(void * id){
    nb_voiture_attente[(int)id]=0;
    bool running=true;
    while(running){
        Peage((int)id); 
        sleep(1);
        running=!stop_thread;
    }    
    pthread_exit(NULL);
}


//fonction thread voiture
void *fct_voiture(void * arg){
    Voiture((int)arg);
    sleep (2);
    pthread_exit(NULL);
}

//fonction choix du poste de peage
int choix_pdp(vehicule v){
    int peage=rand()%NB_PDP;

    while(!state_peage[peage]){
        peage=rand()%NB_PDP;
    }

    bool hdp=moyenne_voit()>NB_PDP/2;

    if(hdp){
        printf("HEURE DE POINTE\n");
        if(((v.passager>=2 && v.classe!=4 )|| v.critair || v.taxi)){//si la voiture a plus de 2 passagers et lourd ou est un taxi ou est critair
            printf("On peut aller sur la voie de covoit\n");
        }else{
            if(peage!=NB_PDP-1){//si le peage n'est pas le dernier
                if(state_peage[peage+1]){
                    peage++;//on ne peut pas aller sur la voie covoiturage 
                }
            }
        }
    }

    if(v.telepeage){

        return peage;
    }
    else{ 

    bool bool1=false;
    bool bool2=false;

    if (NB_TEL+peage < NB_PDP){
        bool1 = true;
    }

    if(state_peage[peage+NB_TEL]){
        bool2 = true;
    }

    while(!bool1 && !bool2){
        peage=rand()%NB_PDP;

        if (NB_TEL+peage < NB_PDP){
            bool1 = true;
        }

        if(state_peage[peage+NB_TEL]){
            bool2 = true;
        }

    }

        return peage;
        
    }
}
    

//fonction moyenne des voitures en attente
float moyenne_voit(){
    int somme=0;
    for(int i=0;i<NB_PDP;i++){
        somme+=nb_voiture_attente[i];
    }
    return (float)somme/NB_PDP;
}

#pragma GCC diagnostic pop