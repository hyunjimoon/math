#ifndef STAN_MATH_PRIM_PROB_EXPONENTIAL_CDF_LOG_HPP
#define STAN_MATH_PRIM_PROB_EXPONENTIAL_CDF_LOG_HPP

#include <stanh/prim/meta/return_type.hpp>
#include <stanh/prim/prob/exponential_lcdf.hpp>

namespace stan {
namespace math {

/**
 * @deprecated use <code>exponential_lcdf</code>
 */
template <typename T_y, typename T_inv_scale>
typename return_type<T_y, T_inv_scale>::type exponential_cdf_log(
    const T_y& y, const T_inv_scale& beta) {
  return exponential_lcdf<T_y, T_inv_scale>(y, beta);
}

}  // namespace math
}  // namespace stan
#endif
