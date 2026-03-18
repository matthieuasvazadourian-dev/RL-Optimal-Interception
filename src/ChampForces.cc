#include "ChampForces.h"
#include <iostream>

using namespace std;

ostream& operator<<(ostream& sortie, const ChampForces& cf)
{
	cf.afficher(sortie);
	return sortie;
}
