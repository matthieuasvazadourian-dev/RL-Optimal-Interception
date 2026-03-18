#include "TextViewer.h"
#include <iostream>

using namespace std;

TextViewer::TextViewer(std::ostream& sortie)
:flot(sortie)
{}

///Les methodes d'affichage utilisent l'operateur d'affichage des classes concernees

void TextViewer::dessine(PointMateriel const& pm)
{
	flot<<pm<<endl;
}

void TextViewer::dessine(Systeme const& s)
{
	flot<<s<<endl;
}

