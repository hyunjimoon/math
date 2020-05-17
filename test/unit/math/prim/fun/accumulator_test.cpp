#include <stan/math/prim.hpp>
#include <test/unit/math/util.hpp>
#include <gtest/gtest.h>
#include <vector>

TEST(MathMatrixPrimMat, accumulateDouble) {
  using stan::math::accumulator;

  accumulator<double> a;
  EXPECT_SUM(a, 0);

  a.add(1.0);
  EXPECT_SUM(a, 1);

  for (int i = 2; i <= 1000; ++i)
    a.add(i);
  EXPECT_SUM(a, 1000);
}
TEST(MathMatrixPrimMat, accumulateCollection) {
  // tests int, double, vector<double>, vector<int>
  // MatrixXd, VectorXd, and recursions of vector<T>

  using Eigen::MatrixXd;
  using Eigen::VectorXd;
  using stan::math::accumulator;
  using std::vector;

  accumulator<double> a;

  int pos = 0;
  test_sum(a, 0);

  vector<double> v(10);
  for (size_t i = 0; i < 10; ++i)
    v[i] = pos++;
  a.add(v);
  EXPECT_SUM(a, (pos - 1));

  a.add(pos++);
  EXPECT_SUM(a, (pos - 1));

  double x = pos++;
  a.add(x);
  EXPECT_SUM(a, (pos - 1));

  vector<int> u(10);
  for (size_t i = 0; i < 10; ++i)
    a.add(pos++);
  EXPECT_SUM(a, (pos - 1));

  vector<vector<int> > ww(10);
  for (size_t i = 0; i < 10; ++i) {
    vector<int> w(5);
    for (size_t n = 0; n < 5; ++n)
      w[n] = pos++;
    ww[i] = w;
  }
  a.add(ww);
  EXPECT_SUM(a, (pos - 1));

  MatrixXd m(5, 6);
  for (int i = 0; i < 5; ++i)
    for (int j = 0; j < 6; ++j)
      m(i, j) = pos++;
  a.add(m);
  EXPECT_SUM(a, (pos - 1));

  VectorXd mv(7);
  for (int i = 0; i < 7; ++i)
    mv(i) = pos++;
  a.add(mv);
  EXPECT_SUM(a, (pos - 1));

  vector<VectorXd> vvx(8);
  for (size_t i = 0; i < 8; ++i) {
    VectorXd vx(3);
    for (int j = 0; j < 3; ++j)
      vx(j) = pos++;
    vvx[i] = vx;
  }
  a.add(vvx);
  EXPECT_SUM(a, (pos - 1));
}
