#include "Champ_Forces_Nul.h"

using namespace std;

Vecteur Champ_Forces_Nul::force(ObjetPhysique const& obj, double t) const
{
    return Vecteur({0,0,0});
}

void Champ_Forces_Nul::afficher(ostream& sortie) const 
{
	sortie<<" Champ de Forces Nul";
}

unique_ptr<ChampForces> Champ_Forces_Nul::copie() const
{
	return unique_ptr<ChampForces>(new Champ_Forces_Nul(*this));
}
