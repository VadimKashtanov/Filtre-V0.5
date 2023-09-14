#include "selection.h"

void score_ft(F_t * ft, Env_t env) {
	FOR(0, i, S_FILTRES) {
		ft->score[i] = prediction(fp->mdl[i], env.l);
		ft->cintu[i] = i;
	}
	uint c;
	FOR(0, i, S_FILTRES) {
		FOR(i+1, j, S_FILTRES) {
			if (ft->score[ft->cintu[i]] < ft->score[ft->cintu[j]]) {
				c = ft->cintu[i];
				ft->cintu[i] = ft->cintu[j];
				ft->cintu[j] = c;
			}
		}
	}
};

void score_pt(P_t * pt, Env_t env) {

};

void score_eig(EIG_t * eig, Env_t env) {

};



void muter_ft(F_t * ft, Env_t env) {

};

void muter_pt(P_t * pt, Env_t env) {

};

void muter_eig(EIG_t * eig, Env_t env) {

};