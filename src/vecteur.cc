#include "Vecteur.h"
#include <stdexcept>

using namespace std;

/// Construceurs ///
	
Vecteur::Vecteur(size_t n)
: vect(n,0)
{}
	
Vecteur::Vecteur(double x, double y, double z)
{
	vect.push_back(x);
	vect.push_back(y);
	vect.push_back(z);
}
	
Vecteur::Vecteur (initializer_list<double> liste)
: vect(liste)
{}
	
Vecteur::Vecteur(vector<double> vector)
: vect(vector)
{}
		
///Accesseurs///
	
vector<double> Vecteur::getvect() const{
	return vect;
}	

size_t Vecteur::getsize() const {
	return vect.size();
}

///Methodes de modification et de comparaison///
void Vecteur::augmente(double new_dim)
{
	vect.push_back(new_dim);
}
	
void Vecteur::set_coord(unsigned int dim, double val)
{
	size_t n(vect.size());
	if (dim<n){vect[dim]=val;}
	else {cerr<<"Taille du vecteur unsuffisante"<<endl;}
}
	
void Vecteur::affiche()
{
	for (double coord : vect)
	{
		cout<<coord<<" ";
	}
	cout<<endl;
}
	
bool Vecteur::compare(Vecteur vect2)
{
	size_t n(vect.size());
	size_t m(vect2.vect.size());
	
	if (n==m)
	{
		for(size_t i(0);i<n;++i)
		{
			if(vect[i]!=vect2.vect[i]){return false;}
		}
		return true;
	}
	else
	{return false;}	
}
	
///Operations vectorielles///
	
Vecteur Vecteur::addition(Vecteur vect2) const   ///Le premier operande determine la dimension du vecteur resultant
{
	vector<double> result;
	for (size_t i = 0; i < vect.size(); ++i) 
	{
		result.push_back(vect[i] + vect2.vect[i]);
	}
	return Vecteur(result);
}
		
Vecteur Vecteur::soustraction(Vecteur vect2) const ///Le premier operande determine la dimension du vecteur resultant
{
	vector<double> result;
	for (size_t i = 0; i < vect.size(); ++i) 
	{
		result.push_back(vect[i] - vect2.vect[i]);
	}
	return Vecteur(result);	
}
		
Vecteur Vecteur::oppose() const
{
	size_t n(vect.size());
	Vecteur vect_res(n);
	
	for (size_t i(0);i<n;++i)
	{
		vect_res.vect[i]=-vect[i];
	}
	
	return vect_res;
}
	
Vecteur Vecteur::mult(double x) const
{
	size_t n(vect.size());
	Vecteur vect_res(n);
	
	for (size_t i(0);i<n;++i)
	{
		vect_res.vect[i]=x*vect[i];
	}
	
	return vect_res;
}
	
double Vecteur::prod_scal(Vecteur vect2) const ///Operation impossible sans egalite des dimensions des operandes
{
	size_t n(vect.size());
	size_t m(vect2.vect.size());
	
	double res(0);
	
	if(n!=m)
	{
		cerr<<"Vecteurs de dimension differentes : produit scalaire impossible. La valeur retournee est 0."<<endl;
		return 0;
	}
	else
	{
		for (size_t i(0);i<n;++i)
		{
			res+=vect[i]*vect2.vect[i];
		}
		
		return res;
	}
}
	
Vecteur Vecteur::prod_vect(Vecteur vect2) ///Operation impossible lorsque les operandes ne sont pas de dimension 3
{
	size_t n(vect.size());
	size_t m(vect2.vect.size());
	
	Vecteur vect_res(3);
	
	if (n!=3 or m!=3)
		{
			cerr<<" La dimension d'un des vecteurs est differente de 3 : produit vectoriel impossible. Le vecteur retourne sera donc le vecteur initial."<<endl;
			vect_res.vect=vect;
			return vect_res;
		}
	else
	{
		vect_res.vect[0]=vect[1]*vect2.vect[2]-vect[2]*vect2.vect[1];
		vect_res.vect[1]=vect[2]*vect2.vect[0]-vect[0]*vect2.vect[2];
		vect_res.vect[2]=vect[0]*vect2.vect[1]-vect[1]*vect2.vect[0];
		
		return vect_res;
	}

}
	
double Vecteur::norme() const
{
	double x(0);
	for (double coord : vect)
	{
		x+=coord*coord;
	}
	
	return sqrt(x);
}
	
double Vecteur::norme2() const
{
	double x(0);
	for (double coord : vect)
	{
		x+=coord*coord;
	}
	
	return x;
}

