import Builder1 as B
import Graphics as G
import json
import subprocess as sub
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.ticker as tic
import matplotlib.animation as anim
import math as m

def AnSolv(Wave, l, N): #l>0
    Wavez=[Wave]
    for i in range(N-1):
        TWave=Wavez[-1].copy()
        for j in range(l):
            del TWave[-1]
            TWave=[0]+TWave
        Wavez.append(TWave)
    return Wavez
F_base='tt'
Inp_f=F_base+'.json'
Outp_f=F_base+'_outp.json'
T=50
dt=1
X=101
dxs=np.linspace(0.6, 1, 5)
c=1
rho=1
E=[]
for dx in dxs:
    initial_state = []
    temp1 = [i for i in np.sin(np.pi * np.linspace(0, 2, int(X / (3 * dx))))]
    temp1 = temp1 + [0] * (int(X / dx) - len(temp1))
    AnWave=np.array(AnSolv(temp1, int(c/dx), int(T/dt)))
    init1 = [temp1]
    for j in range(1, int(T / dt)):
        temp1 = [0] * len(temp1)
        init1.append(temp1)
    initial_state.append(init1)
    Build=B.Builder(F_base, 'c', [int(c/dx)], 't w', initial_state, dt, T, dx, X)
    del Build
    pr=sub.run(['Solver2.exe', Inp_f, Outp_f])
    #print(pr.args, pr.returncode, pr.stdout, pr.stderr)
    with open(Outp_f) as js:
        arr=json.load(js)
    solution=np.array(arr["States"], dtype=np.float64)
    Wave=solution[0]
    E.append(np.max(np.abs(AnWave-Wave)))
#solution=solution.T
#print(solution)
E=np.array(E)
h=np.array(dxs)
coeffs=G.mms_a_bx(h, E)
fig, ax=plt.subplots()
ax.plot(np.log(h), np.log(E), label='ln(E)=({0:.4f}+-{1:.4f})*ln(h)+({2:.4f}+-{3:.4f})'.format(*coeffs))
ax.legend(loc=0)
ax.set_xlabel('h')
ax.set_ylabel('E')
plt.show()
#fig.savefig('E(h).png')