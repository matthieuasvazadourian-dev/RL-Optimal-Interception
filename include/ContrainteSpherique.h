#pragma once
#include "Vecteur.h"
#include "ObjetPhysique.h"
#include <iostream>
#include <memory>

class ContrainteSpherique : public Contrainte 
{

	private:
	double R;        /// Rayon de la sphere
	Vecteur pivot_;  /// Point d'attache du pendule dans l'espace cartesien

	public:

	ContrainteSpherique(double rayon, Vecteur pivot = Vecteur(0.0, 0.0, 0.0));

	virtual Vecteur position(const ObjetPhysique& obj) const override;
	
	virtual Vecteur vitesse(const ObjetPhysique& obj) const override;
	
	virtual Vecteur applique_force(const ObjetPhysique& obj, const Vecteur& force, double t) const override;
	
	void afficher(std::ostream& sortie) const override;
	
	std::unique_ptr<Contrainte> copie() const override;

	Vecteur cart_to_gen_pos(const Vecteur& cart_pos) const override;
	
	Vecteur cart_to_gen_vel(const Vecteur& cart_pos, const Vecteur& cart_vel) const override;
};

