import Builder as B
import json
import subprocess as sub
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.ticker as tic
import matplotlib.animation as anim

F_base='tt'
Inp_f=F_base+'.json'
Outp_f=F_base+'_outp.json'
initial_state_x=[1]+[0]*99
initial_state_t=[1]+[0]*1
T=100
X=100
dt=1
dx=1
l=1
Build=B.Builder(F_base, 'lambda', [l], 't x', initial_state_t, initial_state_x, dt, T, dx, X)
del Build
pr=sub.run(['Solver.exe', Inp_f, Outp_f])
#print(pr.args, pr.returncode, pr.stdout, pr.stderr)
with open(Outp_f) as js:
    arr=json.load(js)
print(arr["States"])