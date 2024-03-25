import Builder as B
import json
import subprocess as sub
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.ticker as tic
import matplotlib.animation as anim
import math as m

F_base='tt'
Inp_f=F_base+'.json'
Outp_f=F_base+'_outp.json'
u=np.linspace(1, 100, 100)
T=100
dt=0.9
initial_state_x=[abs(i) for i in np.sin(np.pi*u/10)]
initial_state_t=[m.sin(u[0]*np.pi/10)]+[0]*(int(T/dt)-1)
X=100
dx=1
l=1
Build=B.Builder(F_base, 'lambda', [l], 't x', initial_state_t, initial_state_x, dt, T, dx, X)
del Build
pr=sub.run(['Solver.exe', Inp_f, Outp_f])
#print(pr.args, pr.returncode, pr.stdout, pr.stderr)
with open(Outp_f) as js:
    arr=json.load(js)
solution=np.array(arr["States"], dtype=np.float64)
#solution=solution.T
#print(solution)
fig, ax=plt.subplots()
lins, =ax.plot([], [])
def init():
    lins.set_xdata([])
    lins.set_ydata([])
def update(frame):
    p=[j for j in range(len(solution))]
    q=[j[frame] for j in solution]
    lins.set_xdata(p)
    lins.set_ydata(q)
    return (lins, )
ani=anim.FuncAnimation(fig=fig, init_func=init, func=update, frames=int(T/dt), interval=30)
dx=len(solution)
dy=np.max(solution)
ax.set_xlim(0-dx*0.01, len(solution)+dx*0.01)
ax.set_ylim(0-dy*0.05, np.max(solution)+dy*0.05)
plt.show()