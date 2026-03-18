#pragma once

#include "Integrateur.h"
#include "ObjetMobile.h"


class IntegrateurNewmark : public Integrateur {
	
	private:
	double epsilon; /// Valeur limite pour le test de convergence
	const unsigned int iterations_max; /// Valeur limite d'iterations (evite que le programme ne dure trop longtemps)
	
	public:
	
	IntegrateurNewmark(unsigned int i=20,double eps = 1e-8);
	
	virtual void integre(ObjetMobile& objet, double t, double dt) const override;
	
};

