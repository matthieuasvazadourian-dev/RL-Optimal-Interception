#pragma once
#include "PointMateriel.h"
#include "Systeme.h"
#include "SupportADessin.h"
#include <iostream>

class TextViewer : public SupportADessin
{
	private: 
	std::ostream& flot;
	
	public:
	TextViewer(std::ostream& sortie);
	
	///Les methodes d'affichage
	virtual void dessine(PointMateriel const& pm) override;  
	virtual void dessine(Systeme const& s) override;
};
