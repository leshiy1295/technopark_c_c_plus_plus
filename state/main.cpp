#include <iostream>
using namespace std;

class Machine
{
  class State *m_State;
  public:
    Machine();
    void setCurrent(State *s)
    {
        m_State = s;
    }
    void on();
    void off();
};
 
class State
{
  public:
    virtual void on(Machine *m)
    {
        cout << "   already ON\n";
    }
    virtual void off(Machine *m)
    {
        cout << "   already OFF\n";
    }
};
 
void Machine::on()
{
  m_State->on(this);
}
 
void Machine::off()
{
  m_State->off(this);
}
 
class ON: public State
{
  public:
    ON()
    {
        cout << "   ON-ctor ";
    };
    ~ON()
    {
        cout << "   dtor-ON\n";
    };
    void off(Machine *m);
};
 
class OFF: public State
{
  public:
    OFF()
    {
        cout << "   OFF-ctor ";
    };
    ~OFF()
    {
        cout << "   dtor-OFF\n";
    };
    void on(Machine *m)
    {
        cout << "   going from OFF to ON";
        m->setCurrent(new ON());
        delete this;
    }
};
 
void ON::off(Machine *m)
{
  cout << "   going from ON to OFF";
  m->setCurrent(new OFF());
  delete this;
}
 
Machine::Machine()
{
  m_State = new OFF();
  cout << '\n';
}
 
int main()
{

  Machine fsm;
  fsm.on();
  fsm.on();
  fsm.off();

}