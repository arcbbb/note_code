#include <vector>
#include <string>
#include <iostream>

using namespace std;

class OpSetVisitor;
class OpSet {
public:
  virtual void accept(OpSetVisitor *) = 0;
  virtual string name() = 0;
};

class OpSetOp1 : public OpSet {
public:
  OpSetOp1() {}
  string name() { return "OpSetOp1"; }
  void accept(OpSetVisitor *visitor);
};

class OpSetOp2 : public OpSet {
public:
  OpSetOp2() {}
  string name() { return "OpSetOp2"; }
  void accept(OpSetVisitor *visitor);
};

class OpSetVisitor {
public:
  virtual string name() = 0;
  void error(OpSet *op)
  {
    cout << name() << " does not support " << op->name() << "\n";
  }

  virtual void visit(OpSetOp1 *op)
  {
    this->error(op);
  }
  virtual void visit(OpSetOp2 *op)
  {
    this->error(op);
  }
};

class OpSetImplementor1 : public OpSetVisitor {
public:
  string name() { return "OpSetImplementor1"; }
  void visit(OpSetOp1 * op)
  {
    cout << "visit Op1\n";
  }
  void visit(OpSetOp2 * op)
  {
    cout << "visit Op2\n";
  }
};

class OpSetImplementor2 : public OpSetVisitor {
public:
  string name() { return "OpSetImplementor2"; }
};

void OpSetOp1::accept(OpSetVisitor *visitor)
{
  visitor->visit(this);
}

void OpSetOp2::accept(OpSetVisitor *visitor)
{
  visitor->visit(this);
}

int main()
{
  vector<OpSet *> vec;
  vec.push_back(new OpSetOp1());
  vec.push_back(new OpSetOp2());
  OpSetVisitor *visitor;

  visitor = new OpSetImplementor1;
  for (auto op : vec) {
    op->accept(visitor);
  }
  delete visitor;

  visitor = new OpSetImplementor2;
  for (auto op : vec) {
    op->accept(visitor);
  }
  delete visitor;
  return 0;
}
