#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <nlohmann/json.hpp>
using namespace std;
using json = nlohmann::json;

template <typename T>
int mean(T val)
{
    return (T(0)<val);
}

template <typename T>
vector <T> operator +(vector <T> &a, vector <T> &b)
{
    bool t=true;
    int N=0;
    int N1=0;
    if (a.size()>b.size())
    {
        N1=b.size();
        N=a.size();
    }
    else
    {
        N=b.size();
        N1=a.size();
        t=!t;
    }
    vector <T> c;
    for (int i=0; i<N; i++)
    {
        if (i>=N1)
        {
            if (t)
            {
                c.push_back(a[i]);
            }
            else
            {
                c.push_back(b[i]);
            }
        }
        else
        {
            c.push_back(a[i]+b[i]);
        }
    }
    return c;
}

template <typename Q, typename P>
vector <Q> operator *(P k, vector <Q> a)
{
    vector <Q> b;
    for (auto &i: a)
    {
        b.push_back(k*i);
    }
    return b;
}

template <typename Q, typename P>
vector <Q> operator *(vector <Q> a, P k)
{
    vector <Q> b;
    for (auto &i: a)
    {
        b.push_back(k*i);
    }
    return b;
}

template <typename Q>
vector <Q> prev(vector <Q> a, long step)
{
    vector <Q> b;
    b.resize(a.size());
    for (long i=0; i<step; i++)
    {
        b[i]=0;
    }
    for (long i=step; i<a.size(); i++)
    {
        b[i]=a[i-step];
    }
    return b;
}

