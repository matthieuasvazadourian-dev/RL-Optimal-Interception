#include "PointMaterielAngulaire.h"
#include "PointMateriel.h"
#include "Vecteur.h"
#include <cmath>

using namespace std;

PointMaterielAngulaire::PointMaterielAngulaire(const Vecteur& p, const Vecteur& v, double m, std::unique_ptr<ChampForces> cf, std::unique_ptr<Contrainte> c)
:PointMateriel(p,v,m,move(cf),move(c))
{
	remise_angles();
}

void PointMaterielAngulaire::remise_angles()
{
	const double deux_pi = 2*acos(-1.0); ///On utilise acos(-1.0) ici car M_PI n'est pas disponible a travers cmath sur windows
	for (size_t i=0; i<position_.getsize(); ++i)
	{
		while (position_[i]>= deux_pi) 
		{
			position_[i] -= deux_pi;
		}
		while (position_[i] <= -deux_pi)
		{
			position_[i] += deux_pi;
		}
	}
}

void PointMaterielAngulaire::setPosition(const Vecteur& pos) 
{
	PointMateriel::setPosition(pos);
	remise_angles();                /// Inclure la remise des angles ici assure que les angles seront remis a chaque etape de l'integrateur qui appelle setEtat() et donc setPosition().
}
