#pragma once

#include "PointMateriel.h"
#include "Vecteur.h"
#include <memory>

class PointMaterielAngulaire: public PointMateriel
{
	private:
	void remise_angles();
	
	public:
	PointMaterielAngulaire(const Vecteur& p = {0,0,0}, const Vecteur& v = {0,0,0} , double m=0, std::unique_ptr<ChampForces> cf=nullptr, std::unique_ptr<Contrainte> c=nullptr);
	
	void setPosition(const Vecteur& pos);
};
	
