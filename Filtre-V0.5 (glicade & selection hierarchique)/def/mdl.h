#pragma once

#include "marchee.h"

//
//	Type de couche:
//		<Filtrique>
//		<Neuronale>
//
//	Chaque couche se connecte (depuis cette version V0.5) automatiquement a la couche precedante
//
//	Les couches :
//		0 - fltr prixs
//		1 - cond
//

#define C_max 10
#define Y_max 50

#define N0_max 8
#define N1_max 3

#define N_MAX MAX2(N0_max, N1_max)

#define POIDS_COND(n) (1<<n)	//  Si il y a n valeurs, il y a donc 2*2*2*2...*2 n fois combinaisons posibles
#define CONSTES_FLTR(n) n 		//

#define P0_max 0
#define P1_max POIDS_COND(N1_max)

#define C0_max CONSTES_FLTR(N0_max)
#define C1_max 0

#define POIDS_max MAX2(P0_max, P1_max)
#define CONSTE_max MAX2(C0_max, C1_max)

typedef struct {
	//	Config dimentions & entree de la pile
	uint C;
	uint y[C_max], n[C_max], type[C_max];
	uint ema[Y_max], intervalles[Y_max], glisser[Y_max];	//0=fltr classique,   i>0=max(fltr(x-i) for i in range(glissade))

	//	Listes config -> mdl
	//uint le_pas[C_max];	//le pas de progressition que prend la couche neu_cond dans les y. (1<le_pas<10)

	//	Espace de Parametres & Calcules
	uint constes, poids, vars;
	float conste[Y_max*CONSTE_max], poid[Y_max*POIDS_max], var[Y_max*C_max];

	//	Depart de chaque couche dans les espaces
	uint conste_depart[C_max], poid_depart[C_max], y_depart[C_max];
} Mdl_t;

//	Allocation Memoire
Mdl_t * cree_mdl(uint C, uint * y, uint * n, uint * type);
void liberer_mdl(Mdl_t * mdl);
Mdl_t * copier_mdl(Mdl_t * mdl);

//  Controle
uint meme_mdl(Mdl_t * A, Mdl_t * B);
uint hash_mdl(Mdl_t * mdl);

//	Memoire dure
void ecrire_mdl(Mdl_t * mdl, char * file);
Mdl_t * lire_mdl(char * file);

//	Plume
void plume_mdl(Mdl_t * mdl);
void comportement(Mdl_t * mdl);

//	Qlqs fonctions directes
float poid_cond_rnd();	//{-1,-.75,-.50,-.25,0,+.25,+.5,+.75,+1}

//	Calcule f(x)
float f(Mdl_t * mdl, uint depart);