#include "mdl.h"

static char * noms[] = {"Filtre-Prix", "Condition"};

static void plume_poids(Mdl_t * mdl) {
	ptr(" === poids ===\n");
	FOR(0, i, mdl->C) {
		ptr("%s #%i\n", noms[mdl->type[i]], i);
		if (mdl->type[i] == 1) {
			uint poids = POIDS_COND(mdl->n[i]);
			FOR(0, j, mdl->y[i]) {
				ptr(" .y%i:\n", j);
				FOR(0, k, poids) {
					ptr("%i| %f\n",
					mdl->poid_depart[i] + j*poids + k,
					mdl->poid[mdl->poid_depart[i] + j*poids + k]);
				}
			}
		}
	}
};

static void plume_constes(Mdl_t * mdl) {
	ptr(" === constantes ===\n");
	FOR(0, i, mdl->C) {
		ptr("%s #%i\n", noms[mdl->type[i]], i);
		if (mdl->type[i] == 0) {
			uint constantes = CONSTES_FLTR(mdl->n[i]);
			FOR(0, j, mdl->y[i]) {
				ptr(" .y%i:\n", j);
				FOR(0, k, constantes) {
					ptr("%i| %f\n",
					mdl->conste_depart[i] + j*constantes + k,
					mdl->conste[mdl->conste_depart[i] + j*constantes + k]);
				}
			}
		}
	}
};

static void plume_var(Mdl_t * mdl) {
	ptr(" === vars ===\n");
	FOR(0, i, mdl->C) {
		ptr("#%i %s\n", i, noms[mdl->type[i]]);
		FOR(0, j, mdl->y[i]) {
			ptr("%i| %f \n", mdl->y_depart[i] + j, mdl->var[mdl->y_depart[i] + j]);
		}
	}
};

void plume_mdl(Mdl_t * mdl) {
	printf("======== Mdl ==========\n");
	uint C = mdl->C;
	for (uint i=0; i < C; i++) {
		printf("%3.i| %s n=%i:\n", i, noms[mdl->type[i]], mdl->n[i]);
		for (uint j=0; j < mdl->y[i]; j++) {
			if (mdl->type[i]==1) {
				printf("%3.i|\ty[%i] : cond(%i,%i)", (mdl->y_depart[i]+j), j, j*2, j*2+1);
				printf("\n");
			} else if (mdl->type[i] == 0) {
				printf("%3.i|\ty[%i] : ema=%i intervalle=%i glissade=%i\n",
                    (mdl->y_depart[i]+j),
					j, mdl->ema[j], mdl->intervalles[j], mdl->glisser[j]);
			}
		}
	};

	plume_poids(mdl);
	plume_constes(mdl);
	//plume_var(mdl);
};

//===============================================

void comportement(Mdl_t * mdl) {
#define T 15

	uint depart = DEPART + (rand() % (PRIXS-DEPART-T-1));

	float f_arr[T];

	float var_par_t[T*mdl->vars];

	for (uint i=0; i < T; i++) {
		f_arr[i] = f(mdl, depart + i);
		memcpy(var_par_t + i*mdl->vars, mdl->var, sizeof(float)*mdl->vars);
	};

	for (uint i=0; i < mdl->vars; i++) {
		printf("%i| ", i);
		for (uint t=0; t < T; t++) {
			if (var_par_t[t*mdl->vars + i] >= 0) printf(" ");
			printf("%f | ", var_par_t[t*mdl->vars + i]);
		}
		printf("\n");
	}

	//gnuplot(ema[1] + depart - 6*6, 6, "ema1");
	//gnuplot(ema[1] + depart + T - 6*6, 6, "ema2");

	gnuplot(prixs + depart, T, "Prixs");
	gnuplot(f_arr, T, "Valeur de f (achat vente)");

	//plume_mdl(mdl);
	//plume_poids(mdl);
	//plume_constes(mdl);
};
