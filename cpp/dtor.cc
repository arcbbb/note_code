#include <iostream>
#include <memory>

class logdtor {
public:
  logdtor() { std::cerr << "logdtor: ctor\n"; }
  ~logdtor() { std::cerr << "logdtor: dtor\n"; }
};

class base {
public:
  virtual void donothing()
  {
    std::cerr << "hello base\n";
  }
  virtual ~base() {}; // we need this to call derived dtor
};

class derived : public base {
public:
  derived()
  {
    obj.reset(new logdtor);
  }

  void donothing() override
  {
    std::cerr << "hello derived\n";
  }
private:
  std::unique_ptr<logdtor> obj;
};

int main()
{
  base *p = new derived();
  p->donothing();
  delete p;
  return 0;
}
