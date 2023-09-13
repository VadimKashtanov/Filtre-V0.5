#include "selection.h"

float gain(Mdl_t * mdl, uint l) {
	float _gain = USDT;
	//
	float _f, p0, p1;
	uint t=0;
	//UNE_COURBE(p);
	for (uint i=DEPART; i < PRIXS-1-l; i++) {
		p1 = prixs[i+l];
		p0 = prixs[i];
		//
		_f = signe( f(mdl, i) );
		//
		if (_gain > 0) {
			_gain += _gain * _f * LEVIER * (p1/p0 - 1.0);
		} else {
			return -USDT;
		}
	//	SUIVIE_COURBE(p, _gain);
	};
	//PLUMER_LA_COURBE(p);

	return _gain - USDT;
};

float prediction(Mdl_t * mdl, uint l) {
	float _prediction = 0.0;
	//
	float _f, p0, p1;
	uint t=0;
	for (uint i=DEPART; i < PRIXS-1-l; i++) {
		p1 = prixs[i+l];
		p0 = prixs[i];
		//
		_f = signe( f(mdl, i) );
		
		if (signe(p1/p0-1.0) == _f)
			_prediction += 1.0;

		//
#define div_par 5*(1+rand()%10)
		i += div_par;
		t++;
	};
	//printf("%i\n", t);

	return _prediction / (float)t;
};