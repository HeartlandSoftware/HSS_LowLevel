#pragma once

#include "types.h"
#ifdef _MSC_VER
#include <comdef.h>
#endif


namespace hss
{
	enum MFCInitOptions
	{
		INIT_NONE = 0x0,
		INIT_WITH_SECURITY = 0x1
	};

	static inline HRESULT InitializeCOM(short coinit = -1, MFCInitOptions options = MFCInitOptions::INIT_NONE) noexcept
	{
#ifdef _MSC_VER
		HRESULT result = E_FAIL;
		if (coinit < 0)
			result = CoInitialize(NULL);
		else
			result = CoInitializeEx(NULL, coinit);

		if (SUCCEEDED(result))
		{
			if (options & INIT_WITH_SECURITY)
				result = CoInitializeSecurity(
					NULL,
					-1,
					NULL,
					NULL,
					RPC_C_AUTHN_LEVEL_DEFAULT,
					RPC_C_IMP_LEVEL_IMPERSONATE,
					NULL,
					EOAC_NONE,
					NULL);
		}

		return result;
#else
		return S_OK;
#endif
	}
}
