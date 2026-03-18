#pragma once

class PointMateriel;
class Systeme;


class SupportADessin
{
 public:
  virtual ~SupportADessin() = default;
  // on ne copie pas les Supports
  SupportADessin(SupportADessin const&)            = delete;
  SupportADessin& operator=(SupportADessin const&) = delete;
   // mais on peut les déplacer
  SupportADessin(SupportADessin&&)            = default;
  SupportADessin& operator=(SupportADessin&&) = default;

  SupportADessin() = default;

  virtual void dessine(PointMateriel const& pm) = 0;
  virtual void dessine(Systeme const& s) = 0;


  /// Mettre ici toutes les méthodes nécessaires pour dessiner tous les objets que l'on veut dessiner.
};
