#include "ObjetMobile.h"
#include "Vecteur.h"
#include <iostream>

using namespace std;

ObjetMobile::ObjetMobile(Vecteur const& position, Vecteur const& vitesse)
:position_(position), vitesse_(vitesse)
{}
  
ObjetMobile::ObjetMobile(Vecteur const& position)
:position_(position), vitesse_(position.getsize()) 
{}  

Vecteur ObjetMobile::getPosition() const { return position_; }
Vecteur ObjetMobile::getVitesse() const { return vitesse_; }

void ObjetMobile::setPosition(Vecteur const& pos) { position_ = pos; }
void ObjetMobile::setVitesse(Vecteur const& vit) { vitesse_ = vit; }


void ObjetMobile::setEtat(Vecteur const& nouvelle_position, Vecteur const& nouvelle_vitesse) /// On s'assure que setEtat() appelle setPosition() pour correctement appeler PointMaterielAngulaire::remise_angles()
{
	setPosition(nouvelle_position);
	setVitesse(nouvelle_vitesse);
}

void ObjetMobile::afficher(ostream& sortie) const
{
	sortie << "position : ";
		for (size_t i = 0; i < position_.getsize(); ++i) 
		{
			sortie << position_.getvect()[i] << " ";
		}
		sortie << endl;
		
		sortie << "vitesse : ";
		for (size_t i = 0; i < position_.getsize(); ++i) 
		{
			sortie << position_.getvect()[i] << " ";
		}
		sortie << endl;
}

ostream& operator<<(ostream& sortie, const ObjetMobile& obj)
{
	obj.afficher(sortie);
	return sortie;
}
