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
T=101
dt=1
X=101
dx=1
initial_state=[]
temp1=[i for i in np.sin(np.pi*np.linspace(0, 2, int(X/(3*dx))))]
temp2=[i for i in np.sin(np.pi*(np.linspace(0, 2, int(X/(3*dx)))+1))]
temp1=temp1+[0]*(int(X/dx)-len(temp1))
temp2=[0]*(int(X/dx)-len(temp2))+temp2
init1=[temp1]
init2=[temp2]
for j in range(1, int(T/dt)):
    temp1=[0]*len(temp1)
    temp2=[0]*len(temp2)
    init1.append(temp1)
    init2.append(temp2)
c1=1
c2=-c1
rho=1
initial_state.append(init1)
initial_state.append(init2)
Build=B.Builder(F_base, 'c1 c2', [c1, c2], 't1 w1 t2 w2', initial_state, dt, T, dx, X)
del Build
pr=sub.run(['Solver1.exe', Inp_f, Outp_f])
#print(pr.args, pr.returncode, pr.stdout, pr.stderr)
with open(Outp_f) as js:
    arr=json.load(js)
solution=np.array(arr["States"], dtype=np.float64)
W1=solution[0]
W2=solution[1]
Wsum=W1+W2
#solution=solution.T
#print(solution)
fig, ax=plt.subplots(1, 3)
lins1, =ax[0].plot([], [], label='W1')
lins2, =ax[1].plot([], [], label='W2')
lins3, =ax[2].plot([], [], label='Wsum')
def init():
    lins1.set_xdata([])
    lins1.set_ydata([])
    lins2.set_xdata([])
    lins2.set_ydata([])
    lins3.set_ydata([])
    lins3.set_ydata([])
def update(frame):
    p1=[j for j in range(len(W1.T))]
    q1=[j[frame] for j in W1.T]
    lins1.set_xdata(p1)
    lins1.set_ydata(q1)
    p2=[j for j in range(len(W2.T))]
    q2=[j[frame] for j in W2.T]
    lins2.set_xdata(p2)
    lins2.set_ydata(q2)
    p3=[j for j in range(len(Wsum.T))]
    q3=[j[frame] for j in Wsum.T]
    lins3.set_xdata(p3)
    lins3.set_ydata(q3)
    return (lins1, lins2, lins3, )
ani=anim.FuncAnimation(fig=fig, init_func=init, func=update, frames=int(T/dt), interval=30)
X=X/dx
dx=X
dy=np.max(Wsum)
ax[0].legend(loc=0)
ax[1].legend(loc=0)
ax[2].legend(loc=0)
ax[0].set_xlim(0-dx*0.01, X+dx*0.01)
ax[0].set_ylim(np.min(W1)-dy*0.05, np.max(W1)+dy*0.05)
ax[1].set_xlim(0-dx*0.01, X+dx*0.01)
ax[1].set_ylim(np.min(W2)-dy*0.05, np.max(W2)+dy*0.05)
ax[2].set_xlim(0-dx*0.01, X+dx*0.01)
ax[2].set_ylim(np.min(Wsum)-dy*0.05, np.max(Wsum)+dy*0.05)
plt.show()