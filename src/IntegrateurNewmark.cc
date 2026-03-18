#include "IntegrateurNewmark.h"
#include "ObjetMobile.h" // for get_pos, get_vit, setEtat, evolution
#include "Vecteur.h"


IntegrateurNewmark::IntegrateurNewmark(unsigned int i,double eps)
: epsilon(eps),iterations_max(i)
{}

void IntegrateurNewmark::integre(ObjetMobile& objet, double t, double dt) const
{
	/// Ici on applique tout simplement l'algorithme de l'integrateur de Newmark tel qu'il est decrit dans le complement mathematique
	/// Note: ici, prev remplace (n-1) et new remplace (n). On a essayer de conserver le nom des variables le mieux possible pour la lisibilite
	
	Vecteur pos_prev = objet.getPosition();
	Vecteur vit_prev = objet.getVitesse();
	
	Vecteur s = objet.evolution(t);
	
	Vecteur pos_new = pos_prev;
	Vecteur vit_new = vit_prev;
	
	Vecteur r(objet.getPosition().getsize()); ///Vecteur position et acceleration de meme dimension
	Vecteur q(objet.getPosition().getsize());
	
	unsigned int iterations = 0; /// On commence logiquement a 0 iterations
	
	
	do {
	q = pos_new;
	
	objet.setEtat(pos_new, vit_new);

	r = objet.evolution(t + dt);
	
	vit_new = vit_prev + ((r + s)*(dt / 2.0)); /// Notre multiplication scalaire est surchargee implicitement donc les vecteurs doivent etre les operandes de gauche
	
	pos_new = pos_prev + (vit_prev*dt) + (	(r*((1.0/2.0))) + s)*((dt * dt) / 3.0); 
	
	iterations++;
	} while ((pos_new - q).norme() >= epsilon and iterations < iterations_max); /// Soit la condition de convergence est remplie, soit on a atteint la valeur maximale d'iterations
	
	objet.setEtat(pos_new, vit_new); /// On met a jour la position et la vitesse de l'objet integre
}

