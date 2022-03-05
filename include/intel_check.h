#pragma once

#if defined(__has_builtin)
#if defined(_MSC_VER)
#if !__has_builtin(__builtin_bit_cast)
template <class _To, class _From>
constexpr _To hss_bit_cast(_From _Val);

#define __builtin_bit_cast(X, Y) hss_bit_cast<X>(Y)
#include <memory>

template <class _To, class _From>
constexpr _To hss_bit_cast(_From _Val)
{
	_To _To_obj; // assumes default-init
	std::memcpy(std::addressof(_To_obj), std::addressof(_Val), sizeof(_To));
	return _To_obj;
}
#endif

#if (defined(__INTEL_COMPILER) || defined(__INTEL_LLVM_COMPILER))
#include <memory>

namespace std
{
	template <class _To, class _From,
		enable_if_t<conjunction_v<bool_constant<sizeof(_To) == 8>, bool_constant<sizeof(_From) == 16>,
		is_trivially_copyable<_To>, is_trivially_copyable<_From>>,
		int> = 0>
	_NODISCARD constexpr _To _Bit_cast(const _From& _Val) noexcept {
		return _Bit_cast<_To>(static_cast<double>(_Val));
	}
}

#endif //__INTEL_COMPILER
#endif //defined(_MSC_VER)
#endif //defined(__has_builtin)
