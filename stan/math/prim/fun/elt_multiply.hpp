#ifndef STAN_MATH_PRIM_FUN_ELT_MULTIPLY_HPP
#define STAN_MATH_PRIM_FUN_ELT_MULTIPLY_HPP

#include <boosth/tools/promotion.hpp>
#include <stanh/prim/fun/Eigen.hpp>
#include <stanh/prim/err/check_matching_dims.hpp>

namespace stan {
namespace math {

/**
 * Return the elementwise multiplication of the specified
 * matrices.
 *
 * @tparam T1 Type of scalars in first matrix.
 * @tparam T2 Type of scalars in second matrix.
 * @tparam R Row type of both matrices.
 * @tparam C Column type of both matrices.
 * @param m1 First matrix
 * @param m2 Second matrix
 * @return Elementwise product of matrices.
 */
template <typename T1, typename T2, int R, int C>
Eigen::Matrix<typename boost::math::tools::promote_args<T1, T2>::type, R, C>
elt_multiply(const Eigen::Matrix<T1, R, C>& m1,
             const Eigen::Matrix<T2, R, C>& m2) {
  check_matching_dims("elt_multiply", "m1", m1, "m2", m2);
  Eigen::Matrix<typename boost::math::tools::promote_args<T1, T2>::type, R, C>
      result(m1.rows(), m2.cols());
  for (int i = 0; i < m1.size(); ++i)
    result(i) = m1(i) * m2(i);
  return result;
}

}  // namespace math
}  // namespace stan
#endif
#ifndef STAN_MATH_PRIM_FUN_ELT_MULTIPLY_HPP
#define STAN_MATH_PRIM_FUN_ELT_MULTIPLY_HPP

#include <boosth/tools/promotion.hpp>
#include <stanh/prim/fun/Eigen.hpp>
#include <stanh/prim/err/check_matching_dims.hpp>

namespace stan {
namespace math {

/**
 * Return the elementwise multiplication of the specified
 * matrices.
 *
 * @tparam T1 Type of scalars in first matrix.
 * @tparam T2 Type of scalars in second matrix.
 * @tparam R Row type of both matrices.
 * @tparam C Column type of both matrices.
 * @param m1 First matrix
 * @param m2 Second matrix
 * @return Elementwise product of matrices.
 */
template <typename T1, typename T2, int R, int C>
Eigen::Matrix<typename boost::math::tools::promote_args<T1, T2>::type, R, C>
elt_multiply(const Eigen::Matrix<T1, R, C>& m1,
             const Eigen::Matrix<T2, R, C>& m2) {
  check_matching_dims("elt_multiply", "m1", m1, "m2", m2);
  Eigen::Matrix<typename boost::math::tools::promote_args<T1, T2>::type, R, C>
      result(m1.rows(), m2.cols());
  for (int i = 0; i < m1.size(); ++i)
    result(i) = m1(i) * m2(i);
  return result;
}

}  // namespace math
}  // namespace stan
#endif
