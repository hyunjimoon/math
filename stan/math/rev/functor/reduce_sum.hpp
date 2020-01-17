#ifndef STAN_MATH_REV_SCAL_FUNCTOR_REDUCE_SUM_HPP
#define STAN_MATH_REV_SCAL_FUNCTOR_REDUCE_SUM_HPP

#include <stan/math/prim/meta.hpp>
#include <stan/math/prim/functor.hpp>
#include <stan/math/rev/fun.hpp>
#include <tbb/task_arena.h>
#include <tbb/parallel_reduce.h>
#include <tbb/blocked_range.h>

#include <iostream>
#include <iterator>
#include <tuple>
#include <algorithm>
#include <vector>

namespace stan {
namespace math {
namespace internal {

template <typename ReduceFunction, typename ReturnType, typename Vec,
          typename... Args>
struct reduce_sum_impl<ReduceFunction, require_var_t<ReturnType>, ReturnType,
                       Vec, Args...> {
  template <typename T>
  static const T& deep_copy(const T& arg) {
    return arg;
  }

  static var deep_copy(const var& arg) { return var(arg.val()); }

  static std::vector<var> deep_copy(const std::vector<var>& arg) {
    std::vector<var> copy(arg.size());
    for (size_t i = 0; i < arg.size(); ++i) {
      copy[i] = arg[i].val();
    }
    return copy;
  }

  template <int RowType, int ColType>
  static Eigen::Matrix<var, RowType, ColType> deep_copy(
      const Eigen::Matrix<var, RowType, ColType>& arg) {
    Eigen::Matrix<var, RowType, ColType> copy(arg.rows(), arg.cols());
    for (size_t i = 0; i < arg.size(); ++i) {
      copy(i) = arg(i).val();
    }
    return copy;
  }

  // TODO(Steve): Move this somewhere smarter
  // Fails to compile if type T does not have member operator(Integral)
  template <typename T>
  using operator_paren_access_t = decltype(std::declval<T>()(int{}));

  template <typename... Pargs>
  static double* accumulate_adjoints(double* dest, const var& x,
                                     const Pargs&... args) {
    *dest += x.adj();
    return accumulate_adjoints(dest + 1, args...);
  }

  // Works with anything that has operator[Integral] defined
  template <typename... Pargs, typename VecVar,
            require_std_vector_vt<is_var, VecVar>...>
  static double* accumulate_adjoints(double* dest, const VecVar& x,
                                     const Pargs&... args) {
    for (size_t i = 0; i < x.size(); ++i) {
      dest[i] += x[i].adj();
    }
    return accumulate_adjoints(dest + x.size(), args...);
  }

  // Works on anything with a operator()
  template <typename... Pargs, typename Mat, require_eigen_vt<is_var, Mat>...>
  static double* accumulate_adjoints(double* dest, const Mat& x,
                                     const Pargs&... args) {
    for (size_t i = 0; i < x.size(); ++i) {
      dest[i] += x(i).adj();
    }
    return accumulate_adjoints(dest + x.size(), args...);
  }

  // Anything with a scalar type of Arithmetic gets tossed
  template <typename Arith, require_arithmetic_t<scalar_type_t<Arith>>...,
            typename... Pargs>
  static double* accumulate_adjoints(double* dest, Arith&& x,
                                     const Pargs&... args) {
    return accumulate_adjoints(dest, args...);
  }

  static double* accumulate_adjoints(double* x) { return x; }

  struct recursive_reducer {
    size_t num_shared_terms_;
    const Vec& vmapped_;
    std::tuple<const Args&...> args_tuple_;
    size_t tuple_size_ = sizeof...(Args);

    double sum_;
    double* sliced_partials_;
    Eigen::VectorXd args_adjoints_;

    recursive_reducer(size_t num_shared_terms, double* sliced_partials,
                      const Vec& vmapped, const Args&... args)
        : num_shared_terms_(num_shared_terms),
          vmapped_(vmapped),
          args_tuple_(args...),
          sum_(0.0),
          sliced_partials_(sliced_partials),
          args_adjoints_(Eigen::VectorXd::Zero(num_shared_terms_)) {}

    recursive_reducer(recursive_reducer& other, tbb::split)
        : num_shared_terms_(other.num_shared_terms_),
          vmapped_(other.vmapped_),
          args_tuple_(other.args_tuple_),
          sum_(0.0),
          sliced_partials_(other.sliced_partials_),
          args_adjoints_(Eigen::VectorXd::Zero(num_shared_terms_)) {}

    void operator()(const tbb::blocked_range<size_t>& r) {
      if (r.empty())
        return;

      try {
        start_nested();

        // create a deep copy of all var's so that these are not
        // linked to any outer AD tree
        Vec local_sub_slice(r.size());
        // local_sub_slice.reserve(r.size());
        int ii = 0;
        for (int i = r.begin(); i < r.end(); ++i) {
          local_sub_slice[ii] = deep_copy(vmapped_[i]);
          ii++;
        }

        // auto local_sub_slice = deep_copy(sub_slice);

        auto args_tuple_local_copy = apply(
            [&](auto&&... args) {
              return std::tuple<decltype(deep_copy(args))...>(
                  deep_copy(args)...);
            },
            args_tuple_);

        var sub_sum_v = apply(
            [&r, &local_sub_slice](auto&&... args) {
              return ReduceFunction()(r.begin(), r.end() - 1, local_sub_slice,
                                      args...);
            },
            args_tuple_local_copy);

        sub_sum_v.grad();

        sum_ += sub_sum_v.val();

        accumulate_adjoints(sliced_partials_ + r.begin(), local_sub_slice);

        apply(
            [&](auto&&... args) {
              return accumulate_adjoints(args_adjoints_.data(), args...);
            },
            args_tuple_local_copy);
      } catch (const std::exception& e) {
        recover_memory_nested();
        throw;
      }
      recover_memory_nested();
    }

    void join(const recursive_reducer& rhs) {
      sum_ += rhs.sum_;
      args_adjoints_ += rhs.args_adjoints_;
    }
  };

  // Fails to compile if type T does not have callable member size()
  template <typename T>
  using member_size_t = decltype(std::declval<T>().size());

  // TODO(Steve): add requires for generic containers
  template <typename Container,
            require_t<is_detected<Container, member_size_t>>...,
            require_t<is_var<value_type_t<Container>>>..., typename... Pargs>
  size_t count_var_impl(size_t count, const Container& x,
                        const Pargs&... args) const {
    return count_var_impl(count + x.size(), args...);
  }

  template <typename... Pargs>
  size_t count_var_impl(size_t count, const var& x,
                        const Pargs&... args) const {
    return count_var_impl(count + 1, args...);
  }

  template <typename... Pargs, typename Arith,
            require_arithmetic_t<scalar_type_t<Arith>>...>
  size_t count_var_impl(size_t count, Arith& x, const Pargs&... args) const {
    return count_var_impl(count, args...);
  }

  size_t count_var_impl(size_t count) const { return count; }

  /**
   * Count the number of scalars of type T in the input argument list
   *
   * @tparam Pargs Types of input arguments
   * @return Number of scalars of type T in input
   */
  template <typename... Pargs>
  size_t count_var(const Pargs&... args) const {
    return count_var_impl(0, args...);
  }

  template <typename... Pargs>
  void save_varis(vari** dest, const var& x, const Pargs&... args) const {
    *dest = x.vi_;
    save_varis(dest + 1, args...);
  }

  template <typename... Pargs, typename VecVar,
            require_std_vector_vt<is_var, VecVar>...>
  void save_varis(vari** dest, const VecVar& x, const Pargs&... args) const {
    for (size_t i = 0; i < x.size(); ++i) {
      dest[i] = x[i].vi_;
    }
    save_varis(dest + x.size(), args...);
  }

  template <typename... Pargs, typename Mat, require_eigen_vt<is_var, Mat>...>
  void save_varis(vari** dest, const Mat& x, const Pargs&... args) const {
    for (size_t i = 0; i < x.size(); ++i) {
      dest[i] = x(i).vi_;
    }
    save_varis(dest + x.size(), args...);
  }

  template <typename R, require_arithmetic_t<scalar_type_t<R>>...,
            typename... Pargs>
  void save_varis(vari** dest, const R& x, const Pargs&... args) const {
    save_varis(dest, args...);
  }

  void save_varis(vari** /* v */) const {}

  var operator()(const Vec& vmapped, std::size_t grainsize,
                 const Args&... args) const {
    const std::size_t num_jobs = vmapped.size();

    if (num_jobs == 0)
      return var(0.0);

    const std::size_t num_sliced_terms = count_var(vmapped);
    const std::size_t num_shared_terms = count_var(args...);

    vari** varis = ChainableStack::instance_->memalloc_.alloc_array<vari*>(
        num_sliced_terms + num_shared_terms);
    double* partials = ChainableStack::instance_->memalloc_.alloc_array<double>(
        num_sliced_terms + num_shared_terms);

    for (size_t i = 0; i < num_sliced_terms; ++i)
      partials[i] = 0.0;

    double sum = 0;

    recursive_reducer worker(num_shared_terms, partials, vmapped, args...);

#ifdef STAN_DETERMINISTIC
    tbb::static_partitioner partitioner;
    tbb::parallel_deterministic_reduce(
        tbb::blocked_range<std::size_t>(0, num_jobs, grainsize), worker,
        partitioner);
#else
    tbb::parallel_reduce(
        tbb::blocked_range<std::size_t>(0, num_jobs, grainsize), worker);
#endif

    save_varis(varis, vmapped);
    save_varis(varis + num_sliced_terms, args...);

    for (size_t i = 0; i < num_shared_terms; ++i) {
      partials[num_sliced_terms + i] = worker.args_adjoints_(i);
    }

    sum = worker.sum_;

    return var(new precomputed_gradients_vari(
        sum, num_sliced_terms + num_shared_terms, varis, partials));
  }
};
}  // namespace internal

}  // namespace math
}  // namespace stan

#endif
