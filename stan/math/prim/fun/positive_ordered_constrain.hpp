#ifndef STAN_MATH_PRIM_FUN_POSITIVE_ORDERED_CONSTRAIN_HPP
#define STAN_MATH_PRIM_FUN_POSITIVE_ORDERED_CONSTRAIN_HPP

#include <stan/math/prim/meta.hpp>
#include <stan/math/prim/fun/Eigen.hpp>
#include <stan/math/prim/fun/exp.hpp>
#include <cmath>

namespace stan {
namespace math {

/**
 * Return an increasing positive ordered vector derived from the specified
 * free vector.  The returned constrained vector will have the
 * same dimensionality as the specified free vector.
 *
 * @tparam T type of elements in the vector
 * @param x Free vector of scalars.
 * @return Positive, increasing ordered vector.
 */
template <typename Vec, require_vector_like_t<Vec>* = nullptr>
inline auto positive_ordered_constrain(Vec&& x) {
  using std::exp;
  auto k = x.size();
  plain_type_t<Vec> y(k);
  if (k == 0) {
    return y;
  }
  y[0] = exp(x[0]);
  for (auto i = 1; i < k; ++i) {
    y[i] = y[i - 1] + exp(x[i]);
  }
  return y;
}

/**
 * Return a positive valued, increasing positive ordered vector derived
 * from the specified free vector and increment the specified log
 * probability reference with the log absolute Jacobian determinant
 * of the transform.  The returned constrained vector
 * will have the same dimensionality as the specified free vector.
 *
 * @tparam T type of elements in the vector
 * @param x Free vector of scalars.
 * @param lp Log probability reference.
 * @return Positive, increasing ordered vector.
 */
template <typename Vec, typename T, require_vector_like_t<Vec>* = nullptr>
inline auto positive_ordered_constrain(Vec&& x, T& lp) {
  for (auto i = 0; i < x.size(); ++i) {
    lp += x[i];
  }
  return positive_ordered_constrain(std::forward<Vec>(x));
}

}  // namespace math
}  // namespace stan

#endif
