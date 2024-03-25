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
T=52
dt=1
X=100
dx=1
initial_state=[]
init_P=[]
init_V=[]
for j in range(int(T/dt)):
    temp_P=[]
    temp_V=[0]*(int(X/dx))
    if (j==0):
        temp_P=[0]*(int(X/(2*dx))-1)+[1]+[0]*(int(X/(2*dx)))
    else:
        temp_P=[0]*(int(X/dx))
    init_P.append(temp_P)
    init_V.append(temp_V)
c1=1
c2=-c1
rho=1
temps1=[]
temps2=[]
for i in range(len(init_P)):
    temp1=[]
    temp2=[]
    for j in range(len(init_P[i])):
        temp1.append(init_P[i][j]/c1/rho+init_V[i][j])
        temp2.append(-init_P[i][j]/c1/rho+init_V[i][j])
    temps1.append(temp1)
    temps2.append(temp2)
initial_state.append(temps1)
initial_state.append(temps2)
Build=B.Builder(F_base, 'c1 c2', [c1, c2], 't1 w1 t2 w2', initial_state, dt, T, dx, X)
del Build
pr=sub.run(['Solver.exe', Inp_f, Outp_f])
#print(pr.args, pr.returncode, pr.stdout, pr.stderr)
with open(Outp_f) as js:
    arr=json.load(js)
solution=np.array(arr["States"], dtype=np.float64)
P=(solution[0]-solution[1])/2*rho*c1
V=(solution[0]+solution[1])/2
#solution=solution.T
#print(solution)
fig, ax=plt.subplots(1, 2)
lins1, =ax[0].plot([], [], label='P')
lins2, =ax[1].plot([], [], label='V')
def init():
    lins1.set_xdata([])
    lins1.set_ydata([])
    lins2.set_xdata([])
    lins2.set_ydata([])
def update(frame):
    p1=[j for j in range(len(P.T))]
    q1=[j[frame] for j in P.T]
    lins1.set_xdata(p1)
    lins1.set_ydata(q1)
    p2=[j for j in range(len(V.T))]
    q2=[j[frame] for j in V.T]
    lins2.set_xdata(p2)
    lins2.set_ydata(q2)
    return (lins1, lins2, )
ani=anim.FuncAnimation(fig=fig, init_func=init, func=update, frames=int(T/dt), interval=30)
X=X/dx
dx=X
dy=np.max(P)
ax[0].legend(loc=0)
ax[1].legend(loc=0)
ax[0].set_xlim(0-dx*0.01, X+dx*0.01)
ax[0].set_ylim(np.min(P)-dy*0.05, np.max(P)+dy*0.05)
ax[1].set_xlim(0-dx*0.01, X+dx*0.01)
ax[1].set_ylim(np.min(V)-dy*0.05, np.max(V)+dy*0.05)
plt.show()