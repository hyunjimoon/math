#include <stan/math/rev.hpp>
#include <gtest/gtest.h>
#include <limits>

TEST(AgradRev, interp1_gradient) {
using stan::math::var;
using stan::math::interp1;

std::vector<double> x = { 0.1, 1.2 };
std::vector<double> y = { 1.0, -2.0 };

var x_int = 0.7;
std::vector<var> x_int_vec = { x_int };

std::vector<var> y_int_vec = interp1(x, y, x_int_vec);
var y_int = y_int_vec[0];

//var y_int = interp1(x, y, x_int_vec);

y_int.grad();

// x_int.adj() should contain dy_int/dx_int
EXPECT_FLOAT_EQ(-3.0 / 1.1, x_int.adj());
}