/*class System {
protected:
    vector <float> initial_state;
    vector <float> params;
    float dt;
    vector <float> station_derivative;
public:
    System(vector <float> initial_state, vector <float> params, float dt): initial_state(initial_state), params(params), dt(dt) {};
    virtual ~System() {};
    virtual vector <float> f()=0;
    vector <float> get_initial_state()
    {
        return initial_state;
    }
    vector <float> get_params()
    {
        return params;
    }
    float get_dt()
    {
        return dt;
    }
};

class Sin_oscillator: public System{
public:
    Sin_oscillator(vector <float> initial_state, vector <float> params, float dt): System(initial_state, params, dt) {};
    virtual ~Sin_oscillator() {};
    vector <float> f()
    {
        station_derivative={1, initial_state[2], -pow(params[0], 2)*sin(initial_state[1])};
        return station_derivative;
    }
};

class Low_vibration_oscillator: public System{
public:
    Low_vibration_oscillator(vector <float> initial_state, vector <float> params, float dt): System(initial_state, params, dt) {};
    virtual ~Low_vibration_oscillator() {};
    vector <float> f()
    {
        station_derivative={1, initial_state[2], -pow(params[0], 2)*initial_state[1]};
        return station_derivative;
    }
};

class LVO_with_dissipative_forces: public System{
public:
    LVO_with_dissipative_forces(vector <float> initial_state, vector <float> params, float dt): System(initial_state, params, dt) {};
    virtual ~LVO_with_dissipative_forces() {};
    vector <float> f()
    {
        station_derivative={1, initial_state[2], -pow(params[0], 2)*initial_state[1]-params[1]*initial_state[2]};
        return station_derivative;
    }
};

class LVOWDnPCF: public System{
public:
    LVOWDnPCF(vector <float> initial_state, vector <float> params, float dt): System(initial_state, params, dt) {};
    virtual ~LVOWDnPCF() {};
    vector <float> f()
    {
        station_derivative={1, initial_state[2], -pow(params[0], 2)*initial_state[1]-params[1]*initial_state[2]+params[2]/params[3]*cos(params[4]*initial_state[0])};
        return station_derivative;
    }
};

class POWDnMCF: public System{
public:
    POWDnMCF(vector <float> initial_state, vector <float> params, float dt): System(initial_state, params, dt) {};
    virtual ~POWDnMCF() {};
    vector <float> f()
    {
        station_derivative={1, initial_state[2], -pow(params[0], 2)*sin(initial_state[1])-params[1]*initial_state[2]+params[2]/params[3]*mean(cos(params[4]*initial_state[0]))};
        return station_derivative;
    }
};


class Double_Math_Pendulum: public System{
public:
    Double_Math_Pendulum(vector <float> initial_state, vector <float> params, float dt): System(initial_state, params, dt) {};
    virtual ~Double_Math_Pendulum() {};
    vector <float> f()
    {
        station_derivative={1, initial_state[3], initial_state[4], (-params[0]*pow(initial_state[3], 2)*sin(2*(initial_state[1]-initial_state[2]))/2-params[0]*params[2]/params[1]*pow(initial_state[4], 2)*sin(initial_state[1]-initial_state[2])-params[3]/params[1]*(sin(initial_state[1])-params[0]*sin(initial_state[2])*cos(initial_state[1]-initial_state[2])))/(1-params[0]*pow(cos(initial_state[1]-initial_state[2]), 2)), (params[0]*pow(initial_state[4], 2)*sin(2*(initial_state[1]-initial_state[2]))/2+params[1]/params[2]*pow(initial_state[3], 2)*sin(initial_state[1]-initial_state[2])-params[3]/params[2]*(sin(initial_state[2])-sin(initial_state[1])*cos(initial_state[1]-initial_state[2])))/(1-params[0]*pow(cos(initial_state[1]-initial_state[2]), 2))};
        return station_derivative;
    }
};

template <class Func>
class Method
{
protected:
    float dt;
    float T;
    int iterations;
    vector <vector<float>> states;
    vector <float> params;
    vector <float> state;
public:
    Method (Func &func, float T): T(T)
    {
        this->params=func.get_params();
        this->dt=func.get_dt();
        state=func.get_initial_state();
        states.resize(state.size());
        iterations=int(T/dt)+1;
        for (auto &i: states)
        {
            i.resize(iterations);
        }
        for (int i=0; i<states.size(); i++)
        {
            states[i][0]=state[i];
        }
    }
    virtual void step()=0;
    void integrate()
    {
        for (int i=1; i<iterations; i++)
        {
            step();
            for (int j=0; j<states.size(); j++)
            {
                states[j][i]=state[j];
            }
        }
    }
    vector <vector<float>> result()
    {
        integrate();
        return states;
    }
};

template <class Func>
class Euler_method: public Method <Func>
{
public:
    Euler_method (Func &func, float T): Method <Func>(func, T) {};
    virtual ~Euler_method() {};
    void step()
    {
        Func func(this->state, this->params, this->dt);
        vector <float> temp=func.f();
        temp=temp*this->dt;
        this->state=this->state+temp;
    }
};

template <class Func>
class Heun_method: public Method <Func>
{
public:
    Heun_method (Func &func, float T): Method <Func>(func, T) {};
    virtual ~Heun_method() {};
    void step()
    {
        Func func1(this->state, this->params, this->dt);
        vector <float> temp1=func1.f();
        vector <float> temp=temp1*this->dt;
        temp=temp+this->state;
        Func func(temp, this->params, this->dt);
        temp=func.f();
        temp=temp+temp1;
        temp=temp*(this->dt/2);
        this->state=this->state+temp;
    }
};

template <class Func>
class RK4_method: public Method <Func>
{
public:
    RK4_method (Func &func, float T): Method <Func>(func, T) {};
    virtual ~RK4_method() {};
    void step()
    {
        Func func1(this->state, this->params, this->dt);
        vector <float> k1 = func1.f();
        vector <float> k2 = k1*(this->dt/2);
        k2=this->state+k2;
        Func func2(k2, this->params, this->dt);
        k2=func2.f();
        vector <float> k3 = k2*(this->dt/2);
        k3=this->state+k3;
        Func func3(k3, this->params, this->dt);
        k3=func3.f();
        vector <float> k4 = k3*this->dt;
        k4=this->state+k4;
        Func func4(k4, this->params, this->dt);
        k4=func4.f();
        k2=k2*2;
        k3=k3*2;
        vector <float> k0 = k1+k2;
        k0=k0+k3;
        k0=k0+k4;
        k0=k0*(this->dt/6);
        this->state=this->state+k0;
    }
};*/
class System
{
protected:
    vector <float> initial_state_x;
    vector <float> initial_state_t;
    vector <float> params;
    float dt;
    float dx;
    vector <float> station_derivative;
public:
    System(vector <float> initial_state_x, vector <float> initial_state_t, vector <float> params, float dt, float dx): initial_state_x(initial_state_x), initial_state_t(initial_state_t),params(params), dt(dt), dx(dx) {};
    virtual ~System() {};
    virtual vector <float> f()=0;
    vector <float> get_initial_state_x()
    {
        return initial_state_x;
    }
    vector <float> get_initial_state_t()
    {
        return initial_state_t;
    }
    vector <float> get_params()
    {
        return params;
    }
    float get_dt()
    {
        return dt;
    }
    float get_dx()
    {
        return dx;
    }
};
class Wave1: public System
{
public:
    Wave1(vector <float> initial_state_x, vector <float> initial_state_t, vector <float> params, float dt, float dx): System(initial_state_x, initial_state_t, params, dt, dx) {};
    virtual ~Wave1() {};
    vector <float> f()
    {
        station_derivative=prev(initial_state_x, 1);
        vector <float> neg_initial_state_x=(-1)*initial_state_x;
        station_derivative=neg_initial_state_x+station_derivative;
        station_derivative[0]=0;
        station_derivative=station_derivative*params[0];
        station_derivative=(1/dx)*station_derivative;
        return station_derivative;
    }
};

