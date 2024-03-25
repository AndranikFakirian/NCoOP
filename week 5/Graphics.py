import numpy as np
import matplotlib.pyplot as plt
import matplotlib.ticker as tic
def sum(p):
    s=p[0]
    for i in range(1, len(p)):
        s+=p[i]
    return s
def mms_a_bx(t, u):
    l=len(t)
    nt=np.array(t)
    nu=np.array(u)
    atu=np.average(nt*nu)
    at=np.average(nt)
    au=np.average(nu)
    at2=np.average(np.square(nt))
    au2=np.average(np.square(nu))
    b=(atu-at*au)/(at2-at**2)
    a=au-b*at
    sb=1/(l**(1/2))*((au2-au**2)/(at2-at**2)-b**2)**(1/2)
    sa=sb*(at2-at**2)**(1/2)
    return (b, a, sb, sa)
def mms_kx(t, u):
    l=len(t)
    nt=np.array(t)
    nu=np.array(u)
    atu=np.average(nt*nu)
    at2=np.average(np.square(nt))
    au2=np.average(np.square(nu))
    k=atu/at2
    sk=((au2/at2-k**2)/l)**(1/2)
    return (k, sk)
def normalizer(n):
    deg=0
    norm=float(n)
    if (abs(norm)<1):
        while True:
            if abs(norm) < 1:
                deg -= 1
                norm *= 10
            else:
                return (norm, deg)
    elif (abs(norm) >= 10):
        while True:
            if abs(norm) >= 10:
                deg += 1
                norm = norm / 10
            else:
                return (norm, deg)
    else:
        return (norm, deg)
def massiveNormalizer(p):
    q=[]
    for i in p:
        for j in normalizer(i):
            q.append(j)
    return q