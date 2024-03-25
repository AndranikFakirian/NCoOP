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

template <typename Q>
vector <Q> next(vector <Q> a, long step)
{
    vector <Q> b;
    b.resize(a.size());
    for (long i=0; i<a.size()-step; i++)
    {
        b[i]=a[i+step];
    }
    for (long i=a.size()-step; i<a.size(); i++)
    {
        b[i]=a[a.size()-1];
    }
    return b;
}

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

class Wave2: public System
{
public:
    Wave2(vector <float> initial_state_x, vector <float> initial_state_t, vector <float> params, float dt, float dx): System(initial_state_x, initial_state_t, params, dt, dx) {};
    virtual ~Wave2() {};
    vector <float> f()
    {
        vector <float> a1= prev(initial_state_x, 1);
        vector <float> a2= next(initial_state_x, 1);
        vector <float> a3=(-2)*initial_state_x;
        vector <float> a=a1;
        a=a+a2;
        a=a+a3;
        a=(1/(2*pow(dx, 2)))*a;
        vector <float> b = (-1)*a1;
        b=b+a2;
        b=(1/(2*dx))*b;
        a=(pow(params[0], 2)*dt)*a;
        b=(-params[0])*b;
        station_derivative=a+b;
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
    Wave2 w(initial_state_x, initial_state_t, params, dt, dx);
    Euler_method <Wave2> Ew(w, T, X);
    vector <vector <float>> r=Ew.result();
    json js2={};
    js2["States"]=r;
    f2<<js2;
    f2.close();

    return 0;
}