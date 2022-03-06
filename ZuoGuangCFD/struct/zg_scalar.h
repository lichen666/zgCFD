#pragma once

namespace zgCFD {
#define WM_DP

#if defined(WM_SP)
	typedef float Scalar;

#elif defined(WM_DP)
	typedef double Scalar;

#elif defined(WM_LP)
	typedef long double Scalar;

#endif
}
