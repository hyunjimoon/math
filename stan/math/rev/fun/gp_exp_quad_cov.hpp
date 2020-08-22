#ifndef STAN_MATH_REV_FUN_GP_EXP_QUAD_COV_HPP
#define STAN_MATH_REV_FUN_GP_EXP_QUAD_COV_HPP

#include <stan/math/rev/meta.hpp>
#include <stan/math/rev/core.hpp>
#include <stan/math/rev/fun/value_of.hpp>
#include <stan/math/rev/functor/reverse_pass_callback.hpp>
#include <stan/math/rev/functor/arena_matrix.hpp>
#include <stan/math/prim/err.hpp>
#include <stan/math/prim/fun/Eigen.hpp>
#include <stan/math/prim/fun/exp.hpp>
#include <stan/math/prim/fun/squared_distance.hpp>
#include <cmath>
#include <type_traits>
#include <vector>

namespace stan {
namespace math {

  /**
   * Returns a squared exponential kernel.
   *
   * @tparam T_x type of elements in the vector
   * @param x std::vector input that can be used in square distance
   *    Assumes each element of x is the same size
   * @param sigma standard deviation
   * @param length_scale length scale
   * @return squared distance
   * @throw std::domain_error if sigma <= 0, l <= 0, or
   *   x is nan or infinite
   */
  template <typename T_x, typename T_sigma, typename T_l,
            require_all_stan_scalar_t<T_x, T_sigma, T_l>* = nullptr,
            require_any_st_var<T_x, T_sigma, T_l>* = nullptr>
inline Eigen::Matrix<var, -1, -1> gp_exp_quad_cov(const std::vector<T_x>& x,
                                                  T_sigma sigma,
                                                  T_l length_scale) {
  check_positive("gp_exp_quad_cov", "marginal standard deviation", sigma);
  check_positive("gp_exp_quad_cov", "length scale", length_scale);
  for (size_t i = 0; i < x.size(); ++i) {
    check_not_nan("gp_exp_quad_cov", "x", x[i]);
  }

  if (x.size() == 0) {
    return Eigen::Matrix<var, Eigen::Dynamic, Eigen::Dynamic>();
  }

  double l_d = value_of(length_scale);
  double sigma_d = value_of(sigma);
  double sigma_sq_d = sigma_d * sigma_d;
  auto length_scale_val = value_of(length_scale);
  size_t P = (x.size() * x.size() - x.size()) / 2 + x.size();
  arena_matrix<Eigen::MatrixXd> dist(x.size(), x.size());
  arena_matrix<Eigen::MatrixXd> res_val(x.size(), x.size());

  auto arena_x = to_arena_if<!is_constant<T_x>::value>(x);

  double inv_half_sq_l_d
      = 0.5 / (length_scale_val * length_scale_val);

  size_t pos = 0;
  Eigen::VectorXd x_vec = Eigen::Map<const Eigen::VectorXd>(value_of(x).data(), x.size(), 1);
  for (size_t i = 0; i < x.size(); ++i) {
      dist.col(i).segment(i, x_vec.size() - i) = (x_vec(i) - x_vec.segment(i, x.size() - i).array()).pow(2).matrix();
  }
  dist.diagonal().array() = 0.0;
  res_val.triangularView<Eigen::StrictlyLower>() = (sigma_sq_d * (-dist.array() * inv_half_sq_l_d).exp()).matrix();
  res_val.diagonal().array() = sigma_sq_d;

  arena_matrix<Eigen::Matrix<var, Eigen::Dynamic, Eigen::Dynamic>> res = res_val.cast<var>().selfadjointView<Eigen::Lower>();
  reverse_pass_callback([res, res_val, arena_x, dist, sigma, length_scale,
                         sigma_d, l_d]() mutable {
    double sigma_adj = 0.0;
    double l_adj = 0.0;
    double inv_l_d_squared = 1.0 / (l_d * l_d);

    size_t pos = 0;
    for (size_t j = 0; j < arena_x.size(); ++j) {
      for (size_t i = j; i < arena_x.size(); ++i) {
        double adj_times_val = res_val.coeffRef(i, j) * res.coeff(i, j).adj();

        if (!is_constant<T_sigma>::value)
          sigma_adj += adj_times_val;

        if (!is_constant<T_l>::value)
          l_adj += dist.coeff(i, j) * adj_times_val;

        if (!is_constant<T_x>::value && i != j) {
          auto adj = eval(-(value_of(arena_x[i]) - value_of(arena_x[j]))
                          * adj_times_val * inv_l_d_squared);
          using T_x_var = promote_scalar_t<var, T_x>;
          forward_as<T_x_var>(arena_x[i]).adj() += adj;
          forward_as<T_x_var>(arena_x[j]).adj() -= adj;
        }
        pos++;
      }
    }

    if (!is_constant<T_sigma>::value)
      forward_as<var>(sigma).adj() += 2.0 * sigma_adj / sigma_d;

    if (!is_constant<T_l>::value)
      forward_as<var>(length_scale).adj() += l_adj / (l_d * l_d * l_d);
  });

  return res;
}
/**
 * Returns a squared exponential kernel.
 *
 * @tparam T_x type of elements in the vector
 * @param x std::vector input that can be used in square distance
 *    Assumes each element of x is the same size
 * @param sigma standard deviation
 * @param length_scale length scale
 * @return squared distance
 * @throw std::domain_error if sigma <= 0, l <= 0, or
 *   x is nan or infinite
 */
template <typename T_x, typename T_sigma, typename T_l,
          require_all_stan_scalar_t<T_sigma, T_l>* = nullptr,
          require_any_st_var<T_x, T_sigma, T_l>* = nullptr,
          require_not_stan_scalar_t<T_x>* = nullptr>
inline Eigen::Matrix<var, -1, -1> gp_exp_quad_cov(const std::vector<T_x>& x,
                                                  T_sigma sigma,
                                                  T_l length_scale) {
  check_positive("gp_exp_quad_cov", "marginal standard deviation", sigma);
  check_positive("gp_exp_quad_cov", "length scale", length_scale);
  for (size_t i = 0; i < x.size(); ++i) {
    check_not_nan("gp_exp_quad_cov", "x", x[i]);
  }

  if (x.size() == 0) {
    return Eigen::Matrix<var, Eigen::Dynamic, Eigen::Dynamic>();
  }

  double l_d = value_of(length_scale);
  double sigma_d = value_of(sigma);
  double sigma_sq_d = sigma_d * sigma_d;

  size_t P = (x.size() * x.size() - x.size()) / 2 + x.size();
  arena_matrix<Eigen::VectorXd> dist(P);
  arena_matrix<Eigen::VectorXd> res_val(P);

  auto arena_x = to_arena_if<!is_constant<T_x>::value>(x);

  double inv_half_sq_l_d
      = 0.5 / (value_of(length_scale) * value_of(length_scale));

  size_t pos = 0;
  for (size_t j = 0; j < x.size(); ++j) {
    for (size_t i = 0; i <= j; ++i) {
      if (i != j) {
        double dist_sq = squared_distance(value_of(x[i]), value_of(x[j]));
        dist.coeffRef(pos) = dist_sq;
        res_val.coeffRef(pos)
            = sigma_sq_d * std::exp(-dist_sq * inv_half_sq_l_d);
      } else {
        dist.coeffRef(pos) = 0.0;
        res_val.coeffRef(pos) = sigma_sq_d;
      }
      pos++;
    }
  }

  arena_matrix<Eigen::Matrix<var, Eigen::Dynamic, Eigen::Dynamic>> res(
      x.size(), x.size());

  pos = 0;
  for (size_t j = 0; j < res.cols(); ++j)
    for (size_t i = 0; i <= j; ++i) {
      res.coeffRef(j, i) = res.coeffRef(i, j) = res_val.coeff(pos);
      pos++;
    }

  reverse_pass_callback([res, res_val, arena_x, dist, sigma, length_scale,
                         sigma_d, l_d]() mutable {
    double sigma_adj = 0.0;
    double l_adj = 0.0;
    double inv_l_d_squared = 1.0 / (l_d * l_d);

    size_t pos = 0;
    for (size_t j = 0; j < arena_x.size(); ++j) {
      for (size_t i = 0; i <= j; ++i) {
        double adj_times_val = res_val.coeffRef(pos) * res.adj().coeff(i, j);

        if (!is_constant<T_sigma>::value)
          sigma_adj += adj_times_val;

        if (!is_constant<T_l>::value)
          l_adj += dist.coeff(pos) * adj_times_val;

        if (!is_constant<T_x>::value && i != j) {
          auto adj = eval(-(value_of(arena_x[i]) - value_of(arena_x[j]))
                          * adj_times_val * inv_l_d_squared);
          using T_x_var = promote_scalar_t<var, T_x>;
          forward_as<T_x_var>(arena_x[i]).adj() += adj;
          forward_as<T_x_var>(arena_x[j]).adj() -= adj;
        }
        pos++;
      }
    }

    if (!is_constant<T_sigma>::value)
      forward_as<var>(sigma).adj() += 2.0 * sigma_adj / sigma_d;

    if (!is_constant<T_l>::value)
      forward_as<var>(length_scale).adj() += l_adj / (l_d * l_d * l_d);
  });

  return res;
}

}  // namespace math
}  // namespace stan
#endif
