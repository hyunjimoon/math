#ifndef STAN_MATH_PRIM_FUN_INTERP1_HPP
#define STAN_MATH_PRIM_FUN_INTERP1_HPP

using namespace std;
namespace stan {
    namespace math {
        template <typename T1, typename T2, typename T3>
        inline std::vector<return_type_t<T1, T2, T3>> interp1(const std::vector<T1>& xData, const std::vector<T2>& yData, const std::vector<T3>& xTest, std::ostream* pstream__ = nullptr)
        {
            using stan::math::index_type_t;
            check_size_match("interp1", "x", xData.size(), "y", yData.size());
            check_sorted("interp1", "x", xData);
            int N = xData.size();
            std::vector<double> dydx(N);
            int M = xTest.size();
            std::vector<return_type_t<T1, T2, T3>> yVals(M);

            for (int i = 0; i < N - 1; i++){
                dydx[i] = (yData[i+1] -  yData[i]) / (xData[i+1] - xData[i]);
            }
            for(int j = 0; j < M; j ++){
                T3 x = xTest[j];
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
