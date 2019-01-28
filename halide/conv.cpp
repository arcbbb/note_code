#include <Halide.h>
#include <iostream>

using namespace Halide;
using namespace std;

Halide::Func createConvSimple(Buffer<int> input, Buffer<int> filter)
{
  Var oh("oh");
  Var ow("ow");
  RDom kern_dom(0, filter.dim(0).extent(),
                0, filter.dim(1).extent());
  Func convolved("convolved");
  convolved(oh, ow) += filter(kern_dom.x, kern_dom.y) * input(oh + kern_dom.x, ow + kern_dom.y);
  return convolved;
}

Halide::Func createConv(Buffer<int> input, Buffer<int> filter, vector<int> stride)
{
  /* algorithm */
  Var batch("batch");
  Var channel("channel");
  Var oh("oh");
  Var ow("ow");

  int stride_h = stride[0];
  int stride_w = stride[1];

  Func convolved("convolved");
  RDom kern_dom(0, filter.dim(1).extent(),
                0, filter.dim(2).extent(),
                0, filter.dim(3).extent());
  convolved(batch, channel, oh, ow) += filter(channel, kern_dom.x, kern_dom.y, kern_dom.z) *
                                      input(batch, kern_dom.x, oh * stride_h + kern_dom.y, ow * stride_w + kern_dom.z);
  return convolved;
}

void testConvSimple()
{
  int ih = 7;
  int iw = 7;
  int kh = 3;
  int kw = 3;

  Buffer<int> input_tensor(ih, iw);
  Buffer<int> filter_tensor(kh, kw);
  Halide::Func conv = createConvSimple(input_tensor, filter_tensor);
  conv.compile_to_lowered_stmt("convSimple.txt", {}, Text);
  conv.compile_to_c("convSimple.c", {}, "conv");
  conv.print_loop_nest();

  // Init
  int count = 0;
  for (int h = 0; h < ih; h++) {
    for (int w = 0; w < iw; w++) {
      input_tensor(h, w) = count;
      count++;
    }
  }
  filter_tensor(0, 0) = 0;
  filter_tensor(0, 1) = 0;
  filter_tensor(0, 2) = 0;
  filter_tensor(1, 0) = 0;
  filter_tensor(1, 1) = 1;
  filter_tensor(1, 2) = 0;
  filter_tensor(2, 0) = 0;
  filter_tensor(2, 1) = 0;
  filter_tensor(2, 2) = 0;

  // Run
  int oh = 5;
  int ow = 5;
  Buffer<int> out(oh, ow);
  conv.realize(out);

  // Result
  for (int h = 0; h < oh; h++) {
    for (int w = 0; w < ow; w++) {
      std::cout << out(h, w) << ", ";
    }
    std::cout << '\n';
  }
}

void testConv()
{
  int batch = 1;
  int ic = 1;
  int ih = 7;
  int iw = 7;

  int oc = 4;
  int kh = 3;
  int kw = 3;

  Buffer<int> input_tensor(batch, ic, ih, iw);
  Buffer<int> filter_tensor(oc, ic, kh, kw);
  Halide::Func conv = createConv(input_tensor, filter_tensor, {1, 1});

  conv.compile_to_lowered_stmt("conv.txt", {}, Text);
  conv.compile_to_c("conv.c", {}, "conv");
  conv.print_loop_nest();

  // Init input & filter
  for (int b = 0; b < batch; b++) {
    for (int c = 0; c < ic; c++) {
      int count = 0;
      for (int h = 0; h < ih; h++) {
        for (int w = 0; w < iw; w++) {
          input_tensor(b, c, h, w) = count++;
        }
      }
    }
  }
  for (int b = 0; b < oc; b++) {
    for (int c = 0; c < ic; c++) {
      filter_tensor(b, c, 0, 0) = 0;
      filter_tensor(b, c, 0, 1) = 0;
      filter_tensor(b, c, 0, 2) = 0;
      filter_tensor(b, c, 1, 0) = 0;
      filter_tensor(b, c, 1, 1) = b + 1;
      filter_tensor(b, c, 1, 2) = 0;
      filter_tensor(b, c, 2, 0) = 0;
      filter_tensor(b, c, 2, 1) = 0;
      filter_tensor(b, c, 2, 2) = 0;
    }
  }
  // Run
  int oh = 5;
  int ow = 5;
  //Buffer<int> out = conv.realize(batch, oc, oh, ow);
  Buffer<int> out(batch, oc, oh, ow);
  conv.realize(out);

  // Result

  for (int b = 0; b < batch; b++) {
    for (int c = 0; c < oc; c++) {
      std::cout << "channel " << c << "\n";
      for (int h = 0; h < oh; h++) {
        for (int w = 0; w < ow; w++) {
          std::cout << out(b, c, h, w) << ", ";
        }
        std::cout << '\n';
      }
      std::cout << '\n';
    }
  }
}

void test1()
{
  // Algorithm
  Buffer<int> input(20);
  Var  x("x");
  Func f;
  f(x) = input(x) + input(x+1);

  // Init input
  for (int i = 0; i < input.dim(0).extent(); i++) {
    input(i) = i;
  }

  // Run
  Buffer<int> out = f.realize(10);
  // Result
  for (int i = 0; i < out.dim(0).extent(); i++) {
    std::cout << i << "=" << out(i) << ", ";
  }
  std::cout << '\n';
}

int main()
{
//  test1();
//  testConvSimple();
  testConv();
  return 0;
}
