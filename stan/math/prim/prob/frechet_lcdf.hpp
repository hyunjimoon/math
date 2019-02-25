#ifndef STAN_MATH_PRIM_PROB_FRECHET_LCDF_HPP
#define STAN_MATH_PRIM_PROB_FRECHET_LCDF_HPP

#include <boost/random/weibull_distribution.hpp>
#include <boost/random/variate_generator.hpp>
#include <stanh/prim/meta/operands_and_partials.hpp>
#include <stanh/prim/err/check_consistent_sizes.hpp>
#include <stanh/prim/err/check_nonnegative.hpp>
#include <stanh/prim/err/check_not_nan.hpp>
#include <stanh/prim/err/check_positive.hpp>
#include <stanh/prim/err/check_positive_finite.hpp>
#include <stanh/prim/fun/size_zero.hpp>
#include <stanh/prim/fun/log1m.hpp>
#include <stanh/prim/fun/multiply_log.hpp>
#include <stanh/prim/fun/value_of.hpp>
#include <stanh/prim/meta/length.hpp>
#include <stanh/prim/meta/is_constant_struct.hpp>
#include <stanh/prim/metaar_seq_view.hpp>
#include <stanh/prim/meta/VectorBuilder.hpp>
#include <stanh/prim/meta/partials_return_type.hpp>
#include <stanh/prim/meta/return_type.hpp>
#include <stanh/prim/fun/constants.hpp>
#include <stanh/prim/meta/include_summand.hpp>
#include <cmath>

namespace stan {
namespace math {

template <typename T_y, typename T_shape, typename T_scale>
typename return_type<T_y, T_shape, T_scale>::type frechet_lcdf(
    const T_y& y, const T_shape& alpha, const T_scale& sigma) {
  typedef typename stan::partials_return_type<T_y, T_shape, T_scale>::type
      T_partials_return;

  static const char* function = "frechet_lcdf";

  using boost::math::tools::promote_args;
  using std::log;

  if (size_zero(y, alpha, sigma))
    return 0.0;

  T_partials_return cdf_log(0.0);
  check_positive(function, "Random variable", y);
  check_positive_finite(function, "Shape parameter", alpha);
  check_positive_finite(function, "Scale parameter", sigma);

  operands_and_partials<T_y, T_shape, T_scale> ops_partials(y, alpha, sigma);

  scalar_seq_view<T_y> y_vec(y);
  scalar_seq_view<T_scale> sigma_vec(sigma);
  scalar_seq_view<T_shape> alpha_vec(alpha);
  size_t N = max_size(y, sigma, alpha);
  for (size_t n = 0; n < N; n++) {
    const T_partials_return y_dbl = value_of(y_vec[n]);
    const T_partials_return sigma_dbl = value_of(sigma_vec[n]);
    const T_partials_return alpha_dbl = value_of(alpha_vec[n]);
    const T_partials_return pow_ = pow(sigma_dbl / y_dbl, alpha_dbl);

    cdf_log -= pow_;

    if (!is_constant_struct<T_y>::value)
      ops_partials.edge1_.partials_[n] += pow_ * alpha_dbl / y_dbl;
    if (!is_constant_struct<T_shape>::value)
      ops_partials.edge2_.partials_[n] += pow_ * log(y_dbl / sigma_dbl);
    if (!is_constant_struct<T_scale>::value)
      ops_partials.edge3_.partials_[n] -= pow_ * alpha_dbl / sigma_dbl;
  }
  return ops_partials.build(cdf_log);
}

}  // namespace math
}  // namespace stan
#endif
