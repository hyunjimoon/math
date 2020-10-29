#ifndef STAN_MATH_PRIM_FUN_INTERP1_HPP
#define STAN_MATH_PRIM_FUN_INTERP1_HPP

//#include <stan/math/prim/fun/sort_asc.hpp>

using namespace std;
namespace stan {
    namespace math {
        template <typename T>
        inline std::vector<double> interp1(const std::vector<T>& xDataPre, const std::vector<T>& yDataPre, const std::vector<T>& xTest)
        {
            using stan::math::index_type_t;
            int size_xData = xDataPre.size();
            std::vector<double> dydx(size_xData);
            int size_x = xTest.size();
            std::vector<double> yVals(size_x);

            // sort if xData is not monotone
            std::vector< pair<int, int> > vec;
            for(int i=0; i<size_xData;i++){
                vec.push_back(make_pair(xDataPre[i], yDataPre[i]));
            }
            std::sort(vec.begin(), vec.end());
            std::vector<double > xData(size_xData);
            std::vector<double > yData(size_xData);
            for (int i=0; i<size_xData; i++){
                xData[i] = vec[i].first;
                yData[i] = vec[i].second;
            }
            for (int i = 0; i < size_xData; i++){
                dydx[i] = (yData[i+1] -  yData[i]) / (xData[i+1] - xData[i]);
            }
            for(int j = 0; j <size_x; j ++){
                double x = xTest[j];
                int i= 0;
                while(x > xData[i]){
                    i++;
                }
                if(x < xData[0]){yVals[j] = yData[0];}
                else if (x > xData[size_xData-1]){yVals[j] = yData[size_xData-1];}
                else{yVals[j] = yData[i-1] + dydx[i-1] * (x - xData[i-1]);}
            }
            return yVals;
        }
    }
}

#endif
