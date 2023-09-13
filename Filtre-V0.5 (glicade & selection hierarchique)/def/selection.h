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