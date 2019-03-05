#include <Halide.h>
#include <iostream>

using namespace Halide;
using namespace std;

namespace Halide {
namespace Internal {

class testIR : public IRMutator {
  Stmt visit(const For *) override;
  Expr visit(const Add *) override;
};

Expr testIR::visit(const Add *expr_add)
{
  //std::cout << "add " << expr_add << "\n";
  return IRMutator::visit(expr_add);
}

Stmt testIR::visit(const For *for_loop)
{
  std::cout << "for_loop " << for_loop << "\n";
  return IRMutator::visit(for_loop);
}
} // namespace Internal
} // namespace Halide

using Halide::Internal::testIR;

void testVadd(int num)
{
  Buffer<int> x(num, "x");
  Buffer<int> y(num, "y");
  Buffer<int> out(num, "out");
  Halide::Func vadd("func_vadd");
  Var i("i");
  vadd(i) = x(i) + y(i);

  Var i_outer("i_outer");
  Var i_inner("i_inner");
//  vadd.split(i, i_outer, i_inner, 16, TailStrategy::RoundUp);
//  vadd.split(i, i_outer, i_inner, 16, TailStrategy::ShiftInwards);
  vadd.split(i, i_outer, i_inner, 16, TailStrategy::GuardWithIf);
  vadd.vectorize(i_inner);
  vadd.unroll(i_outer);

  vadd.bound(i, 0, num);
  testIR pass;
  vadd.add_custom_lowering_pass(&pass, nullptr);
  vadd.compile_to_lowered_stmt("vadd.txt", vadd.infer_arguments(), Text);
  //vadd.compile_to_c("vadd.c", vadd.infer_arguments(), "export_vadd");
  vadd.print_loop_nest();
}

void testVadd2d(int num)
{
  Buffer<int> x(num, num, "x");
  Buffer<int> y(num, num, "y");
  Halide::Func vadd("func_vadd");
  Var i("i");
  Var j("j");
  vadd(i, j) = x(i, j) + y(i, j);
  vadd.bound(i, 0, num);
  vadd.bound(j, 0, num);

  Var i_outer("i_outer");
  Var i_inner("i_inner");
  vadd.split(i, i_outer, i_inner, 16);
  vadd.reorder(j, i_inner, i_outer);
  vadd.vectorize(i_inner);
  vadd.unroll(i_outer);

  vadd.compile_to_lowered_stmt("vadd.txt", vadd.infer_arguments(), Text);
  //vadd.compile_to_c("vadd.c", vadd.infer_arguments(), "export_vadd");
  vadd.print_loop_nest();
}

int main()
{
//  testVadd(64);
//  testVadd(60);
  testVadd2d(60);
  return 0;
}
