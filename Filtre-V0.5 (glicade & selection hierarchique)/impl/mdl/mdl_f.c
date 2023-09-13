#include "mdl.h"

static float filtre_n(float * x, float * filtre, uint n) {  //[0;1]
	float _min=x[0], _max=x[0];
	float _x;
	for (uint i=1; i < n; i++) {
		_x = x[i];
		if (_x > _max) _max = _x;
		if (_x < _min) _min = _x;
	}
	if (_min == _max) {
		_min=0;_max=1;
		for (uint i=0; i < n; i++) x[i]=0.5;
		//ERR("Impossible de normaliser un vecteur uniforme");
	}

	//
	float x_norm[n];

	//
	float _s = 0;
	const float d = _max-_min;
	float tmp, signe;
	for (uint i=0; i < n; i++) {
		x_norm[i] = (x[i]-_min)/d;
		_s += sqrtf(1.0f+fabs(x_norm[i] - filtre[i]));
	}
	_s = _s/n - 1;

	//
	float _d = 0.0;
	for (int i=0; i < n-1; i++) {
		_d += powf(1.0f + fabs((x_norm[i+1]-x_norm[i]) - (filtre[i+1]-filtre[i])),2);
	}
	_d = _d/(n-1) - 1;

	//
	return ___exp(-_s*_s - _d*_d);
};

//========================================  Neurone Condition  ===========================================

//	Macro de detection si x c [-1;1] est haut(proche de 1) ou bas (proche de 0)
#define HAUT(x) (1.0 - (x+1.0)/2.0)
#define _BAS(x) (0.0 + (x+1.0)/2.0)

static float neurone_condition_n(float * x, float * poid, uint n) { //[-1;+1]
	if (n < 2) {
		ERR("Comparer 0 ou 1 valeurs entre elles ?");
	} else if (n == 2) {
		float x0=x[0], x1=x[1];
		float HAUT_x0 = HAUT(x0), HAUT_x1 = HAUT(x1);
		float _BAS_x0 = _BAS(x0), _BAS_x1 = _BAS(x1);
		//
		float _00 = HAUT_x0 * HAUT_x1 * poid[0];
		float _01 = _BAS_x0 * HAUT_x1 * poid[1];
		float _10 = HAUT_x0 * _BAS_x1 * poid[2];
		float _11 = _BAS_x0 * _BAS_x1 * poid[3];
		return (_00+_01+_10+_11);
	} else if (n == 3) {
		float x0=x[0], x1=x[1], x2=x[2];
		float h0=HAUT(x0), h1=HAUT(x1), h2=HAUT(x2);
		float b0=_BAS(x0), b1=_BAS(x1), b2=_BAS(x2);
		//
		float _000 = h0 * h1 * h2 * poid[0];
		float _001 = h0 * h1 * b2 * poid[1];
		float _010 = h0 * b1 * h2 * poid[2];
		float _011 = h0 * b1 * b2 * poid[3];
		float _100 = b0 * h1 * h2 * poid[4];
		float _101 = b0 * h1 * b2 * poid[5];
		float _110 = b0 * b1 * h2 * poid[6];
		float _111 = b0 * b1 * b2 * poid[7];
		return (_000+_001+_010+_011+_100+_101+_110+_111);
	} else {
		ERR("Implementation Supprimee des neu_cond pour n>3");
	}
};

//===========================================================================================

float f(Mdl_t * mdl, uint depart) {
	uint C = mdl->C;
	uint * type = mdl->type;
	uint * y = mdl->y;
	uint * n = mdl->n;

	uint d;

	//	Filtres N
	float _x[N_MAX];

	//  Couche[0], fltr prixs
	float _max, _f;
	FOR(0, i, y[0]) {
		uint _ema = mdl->ema[i];
		uint interv = intervalles[mdl->intervalles[i]];
		uint glisser = mdl->glisser[i];
		//
		_max = 0;
		FOR(0, g, glisser) {
			FOR(0, j, n[0]) {
				_x[j] = ema[_ema][depart - (j-g)*interv];
			}

			_f = filtre_n(
				_x, mdl->conste + i*CONSTES_FLTR(n[0]), n[0]);

			if (_max < _f) _max = _f;
		}
		mdl->var[i] = _max;
	};

	//  Autres insts principales

	FOR(1, i, C) {
		FOR(0, j, y[i]) {
			if (type[i] == 1) {
				//  ======= Neurone Condition ======
				d = (C-i);
				//
				FOR(0, k, n[i]) {
					_x[k] = mdl->var[mdl->y_depart[i-1] + j*n[i] + k];//mdl->neu_depuis[i][j][k]];
				}
				//
				mdl->var[mdl->y_depart[i] + j] = neurone_condition_n(
					_x, mdl->poid + mdl->poid_depart[i] + j*POIDS_COND(mdl->n[i]),
					mdl->n[i]
				);
			} else {
				ERR("Pas de couche %i", mdl->type[i]);
			}
		}
	}
	//
	return mdl->var[mdl->vars-1];
};


















