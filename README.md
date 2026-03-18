This reinforcement learning project's purpose is to compute the optimal launch velocity for a projectile to intercept a target.
The agent uses CMA-ES to determine the initial velocity vector that maximises a reward engineered to include:
- Hit bonus
- Proximity reward
- Time penalty (Encourages early interception)
- Speed penalty (Allows projectile speed regulation)
- Bounce constraints (Optional target bounce enforcement)

The physics simulation is implemented in C++. It utilises the Newmark method for numerical integration and allows for simulation under different constraints and forces, and models collision physics as elastic collisions. 
It is exposed to Python through pybind11.

After the model's training converges or reaches its stopping conditions (can be set), a 3D animation of the optimal trajectory and objects in the system is displayed automatically through a Matplotlib animation.\



# Use
- Use "make" to compile
- Modify parameters and run with train.py


# The parameters of the agent can be modified in agent.py and are:

initial_guess: Initial velocity guess for CMA-ES

initial_stand_dev: Initial standard deviation used by CMA-ES

target_p0: Target's initial position

target_v0: Target initial velocity 

v0_max: Maximum allowed launch speed

T_max: Maximum simulation time

projectile/target_constraint: Projectile or target's physical constraints (free or spherical)

proj/targ_mass: Projectile or target's mass

popsize: Population size used during one iteration of CMA-ES

maxiter: Maximum number of generations used in CMA-ES

tolfun: Tolerance on reward improvement used in CMA-ES

dt: Time step for numerical integration

hit_threshold: Distance under which we consider the projectile and target to be colliding

proj/targ_force_field: Projectile or target's force field (none, uniform gravitational field)

enforce_target_bounce: Require target to bounce off the ground before hit

allow_projectile_bounces: Allow the projectile to bounce off ground
