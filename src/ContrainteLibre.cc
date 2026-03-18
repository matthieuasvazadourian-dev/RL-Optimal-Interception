#include "ContrainteLibre.h"
#include "ObjetPhysique.h"
#include "Vecteur.h"
#include <iostream>
#include <memory>

using namespace std;

  Vecteur ContrainteLibre::applique_force(const ObjetPhysique& obj_phys, const Vecteur& force, double temps) const /// La contrainte ne fait rien a l'acceleration
  {
    double m=obj_phys.getMasse();
    if(m==0) {
		return force;
	} else {
		return force * (1/m);
		} 
  }
  
  Vecteur ContrainteLibre::position(const ObjetPhysique& obj_phys) const 
  {
	  return obj_phys.getPosition();
  }
  
  Vecteur ContrainteLibre::vitesse(const ObjetPhysique& obj_phys) const 
  {
	  return obj_phys.getVitesse();
  }
  
  void ContrainteLibre::afficher(ostream& sortie) const
  {
    sortie<<"Libre";
  }

  unique_ptr<Contrainte> ContrainteLibre::copie() const
  {
	  return unique_ptr<Contrainte>(new ContrainteLibre(*this));
  }
	  
