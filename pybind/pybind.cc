#include <vector>
#include <cmath>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
#include "Systeme.h"
#include "IntegrateurNewmark.h"
#include <memory>
#include <stdexcept>
#include <string>
#include <fstream>
#include <filesystem>
#include "PointMateriel.h"
#include "Vecteur.h"
#include "Contrainte.h"
#include "ContrainteLibre.h"
#include "ContrainteSpherique.h"
#include "GravitationConstante.h"
#include "Champ_Forces_Nul.h"
#include <cmath>


namespace py = pybind11;


struct RL_Sim_Output
{
    bool hit;
    double t_hit;
    double min_distance;
    bool target_hit_ground;
    bool proj_hit_ground;
};

double calc_distance(const std::vector<double>& x,const std::vector<double>& y)
{
    double sum = 0.0;
    for (size_t i(0);i<x.size();++i)
    {
        sum+= (x[i] - y[i]) * (x[i] - y[i]);
    }
    return std::sqrt(sum);
}

class Simulator
{
    private:
        Systeme sys;
        size_t projectile_index;
        size_t target_index;

    public:
        Simulator()=default;

        /// We add masses in constructor since I envision adding collisions
        void reset(std::vector<double> projectile_initial_p, double projectile_mass, const Contrainte* projectile_constraint,
           std::vector<double> target_initial_p,std::vector<double> target_initial_v,double target_mass,
           const Contrainte* target_constraint, const ChampForces* proj_force_field, const ChampForces* targ_force_field)
        {
            sys = Systeme();
            projectile_index = 0;
            target_index =1;

            // Convert Cartesian initial conditions to the generalized coordinates
            // expected by the constraint (e.g. (theta, phi) for ContrainteSpherique).
            // For ContrainteLibre the default implementation is the identity, so
            // this is safe for all constraint types.
            Vecteur proj_gen_pos = projectile_constraint
                ? projectile_constraint->cart_to_gen_pos(Vecteur(projectile_initial_p))
                : Vecteur(projectile_initial_p);
            Vecteur proj_gen_vel = projectile_constraint
                ? projectile_constraint->cart_to_gen_vel(Vecteur(projectile_initial_p), Vecteur(0.0, 0.0, 0.0))
                : Vecteur(0.0, 0.0, 0.0);

            Vecteur targ_gen_pos = target_constraint
                ? target_constraint->cart_to_gen_pos(Vecteur(target_initial_p))
                : Vecteur(target_initial_p);
            Vecteur targ_gen_vel = target_constraint
                ? target_constraint->cart_to_gen_vel(Vecteur(target_initial_p), Vecteur(target_initial_v))
                : Vecteur(target_initial_v);

            auto projectile = std::make_unique<PointMateriel>(proj_gen_pos, proj_gen_vel, projectile_mass);
            auto target = std::make_unique<PointMateriel>(targ_gen_pos, targ_gen_vel, target_mass);

            std::unique_ptr<Contrainte> p_constr;
            if (!projectile_constraint) 
            {
                p_constr = std::make_unique<ContrainteLibre>();
            }
            else
            {
                p_constr = projectile_constraint->copie();
            }

            std::unique_ptr<Contrainte> t_constr;
            if (!target_constraint) 
            {
                t_constr = std::make_unique<ContrainteLibre>();
            }
            else
            {
                t_constr = target_constraint->copie();
            }

            std::unique_ptr<ChampForces> p_ff;
            if (!proj_force_field) 
            {
                p_ff = std::make_unique<GravitationConstante>(Vecteur({0,0,-9.81}));
            }
            else
            {
                p_ff = proj_force_field->copie();
            }

            std::unique_ptr<ChampForces> t_ff;
            if (!targ_force_field) 
            {
                t_ff = std::make_unique<Champ_Forces_Nul>();
            }
            else
            {
                t_ff = targ_force_field->copie();
            }

            sys.ajouter_Objet(std::move(projectile));
            sys.ajouter_Objet(std::move(target));

            sys.ajouter_Contrainte(std::move(p_constr));
            sys.ajouter_Contrainte(std::move(t_constr));

            sys.ajouter_Contrainte_Objet(0,0);
            sys.ajouter_Contrainte_Objet(1,1);

            sys.ajouter_Champ_Forces(std::move(p_ff));
            sys.ajouter_Champ_Forces(std::move(t_ff));

            sys.ajouter_Champ_Forces_Objet(0,0);
            sys.ajouter_Champ_Forces_Objet(1,1);
        }

        void set_projectile_initial_velocity(std::vector<double> v0)
        {
            auto& objets = sys.getListe_Objets();
            objets[projectile_index]->setVitesse(Vecteur(v0));
        }

