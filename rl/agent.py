from pathlib import Path
import sys
import os
import subprocess

PROJECT_ROOT = Path(__file__).resolve().parents[1]

sys.path.insert(0, str(PROJECT_ROOT / "simulation"))

os.chdir(PROJECT_ROOT)

if hasattr(os, 'add_dll_directory'):
    _dll_dir = os.environ.get('UCRT64_BIN', r'C:\msys64\ucrt64\bin')
    if os.path.isdir(_dll_dir):
        os.add_dll_directory(_dll_dir)
import simulation
import numpy as np
import cma

class InterceptionAgent:
    def __init__(self, initial_guess, initial_stand_dev, target_p0, target_v0, v0_max, T_max, projectile_constraint = None,
        target_constraint = None, proj_mass=1, targ_mass=1, popsize = 16, maxiter = 1000, tolfun = 1e-12, dt=0.01, hit_threshold=0.05,
        proj_force_field = None, targ_force_field = None, enforce_target_bounce = False, allow_projectile_bounces = False):
        
        self.x0 = initial_guess
        self.sigma0 = initial_stand_dev
        self.popsize = popsize
        self.maxiter= maxiter
        self.tolfun = tolfun # Stopping condition: Tolerance on reward improvement
        self.v0_max = v0_max
        self.T_max = T_max
        self.target_p0 = target_p0
        self.target_v0 = target_v0
        self.dt = dt
        self.hit_threshold = hit_threshold
        self.projectile_constraint = projectile_constraint
        self.target_constraint = target_constraint
        self.proj_mass = proj_mass
        self.targ_mass = targ_mass
        self.proj_ff = proj_force_field
        self.targ_ff = targ_force_field
        self.enforce_target_bounce = enforce_target_bounce
        self.allow_projectile_bounces = allow_projectile_bounces

        self.sim = simulation.Simulator()

    def reset_sim(self):
        self.sim.reset([0,0,0],self.proj_mass,self.projectile_constraint,self.target_p0,self.target_v0,self.targ_mass,self.target_constraint,self.proj_ff,self.targ_ff)

    def compute_reward(self,v):
        self.reset_sim()
        self.sim.set_projectile_initial_velocity(v)
        v_norm = np.linalg.norm(v)
        res = self.sim.run_sim(int(self.T_max/self.dt),self.dt,self.hit_threshold)

        if (self.enforce_target_bounce):
            valid_hit = res.hit and res.target_hit_ground
            hit_reward = 50 * int(valid_hit)
            projectile_proximity_to_target_reward = 100 * np.exp(- res.min_distance/50)
            time_penalty = 2 * res.t_hit/self.T_max
            speed_penalty = v_norm/self.v0_max
            bounce_penalty = 500 * int(not res.target_hit_ground)
            reward = hit_reward + projectile_proximity_to_target_reward - time_penalty - speed_penalty - bounce_penalty
        else:
            reward = 50 * int(res.hit) + 100 * np.exp(- res.min_distance/50) - 2 * res.t_hit/self.T_max - v_norm/self.v0_max

        if (not self.allow_projectile_bounces):
            forbidden_bounce_penalty = 500 * int(res.proj_hit_ground)
            reward -= forbidden_bounce_penalty

        return reward

    def visualize(self, v_init):
        self.reset_sim()
        self.sim.set_projectile_initial_velocity(v_init)
        res = self.sim.run_sim(int(self.T_max/self.dt),self.dt,self.hit_threshold, write_csv = True)
        subprocess.run([sys.executable, str(PROJECT_ROOT / "visualization" / "visualize.py")], check=True)

    def train(self):
        inops = {
            "popsize": self.popsize,
            "maxiter": self.maxiter,
            "tolfun": self.tolfun
        }

        es = cma.CMAEvolutionStrategy(self.x0, self.sigma0, inops)

        while not es.stop():
            X = es.ask()
            objectives = np.empty(len(X))
            for i, x in enumerate(X):
                objectives[i] = -self.compute_reward(x) # Negative sign since CMA-ES minimizes the objective function
            es.tell(X, objectives)

        optimal_action = es.best.x
        self.visualize(optimal_action)

        return optimal_action
