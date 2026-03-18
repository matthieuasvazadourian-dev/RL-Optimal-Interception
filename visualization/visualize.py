import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

def load_data(sim_output):
    data = np.genfromtxt(sim_output,delimiter=",",names=True)
    t = data["t"].astype(float)
    x = data["x"].astype(float)
    y = data["y"].astype(float)
    z = data["z"].astype(float)

    return t,x,y,z

tp,xp,yp,zp = load_data("data/PM0.csv")
tt,xt,yt,zt = load_data("data/PM1.csv")

dt = float(tp[1]-tp[0])
speed = 1000
interval_ms = 1000 * dt/speed

fig = plt.figure()
ax = fig.add_subplot(projection="3d")

line0, = ax.plot([], [], [], lw=2, label="PM0")
line1, = ax.plot([], [], [], lw=2, label="PM1")
pt0, = ax.plot([], [], [], "o")
pt1, = ax.plot([], [], [], "o")
time_text = ax.text2D(0.02, 0.95, "", transform=ax.transAxes)

allx = np.concatenate([xp, xt])
ally = np.concatenate([yp, yt])
allz = np.concatenate([zp, zt])

pad = 0.05
def lims(a):
    lo, hi = float(a.min()), float(a.max())
    span = hi - lo + 1e-9
    return lo - pad * span, hi + pad * span

ax.set_xlim(*lims(allx))
ax.set_ylim(*lims(ally))
ax.set_zlim(*lims(allz))
ax.legend(loc="upper right")

def update(i):
    line0.set_data(xp[:i+1], yp[:i+1])
    line0.set_3d_properties(zp[:i+1])

    line1.set_data(xt[:i+1], yt[:i+1])
    line1.set_3d_properties(zt[:i+1])

    pt0.set_data([xp[i]], [yp[i]])
    pt0.set_3d_properties([zp[i]])

    pt1.set_data([xt[i]], [yt[i]])
    pt1.set_3d_properties([zt[i]])

    time_text.set_text(f"t = {tp[i]:.3f} s")
    return line0, line1, pt0, pt1, time_text

ani = FuncAnimation(fig, update, frames=len(tp), interval=interval_ms, blit=False)
plt.show()


