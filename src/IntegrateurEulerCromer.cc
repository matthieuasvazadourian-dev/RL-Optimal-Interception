#include "IntegrateurEulerCromer.h"
#include "ObjetMobile.h"

using namespace std;

void IntegrateurEulerCromer::integre(ObjetMobile& objet, double t, double dt) const 
{
	/// Nous appliquons l'algorithme de l'integrateur Euler-Cromer tel qu'il est decrit dans le complement mathematique
	
	Vecteur acceleration = objet.evolution(t); 

	Vecteur nouvelle_vitesse = objet.getVitesse() + acceleration * dt;

	Vecteur nouvelle_position = objet.getPosition() + nouvelle_vitesse * dt;

	objet.setEtat(nouvelle_position, nouvelle_vitesse);
}
