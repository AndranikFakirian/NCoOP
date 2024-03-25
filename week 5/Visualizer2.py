import Builder1 as B
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
T=50
dt=1
X=101
dx=2
initial_state=[]
temp1=[i for i in np.sin(np.pi*np.linspace(0, 2, int(X/(3*dx))))]
temp1=temp1+[0]*(int(X/dx)-len(temp1))
init1=[temp1]
for j in range(1, int(T/dt)):
    temp1=[0]*len(temp1)
    init1.append(temp1)
c=1
rho=1
initial_state.append(init1)
Build=B.Builder(F_base, 'c', [int(c/dx)], 't w', initial_state, dt, T, dx, X)
del Build
pr=sub.run(['Solver2.exe', Inp_f, Outp_f])
#print(pr.args, pr.returncode, pr.stdout, pr.stderr)
with open(Outp_f) as js:
    arr=json.load(js)
solution=np.array(arr["States"], dtype=np.float64)
W=solution[0]
print(int(T/dt), np.shape(W))
#solution=solution.T
#print(solution)
fig, ax=plt.subplots()
lins, =ax.plot([], [], label='W')
def init():
    lins.set_xdata([])
    lins.set_ydata([])
def update(frame):
    p=[j for j in range(len(W.T))]
    q=[j[frame] for j in W.T]
    lins.set_xdata(p)
    lins.set_ydata(q)
    return (lins, )
ani=anim.FuncAnimation(fig=fig, init_func=init, func=update, frames=int(T/dt), interval=30)
X=X/dx
dx=X
dy=np.max(W)
ax.legend(loc=0)
ax.set_xlim(0-dx*0.01, X+dx*0.01)
ax.set_ylim(np.min(W)-dy*0.05, np.max(W)+dy*0.05)
plt.show()