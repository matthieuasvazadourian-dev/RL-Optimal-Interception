#pragma once
#include <memory>
#include <vector>
#include <iostream>
#include "Dessinable.h"
#include "Integrateur.h"
#include "IntegrateurEulerCromer.h"
#include "PointMateriel.h"
#include "ChampForces.h"
#include "Contrainte.h"
#include "SupportADessin.h"
#include <vector>


class Systeme : public Dessinable
{	

  private:
 
  std::vector<std::unique_ptr<PointMateriel>> Liste_Objets;
  std::vector<std::unique_ptr<Contrainte>> Liste_Contraintes;
  std::vector<std::unique_ptr<ChampForces>> Liste_Champs_Forces; 
  double temps;
  std::unique_ptr<Integrateur> integrateur;  ///Permet un constructeur par defaut
										
 public: 
  ///Constructeurs///
  Systeme();
  
  Systeme(std::vector<std::unique_ptr<PointMateriel>> LO, std::vector<std::unique_ptr<ChampForces>> LCF, double t=0,  
  std::unique_ptr<Integrateur> integ = std::unique_ptr<IntegrateurEulerCromer>(new IntegrateurEulerCromer()));
  
  Systeme(std::vector<std::unique_ptr<PointMateriel>> O, std::vector<std::unique_ptr<ChampForces>> LCF, std::vector<std::unique_ptr<Contrainte>> LCS, double t=0,
  std::unique_ptr<Integrateur> integ = std::unique_ptr<IntegrateurEulerCromer>(new IntegrateurEulerCromer()));
  
  ///Methodes///
  void ajouter_Objet(std::unique_ptr<PointMateriel> P);
  void ajouter_Contrainte(std::unique_ptr<Contrainte> C);
  void ajouter_Champ_Forces(std::unique_ptr<ChampForces> CF);
  
  void ajouter_Contrainte_Objet(size_t i, size_t j);
  
  void ajouter_Champ_Forces_Objet(size_t i, size_t j);

  void set_integrateur(std::unique_ptr<Integrateur> i);
  
  const std::vector<std::unique_ptr<PointMateriel>>& getListe_Objets() const;
  
  const std::vector<std::unique_ptr<Contrainte>>& getListe_Contraintes() const;
  
  const std::vector<std::unique_ptr<ChampForces>>& getListe_Champs_Forces() const;

  double getTime();
   
  void afficher(std::ostream& sortie) const;
  void afficher_CSV(std::vector<std::ofstream>& list_CSV) const;
  
  void dessine_sur(SupportADessin& support) override;
  
  void evolue(double dt);

  /// Our current collision check mechanism does not deal with the edge case where both collision types occur in the same time step
  /// The design choice is to first deal with ground collision
  /// The collision check also does not deal with multiple objects colliding simultaneously at the same time step

  std::vector<std::vector<int>> check_collision() const; /// For each object: returns 0 if no collision, 1 if collision with another object, and 2 if collision with the ground 
  
  void make_collide_pair(PointMateriel& pm1, PointMateriel& pm2) const;
  
  void make_collide_ground(PointMateriel& pm) const; 
};

std::ostream& operator<<(std::ostream& sortie, const Systeme& s);



