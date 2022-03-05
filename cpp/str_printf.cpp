#include "intel_check.h"
#include "str_printf.h"
#include <cstdio>
#include <cstdarg>

std::string strprintf(const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	char buf[32];
	size_t n = std::vsnprintf(buf, sizeof(buf), fmt, args);
	va_end(args);

	if (n < sizeof(buf)) {				// Static buffer large enough?
		return{ buf, n };
	}

	// Static buffer too small
	std::string s(n + 1, 0);
	va_start(args, fmt);
	std::vsnprintf(const_cast<char*>(s.data()), s.size(), fmt, args);
	va_end(args);
	s.resize(n);						// vsnprintf() needs to tack on the 0 character on the end but we then need to remove that 0 character before returning it

	return s;
}