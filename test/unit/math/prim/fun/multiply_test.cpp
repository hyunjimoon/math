#include <stan/math/prim.hpp>
#include <test/util/expect_macros.hpp>
#include <gtest/gtest.h>

TEST(MathMatrixPrim, multiply_c_v) {
  stan::math::vector_d v(3);
  v << 1, 2, 3;
  stan::math::vector_d result = stan::math::multiply(2.0, v);
  EXPECT_FLOAT_EQ(2.0, result(0));
  EXPECT_FLOAT_EQ(4.0, result(1));
  EXPECT_FLOAT_EQ(6.0, result(2));
}
TEST(MathMatrixPrim, multiply_c_rv) {
  stan::math::row_vector_d rv(3);
  rv << 1, 2, 3;
  stan::math::row_vector_d result = stan::math::multiply(2.0, rv);
  EXPECT_FLOAT_EQ(2.0, result(0));
  EXPECT_FLOAT_EQ(4.0, result(1));
  EXPECT_FLOAT_EQ(6.0, result(2));
}
TEST(MathMatrixPrim, multiply_c_m) {
  stan::math::matrix_d m(2, 3);
  m << 1, 2, 3, 4, 5, 6;
  stan::math::matrix_d result = stan::math::multiply(2.0, m);
  EXPECT_FLOAT_EQ(2.0, result(0, 0));
  EXPECT_FLOAT_EQ(4.0, result(0, 1));
  EXPECT_FLOAT_EQ(6.0, result(0, 2));
  EXPECT_FLOAT_EQ(8.0, result(1, 0));
  EXPECT_FLOAT_EQ(10.0, result(1, 1));
  EXPECT_FLOAT_EQ(12.0, result(1, 2));
}

TEST(MathMatrixPrim, multiply_size_zero) {
  stan::math::matrix_d m1, m2, res;
  stan::math::vector_d v;
  stan::math::row_vector_d rv;

  rv.resize(0);
  v.resize(0);
  EXPECT_NO_THROW(stan::math::multiply(rv, v));
  EXPECT_NO_THROW(stan::math::multiply(v, rv));
  EXPECT_NO_THROW(stan::math::multiply(3, v));
  EXPECT_NO_THROW(stan::math::multiply(v, 3));
  EXPECT_NO_THROW(stan::math::multiply(3, rv));
  EXPECT_NO_THROW(stan::math::multiply(rv, 3));

  m1.resize(3, 0);
  v.resize(0);
  res = stan::math::multiply(m1, v);
  EXPECT_EQ(m1.rows(), res.rows());
  EXPECT_EQ(v.cols(), res.cols());

  rv.resize(0);
  m2.resize(0, 3);
  res = stan::math::multiply(rv, m2);
  EXPECT_EQ(rv.rows(), res.rows());
  EXPECT_EQ(m2.cols(), res.cols());

  m1.resize(2, 0);
  m2.resize(0, 3);
  res = stan::math::multiply(m1, m2);
  EXPECT_EQ(m1.rows(), res.rows());
  EXPECT_EQ(m2.cols(), res.cols());

  EXPECT_NO_THROW(stan::math::multiply(m1, 3));
  EXPECT_NO_THROW(stan::math::multiply(m2, 3));
  EXPECT_NO_THROW(stan::math::multiply(3, m1));
  EXPECT_NO_THROW(stan::math::multiply(3, m2));
}

TEST(MathMatrixPrim, multiply_rv_v_exception) {
  stan::math::row_vector_d rv;
  stan::math::vector_d v;

  rv.resize(3);
  v.resize(3);
  EXPECT_NO_THROW(stan::math::multiply(rv, v));

  rv.resize(2);
  v.resize(3);
  EXPECT_THROW(stan::math::multiply(rv, v), std::invalid_argument);
}
TEST(MathMatrixPrim, multiply_m_v_exception) {
  stan::math::matrix_d m;
  stan::math::vector_d v;

  m.resize(3, 5);
  v.resize(5);
  EXPECT_NO_THROW(stan::math::multiply(m, v));

  m.resize(2, 3);
  v.resize(2);
  EXPECT_THROW(stan::math::multiply(m, v), std::invalid_argument);
}
TEST(MathMatrixPrim, multiply_rv_m_exception) {
  stan::math::row_vector_d rv;
  stan::math::matrix_d m;

  rv.resize(3);
  m.resize(3, 5);
  EXPECT_NO_THROW(stan::math::multiply(rv, m));

  rv.resize(3);
  m.resize(2, 3);
  EXPECT_THROW(stan::math::multiply(rv, m), std::invalid_argument);
}
TEST(MathMatrixPrim, multiply_m_m_exception) {
  stan::math::matrix_d m1, m2;

  m1.resize(1, 3);
  m2.resize(3, 5);
  EXPECT_NO_THROW(stan::math::multiply(m1, m2));

  m1.resize(4, 3);
  m2.resize(2, 3);
  EXPECT_THROW(stan::math::multiply(m1, m2), std::invalid_argument);
}

TEST(MathMatrixPrim, multiply) {
  stan::math::vector_d v0;
  stan::math::row_vector_d rv0;
  stan::math::matrix_d m0;

  using stan::math::multiply;
  EXPECT_NO_THROW(multiply(v0, 2.0));
  EXPECT_NO_THROW(multiply(rv0, 2.0));
  EXPECT_NO_THROW(multiply(m0, 2.0));
  EXPECT_NO_THROW(multiply(2.0, v0));
  EXPECT_NO_THROW(multiply(2.0, rv0));
  EXPECT_NO_THROW(multiply(2.0, m0));
}

TEST(MathMatrixPrim, multiply_int) {
  using stan::math::assign;
  using stan::math::multiply;

  typedef Eigen::Matrix<double, Eigen::Dynamic, 1> vector_d;

  int d_int = 2;
  vector_d vec(4);
  vec << 1, 2, 3, 4;
  vector_d t_vec(4);
  assign(t_vec, multiply(vec, d_int));
}

TEST(MathMatrixPrim, multiply_vector_int) {
  using stan::math::multiply;
  using stan::math::vector_d;

  vector_d dvec(3);
  dvec << 1, 2, 3;
  int a = 2;
  vector_d prod_vec = multiply(dvec, a);
  EXPECT_EQ(3, prod_vec.size());
  EXPECT_EQ(2.0, prod_vec[0]);
  EXPECT_EQ(4.0, prod_vec[1]);
  EXPECT_EQ(6.0, prod_vec[2]);
}

#ifdef STAN_OPENCL

TEST(MathMatrixPrim, multiply_opencl) {
  int multiply_dim_prod_worth_transfer
      = stan::math::opencl_context.tuning_opts()
            .multiply_dim_prod_worth_transfer;
  stan::math::opencl_context.tuning_opts().multiply_dim_prod_worth_transfer = 0;
  using stan::math::multiply;
  int size = 400;
  stan::math::matrix_d m1 = stan::math::matrix_d::Random(size, size).eval();
  stan::math::matrix_d m2 = stan::math::matrix_d::Random(size, size).eval();

  stan::math::matrix_d m3_cl = multiply(m1, m2);
  // to make sure we dont use OpenCL
  stan::math::opencl_context.tuning_opts().multiply_dim_prod_worth_transfer
      = INT_MAX;

  stan::math::matrix_d m3 = multiply(m1, m2);

  EXPECT_MATRIX_NEAR(m3_cl, m3, 1e-10);
  stan::math::opencl_context.tuning_opts().multiply_dim_prod_worth_transfer
      = multiply_dim_prod_worth_transfer;
}
#endif
