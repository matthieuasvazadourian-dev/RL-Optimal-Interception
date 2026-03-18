#pragma once

#include "ObjetMobile.h"

class Integrateur 
{
	public:
	
	virtual ~Integrateur() = default;
	
	virtual void integre(ObjetMobile& objet, double t, double dt) const = 0;
};
