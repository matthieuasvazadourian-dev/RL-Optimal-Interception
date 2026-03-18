#pragma once 


#include "Vecteur.h"
#include <iostream>
#include <memory>

class ObjetPhysique;

class ChampForces 
{
	public:
	virtual ~ChampForces() = default;
	
	virtual Vecteur force(const ObjetPhysique& obj, double t) const = 0;
	
	virtual void afficher(std::ostream& sortie) const=0;
	
	virtual std::unique_ptr<ChampForces> copie() const=0; ///Va permettre la copie profonde dans ObjetPhysique
};

std::ostream& operator<<(std::ostream& sortie, const ChampForces& cf);
