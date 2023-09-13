#include "selection.h"

uint C      = 2;
uint type[] = {0,1};
//
uint y[]    = {2,1};
uint n[]    = {8,2};

Env_t env = {
	//  Le gagant ne muteras jamais
	.MUTP_cst           =0.50,  .COEF_G_cst           =0.40,
	.MUTP_p             =0.50,	.COEF_G_p             =0.40,
	//
	.MUTP_ema           =0.50,  .COEF_G_ema           =0.80,
	.MUTP_int           =0.50,  .COEF_G_int           =0.80,
	.MUTP_gliss         =0.50,  .COEF_G_gliss         =0.80,

	//  Longeure
	.l=2
};

void score(Mdl_t * mdl) {
	printf("%f\n", prediction(mdl, env.l));
};

int main() {
	srand(0);
	charger_les_prixs();

	Mdl_t * mdl[5];
	FOR(0, i, 5) mdl[i] = cree_mdl(C, y, n, type);

	float scores[5];
	uint rang[5];
	uint c;
	float tmp;
	FOR(0, t, 50) {
		FOR(0, i, 5) {
			scores[i] = prediction(mdl[i], env.l);
			rang[i] = i;
		//	printf("%f, ", scores[i]);
		}
		//printf("\n");
		//
		FOR(0, i, 5) {
			FOR(i, j, 5) {
				if (scores[rang[i]] < scores[rang[j]]) {
					//tmp = scores[i];
					//scores[i] = scores[j]
					//scores[j] = tmp;
					//
					c = rang[i];
					rang[i] = rang[j];
					rang[j] = c;
				}
			}
		}
		FOR(0, i, 5) {
			if (i != 0) {
				muter_filtres(mdl[rang[0]], mdl[rang[i]], env.MUTP_cst, env.COEF_G_cst);
				muter_poids(mdl[rang[0]], mdl[rang[i]], env.MUTP_p, env.COEF_G_p);
			}
			printf("#%i:%i:%f  ", i, rang[i], scores[rang[i]]);
		};
		printf("\n");
	};
	//score(mdl);
	//score(mdl);
	//score(mdl);
	//score(mdl);
	//printf(">> %f\n", gain(mdl, env.l));
	//muter_filtres(mdl, mdl1, env.MUTP_cst, env.COEF_G_cst);
	//muter_poids(mdl, mdl1, env.MUTP_p, env.COEF_G_p);
	//muter_glissade(mdl, mdl1, env.MUTP_ema, env.COEF_G_ema);
	//muter_ema(mdl, mdl1, env.MUTP_int, env.COEF_G_int);
	//muter_intervalle(mdl, mdl1, env.MUTP_gliss, env.COEF_G_gliss);
	//plume_mdl(mdl);
	//plume_mdl(mdl1);
	//
	/*for (uint i=0; i < 100; i++) {
		//plume_mdl(mdl);
		mdl = cree_mdl(C, y, n, type);
		score(mdl);
	}*/

	comportement(mdl[rang[0]]);
	plume_mdl(mdl[rang[0]]);
};

























