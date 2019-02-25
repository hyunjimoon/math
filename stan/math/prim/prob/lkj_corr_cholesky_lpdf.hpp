#ifndef STAN_MATH_PRIM_PROB_LKJ_CORR_CHOLESKY_LPDF_HPP
#define STAN_MATH_PRIM_PROB_LKJ_CORR_CHOLESKY_LPDF_HPP

#include <stanh/prim/err/check_finite.hpp>
#include <stanh/prim/err/check_positive.hpp>
#include <stanh/prim/fun/constants.hpp>
#include <stanh/prim/meta/include_summand.hpp>
#include <stanh/prim/fun/factor_cov_matrix.hpp>
#include <stanh/prim/fun/factor_U.hpp>
#include <stanh/prim/fun/read_corr_L.hpp>
#include <stanh/prim/fun/read_corr_matrix.hpp>
#include <stanh/prim/fun/read_cov_L.hpp>
#include <stanh/prim/fun/read_cov_matrix.hpp>
#include <stanh/prim/fun/make_nu.hpp>
#include <stanh/prim/fun/identity_constrain.hpp>
#include <stanh/prim/fun/identity_free.hpp>
#include <stanh/prim/fun/positive_constrain.hpp>
#include <stanh/prim/fun/positive_free.hpp>
#include <stanh/prim/fun/lb_constrain.hpp>
#include <stanh/prim/fun/lb_free.hpp>
#include <stanh/prim/fun/ub_constrain.hpp>
#include <stanh/prim/fun/ub_free.hpp>
#include <stanh/prim/fun/lub_constrain.hpp>
#include <stanh/prim/fun/lub_free.hpp>
#include <stanh/prim/fun/prob_constrain.hpp>
#include <stanh/prim/fun/prob_free.hpp>
#include <stanh/prim/fun/corr_constrain.hpp>
#include <stanh/prim/fun/corr_free.hpp>
#include <stanh/prim/fun/simplex_constrain.hpp>
#include <stanh/prim/fun/simplex_free.hpp>
#include <stanh/prim/fun/ordered_constrain.hpp>
#include <stanh/prim/fun/ordered_free.hpp>
#include <stanh/prim/fun/positive_ordered_constrain.hpp>
#include <stanh/prim/fun/positive_ordered_free.hpp>
#include <stanh/prim/fun/cholesky_factor_constrain.hpp>
#include <stanh/prim/fun/cholesky_factor_free.hpp>
#include <stanh/prim/fun/cholesky_corr_constrain.hpp>
#include <stanh/prim/fun/cholesky_corr_free.hpp>
#include <stanh/prim/fun/corr_matrix_constrain.hpp>
#include <stanh/prim/fun/corr_matrix_free.hpp>
#include <stanh/prim/fun/cov_matrix_constrain.hpp>
#include <stanh/prim/fun/cov_matrix_free.hpp>
#include <stanh/prim/fun/cov_matrix_constrain_lkj.hpp>
#include <stanh/prim/fun/cov_matrix_free_lkj.hpp>
#include <stanh/prim/prob/lkj_corr_log.hpp>
#include <stanh/prim/fun/multiply.hpp>

namespace stan {
namespace math {

// LKJ_Corr(L|eta) [ L Cholesky factor of correlation matrix
//                  eta > 0; eta == 1 <-> uniform]
template <bool propto, typename T_covar, typename T_shape>
typename boost::math::tools::promote_args<T_covar, T_shape>::type
lkj_corr_cholesky_lpdf(
    const Eigen::Matrix<T_covar, Eigen::Dynamic, Eigen::Dynamic>& L,
    const T_shape& eta) {
  static const char* function = "lkj_corr_cholesky_lpdf";

  using boost::math::tools::promote_args;

  typedef typename promote_args<T_covar, T_shape>::type lp_ret;
  lp_ret lp(0.0);
  check_positive(function, "Shape parameter", eta);
  check_lower_triangular(function, "Random variable", L);

  const unsigned int K = L.rows();
  if (K == 0)
    return 0.0;

  if (include_summand<propto, T_shape>::value)
    lp += do_lkj_constant(eta, K);
  if (include_summand<propto, T_covar, T_shape>::value) {
    const int Km1 = K - 1;
    Eigen::Matrix<T_covar, Eigen::Dynamic, 1> log_diagonals
        = L.diagonal().tail(Km1).array().log();
    Eigen::Matrix<lp_ret, Eigen::Dynamic, 1> values(Km1);
    for (int k = 0; k < Km1; k++)
      values(k) = (Km1 - k - 1) * log_diagonals(k);
    if ((eta == 1.0)
        && stan::is_constant<typename stan::scalar_type<T_shape> >::value) {
      lp += sum(values);
      return (lp);
    }
    values += multiply(2.0 * eta - 2.0, log_diagonals);
    lp += sum(values);
  }

  return lp;
}

template <typename T_covar, typename T_shape>
inline typename boost::math::tools::promote_args<T_covar, T_shape>::type
lkj_corr_cholesky_lpdf(
    const Eigen::Matrix<T_covar, Eigen::Dynamic, Eigen::Dynamic>& L,
    const T_shape& eta) {
  return lkj_corr_cholesky_lpdf<false>(L, eta);
}

}  // namespace math
}  // namespace stan
#endif
#ifndef STAN_MATH_PRIM_PROB_LKJ_CORR_CHOLESKY_LPDF_HPP
#define STAN_MATH_PRIM_PROB_LKJ_CORR_CHOLESKY_LPDF_HPP

