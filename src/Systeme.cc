#include "Systeme.h"
#include <vector>
#include <fstream>
#include <sstream>
#include <memory>
#include "Integrateur.h"
#include "IntegrateurEulerCromer.h"
#include "IntegrateurNewmark.h"
#include "PointMateriel.h"
#include "ChampForces.h"
#include "Contrainte.h"
#include "SupportADessin.h"
#include <cmath>

using namespace std;

Systeme::Systeme()
:temps(0), integrateur(unique_ptr<Integrateur>(new IntegrateurNewmark()))
{}

Systeme::Systeme(vector<unique_ptr<PointMateriel>> P, vector<unique_ptr<ChampForces>> LCF, double t, unique_ptr<Integrateur> integ)
: Liste_Objets(move(P)), Liste_Champs_Forces(move(LCF)), temps(t)
{
	integrateur=move(integ);
}

Systeme::Systeme(vector<unique_ptr<PointMateriel>> P, vector<unique_ptr<ChampForces>> LCF, vector<unique_ptr<Contrainte>> LCS, double t, unique_ptr<Integrateur> integ)
: Liste_Objets(move(P)), Liste_Contraintes(move(LCS)), Liste_Champs_Forces(move(LCF)), temps(t)
{
	integrateur=move(integ);  
}

void Systeme::ajouter_Objet(unique_ptr<PointMateriel> P){Liste_Objets.push_back(move(P));} 
void Systeme::ajouter_Contrainte(unique_ptr<Contrainte> C){Liste_Contraintes.push_back(move(C));}
void Systeme::ajouter_Champ_Forces(unique_ptr<ChampForces> CF){Liste_Champs_Forces.push_back(move(CF));}
void Systeme::ajouter_Contrainte_Objet(size_t i, size_t j)
{
	Liste_Objets[j]->set_contrainte(move(Liste_Contraintes[i]));
}
  
void Systeme::ajouter_Champ_Forces_Objet(size_t i, size_t j)
{
	Liste_Objets[j]->set_champ_forces(move(Liste_Champs_Forces[i]));
}
  
  
void Systeme::set_integrateur(unique_ptr<Integrateur> i)
{
	integrateur=move(i);
}

const vector<unique_ptr<PointMateriel>>& Systeme::getListe_Objets() const {return Liste_Objets;}  ///On a une reference constante pour eviter que les getters soient utilises pour modifier les attributs

const vector<unique_ptr<Contrainte>>& Systeme::getListe_Contraintes() const {return Liste_Contraintes;}
  
const vector<unique_ptr<ChampForces>>& Systeme::getListe_Champs_Forces() const {return Liste_Champs_Forces;}

double Systeme::getTime() {return temps;} 
  

void Systeme::afficher(ostream& sortie) const
{
	sortie<<"Systeme : a t = "<<temps<<" : "<<endl;
    int i(1);
    for (auto& PM : Liste_Objets)
    {
      sortie<<"Objet no "<<i<<" : Point Materiel : "<<endl; ///On a que des Point Materiels donc pas besoin de rendre cet affichage sensible au type d'objet
      sortie<<*PM;
      ++i;
    }
    i=1;
    for (auto& CF : Liste_Champs_Forces)
    {
      sortie<<" Champ no "<<i<<" : ";
      sortie<<*CF<<endl;
      ++i;
    }
    i=1;
    for (auto& C : Liste_Contraintes)
    {
      sortie<<"Contrainte no "<<i<<" : ";
      sortie<<*C<<endl;
      ++i;
    }
}

void Systeme::afficher_CSV(vector<ofstream>& list_CSV) const
{
  for (size_t i(0);i<list_CSV.size();++i)
  {
    list_CSV[i]<<temps<<","<<Liste_Objets[i]->position()<<"\n";
  }
}
void Systeme::dessine_sur(SupportADessin& support)
{
	support.dessine(*this);
}

void Systeme::evolue(double dt)
{
	for (auto& pm : Liste_Objets)
	{
		integrateur->integre(*pm, temps, dt);
	}

  vector<vector<int>> collision_statuses=check_collision();
  for (size_t i(0);i<Liste_Objets.size();++i)
  {
    if (collision_statuses[i][0]==2){make_collide_ground(*Liste_Objets[i]);}
    else if (collision_statuses[i][0]==1)
    {
      for (size_t k = 1; k < collision_statuses[i].size(); k++) 
      {
        int j = collision_statuses[i][k];
        if (i<j)
        { 
          make_collide_pair(*Liste_Objets[i], *Liste_Objets[j]);
        }
      }
    }
  }
	temps+=dt;
}

ostream& operator<<(ostream& sortie, const Systeme& s)
{
  s.afficher(sortie);
  return sortie;
}

vector<vector<int>> Systeme::check_collision() const
{
  size_t n = Liste_Objets.size();
  vector<vector<int>> res(n);
  for (size_t i(0);i<n;++i)
  {
    if (Liste_Objets[i]->position()[2]<0.01 and Liste_Objets[i]->vitesse()[2]<0)
    {
      res[i].push_back(2);
    }
    else
    {
      for (size_t j(0);j<n;++j)
      {
        Vecteur delta_x = Liste_Objets[j]->position()-Liste_Objets[i]->position();
        Vecteur delta_v = Liste_Objets[i]->vitesse()-Liste_Objets[j]->vitesse();
        if (j!=i and delta_x.norme()<0.01 and delta_v.prod_scal(delta_x)<0)
        {
          if (res[i].empty())
          {
            res[i].push_back(1);
          }
      
          res[i].push_back(j);
        }
      }
    }
  }

  for (size_t k(0);k<n;++k)
  {
    if (res[k].empty())
    {
      res[k].push_back(0);
    }
  }
  return res;
}

void Systeme::make_collide_pair(PointMateriel& pm1, PointMateriel& pm2) const
{
  Vecteur v1 = pm1.vitesse();
  Vecteur v2 = pm2.vitesse();
  Vecteur x1 = pm1.position();
  Vecteur x2 = pm2.position();
  double m1 = pm1.getMasse();
  double m2 = pm2.getMasse();

  double k = (v1-v2).prod_scal(x1-x2)/(x1-x2).norme2();
  Vecteur v1_update = v1 - (x1-x2)*(2.0*m2/(m1+m2)*k);
  Vecteur v2_update = v2 - (x2-x1)*(2.0*m1/(m1+m2)*k);

  pm1.setVitesse(v1_update);
  pm2.setVitesse(v2_update);
}

void Systeme::make_collide_ground(PointMateriel& pm) const
{
  Vecteur v = pm.vitesse();
  Vecteur normal = Vecteur({0,0,1});

  Vecteur v_update = v - normal*2*v.prod_scal(normal);

  pm.setVitesse(v_update);

  /// Allows us to push particle above ground if it had gone under it during integration

  Vecteur x = pm.position();
  x[2] = 0.01; 
  pm.setPosition(x);

}