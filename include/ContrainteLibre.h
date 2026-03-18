#pragma once

#include "Contrainte.h"
#include "ObjetPhysique.h"
#include "Vecteur.h"
#include <iostream>
#include <memory>

class ContrainteLibre : public Contrainte 
{
	public:

	Vecteur applique_force(const ObjetPhysique& obj_phys, const Vecteur& force, double temps) const override;
  
	Vecteur position(const ObjetPhysique& obj_phys) const override;
  
	Vecteur vitesse(const ObjetPhysique& obj_phys) const override;
  
	void afficher(std::ostream& sortie) const override;
	
	std::unique_ptr<Contrainte> copie() const override;
};
