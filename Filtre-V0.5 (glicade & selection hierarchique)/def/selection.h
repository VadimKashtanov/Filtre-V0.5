#include "mdl.h"

//	Le gagant se conserve, le perdant copie le gagant
//	puis mute selon des regles definie dans `Env_t`
typedef struct  {
	//  %[0;1] de chance que P mute
	float MUTP_cst;		//% filtres
	float MUTP_p;		//% poids
	float MUTP_ema;		//% ema
	float MUTP_int;		//% intervalles
	float MUTP_gliss;	//% glissade

	//	%[0;1] d'influence de la valeure du G, quand P mute
	float COEF_G_cst;
	float COEF_G_p;
	float COEF_G_ema;
	float COEF_G_int;
	float COEF_G_gliss;
	
	//  u += u*f*levier*( prix[i+l]/prix[i]-1 )
	uint l; //=2;
} Env_t;

//  Fonctions de score
float gain(Mdl_t * mdl, uint l);
float prediction(Mdl_t * mdl, uint l);

//  Fonctions de Mutation
void muter_filtres(Mdl_t * G, Mdl_t * P, float proba, float COEF_G);
void muter_poids(Mdl_t * G, Mdl_t * P, float proba, float COEF_G);
void muter_ema(Mdl_t * G, Mdl_t * P, float proba, float COEF_G);
void muter_intervalle(Mdl_t * G, Mdl_t * P, float proba, float COEF_G);
void muter_glissade(Mdl_t * G, Mdl_t * P, float proba, float COEF_G);

//couches
//  n
//      y
//			glissades
//          	intervalles
//              	ema
//                      poids
//                          filtres

//	score[i] -> prediction(mdl[i])
//	cintu[i] -> #rang du i-eme mdl

#define S_FILTRES 			4
typedef struct {
	float score[S_FILTRES];
	uint  cintu[S_FILTRES];
	Mdl_t * mdl[S_FILTRES];
} F_t;

#define S_POIDS 			3
typedef struct {
	float score[S_POIDS];
	uint  cintu[S_POIDS];
	F_t *    ft[S_POIDS];
} P_t;

#define S_EMA_INT_GLISSE  	5
typedef struct poids_filtres {
	float score[S_EMA_INT_GLISSE];
	uint  cintu[S_EMA_INT_GLISSE];
	P_t *    pt[S_EMA_INT_GLISSE];
} EIG_t;

F_t   *  cr_ft(Mdl_t * depart);
P_t   *  cr_pt(Mdl_t * depart);
EIG_t * cr_eig(Mdl_t * depart);

void  lib_ft(F_t * ft);
void  lib_pt(P_t * pt);
void lib_eig(EIG_t * eig);

void score_ft(F_t * ft, Env_t env);
void score_pt(P_t * pt, Env_t env);
void score_eig(EIG_t * eig, Env_t env);

void muter_ft(F_t * ft, Env_t env);
void muter_pt(P_t * pt, Env_t env);
void muter_eig(EIG_t * eig, Env_t env);

//	Tout est base sur 1 seul gagant
//	#define combien de perdants pour 1 gagant
//
typedef struct grande_selection {
	/*	

	*/
	EIG_t eig;
	
	/*	Chaque bloque de selection a un .MUTP et .COEF_G
Ces parametres vont decroitre de `de` a `a` durant le
bloque locale.
		Si de.MUTP_cst=0.5, a.MUTP_cst=0.1 et que T=100
	alors coef_g a t=30 sera
	== a.MUTP_cst+f(t,T)*(de.MUTP_cst=0.5-a.MUTP_cst)
	*/
	Env_t de, a;
	uint fonction[];	/*
	0|	f(t,T) = (T-t-1)/T
	1|	f(t,T) = exp(-t/a)      a=-T/ln(0.01)
	2|	f(t,T) = tanh(-t/a)+1   a=-T/atanh(0.01-1)
	*/
} Grande_t;