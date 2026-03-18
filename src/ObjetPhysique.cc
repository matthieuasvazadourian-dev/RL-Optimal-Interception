#include "ObjetPhysique.h"
#include "Vecteur.h"
#include "ContrainteLibre.h"
#include "ObjetMobile.h"
#include "ChampForces.h"
#include <iostream>
#include <memory>

using namespace std;


ObjetPhysique::ObjetPhysique( Vecteur p, Vecteur v, double m, unique_ptr<ChampForces> cf, unique_ptr<Contrainte> c) 
: ObjetMobile(p,v), masse_(m), champ_forces_(move(cf))
{
	if (c==nullptr) 
	{
		contrainte_=unique_ptr<ContrainteLibre>(new ContrainteLibre());
	}
	else 
	{
		contrainte_=move(c);
	}
}

ObjetPhysique::ObjetPhysique(const ObjetPhysique& obj)
:ObjetMobile(obj), masse_(obj.masse_)
{
	if (obj.champ_forces_!=nullptr)
	{
		champ_forces_= obj.champ_forces_->copie();
	}
	if (obj.contrainte_!=nullptr)
	{
		contrainte_=obj.contrainte_->copie();
	}
}

ObjetPhysique& ObjetPhysique::operator=(const ObjetPhysique& obj)
{
	if (this==&obj)
	{
		return *this;
	}
	else 
	{
		ObjetMobile::operator=(obj);
		masse_=obj.masse_;
		
		if (obj.champ_forces_!=nullptr)
		{
			champ_forces_= obj.champ_forces_->copie();
		}
		else
		{
			champ_forces_=nullptr;
		}
		
		if (obj.contrainte_!=nullptr)
		{
			contrainte_=obj.contrainte_->copie();
		}
		else
		{
			contrainte_=nullptr;
		}
	}
	
	return *this;
}

double ObjetPhysique::getMasse() const {return masse_;}

const Contrainte& ObjetPhysique::getContrainte() const {return *contrainte_;}
const ChampForces& ObjetPhysique::getChampForces() const {return *champ_forces_;}

void ObjetPhysique::set_contrainte(unique_ptr<Contrainte> contrainte){contrainte_=move(contrainte);}
void ObjetPhysique::set_champ_forces(unique_ptr<ChampForces> cf){champ_forces_=move(cf);}


Vecteur ObjetPhysique::force(double t) const
{
	Vecteur res(position_.getsize());
	if(champ_forces_ != nullptr) res = champ_forces_->force(*this, t);
    if (contrainte_ != nullptr) return contrainte_->applique_force(*this, res, t);
    return res;
}

Vecteur ObjetPhysique::position() const
{
	return contrainte_->position(*this);
}

Vecteur ObjetPhysique::vitesse() const
{
	return contrainte_->vitesse(*this);
}
	
void ObjetPhysique::afficher(ostream& sortie) const
{
	sortie<<"Contrainte: "<<*contrainte_<<endl;
    sortie<<"Champ de Forces: "<<*champ_forces_<<endl;  
}

ostream& operator<<(ostream& sortie, const ObjetPhysique& obj)
{
	obj.afficher(sortie);
	return sortie;
}
