#ifndef STAN_MATH_PRIM_FUN_INTERP1_HPP
#define STAN_MATH_PRIM_FUN_INTERP1_HPP

//#include <stan/math/prim/fun/sort_asc.hpp>

using namespace std;
namespace stan {
    namespace math {
        template <typename T>
        inline std::vector<double> interp1(const std::vector<T>& xData, const std::vector<T>& yData, const std::vector<T>& xVector)
        {
            using stan::math::index_type_t;

            std::vector<double> yVals;
            int size_x = xVector.size();
            int size_xData = xData.size();

            for(int j=0; j < size_x; j++) {

                double x = xVector[j];
                int i = 0;
                if (x >= xVector[size_xData - 2])                 // special case: beyond right end
                {
                    i = size_xData - 2;
                } else {
                    while (x > xVector[i + 1])
                        i++;                                   // find left end of interval for interpolation
                }
                double xL = xVector[i], yL = yData[i], xR = xVector[i + 1], yR = yData[i + 1]; // points on either side (unless beyond ends)
                if (x < xL) yR = yL;                                                 // if beyond ends of array
                if (x > xR) yL = yR;
                double dydx = (yR - yL) / (xR - xL);                               // gradient
                yVals[j] = yL + dydx * (x - xL);
            }
            return yVals;                                         // linear interpolation
        }
    }  // namespace math
}  // namespace stan

#endif
/*

#ifndef STAN_MATH_PRIM_FUN_INTERP1_HPP
#define STAN_MATH_PRIM_FUN_INTERP1_HPP

//#include <stan/math/prim/fun/sort_asc.hpp>

using namespace std;
namespace stan {
    namespace math {

        inline Eigen::VectorXd interp1(Eigen::VectorXd& xData, Eigen::VectorXd& yData, Eigen::VectorXd& xVector)
        {
            using stan::math::index_type_t;

            Eigen::VectorXd yVals;
            int size_x = xVector.size();
            int size_xData = xData.size();

            for(int j=0; j < size_x; j++) {

                double x = xVector[j];
                int i = 0;
                if (x >= xVector[size_xData - 2])                 // special case: beyond right end
                {
                    i = size_xData - 2;
                } else {
                    while (x > xVector[i + 1])
                        i++;                                   // find left end of interval for interpolation
                }
                double xL = xVector[i], yL = yData[i], xR = xVector[i + 1], yR = yData[i + 1]; // points on either side (unless beyond ends)
                if (x < xL) yR = yL;                                                 // if beyond ends of array
                if (x > xR) yL = yR;
                double dydx = (yR - yL) / (xR - xL);                               // gradient
                yVals[j] = yL + dydx * (x - xL);
            }
            return yVals;                                         // linear interpolation
        }
    }  // namespace math
}  // namespace stan

#endif

*/
