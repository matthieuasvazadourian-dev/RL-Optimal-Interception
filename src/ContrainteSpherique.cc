#include <iostream>
#include <cmath>
#include "ContrainteSpherique.h"
#include "Vecteur.h"
#include "ObjetPhysique.h"
#include <memory>


using namespace std;

ContrainteSpherique::ContrainteSpherique(double rayon, Vecteur pivot)
: R(rayon), pivot_(pivot)
{}

Vecteur ContrainteSpherique::position(const ObjetPhysique& obj) const 
{
	
	const Vecteur& param = obj.getPosition();
	
	double theta = param[0];
	double phi = param[1];
	
	
	double sin_theta = sin(theta);
	double cos_theta = cos(theta);
	double sin_phi = sin(phi);
	double cos_phi = cos(phi);
	
	/// Expression des positions dans le repere cartesien
	
	double x = pivot_[0] + R * sin_theta * cos_phi;
	double y = pivot_[1] + R * sin_theta * sin_phi;
	double z = pivot_[2] + R * cos_theta;

	return Vecteur({x, y, z});
}

Vecteur ContrainteSpherique::vitesse(const ObjetPhysique& obj) const 
{
	
	const Vecteur& param_dot = obj.getVitesse();
	const Vecteur& param = obj.getPosition();
	
	double theta = param[0];
	double phi = param[1];
	
	double theta_dot = param_dot[0];
	double phi_dot = param_dot[1];
	
	
	double sin_theta = sin(theta);
	double cos_theta = cos(theta);
	double sin_phi = sin(phi);
	double cos_phi = cos(phi);
	
	/// Expression des vitesse dans le repere cartesien
	
	double v_x = R * cos_theta * cos_phi * theta_dot - R * sin_theta * sin_phi* phi_dot;
	
	double v_y = R * cos_theta * sin_phi * theta_dot + R * sin_theta * cos_phi* phi_dot;

	double v_z = - R * sin_theta * theta_dot;
	
	return Vecteur({v_x, v_y, v_z});
}

Vecteur ContrainteSpherique::applique_force(const ObjetPhysique& obj, const Vecteur& force, double t) const 
{
	
	const Vecteur& param = obj.getPosition();
	const Vecteur& param_dot = obj.getVitesse();
	
	double theta = param[0];
	double phi = param[1];


	double theta_dot = param_dot[0];
	double phi_dot = param_dot[1];
	
	double sin_theta = sin(theta);
	double cos_theta = cos(theta);
	double sin_phi = sin(phi);
	double cos_phi = cos(phi); 			/// Permet d'eviter trop de parentheses pour la lisibilite

	double m = obj.getMasse();

	Vecteur u_theta(cos_theta*cos_phi, cos_theta*sin_phi, -sin_theta); /// car z vers le bas
	Vecteur u_phi(-sin_phi, cos_phi, 0.0); 

	/// Application des formules du complement mathematique


	double theta_ddot = (1.0/(m*R))*(force*u_theta)+sin_theta*cos_theta*phi_dot*phi_dot;
	double phi_ddot;
	if (fabs(sin_theta)<1e-5) 
	{
		phi_ddot = 0.0;
	} 
	else 
	{
		phi_ddot = (1/(m*R*sin_theta))*(force*u_phi)-2*theta_dot*phi_dot*(cos_theta/sin_theta);
	}
	
	
	return Vecteur({theta_ddot, phi_ddot});
}

Vecteur ContrainteSpherique::cart_to_gen_pos(const Vecteur& cart_pos) const
{
	double x = cart_pos[0] - pivot_[0];
	double y = cart_pos[1] - pivot_[1];
	double z = cart_pos[2] - pivot_[2];

	double r     = sqrt(x*x + y*y + z*z);
	double theta = acos(z / r);
	double phi   = atan2(y, x);

	return Vecteur({theta, phi});
}

Vecteur ContrainteSpherique::cart_to_gen_vel(const Vecteur& cart_pos, const Vecteur& cart_vel) const
{
	double x = cart_pos[0] - pivot_[0];
	double y = cart_pos[1] - pivot_[1];
	double z = cart_pos[2] - pivot_[2];

	double r     = sqrt(x*x + y*y + z*z);
	double theta = acos(z / r);
	double phi   = atan2(y, x);

	double sin_theta = sin(theta);
	double cos_theta = cos(theta);
	double sin_phi   = sin(phi);
	double cos_phi   = cos(phi);

	Vecteur u_theta(cos_theta*cos_phi, cos_theta*sin_phi, -sin_theta);
	Vecteur u_phi(-sin_phi, cos_phi, 0.0); 

	double v_dot_u_theta = cart_vel.prod_scal(u_theta);
	double v_dot_u_phi = cart_vel.prod_scal(u_phi);

	double theta_dot = v_dot_u_theta / R;
	double phi_dot;
	if (std::fabs(sin_theta) < 1e-5)
	{
		phi_dot = 0.0;
	}
	else
	{
		phi_dot = v_dot_u_phi / (R * sin_theta);
	}

	return Vecteur({theta_dot, phi_dot});
}

void ContrainteSpherique::afficher(ostream& sortie) const
{
	sortie<<"Spherique";
}

unique_ptr<Contrainte> ContrainteSpherique::copie() const
{
	return unique_ptr<Contrainte>(new ContrainteSpherique(*this));
}
