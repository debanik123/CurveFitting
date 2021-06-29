#include <iostream>
#include <ceres/ceres.h>
#include <fstream>

#include <algorithm>
#include <iterator>


namespace za{
// The calculation model of the cost function
struct CURVE_FITTING_COST
{
    CURVE_FITTING_COST ( double x, double y ) : _x ( x ), _y ( y ) {}
    // Calculation of residuals
    template <typename T>
    bool operator() (
        const T* const abc,     // Model parameters, there are 3 dimensions
        T* residual ) const     // Residual
    {
        residual[0] = T ( _y ) - ceres::exp ( abc[0]*T ( _x ) *T ( _x ) + abc[1]*T ( _x ) + abc[2] ); // y-exp(ax^2+bx+c)
        return true;
    }
    const double _x, _y;    // x,y data
};
}