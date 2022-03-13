#pragma once

namespace zgCFD {
    enum class BoundaryCondationType
    {
        SPECIFIEDVALUE,
        SPECIFIEDFLUX,
        ZEROFLUX,
        ZEROGRADIENT,
        SLIP,
        NOSLIP,
        INLET,
        OUTLET,
        EMPTY,
        CALCULATED
    };

    enum class TheInterpolationScheme
    {
        AVERAGE,
        UPWIND,
        HYPERBOLIC,
        AVERAGECORRECTED
    };
    enum class ComputeGradientScheme
    {
        GAUSS,
        NODAL
    };

}