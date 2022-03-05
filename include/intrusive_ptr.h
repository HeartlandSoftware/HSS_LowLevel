#pragma once


#if defined(__clang__)
#define BIP_GSL_SUPPRESS(x) [[gsl::suppress("x")]]
#define _NODISCARD [[nodiscard]]
#else
#if defined(_MSC_VER)
#define BIP_GSL_SUPPRESS(x) [[gsl::suppress(x)]]
#else
#define BIP_GSL_SUPPRESS(x)
#define _NODISCARD [[nodiscard]]
#endif // _MSC_VER
#endif // __clang__

#include <boost/intrusive_ptr.hpp>

namespace boost
{
    // FUNCTION TEMPLATE make_intrusive
    template <class _Ty, class... _Types, std::enable_if_t<!std::is_array_v<_Ty>, int> = 0>
    BIP_GSL_SUPPRESS(r.11)
    _NODISCARD intrusive_ptr<_Ty> make_intrusive(_Types&&... _Args)
    {
        return intrusive_ptr<_Ty>(new _Ty(std::forward<_Types>(_Args)...));
    }
}
