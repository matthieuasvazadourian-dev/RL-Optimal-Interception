#pragma once

#include "ObjetMobile.h"
#include "ChampForces.h"
#include "Contrainte.h"
#include "Vecteur.h"
#include <iostream>
#include <memory>


class ObjetPhysique : public ObjetMobile
{
	protected:
	double masse_;
	std::unique_ptr<ChampForces> champ_forces_; 
	std::unique_ptr<Contrainte> contrainte_;
	
	
	public:
	
	/// Constructeur (par defaut)
	
	ObjetPhysique( Vecteur p = {0,0,0}, Vecteur v = {0,0,0}, double m=0, std::unique_ptr<ChampForces> cf=nullptr, std::unique_ptr<Contrainte> c=nullptr); 
	
	///Redefinition pour copie profonde
	ObjetPhysique(const ObjetPhysique& obj); 
	ObjetPhysique& operator=(const ObjetPhysique& obj);
	
	/// Getters
	double getMasse() const;
	const Contrainte& getContrainte() const;
	const ChampForces& getChampForces() const;
	
	/// Setters
	void set_contrainte(std::unique_ptr<Contrainte> c);
	void set_champ_forces(std::unique_ptr<ChampForces> cf);
	
	/// Methodes
	Vecteur force(double t=0) const;
	Vecteur position() const;
	Vecteur vitesse() const;
	
	void afficher(std::ostream& sortie) const;
};

std::ostream& operator<<(std::ostream& sortie, const ObjetPhysique& obj);

