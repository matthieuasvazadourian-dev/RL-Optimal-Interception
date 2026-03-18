#include "Contrainte.h"
#include <iostream>

using namespace std;

ostream& operator<<(ostream& sortie, const Contrainte& contrainte)
{
	contrainte.afficher(sortie);
	return sortie;
}

Vecteur Contrainte::cart_to_gen_pos(const Vecteur& cart_pos) const 
{ 
	return cart_pos; 
}

Vecteur Contrainte::cart_to_gen_vel(const Vecteur& cart_pos, const Vecteur& cart_vel) const 
{
	(void)cart_pos; 
	return cart_vel; 
}