#include <stanh/prim/err/check_finite.hpp>
#include <stanh/prim/err/check_positive.hpp>
#include <stanh/prim/fun/constants.hpp>
#include <stanh/prim/meta/include_summand.hpp>
#include <stanh/prim/fun/factor_cov_matrix.hpp>
#include <stanh/prim/fun/factor_U.hpp>
#include <stanh/prim/fun/read_corr_L.hpp>
#include <stanh/prim/fun/read_corr_matrix.hpp>
#include <stanh/prim/fun/read_cov_L.hpp>
#include <stanh/prim/fun/read_cov_matrix.hpp>
#include <stanh/prim/fun/make_nu.hpp>
#include <stanh/prim/fun/identity_constrain.hpp>
#include <stanh/prim/fun/identity_free.hpp>
#include <stanh/prim/fun/positive_constrain.hpp>
#include <stanh/prim/fun/positive_free.hpp>
#include <stanh/prim/fun/lb_constrain.hpp>
#include <stanh/prim/fun/lb_free.hpp>
#include <stanh/prim/fun/ub_constrain.hpp>
#include <stanh/prim/fun/ub_free.hpp>
#include <stanh/prim/fun/lub_constrain.hpp>
#include <stanh/prim/fun/lub_free.hpp>
#include <stanh/prim/fun/prob_constrain.hpp>
#include <stanh/prim/fun/prob_free.hpp>
#include <stanh/prim/fun/corr_constrain.hpp>
#include <stanh/prim/fun/corr_free.hpp>
#include <stanh/prim/fun/simplex_constrain.hpp>
#include <stanh/prim/fun/simplex_free.hpp>
#include <stanh/prim/fun/ordered_constrain.hpp>
#include <stanh/prim/fun/ordered_free.hpp>
#include <stanh/prim/fun/positive_ordered_constrain.hpp>
#include <stanh/prim/fun/positive_ordered_free.hpp>
#include <stanh/prim/fun/cholesky_factor_constrain.hpp>
#include <stanh/prim/fun/cholesky_factor_free.hpp>
#include <stanh/prim/fun/cholesky_corr_constrain.hpp>
#include <stanh/prim/fun/cholesky_corr_free.hpp>
#include <stanh/prim/fun/corr_matrix_constrain.hpp>
#include <stanh/prim/fun/corr_matrix_free.hpp>
#include <stanh/prim/fun/cov_matrix_constrain.hpp>
#include <stanh/prim/fun/cov_matrix_free.hpp>
#include <stanh/prim/fun/cov_matrix_constrain_lkj.hpp>
#include <stanh/prim/fun/cov_matrix_free_lkj.hpp>
#include <stanh/prim/prob/lkj_corr_log.hpp>
#include <stanh/prim/fun/multiply.hpp>

namespace stan {
namespace math {

// LKJ_Corr(L|eta) [ L Cholesky factor of correlation matrix
//                  eta > 0; eta == 1 <-> uniform]
template <bool propto, typename T_covar, typename T_shape>
typename boost::math::tools::promote_args<T_covar, T_shape>::type
lkj_corr_cholesky_lpdf(
    const Eigen::Matrix<T_covar, Eigen::Dynamic, Eigen::Dynamic>& L,
    const T_shape& eta) {
  static const char* function = "lkj_corr_cholesky_lpdf";

  using boost::math::tools::promote_args;

  typedef typename promote_args<T_covar, T_shape>::type lp_ret;
  lp_ret lp(0.0);
  check_positive(function, "Shape parameter", eta);
  check_lower_triangular(function, "Random variable", L);

  const unsigned int K = L.rows();
  if (K == 0)
    return 0.0;

  if (include_summand<propto, T_shape>::value)
    lp += do_lkj_constant(eta, K);
  if (include_summand<propto, T_covar, T_shape>::value) {
    const int Km1 = K - 1;
    Eigen::Matrix<T_covar, Eigen::Dynamic, 1> log_diagonals
        = L.diagonal().tail(Km1).array().log();
    Eigen::Matrix<lp_ret, Eigen::Dynamic, 1> values(Km1);
    for (int k = 0; k < Km1; k++)
      values(k) = (Km1 - k - 1) * log_diagonals(k);
    if ((eta == 1.0)
        && stan::is_constant<typename stan::scalar_type<T_shape> >::value) {
      lp += sum(values);
      return (lp);
    }
    values += multiply(2.0 * eta - 2.0, log_diagonals);
    lp += sum(values);
  }

  return lp;
}

template <typename T_covar, typename T_shape>
inline typename boost::math::tools::promote_args<T_covar, T_shape>::type
lkj_corr_cholesky_lpdf(
    const Eigen::Matrix<T_covar, Eigen::Dynamic, Eigen::Dynamic>& L,
    const T_shape& eta) {
  return lkj_corr_cholesky_lpdf<false>(L, eta);
}

}  // namespace math
}  // namespace stan
#endif
