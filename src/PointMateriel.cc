#include "PointMateriel.h"
#include "Vecteur.h"
#include <iostream>

using namespace std;

PointMateriel::PointMateriel( Vecteur p, Vecteur v, double m, std::unique_ptr<ChampForces> cf, std::unique_ptr<Contrainte> c)
:ObjetPhysique(p,v,m,move(cf),move(c))
{}

Vecteur PointMateriel::evolution(double temps) const 
{
	return force(temps);
}

void PointMateriel::affiche(ostream& sortie) const
{
	sortie << "Masse: " << masse_ << "," << endl;
    sortie << "Position: " << position_ << "," << endl;
    sortie << "Vitesse: " << vitesse_ << "," << endl;
    sortie << "Position physique: " << position() << "," <<endl;
    sortie << "Vitesse physique: " << vitesse() << "," <<endl;
    sortie<< "Contrainte: "<<*contrainte_<< "," <<endl;
}
	

ostream& operator<<(ostream& sortie, const PointMateriel& pm)
{
	pm.affiche(sortie);
	return sortie;
}

void PointMateriel::affiche() const
{
	cout<<*this;
}

void PointMateriel::affiche(double t) const
{
	affiche();
	cout << "force : " << champ_forces_->force(*this,t) << endl;
}

void PointMateriel::dessine_sur(SupportADessin& support)
{
	support.dessine(*this);
}

