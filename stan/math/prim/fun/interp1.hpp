#ifndef STAN_MATH_PRIM_FUN_INTERP1_HPP
#define STAN_MATH_PRIM_FUN_INTERP1_HPP

//#include <stan/math/prim/fun/sort_asc.hpp>

using namespace std;
namespace stan {
    namespace math {
        template <typename T>
        inline std::vector<T> interp1(const std::vector<double>& xData, const std::vector<double>& yData, const std::vector<T>& xTest, std::ostream* pstream__)
        {
            using stan::math::index_type_t;
            check_size_match("interp1", "x", xData.size(), "y", yData.size());
            check_sorted("interp1", "x", xData);
            int N = xData.size();
            std::vector<double> dydx(N);
            int M = xTest.size();
            std::vector<T> yVals(M);

            for (int i = 0; i < N - 1; i++){
                dydx[i] = (yData[i+1] -  yData[i]) / (xData[i+1] - xData[i]);
            }
            for(int j = 0; j < M; j ++){
                T x = xTest[j];
                int i= 0;
                while(x > xData[i]){
                    i++;
                }
                if(x < xData[0]){yVals[j] = yData[0];}
                else if (x > xData[N-1]){yVals[j] = yData[N-1];}
                else{yVals[j] = yData[i-1] + dydx[i-1] * (x - xData[i-1]);}
            }
            return yVals;
        }
    }
}

#endif
