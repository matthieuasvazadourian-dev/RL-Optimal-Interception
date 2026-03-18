#include "GravitationConstante.h"
#include "ObjetPhysique.h"

using namespace std;

Vecteur GravitationConstante::force(ObjetPhysique const& obj, double t) const
{
	return g_*obj.getMasse();
}

GravitationConstante::GravitationConstante(Vecteur const& g)
: g_(g) 
{}

void GravitationConstante::afficher(ostream& sortie) const 
{
	sortie<<" Champ de Gravitation Constante, vecteur g: "<< g_;
}

unique_ptr<ChampForces> GravitationConstante::copie() const
{
	return unique_ptr<ChampForces>(new GravitationConstante(*this));
}
