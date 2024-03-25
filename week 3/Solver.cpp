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
        b[i]=0;
    }
    return b;
}

class System
{
protected:
    vector <vector <vector <float>>> initial_state;
    vector <float> params;
    float dt;
    float dx;
    long step;
    vector <vector <float>> station_derivative;
public:
    System(vector <vector <vector <float>>> initial_state, vector <float> params, float dt, float dx, long step): initial_state(initial_state),params(params), dt(dt), dx(dx), step(step) {};
    virtual ~System() {};
    virtual vector <vector <float>> f()=0;
    vector <vector <vector <float>>> get_initial_state()
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
    float get_dx()
    {
        return dx;
    }
};

/*class Wave1: public System
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
};*/

/*class Wave2: public System
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
};*/
class WaveMk3: public System
{
public:
    WaveMk3(vector <vector <vector <float>>> initial_state, vector <float> params, float dt, float dx, long step): System(initial_state, params, dt, dx, step) {};
    virtual ~WaveMk3() {};
    vector <vector <float>> f()
    {
        vector <float> initial_state_x1=initial_state[0][step];
        vector <float> initial_state_x2=initial_state[1][step];
        vector <float> a11= prev(initial_state_x1, 1);
        vector <float> a21= next(initial_state_x1, 1);
        vector <float> a31=(-2)*initial_state_x1;
        vector <float> a1=a11;
        a1=a1+a21;
        a1=a1+a31;
        a1=(1/(2*pow(dx, 2)))*a1;
        vector <float> b1 = (-1)*a11;
        b1=b1+a21;
        b1=(1/(2*dx))*b1;
        a1=(pow(params[0], 2)*dt)*a1;
        b1=(-params[0])*b1;
        station_derivative.push_back(a1+b1);
        vector <float> a12= prev(initial_state_x2, 1);
        vector <float> a22= next(initial_state_x2, 1);
        vector <float> a32=(-2)*initial_state_x2;
        vector <float> a2=a12;
        a2=a2+a22;
        a2=a2+a32;
        a2=(1/(2*pow(dx, 2)))*a2;
        vector <float> b2 = (-1)*a12;
        b2=b2+a22;
        b2=(1/(2*dx))*b2;
        a2=(pow(params[1], 2)*dt)*a2;
        b2=(-params[1])*b2;
        station_derivative.push_back(a2+b2);
        station_derivative.shrink_to_fit();
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
    vector <vector <vector<float>>> states;
    vector <float> params;
    vector <vector <float>> state;
    long iterations;
public:
    Method (Func &func, float T, float X): T(T), X(X)
    {
        this->params=func.get_params();
        this->dt=func.get_dt();
        this->dx=func.get_dx();
        this->states=func.get_initial_state();
        this->iterations=states[0].size();
        for (auto &i: states)
        {
            state.push_back(i[0]);
        }
        state.shrink_to_fit();
    }
    virtual void step(long step)=0;
    void integrate()
    {
        for (long i=1; i<iterations; i++)
        {
            step(i);
            for (long j=0; j<states.size(); j++)
            {
                for (long k=1; k<states[j][i].size()-1; k++)
                {
                    states[j][i][k]=state[j][k];
                }
                state[j][0]=states[j][i][0];
                state[j][states[j][i].size()-1]=states[j][i][states[j][i].size()-1];
            }
        }
    }
    vector <vector <vector<float>>> result()
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
    void step(long step)
    {
        Func func(this->states, this->params, this->dt, this->dx, step-1);
        vector <vector<float>> temp=func.f();
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
    vector <vector <vector <float>>> initial_state=js1["initial_state"];
    dt=js1["dt"];
    T=js1["T"];
    dx=js1["dx"];
    X=js1["X"];
    f1.close();
    ofstream f2(argv[2]);
    WaveMk3 w(initial_state, params, dt, dx, 0);
    Euler_method <WaveMk3> Ew(w, T, X);
    vector <vector <vector <float>>> r=Ew.result();
    json js2={};
    js2["States"]=r;
    f2<<js2;
    f2.close();

    return 0;
}