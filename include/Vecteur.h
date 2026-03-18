#pragma once

#include <vector>
#include <iostream>
#include <cmath>
#include <initializer_list>

class Vecteur
{
	private:
	std::vector <double> vect;
	
	public:
	
	/// Construceurs ///
	
	Vecteur(size_t n);
	
	Vecteur(double x, double y, double z);
	
	Vecteur (std::initializer_list<double> liste);
	
	Vecteur(std::vector<double> vector);
	
	Vecteur(const Vecteur& vect2)=default;
	
	///Accesseur///
	
	std::vector<double> getvect() const;
	
	size_t getsize() const;
	
	
	///Methodes de modification et de comparaison///
	void augmente(double new_dim);
	
	void set_coord(unsigned int dim, double val);
	
	void affiche();
	
	bool compare(Vecteur vect2);
	
	///Operations vectorielles///
	
	Vecteur addition(Vecteur vect2) const;
		
	Vecteur soustraction(Vecteur vect2) const;
	
	Vecteur oppose() const;
	
	Vecteur mult(double x) const;
	
	double prod_scal(Vecteur vect2) const;
	
	Vecteur prod_vect(Vecteur vect2);
	
	double norme() const;
	
	double norme2() const;
	
	Vecteur unitaire() const;
	
	///Opérateurs algébriques surchargés de manière interne///
	
	Vecteur operator+(const Vecteur& other) const;
	
	Vecteur& operator+=(const Vecteur& other);
    
    Vecteur operator-(const Vecteur& other) const;
    
    Vecteur& operator-=(const Vecteur& other);
    
    double operator*(const Vecteur& other) const;
    
    Vecteur operator^(const Vecteur& other) const;
    
    Vecteur operator~() const;
    
    Vecteur& operator=(const Vecteur& other) ;
    
    Vecteur operator*(double scalaire) const;

	Vecteur operator/(double scalaire) const;
	
	double& operator[](std::size_t n);
	
	const double& operator[](std::size_t n) const;
    
};

bool operator==(Vecteur vect1, Vecteur vect2);
bool operator!=(Vecteur vect1, Vecteur vect2);
std::ostream& operator<<(std::ostream& sortie, const Vecteur& vector_to_display);


