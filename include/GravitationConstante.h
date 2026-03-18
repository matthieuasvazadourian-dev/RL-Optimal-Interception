#pragma once

#include "ChampForces.h"
#include "Vecteur.h"
#include "ObjetPhysique.h"
#include <memory>

class GravitationConstante : public ChampForces 
{
	private:
	Vecteur g_;
	
	public:
	
	GravitationConstante(Vecteur const& g);
	
	Vecteur force(ObjetPhysique const& obj, double t) const override;
	
	void afficher(std::ostream& sortie) const override;
	
	std::unique_ptr<ChampForces> copie() const override; 
};