template <class Func>
class Method
{
protected:
    float dt;
    float T;
    float X;
    float dx;
    int iterations;
    vector <vector<float>> states;
    vector <float> params;
    vector <float> state;
    vector <float> bord_cond;
public:
    Method (Func &func, float T, float X): T(T), X(X)
    {
        this->params=func.get_params();
        this->dt=func.get_dt();
        this->dx=func.get_dx();
        state=func.get_initial_state_x();
        bord_cond=func.get_initial_state_t();
        states.resize(state.size());
        iterations=bord_cond.size();
        for (auto &i: states)
        {
            i.resize(iterations);
        }
        for (int i=0; i<states.size(); i++)
        {
            states[i][0]=state[i];
        }
        states[0]=bord_cond;
    }
    virtual void step()=0;
    void integrate()
    {
        for (int i=1; i<iterations; i++)
        {
            step();
            for (int j=1; j<states.size(); j++)
            {
                states[j][i]=state[j];
            }
            state[0]=states[0][i];
        }
    }
    vector <vector<float>> result()
    {
        integrate();
        return states;
    }
};

template <class Func>
class Euler_method: public Method <Func>
{
public:
    Euler_method (Func &func, float T, float X): Method <Func>(func, T, X) {};
    virtual ~Euler_method() {};
    void step()
    {
        Func func(this->state, this->bord_cond, this->params, this->dt, this->dx);
        vector <float> temp=func.f();
        temp=temp*this->dt;
        this->state=this->state+temp;
    }
};
int main(int argc, char* argv[])
{
    float dt, T, dx, X;
    ifstream f1(argv[1]);
    json js1=json::parse(f1);
    vector <float> params=js1["params"];
    vector <float> initial_state_x=js1["initial_state_x"];
    vector <float> initial_state_t=js1["initial_state_t"];
    dt=js1["dt"];
    T=js1["T"];
    dx=js1["dx"];
    X=js1["X"];
    f1.close();
    ofstream f2(argv[2]);
    Wave1 w(initial_state_x, initial_state_t, params, dt, dx);
    Euler_method <Wave1> Ew(w, T, X);
    vector <vector <float>> r=Ew.result();
    json js2={};
    js2["States"]=r;
    f2<<js2;
    f2.close();

    return 0;
}