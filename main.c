#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "peage.h"
#include "voiture.h"

int main(){
    vehicule v=creer_vehicule(1);
    afficher_vehicule(v);
    return 0;
}