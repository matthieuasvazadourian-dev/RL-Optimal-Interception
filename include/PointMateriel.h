#pragma once

#include "ObjetPhysique.h"
#include "Vecteur.h"
#include "Dessinable.h"
#include "SupportADessin.h"
#include <iostream>

class PointMateriel : public ObjetPhysique, public Dessinable 
{
	public:
	PointMateriel( Vecteur p = {0,0,0}, Vecteur v = {0,0,0}, double m=0, std::unique_ptr<ChampForces> cf=nullptr, std::unique_ptr<Contrainte> c=nullptr);
	
	Vecteur evolution(double temps) const override;
	void affiche() const;
	void affiche(double t) const;
	void affiche(std::ostream& sortie) const;
	void dessine_sur(SupportADessin& support) override;
	
};

std::ostream& operator<<(std::ostream& sortie, const PointMateriel& pm);
