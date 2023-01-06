#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include "peage.h"
#include "voiture.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpointer-to-int-cast" //pour les warnings de conversion de pointeur en int

pthread_mutex_t mutex[NB_PDP],sommemutex;
pthread_cond_t attente_peage[NB_PDP], attente_voiture[NB_PDP];

int nb_voiture_attente[NB_PDP];
bool stop_thread=false;
bool state_peage[NB_PDP];
int benefice[NB_PDP];


//fonction peage
void Peage(int id){
    pthread_mutex_lock(&mutex[id]);
    //printf("%d -> %d\n",nb_voiture_attente[id],id);
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

    printf("\nLISTE DES PEAGES OUVERT");

  for (int i=0;i<NB_PDP;i++){printf("|%d",state_peage[i]);}
    printf("|\n");
} 

//fonction voiture
void Voiture(int idVehicule){
    vehicule v=creer_vehicule(rand()%4+1);
    pthread_mutex_lock(&sommemutex);
    int pdp_id=choix_pdp(v);
    //afficher_vehicule(v);
    pthread_mutex_lock(&mutex[pdp_id]);    
    nb_voiture_attente[pdp_id]++;
    pthread_mutex_unlock(&sommemutex);

    //printf("nombre de voiture au peage %d -> %d \n",pdp_id,nb_voiture_attente[pdp_id]);
    pthread_cond_signal(&attente_peage[pdp_id]);//on reveille le peage
    printf("La voiture %s (%d) de classe %d (telepeage=%d) attend au peage %d\n",v.immatriculation,idVehicule, v.classe,v.telepeage,pdp_id);
    attente(v);
    pthread_mutex_unlock(&mutex[pdp_id]);

    pthread_mutex_lock(&mutex[pdp_id]);
    pthread_cond_wait(&attente_voiture[pdp_id],&mutex[pdp_id]);//on attend le peage
    printf("La voiture %s (%d) passe au peage %d\n",v.immatriculation,idVehicule,pdp_id);
    if(v.telepeage && pdp_id <= NB_TEL){
        benefice[pdp_id]+=1; 
    }else{
        benefice[pdp_id]+=5;
    }

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
    printf("BENEFICE SUR LA VOIE %d = %d\n",(int)id,benefice[(int)id]);
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

    bool hdp=moyenne_voit()>NB_PDP/3;

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
        if(state_peage[peage+NB_TEL]){
            bool2 = true;
        }
    }
    


    while(!bool1 || !bool2 || !state_peage[peage] || peage ==0){//erreur de condition
        peage=rand()%NB_PDP;

        if (NB_TEL+peage < NB_PDP){
            bool1 = true;
            if(state_peage[peage+NB_TEL]){
                bool2 = true;
            }else{//erreur de condition
                bool1 = false;
                bool2 = false;
            }
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
    //printf("Somme %d\n",somme);

    return (float)somme/NB_PDP;
}

void attente(vehicule v){

    int temps_attente=0;

	switch (v.classe){
		case 1://véhicule léger
            temps_attente=rand()%2; //attend entre 0 ou 1 min
			break;
		case 2://véhicule intermédiaire
            temps_attente=rand()%3+1; //attend entre 1 et 3 min
			break;
		case 3://poids lourds, autocars et autres véhicules à 2 essieux
			temps_attente=rand()%4+2; //attend entre 2 et 5 min
			break;
		case 4://poids lourds et autres véhicules à 3 essieux et plus
            temps_attente=rand()%5+2; //attend entre 2 et 6 min
			break;
		case 5://motos, sidecars et trikes
            temps_attente=rand()%2; //attend entre 0 ou 1 min
			break;
	}

    if(!v.telepeage){
        temps_attente++; //vehicule sans telepeage attendent une minute de plus
    }

    sleep(temps_attente);
}

#pragma GCC diagnostic pop