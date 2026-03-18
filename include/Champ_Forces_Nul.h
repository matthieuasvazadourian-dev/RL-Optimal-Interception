#pragma once

#include "ChampForces.h"
#include "Vecteur.h"
#include "ObjetPhysique.h"
#include <memory>

class Champ_Forces_Nul: public ChampForces
{
    public:
    
    Champ_Forces_Nul()=default;

    Vecteur force(ObjetPhysique const& obj, double t) const override;
	
	void afficher(std::ostream& sortie) const override;
	
	std::unique_ptr<ChampForces> copie() const override; 
};