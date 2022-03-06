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
        EMPTY
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