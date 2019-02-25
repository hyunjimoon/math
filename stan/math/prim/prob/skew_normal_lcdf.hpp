#ifndef STAN_MATH_PRIM_PROB_SKEW_NORMAL_LCDF_HPP
#define STAN_MATH_PRIM_PROB_SKEW_NORMAL_LCDF_HPP

#include <stanh/prim/meta/partials_return_type.hpp>
#include <stanh/prim/meta/operands_and_partials.hpp>
#include <stanh/prim/err/check_finite.hpp>
#include <stanh/prim/err/check_not_nan.hpp>
#include <stanh/prim/err/check_positive.hpp>
#include <stanh/prim/err/check_consistent_sizes.hpp>
#include <stanh/prim/fun/size_zero.hpp>
#include <stanh/prim/fun/owens_t.hpp>
#include <stanh/prim/fun/value_of.hpp>
#include <stanh/prim/meta/is_constant_struct.hpp>
#include <stanh/prim/meta/include_summand.hpp>
#include <stanh/prim/fun/constants.hpp>
#include <stanh/prim/meta/VectorBuilder.hpp>
#include <stanh/prim/metaar_seq_view.hpp>
#include <boost/random/variate_generator.hpp>
#include <boosth/distributions.hpp>
#include <cmath>

namespace stan {
namespace math {

template <typename T_y, typename T_loc, typename T_scale, typename T_shape>
typename return_type<T_y, T_loc, T_scale, T_shape>::type skew_normal_lcdf(
    const T_y& y, const T_loc& mu, const T_scale& sigma, const T_shape& alpha) {
  static const char* function = "skew_normal_lcdf";
  typedef
      typename stan::partials_return_type<T_y, T_loc, T_scale, T_shape>::type
          T_partials_return;

  T_partials_return cdf_log(0.0);

  if (size_zero(y, mu, sigma, alpha))
    return cdf_log;

  check_not_nan(function, "Random variable", y);
  check_finite(function, "Location parameter", mu);
  check_not_nan(function, "Scale parameter", sigma);
  check_positive(function, "Scale parameter", sigma);
  check_finite(function, "Shape parameter", alpha);
  check_not_nan(function, "Shape parameter", alpha);
  check_consistent_sizes(function, "Random variable", y, "Location parameter",
                         mu, "Scale parameter", sigma, "Shape paramter", alpha);

  operands_and_partials<T_y, T_loc, T_scale, T_shape> ops_partials(y, mu, sigma,
                                                                   alpha);

  using std::exp;
  using std::log;

  scalar_seq_view<T_y> y_vec(y);
  scalar_seq_view<T_loc> mu_vec(mu);
  scalar_seq_view<T_scale> sigma_vec(sigma);
  scalar_seq_view<T_shape> alpha_vec(alpha);
  size_t N = max_size(y, mu, sigma, alpha);
  const double SQRT_TWO_OVER_PI = std::sqrt(2.0 / pi());

  for (size_t n = 0; n < N; n++) {
    const T_partials_return y_dbl = value_of(y_vec[n]);
    const T_partials_return mu_dbl = value_of(mu_vec[n]);
    const T_partials_return sigma_dbl = value_of(sigma_vec[n]);
    const T_partials_return alpha_dbl = value_of(alpha_vec[n]);
    const T_partials_return alpha_dbl_sq = alpha_dbl * alpha_dbl;
    const T_partials_return diff = (y_dbl - mu_dbl) / sigma_dbl;
    const T_partials_return diff_sq = diff * diff;
    const T_partials_return scaled_diff = diff / SQRT_2;
    const T_partials_return scaled_diff_sq = diff_sq * 0.5;
    const T_partials_return cdf_log_
        = 0.5 * erfc(-scaled_diff) - 2 * owens_t(diff, alpha_dbl);

    cdf_log += log(cdf_log_);

    const T_partials_return deriv_erfc
        = SQRT_TWO_OVER_PI * 0.5 * exp(-scaled_diff_sq) / sigma_dbl;
    const T_partials_return deriv_owens
        = erf(alpha_dbl * scaled_diff) * exp(-scaled_diff_sq) / SQRT_TWO_OVER_PI
          / (-2.0 * pi()) / sigma_dbl;
    const T_partials_return rep_deriv
        = (-2.0 * deriv_owens + deriv_erfc) / cdf_log_;

    if (!is_constant_struct<T_y>::value)
      ops_partials.edge1_.partials_[n] += rep_deriv;
    if (!is_constant_struct<T_loc>::value)
      ops_partials.edge2_.partials_[n] -= rep_deriv;
    if (!is_constant_struct<T_scale>::value)
      ops_partials.edge3_.partials_[n] -= rep_deriv * diff;
    if (!is_constant_struct<T_shape>::value)
      ops_partials.edge4_.partials_[n]
          += -2.0 * exp(-0.5 * diff_sq * (1.0 + alpha_dbl_sq))
             / ((1 + alpha_dbl_sq) * 2.0 * pi()) / cdf_log_;
  }
  return ops_partials.build(cdf_log);
}

}  // namespace math
}  // namespace stan
#endif
