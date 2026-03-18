#pragma once

#include "Vecteur.h"
#include <iostream>
#include <memory>

class ObjetPhysique;

class Contrainte 
{
	public:
	virtual ~Contrainte() = default;
	
	virtual Vecteur applique_force(const ObjetPhysique& obj, const Vecteur& force, double t) const = 0;
	
	virtual Vecteur position(const ObjetPhysique& obj_phys) const =0;
	
	virtual Vecteur vitesse(const ObjetPhysique& obj_phys) const =0;
	
	virtual void afficher(std::ostream& sortie) const =0;
	
	virtual std::unique_ptr<Contrainte> copie() const=0; ///Va permettre la copie profonde dans ObjetPhysique

	virtual Vecteur cart_to_gen_pos(const Vecteur& cart_pos) const;

	virtual Vecteur cart_to_gen_vel(const Vecteur& cart_pos, const Vecteur& cart_vel) const;
};

std::ostream& operator<<(std::ostream& sortie, const Contrainte& contrainte);

