#pragma once

#include "Vecteur.h"
#include <iostream>

class ObjetMobile  
{
	
	protected:
	
	Vecteur position_;
	Vecteur vitesse_;
	
	public:
	
	ObjetMobile(Vecteur const& position, Vecteur const& vitesse);
	ObjetMobile(Vecteur const& position);
	
	virtual ~ObjetMobile()=default;
	
	// Accessors and mutators for state:
	
	Vecteur getPosition() const;
	Vecteur getVitesse() const;  
	void setPosition(Vecteur const& pos); 
	void setVitesse(Vecteur const& vit);
	
	virtual Vecteur evolution(double temps) const=0; /// Redefini dans PointMateriel
	
	void setEtat(Vecteur const& nouvelle_position, Vecteur const& nouvelle_vitesse);
	
	virtual void afficher(std::ostream& sortie) const;
};

std::ostream& operator<<(std::ostream& sortie, const ObjetMobile& obj);