        RL_Sim_Output run_sim(int num_iter,double dt, double hit_threshold, bool write_csv = false)
        {
            RL_Sim_Output res;
            res.hit = false;
            res.t_hit = 0;
            res.min_distance = 1e300;  ///Ensures actual minimum distance is logged
            res.target_hit_ground = false;
            res.proj_hit_ground = false;
            
            
            std::vector<std::ofstream> vect_csv;
            if (write_csv)
            {
                std::filesystem::create_directories("data");
                vect_csv.reserve(sys.getListe_Objets().size());
                for (size_t i=0;i<sys.getListe_Objets().size();++i)
                {
                    std::string name = "data/PM" + std::to_string(i) + ".csv";
                    vect_csv.emplace_back(name);
                    vect_csv[i] << "t,x,y,z\n";
                }
                sys.afficher_CSV(vect_csv);
            }

            for (int i(0);i<num_iter;++i)
            {
                sys.evolue(dt);

                auto& objects = sys.getListe_Objets();
                auto pos_proj = objects[projectile_index]->position();
                auto pos_tar = objects[target_index]->position();

                double dist = calc_distance(pos_proj.getvect(),pos_tar.getvect());
                if (dist<res.min_distance) {res.min_distance=dist;}
                if (pos_tar[2] <= 0.01) {res.target_hit_ground = true;}
                if (pos_proj[2] <= 0.01) {res.proj_hit_ground = true;}
                if (dist<hit_threshold)
                {
                    res.hit = true;
                    res.t_hit = sys.getTime();
                    break;
                }
                
                if (write_csv)
                {
                    sys.afficher_CSV(vect_csv);
                }
                
            }
            if (!res.hit)
            {
                res.t_hit = sys.getTime();  /// We make the hit the final time
            }

            return res;
        }
};

PYBIND11_MODULE(simulation, m) {

    py::class_<RL_Sim_Output>(m, "RL_Sim_Output")
        .def_readonly("hit", &RL_Sim_Output::hit)
        .def_readonly("t_hit", &RL_Sim_Output::t_hit)
        .def_readonly("min_distance", &RL_Sim_Output::min_distance)
        .def_readonly("target_hit_ground", &RL_Sim_Output::target_hit_ground)
        .def_readonly("proj_hit_ground", &RL_Sim_Output::proj_hit_ground);

    py::class_<Contrainte, std::unique_ptr<Contrainte>>(m, "Contrainte");

    py::class_<ContrainteLibre, Contrainte, std::unique_ptr<ContrainteLibre>>(m, "ContrainteLibre")
        .def(py::init<>());

    py::class_<ChampForces, std::unique_ptr<ChampForces>>(m, "ChampForces");

    py::class_<Champ_Forces_Nul, ChampForces, std::unique_ptr<Champ_Forces_Nul>>(m, "Champ_Forces_Nul")
    .def(py::init<>());

    py::class_<GravitationConstante, ChampForces, std::unique_ptr<GravitationConstante>>(m, "GravitationConstante")
    .def(py::init([](std::vector<double> gvec) {
        return std::make_unique<GravitationConstante>(Vecteur(gvec));
    }), py::arg("g"));

    py::class_<ContrainteSpherique, Contrainte, std::unique_ptr<ContrainteSpherique>>(m, "ContrainteSpherique")
        .def(py::init([](double rayon, std::vector<double> pivot) {
            return std::make_unique<ContrainteSpherique>(rayon, Vecteur(pivot));
        }), py::arg("rayon"), py::arg("pivot") = std::vector<double>{0.0, 0.0, 0.0});


    py::class_<Simulator>(m, "Simulator")
        .def(py::init<>())

        .def("reset", &Simulator::reset,
            py::arg("projectile_initial_p") = std::vector<double>{0.0, 0.0, 0.0},
            py::arg("projectile_mass") = 1.0,
            py::arg("projectile_constraint") = (const Contrainte*) nullptr,
            py::arg("target_initial_p") = std::vector<double>{0.0, 0.0, 0.0},
            py::arg("target_initial_v") = std::vector<double>{0.0, 0.0, 0.0},
            py::arg("target_mass") = 1.0,
            py::arg("target_constraint") = (const Contrainte*) nullptr,
            py::arg("proj_force_field") = (const ChampForces*) nullptr,
            py::arg("targ_force_field") = (const ChampForces*) nullptr)


        .def("set_projectile_initial_velocity", &Simulator::set_projectile_initial_velocity,
             py::arg("v0"))

        .def("run_sim",
             &Simulator::run_sim,
             py::arg("num_iter"),
             py::arg("dt"),
             py::arg("hit_threshold"),
             py::arg("write_csv") = false);
}



