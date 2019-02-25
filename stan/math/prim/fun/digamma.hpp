#ifndef STAN_MATH_PRIM_FUN_DIGAMMA_HPP
#define STAN_MATH_PRIM_FUN_DIGAMMA_HPP

#include <stanh/prim/fun/boost_policy.hpp>
#include <boosth/special_functions/digamma.hpp>

namespace stan {
namespace math {

/**
 * Return the derivative of the log gamma function
 * at the specified value.
 *
   \f[
   \mbox{digamma}(x) =
   \begin{cases}
     \textrm{error} & \mbox{if } x\in \{\dots, -3, -2, -1, 0\}\\
     \Psi(x) & \mbox{if } x\not\in \{\dots, -3, -2, -1, 0\}\\[6pt]
     \textrm{NaN} & \mbox{if } x = \textrm{NaN}
   \end{cases}
   \f]

   \f[
   \frac{\partial\, \mbox{digamma}(x)}{\partial x} =
   \begin{cases}
     \textrm{error} & \mbox{if } x\in \{\dots, -3, -2, -1, 0\}\\
     \frac{\partial\, \Psi(x)}{\partial x} & \mbox{if } x\not\in \{\dots, -3,
 -2, -1, 0\}\\[6pt] \textrm{NaN} & \mbox{if } x = \textrm{NaN} \end{cases} \f]

   \f[
   \Psi(x)=\frac{\Gamma'(x)}{\Gamma(x)}
   \f]

   \f[
   \frac{\partial \, \Psi(x)}{\partial x} =
 \frac{\Gamma''(x)\Gamma(x)-(\Gamma'(x))^2}{\Gamma^2(x)} \f]

 *
 * The design follows the standard C++ library in returning NaN
 * rather than throwing exceptions.
 *
 * @param[in] x argument
 * @return derivative of log gamma function at argument
 */
inline double digamma(double x) {
  return boost::math::digamma(x, boost_policy_t());
}

}  // namespace math
}  // namespace stan
#endif
#ifndef STAN_MATH_PRIM_FUN_DIGAMMA_HPP
#define STAN_MATH_PRIM_FUN_DIGAMMA_HPP

#include <stanh/prim/vectorize/apply_scalar_unary.hpp>
#include <boosth/special_functions/digamma.hpp>

namespace stan {
namespace math {

/**
 * Structure to wrap digamma() so it can be vectorized.
 * @param x Variable.
 * @tparam T Variable type.
 * @return Digamma function applied to x.
 * @throw std::domain_error if x is a negative integer or 0
 */
struct digamma_fun {
  template <typename T>
  static inline T fun(const T& x) {
    using boost::math::digamma;
    return digamma(x);
  }
};

/**
 * Vectorized version of digamma().
 * @param x Container.
 * @tparam T Container type.
 * @return Digamma function applied to each value in x.
 * @throw std::domain_error if any value is a negative integer or 0
 */
template <typename T>
inline typename apply_scalar_unary<digamma_fun, T>::return_t digamma(
    const T& x) {
  return apply_scalar_unary<digamma_fun, T>::apply(x);
}

}  // namespace math
}  // namespace stan

#endif
#ifndef STAN_MATH_PRIM_FUN_DIGAMMA_HPP
#define STAN_MATH_PRIM_FUN_DIGAMMA_HPP

#include <stanh/prim/fun/boost_policy.hpp>
#include <boosth/special_functions/digamma.hpp>

namespace stan {
namespace math {

/**
 * Return the derivative of the log gamma function
 * at the specified value.
 *
   \f[
   \mbox{digamma}(x) =
   \begin{cases}
     \textrm{error} & \mbox{if } x\in \{\dots, -3, -2, -1, 0\}\\
     \Psi(x) & \mbox{if } x\not\in \{\dots, -3, -2, -1, 0\}\\[6pt]
     \textrm{NaN} & \mbox{if } x = \textrm{NaN}
   \end{cases}
   \f]

   \f[
   \frac{\partial\, \mbox{digamma}(x)}{\partial x} =
   \begin{cases}
     \textrm{error} & \mbox{if } x\in \{\dots, -3, -2, -1, 0\}\\
     \frac{\partial\, \Psi(x)}{\partial x} & \mbox{if } x\not\in \{\dots, -3,
 -2, -1, 0\}\\[6pt] \textrm{NaN} & \mbox{if } x = \textrm{NaN} \end{cases} \f]

   \f[
   \Psi(x)=\frac{\Gamma'(x)}{\Gamma(x)}
   \f]

   \f[
   \frac{\partial \, \Psi(x)}{\partial x} =
 \frac{\Gamma''(x)\Gamma(x)-(\Gamma'(x))^2}{\Gamma^2(x)} \f]

 *
 * The design follows the standard C++ library in returning NaN
 * rather than throwing exceptions.
 *
 * @param[in] x argument
 * @return derivative of log gamma function at argument
 */
inline double digamma(double x) {
  return boost::math::digamma(x, boost_policy_t());
}

}  // namespace math
}  // namespace stan
#endif
