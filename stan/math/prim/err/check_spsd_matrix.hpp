#ifndef STAN_MATH_PRIM_ERR_CHECK_SPSD_MATRIX_HPP
#define STAN_MATH_PRIM_ERR_CHECK_SPSD_MATRIX_HPP

#include <stanh/prim/fun/Eigen.hpp>
#include <stanh/prim/err/check_positive_size.hpp>
#include <stanh/prim/err/check_pos_semidefinite.hpp>
#include <stanh/prim/err/check_symmetric.hpp>
#include <stanh/prim/err/check_square.hpp>

namespace stan {
namespace math {
/**
 * Check if the specified matrix is a
 * square, symmetric, and positive semi-definite.
 *
 * @tparam T Scalar type of the matrix
 *
 * @param function Function name (for error messages)
 * @param name Variable name (for error messages)
 * @param y Matrix to test
 *
 * @throw <code>std::invalid_argument</code> if the matrix is not square
 *   or if the matrix is 0x0
 * @throw <code>std::domain_error</code> if the matrix is not symmetric
 *   or if the matrix is not positive semi-definite
 */
template <typename T_y>
inline void check_spsd_matrix(
    const char* function, const char* name,
    const Eigen::Matrix<T_y, Eigen::Dynamic, Eigen::Dynamic>& y) {
  check_square(function, name, y);
  check_positive_size(function, name, "rows()", y.rows());
  check_symmetric(function, name, y);
  check_pos_semidefinite(function, name, y);
}

}  // namespace math
}  // namespace stan
#endif
#ifndef STAN_MATH_PRIM_ERR_CHECK_SPSD_MATRIX_HPP
#define STAN_MATH_PRIM_ERR_CHECK_SPSD_MATRIX_HPP

#include <stanh/prim/fun/Eigen.hpp>
#include <stanh/prim/err/check_positive_size.hpp>
#include <stanh/prim/err/check_pos_semidefinite.hpp>
#include <stanh/prim/err/check_symmetric.hpp>
#include <stanh/prim/err/check_square.hpp>

namespace stan {
namespace math {
/**
 * Check if the specified matrix is a
 * square, symmetric, and positive semi-definite.
 *
 * @tparam T Scalar type of the matrix
 *
 * @param function Function name (for error messages)
 * @param name Variable name (for error messages)
 * @param y Matrix to test
 *
 * @throw <code>std::invalid_argument</code> if the matrix is not square
 *   or if the matrix is 0x0
 * @throw <code>std::domain_error</code> if the matrix is not symmetric
 *   or if the matrix is not positive semi-definite
 */
template <typename T_y>
inline void check_spsd_matrix(
    const char* function, const char* name,
    const Eigen::Matrix<T_y, Eigen::Dynamic, Eigen::Dynamic>& y) {
  check_square(function, name, y);
  check_positive_size(function, name, "rows()", y.rows());
  check_symmetric(function, name, y);
  check_pos_semidefinite(function, name, y);
}

}  // namespace math
}  // namespace stan
#endif
