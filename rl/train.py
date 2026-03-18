import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))

from rl.agent import InterceptionAgent 
import simulation

alg = InterceptionAgent(
    initial_guess=[1, 1, 2],
    initial_stand_dev=1,
    target_p0=[45, 10, 20],
    target_v0=[10, 5, 8],
    v0_max=200,
    T_max=5.0,
    popsize=64,
    maxiter=50000,
    targ_force_field=simulation.GravitationConstante([0, 0, -9.81]),
    proj_force_field=simulation.GravitationConstante([0, 0, -9.81]),
    enforce_target_bounce=True,
    allow_projectile_bounces=True,
)
alg.train()
