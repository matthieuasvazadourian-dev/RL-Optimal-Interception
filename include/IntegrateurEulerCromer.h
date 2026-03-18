#pragma once

#include "Integrateur.h"
#include "ObjetMobile.h"


class IntegrateurEulerCromer : public Integrateur {
	public:
	
	void integre(ObjetMobile& objet, double t, double dt) const override;
};