Vecteur Vecteur::unitaire() const
{
	return *this/norme();
}

	
///Opérateurs algébriques surchargés de manière interne///
	
Vecteur Vecteur::operator+(const Vecteur& vect2) const  ///Le premier operande determine la dimension du vecteur resultant
{
	vector<double> res;
	for (size_t i = 0; i < vect.size(); ++i) 
	{
		res.push_back(vect[i] + vect2.vect[i]);
	}
	return Vecteur(res);
}
	
Vecteur& Vecteur::operator+=(const Vecteur& vect2) 
{
	for (size_t i = 0; i < vect.size(); ++i) 
	{
		vect[i] += vect2.vect[i];
	}
	return *this;
}
    
Vecteur Vecteur::operator-(const Vecteur& vect2) const ///Le premier operande determine la dimension du vecteur resultant
{
	vector<double> res;
	for (size_t i = 0; i < vect.size(); ++i) 
	{
		res.push_back(vect[i] - vect2.vect[i]);
	}
	return Vecteur(res);
}
    
Vecteur& Vecteur::operator-=(const Vecteur& vect2) 
{
	for (size_t i = 0; i < vect.size(); ++i) 
	{
		vect[i] -= vect2.vect[i];
	}
	return *this;
}
    
double Vecteur::operator*(const Vecteur& other) const  ///Nous permettons cette operation entre dimensions differentes suite a l'implementation de la classe PointMaterielAngulaire
{
	size_t n(vect.size());
	size_t m(other.vect.size());
	
	double res(0);
	
	if (n<=m)
	{
		for (size_t i(0); i<n; ++i)
		{
			res+=vect[i]*other.vect[i];
		}
		
		return res;
	}
	else 
	{
		for (size_t i(0); i<m; ++i)
		{
			res+=vect[i]*other.vect[i];
		}
	}
	
	return res;	
}
    
Vecteur Vecteur::operator^(const Vecteur& other) const  ///Operation impossible lorsque les operandes ne sont pas de dimension 3
{
	if (vect.size() != 3 || other.vect.size() != 3) 
	{
		cerr<<" La dimension d'un des vecteurs est differente de 3 : produit vectoriel impossible. Le vecteur retourne sera donc le vecteur initial."<<endl;
		Vecteur error_vect(vect);
		return error_vect;
	}
	return Vecteur(
	{
		vect[1] * other.vect[2] - vect[2] * other.vect[1],
		vect[2] * other.vect[0] - vect[0] * other.vect[2],
		vect[0] * other.vect[1] - vect[1] * other.vect[0]
	});
}
    
Vecteur Vecteur::operator~() const 
{
	return *this/norme();
}

Vecteur& Vecteur::operator=(const Vecteur& other)  
{
    if (this != &other) {
        vect = other.vect;
    }
    return *this;
}

Vecteur Vecteur::operator*(double scalaire) const 
{
    vector<double> result;
    for (double val : vect) {
        result.push_back(val * scalaire);
    }
    return Vecteur(result);
}

Vecteur Vecteur::operator/(double scalaire) const 
{
    if (scalaire == 0.0) {
        throw runtime_error("Division par zéro dans Vecteur::operator/.");
    }

    vector<double> result;
    for (double val : vect) {
        result.push_back(val / scalaire);
    }
    return Vecteur(result);
}

double& Vecteur::operator[](size_t n) ///Version non const
{
	return vect[n];
}

const double& Vecteur::operator[](size_t n) const ///Version const
{
	return vect[n];
}
	
	///Operateurs non-algébriques surchargés de manière externe///
	
bool operator==(Vecteur vect1, Vecteur vect2)
{
	vector<double> vector1(vect1.getvect());
	vector<double> vector2(vect2.getvect());
	
	if (vector1.size()!=vector2.size()){return false;}
	else
	{
		for (size_t i(0); i<vector1.size();++i)
		{
			if(vector1[i]!=vector2[i]){return false;}
		}
	}
	return true;
}

bool operator!=(Vecteur vect1, Vecteur vect2)
{
	if (vect1 == vect2){
		return false;
	} else {
		return true;
	}
}

ostream& operator<<(ostream& sortie, const Vecteur& vector_to_display)
{
	vector<double> vector = vector_to_display.getvect();
	
    if (not vector.empty()) {
		sortie<<vector[0];
		for (size_t i = 1; i < vector.size(); ++i)
		{
			sortie<< "," << vector[i];
		}
	}
	return sortie;
}
