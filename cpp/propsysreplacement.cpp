/**
 * propsysreplacement.cpp
 *
 * Copyright 2008-2023 Heartland Software Solutions Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the license at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the LIcense is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <cassert>
#include "types.h"
#include "propsysreplacement.h"

#ifndef _NO_MFC

HRESULT VariantToBoolean_(const VARIANT &varIn, BOOL *retval) {

	switch (varIn.vt) {
	case VT_BOOL:	*retval = (varIn.boolVal) ? TRUE : FALSE;	break;
	case VT_I1:	*retval = (varIn.cVal) ? TRUE : FALSE;		break;
	case VT_I2:	*retval = (varIn.iVal) ? TRUE : FALSE;		break;
	case VT_I4:	*retval = (varIn.lVal) ? TRUE : FALSE;		break;
	case VT_I8:	*retval = (varIn.llVal) ? TRUE : FALSE;		break;
	case VT_UI1:	*retval = (varIn.bVal) ? TRUE : FALSE;		break;
	case VT_UI2:	*retval = (varIn.uiVal) ? TRUE : FALSE;		break;
	case VT_UI4:	*retval = (varIn.ulVal) ? TRUE : FALSE;		break;
	case VT_UI8:	*retval = (varIn.ullVal) ? TRUE : FALSE;	break;
	default:
		weak_assert(false);
		*retval = FALSE;
		return E_FAIL;
	}
	return S_OK;
}


HRESULT VariantToInt16_(const VARIANT &varIn, SHORT *retval) {

	switch (varIn.vt) {
	case VT_BOOL:	*retval = (varIn.boolVal) ? 1 : 0;	break;
	case VT_I1:	*retval = (varIn.cVal);			break;
	case VT_I2:	*retval = (varIn.iVal);			break;
	case VT_I4:	*retval = (SHORT)(varIn.lVal);		break;
	case VT_I8:	*retval = (SHORT)(varIn.llVal);		break;
	case VT_UI1:	*retval = (varIn.bVal);			break;
	case VT_UI2:	*retval = (varIn.uiVal);		break;
	case VT_UI4:	*retval = (SHORT)(varIn.ulVal);		break;
	case VT_UI8:	*retval = (SHORT)(varIn.ullVal); 	break;
	case VT_R8:	*retval = (SHORT)(varIn.dblVal + 0.5);	break;
	case VT_R4:	*retval = (SHORT)(varIn.fltVal + 0.5);	break;
	default:
		weak_assert(false);
		*retval = 0;
		return E_FAIL;
	}
	return S_OK;
}


HRESULT VariantToInt32_(const VARIANT &varIn, LONG *retval) {

	switch (varIn.vt) {
	case VT_BOOL:	*retval = (varIn.boolVal) ? 1 : 0;	break;
	case VT_I1:	*retval = (varIn.cVal);			break;
	case VT_I2:	*retval = (varIn.iVal);			break;
	case VT_I4:	*retval = (varIn.lVal);			break;
	case VT_I8:	*retval = (LONG)(varIn.llVal);		break;
	case VT_UI1:	*retval = (varIn.bVal);			break;
	case VT_UI2:	*retval = (varIn.uiVal);		break;
	case VT_UI4:	*retval = (varIn.ulVal);		break;
	case VT_UI8:	*retval = (LONG)(varIn.ullVal); 	break;
	case VT_R8:	*retval = (LONG)(varIn.dblVal + 0.5);	break;
	case VT_R4:	*retval = (LONG)(varIn.fltVal + 0.5);	break;
	default:
		weak_assert(false);
		*retval = 0;
		return E_FAIL;
	}
	return S_OK;
}


HRESULT VariantToInt64_(const VARIANT &varIn, LONGLONG *retval) {

	switch (varIn.vt) {
	case VT_BOOL:	*retval = (varIn.boolVal) ? 1 : 0;		break;
	case VT_I1:	*retval = (varIn.cVal);				break;
	case VT_I2:	*retval = (varIn.iVal);				break;
	case VT_I4:	*retval = (varIn.lVal);				break;
	case VT_I8:	*retval = (varIn.llVal);			break;
	case VT_UI1:	*retval = (varIn.bVal);				break;
	case VT_UI2:	*retval = (varIn.uiVal);			break;
	case VT_UI4:	*retval = (varIn.ulVal);			break;
	case VT_UI8:	*retval = (varIn.ullVal); 			break;
	case VT_R8:	*retval = (LONGLONG)(varIn.dblVal + 0.5);	break;
	case VT_R4:	*retval = (LONGLONG)(varIn.fltVal + 0.5);	break;
	default:
		weak_assert(false);
		*retval = 0;
		return E_FAIL;
	}
	return S_OK;
}


HRESULT VariantToUInt16_(const VARIANT &varIn, USHORT *retval) {

	switch (varIn.vt) {
	case VT_BOOL:	*retval = (varIn.boolVal) ? 1 : 0;	break;
	case VT_I1:	*retval = (varIn.cVal);			break;
	case VT_I2:	*retval = (varIn.iVal);			break;
	case VT_I4:	*retval = (USHORT)(varIn.lVal);		break;
	case VT_I8:	*retval = (USHORT)(varIn.llVal);	break;
	case VT_UI1:	*retval = (varIn.bVal);			break;
	case VT_UI2:	*retval = (varIn.uiVal);		break;
	case VT_UI4:	*retval = (USHORT)(varIn.ulVal);	break;
	case VT_UI8:	*retval = (USHORT)(varIn.ullVal); 	break;
	case VT_R8:	*retval = (USHORT)(varIn.dblVal + 0.5);	break;
	case VT_R4:	*retval = (USHORT)(varIn.fltVal + 0.5);	break;
	default:
		weak_assert(false);
		*retval = 0;
		return E_FAIL;
	}
	return S_OK;
}


HRESULT VariantToUInt32_(const VARIANT &varIn, ULONG *retval) {

	switch (varIn.vt) {
	case VT_BOOL:	*retval = (varIn.boolVal) ? 1 : 0;	break;
	case VT_I1:	*retval = (varIn.cVal);			break;
	case VT_I2:	*retval = (varIn.iVal);			break;
	case VT_I4:	*retval = (varIn.lVal);			break;
	case VT_I8:	*retval = (ULONG)(varIn.llVal);		break;
	case VT_UI1:	*retval = (varIn.bVal);			break;
	case VT_UI2:	*retval = (varIn.uiVal);		break;
	case VT_UI4:	*retval = (varIn.ulVal);		break;
	case VT_UI8:	*retval = (ULONG)(varIn.ullVal); 	break;
	case VT_R8:	*retval = (ULONG)(varIn.dblVal + 0.5);	break;
	case VT_R4:	*retval = (ULONG)(varIn.fltVal + 0.5);	break;
	default:
		weak_assert(false);
		*retval = 0;
		return E_FAIL;
	}
	return S_OK;
}


HRESULT VariantToUInt64_(const VARIANT &varIn, ULONGLONG *retval) {

	switch (varIn.vt) {
	case VT_BOOL:	*retval = (varIn.boolVal) ? 1 : 0;		break;
	case VT_I1:	*retval = (varIn.cVal);				break;
	case VT_I2:	*retval = (varIn.iVal);				break;
	case VT_I4:	*retval = (varIn.lVal);				break;
	case VT_I8:	*retval = (varIn.llVal);			break;
	case VT_UI1:	*retval = (varIn.bVal);				break;
	case VT_UI2:	*retval = (varIn.uiVal);			break;
	case VT_UI4:	*retval = (varIn.ulVal);			break;
	case VT_UI8:	*retval = (varIn.ullVal); 			break;
	case VT_R8:	*retval = (ULONGLONG)(varIn.dblVal + 0.5);	break;
	case VT_R4:	*retval = (ULONGLONG)(varIn.fltVal + 0.5);	break;
	default:
		weak_assert(false);
		*retval = 0;
		return E_FAIL;
	}
	return S_OK;
}


HRESULT VariantToDouble_(const VARIANT &varIn, DOUBLE *pdblRet) {

	switch (varIn.vt) {
	case VT_R8:	*pdblRet = varIn.dblVal;		break;
	case VT_R4:	*pdblRet = varIn.fltVal;		break;
	case VT_I1:	*pdblRet = (double)varIn.cVal;		break;
	case VT_I2:	*pdblRet = (double)varIn.iVal;		break;
	case VT_I4:	*pdblRet = (double)varIn.lVal;		break;
	case VT_I8:	*pdblRet = (double)varIn.llVal;		break;
	case VT_UI1:	*pdblRet = (double)varIn.bVal;		break;
	case VT_UI2:	*pdblRet = (double)varIn.uiVal;		break;
	case VT_UI4:	*pdblRet = (double)varIn.ulVal;		break;
	case VT_UI8:	*pdblRet = (double)varIn.ullVal;	break;
	default:
		weak_assert(false);
		*pdblRet = 0.0;
		return E_FAIL;
	}
	return S_OK;
}

#endif

bool variantToBoolean(const NumericVariant &varIn, bool *retval) {

	if (std::holds_alternative<bool>(varIn))
		*retval = (std::get<bool>(varIn)) ? true : false;

	else if (std::holds_alternative<std::int8_t>(varIn))
		*retval = (std::get<std::int8_t>(varIn)) ? true : false;

	else if (std::holds_alternative<std::int16_t>(varIn))
		*retval = (std::get<std::int16_t>(varIn)) ? true : false;

	else if (std::holds_alternative<std::int32_t>(varIn))
		*retval = (std::get<std::int32_t>(varIn)) ? true : false;

	else if (std::holds_alternative<std::int64_t>(varIn))
		*retval = (std::get<std::int64_t>(varIn)) ? true : false;

	else if (std::holds_alternative<std::uint8_t>(varIn))
		*retval = (std::get<std::uint8_t>(varIn)) ? true : false;

	else if (std::holds_alternative<std::uint16_t>(varIn))
		*retval = (std::get<std::uint16_t>(varIn)) ? true : false;

	else if (std::holds_alternative<std::uint32_t>(varIn))
		*retval = (std::get<std::uint32_t>(varIn)) ? true : false;

	else if (std::holds_alternative<std::uint64_t>(varIn))
		*retval = (std::get<std::uint64_t>(varIn)) ? true : false;

	else {
		weak_assert(false);
		*retval = false;
		return false;
	}

	return true;
}


bool variantToInt8(const NumericVariant &varIn, std::int8_t *retval) {

	if (std::holds_alternative<bool>(varIn))
		*retval = (std::get<bool>(varIn)) ? 1 : 0;

	else if (std::holds_alternative<std::int8_t>(varIn))
		*retval = (std::get<std::int8_t>(varIn));

	else if (std::holds_alternative<std::int16_t>(varIn))
		*retval = (std::int8_t)(std::get<std::int16_t>(varIn));

	else if (std::holds_alternative<std::int32_t>(varIn))
		*retval = (std::int8_t)(std::get<std::int32_t>(varIn));

	else if (std::holds_alternative<std::int64_t>(varIn))
		*retval = (std::int8_t)(std::get<std::int64_t>(varIn));

	else if (std::holds_alternative<std::uint8_t>(varIn))
		*retval = (std::get<std::uint8_t>(varIn));

	else if (std::holds_alternative<std::uint16_t>(varIn))
		*retval = (std::int8_t)(std::get<std::uint16_t>(varIn));

	else if (std::holds_alternative<std::uint32_t>(varIn))
		*retval = (std::int8_t)(std::get<std::uint32_t>(varIn));

	else if (std::holds_alternative<std::uint64_t>(varIn))
		*retval = (std::int8_t)(std::get<std::uint64_t>(varIn));

	else if (std::holds_alternative<float>(varIn))
		*retval = (std::int8_t)(std::get<float>(varIn) + 0.5);

	else if (std::holds_alternative<double>(varIn))
		*retval = (std::int8_t)(std::get<double>(varIn) + 0.5);

	else {
		weak_assert(false);
		*retval = 0;
		return false;
	}

	return true;
}


bool variantToInt16(const NumericVariant &varIn, std::int16_t *retval) {

	if (std::holds_alternative<bool>(varIn))
		*retval = (std::get<bool>(varIn)) ? 1 : 0;

	else if (std::holds_alternative<std::int8_t>(varIn))
		*retval = (std::get<std::int8_t>(varIn));

	else if (std::holds_alternative<std::int16_t>(varIn))
		*retval = (std::get<std::int16_t>(varIn));

	else if (std::holds_alternative<std::int32_t>(varIn))
		*retval = (std::int16_t)(std::get<std::int32_t>(varIn));

	else if (std::holds_alternative<std::int64_t>(varIn))
		*retval = (std::int16_t)(std::get<std::int64_t>(varIn));

	else if (std::holds_alternative<std::uint8_t>(varIn))
		*retval = (std::get<std::uint8_t>(varIn));

	else if (std::holds_alternative<std::uint16_t>(varIn))
		*retval = (std::get<std::uint16_t>(varIn));

	else if (std::holds_alternative<std::uint32_t>(varIn))
		*retval = (std::int16_t)(std::get<std::uint32_t>(varIn));

	else if (std::holds_alternative<std::uint64_t>(varIn))
		*retval = (std::int16_t)(std::get<std::uint64_t>(varIn));

	else if (std::holds_alternative<float>(varIn))
		*retval = (std::int16_t)(std::int16_t)(std::get<float>(varIn) + 0.5);

	else if (std::holds_alternative<double>(varIn))
		*retval = (std::int16_t)(std::get<double>(varIn) + 0.5);

	else {
		weak_assert(false);
		*retval = 0;
		return false;
	}

	return true;
}


bool variantToInt32(const NumericVariant &varIn, std::int32_t *retval) {

	if (std::holds_alternative<bool>(varIn))
		*retval = (std::get<bool>(varIn)) ? 1 : 0;

	else if (std::holds_alternative<std::int8_t>(varIn))
		*retval = (std::get<std::int8_t>(varIn));

	else if (std::holds_alternative<std::int16_t>(varIn))
		*retval = (std::get<std::int16_t>(varIn));

	else if (std::holds_alternative<std::int32_t>(varIn))
		*retval = (std::get<std::int32_t>(varIn));

	else if (std::holds_alternative<std::int64_t>(varIn))
		*retval = (std::int32_t)(std::get<std::int64_t>(varIn));

	else if (std::holds_alternative<std::uint8_t>(varIn))
		*retval = (std::get<std::uint8_t>(varIn));

	else if (std::holds_alternative<std::uint16_t>(varIn))
		*retval = (std::get<std::uint16_t>(varIn));

	else if (std::holds_alternative<std::uint32_t>(varIn))
		*retval = (std::get<std::uint32_t>(varIn));

	else if (std::holds_alternative<std::uint64_t>(varIn))
		*retval = (std::int32_t)(std::get<std::uint64_t>(varIn));

	else if (std::holds_alternative<float>(varIn))
		*retval = (std::int32_t)(std::get<float>(varIn) + 0.5);

	else if (std::holds_alternative<double>(varIn))
		*retval = (std::int32_t)(std::get<double>(varIn) + 0.5);

	else {
		weak_assert(false);
		*retval = 0;
		return false;
	}

	return true;
}


bool variantToInt64(const NumericVariant &varIn, std::int64_t *retval) {

	if (std::holds_alternative<bool>(varIn))
		*retval = (std::get<bool>(varIn)) ? 1 : 0;

	else if (std::holds_alternative<std::int8_t>(varIn))
		*retval = (std::get<std::int8_t>(varIn));

	else if (std::holds_alternative<std::int16_t>(varIn))
		*retval = (std::get<std::int16_t>(varIn));

	else if (std::holds_alternative<std::int32_t>(varIn))
		*retval = (std::get<std::int32_t>(varIn));

	else if (std::holds_alternative<std::int64_t>(varIn))
		*retval = (std::get<std::int64_t>(varIn));

	else if (std::holds_alternative<std::uint8_t>(varIn))
		*retval = (std::get<std::uint8_t>(varIn));

	else if (std::holds_alternative<std::uint16_t>(varIn))
		*retval = (std::get<std::uint16_t>(varIn));

	else if (std::holds_alternative<std::uint32_t>(varIn))
		*retval = (std::get<std::uint32_t>(varIn));

	else if (std::holds_alternative<std::uint64_t>(varIn))
		*retval = (std::get<std::uint64_t>(varIn));

	else if (std::holds_alternative<float>(varIn))
		*retval = (std::int64_t)(std::get<float>(varIn) + 0.5);

	else if (std::holds_alternative<double>(varIn))
		*retval = (std::int64_t)(std::get<double>(varIn) + 0.5);

	else {
		weak_assert(false);
		*retval = 0;
		return false;
	}

	return true;
}


bool variantToUInt8(const NumericVariant &varIn, std::uint8_t *retval) {

	if (std::holds_alternative<bool>(varIn))
		*retval = (std::get<bool>(varIn)) ? 1 : 0;

	else if (std::holds_alternative<std::int8_t>(varIn))
		*retval = (std::get<std::int8_t>(varIn));

	else if (std::holds_alternative<std::int16_t>(varIn))
		*retval = (std::uint8_t)(std::get<std::int16_t>(varIn));

	else if (std::holds_alternative<std::int32_t>(varIn))
		*retval = (std::uint8_t)(std::get<std::int32_t>(varIn));

	else if (std::holds_alternative<std::int64_t>(varIn))
		*retval = (std::uint8_t)(std::get<std::int64_t>(varIn));

	else if (std::holds_alternative<std::uint8_t>(varIn))
		*retval = (std::get<std::uint8_t>(varIn));

	else if (std::holds_alternative<std::uint16_t>(varIn))
		*retval = (std::uint8_t)(std::get<std::uint16_t>(varIn));

	else if (std::holds_alternative<std::uint32_t>(varIn))
		*retval = (std::uint8_t)(std::get<std::uint32_t>(varIn));

	else if (std::holds_alternative<std::uint64_t>(varIn))
		*retval = (std::uint8_t)(std::get<std::uint64_t>(varIn));

	else if (std::holds_alternative<float>(varIn))
		*retval = (std::uint8_t)(std::get<float>(varIn) + 0.5);

	else if (std::holds_alternative<double>(varIn))
		*retval = (std::uint8_t)(std::get<double>(varIn) + 0.5);

	else {
		weak_assert(false);
		*retval = 0;
		return false;
	}

	return true;
}


bool variantToUInt16(const NumericVariant &varIn, std::uint16_t *retval) {

	if (std::holds_alternative<bool>(varIn))
		*retval = (std::get<bool>(varIn)) ? 1 : 0;

	else if (std::holds_alternative<std::int8_t>(varIn))
		*retval = (std::get<std::int8_t>(varIn));

	else if (std::holds_alternative<std::int16_t>(varIn))
		*retval = (std::get<std::int16_t>(varIn));

	else if (std::holds_alternative<std::int32_t>(varIn))
		*retval = (std::uint16_t)(std::get<std::int32_t>(varIn));

	else if (std::holds_alternative<std::int64_t>(varIn))
		*retval = (std::uint16_t)(std::get<std::int64_t>(varIn));

	else if (std::holds_alternative<std::uint8_t>(varIn))
		*retval = (std::get<std::uint8_t>(varIn));

	else if (std::holds_alternative<std::uint16_t>(varIn))
		*retval = (std::get<std::uint16_t>(varIn));

	else if (std::holds_alternative<std::uint32_t>(varIn))
		*retval = (std::uint16_t)(std::get<std::uint32_t>(varIn));

	else if (std::holds_alternative<std::uint64_t>(varIn))
		*retval = (std::uint16_t)(std::get<std::uint64_t>(varIn));

	else if (std::holds_alternative<float>(varIn))
		*retval = (std::uint16_t)(std::get<float>(varIn) + 0.5);

	else if (std::holds_alternative<double>(varIn))
		*retval = (std::uint16_t)(std::get<double>(varIn) + 0.5);

	else {
		weak_assert(false);
		*retval = 0;
		return false;
	}

	return true;
}


bool variantToUInt32(const NumericVariant &varIn, std::uint32_t *retval) {

	if (std::holds_alternative<bool>(varIn))
		*retval = (std::get<bool>(varIn)) ? 1 : 0;

	else if (std::holds_alternative<std::int8_t>(varIn))
		*retval = (std::get<std::int8_t>(varIn));

	else if (std::holds_alternative<std::int16_t>(varIn))
		*retval = (std::get<std::int16_t>(varIn));

	else if (std::holds_alternative<std::int32_t>(varIn))
		*retval = (std::get<std::int32_t>(varIn));

	else if (std::holds_alternative<std::int64_t>(varIn))
		*retval = (std::uint32_t)(std::get<std::int64_t>(varIn));

	else if (std::holds_alternative<std::uint8_t>(varIn))
		*retval = (std::get<std::uint8_t>(varIn));

	else if (std::holds_alternative<std::uint16_t>(varIn))
		*retval = (std::get<std::uint16_t>(varIn));

	else if (std::holds_alternative<std::uint32_t>(varIn))
		*retval = (std::get<std::uint32_t>(varIn));

	else if (std::holds_alternative<std::uint64_t>(varIn))
		*retval = (std::uint32_t)(std::get<std::uint64_t>(varIn));

	else if (std::holds_alternative<float>(varIn))
		*retval = (std::uint32_t)(std::get<float>(varIn) + 0.5);

	else if (std::holds_alternative<double>(varIn))
		*retval = (std::uint32_t)(std::get<double>(varIn) + 0.5);

	else {
		weak_assert(false);
		*retval = 0;
		return false;
	}

	return true;
}


bool variantToUInt64(const NumericVariant &varIn, std::uint64_t *retval) {

	if (std::holds_alternative<bool>(varIn))
		*retval = (std::get<bool>(varIn)) ? 1 : 0;

	else if (std::holds_alternative<std::int8_t>(varIn))
		*retval = (std::get<std::int8_t>(varIn));

	else if (std::holds_alternative<std::int16_t>(varIn))
		*retval = (std::get<std::int16_t>(varIn));

	else if (std::holds_alternative<std::int32_t>(varIn))
		*retval = (std::get<std::int32_t>(varIn));

	else if (std::holds_alternative<std::int64_t>(varIn))
		*retval = (std::get<std::int64_t>(varIn));

	else if (std::holds_alternative<std::uint8_t>(varIn))
		*retval = (std::get<std::uint8_t>(varIn));

	else if (std::holds_alternative<std::uint16_t>(varIn))
		*retval = (std::get<std::uint16_t>(varIn));

	else if (std::holds_alternative<std::uint32_t>(varIn))
		*retval = (std::get<std::uint32_t>(varIn));

	else if (std::holds_alternative<std::uint64_t>(varIn))
		*retval = (std::get<std::uint64_t>(varIn));

	else if (std::holds_alternative<float>(varIn))
		*retval = (std::uint64_t)(std::get<float>(varIn) + 0.5);

	else if (std::holds_alternative<double>(varIn))
		*retval = (std::uint64_t)(std::get<double>(varIn) + 0.5);

	else {
		weak_assert(false);
		*retval = 0;
		return false;
	}

	return true;
}


bool variantToFloat(const NumericVariant &varIn, float *pdblRet) {

	if (std::holds_alternative<double>(varIn))
		*pdblRet = (float)std::get<double>(varIn);

	else if (std::holds_alternative<float>(varIn))
		*pdblRet = std::get<float>(varIn);

	else if (std::holds_alternative<std::int8_t>(varIn))
		*pdblRet = (float)std::get<std::int8_t>(varIn);

	else if (std::holds_alternative<std::int16_t>(varIn))
		*pdblRet = (float)std::get<std::int16_t>(varIn);

	else if (std::holds_alternative<std::int32_t>(varIn))
		*pdblRet = (float)std::get<std::int32_t>(varIn);

	else if (std::holds_alternative<std::int64_t>(varIn))
		*pdblRet = (float)std::get<std::int64_t>(varIn);

	else if (std::holds_alternative<std::uint8_t>(varIn))
		*pdblRet = (float)std::get<std::uint8_t>(varIn);

	else if (std::holds_alternative<std::uint16_t>(varIn))
		*pdblRet = (float)std::get<std::uint16_t>(varIn);

	else if (std::holds_alternative<std::uint32_t>(varIn))
		*pdblRet = (float)std::get<std::uint32_t>(varIn);

	else if (std::holds_alternative<std::uint64_t>(varIn))
		*pdblRet = (float)std::get<std::uint64_t>(varIn);

	else {
		weak_assert(false);
		*pdblRet = 0.0;
		return false;
	}

	return true;
}


bool variantToDouble(const NumericVariant &varIn, double *pdblRet) {

	if (std::holds_alternative<double>(varIn))
		*pdblRet = std::get<double>(varIn);

	else if (std::holds_alternative<float>(varIn))
		*pdblRet = std::get<float>(varIn);

	else if (std::holds_alternative<std::int8_t>(varIn))
		*pdblRet = (double)std::get<std::int8_t>(varIn);

	else if (std::holds_alternative<std::int16_t>(varIn))
		*pdblRet = (double)std::get<std::int16_t>(varIn);

	else if (std::holds_alternative<std::int32_t>(varIn))
		*pdblRet = (double)std::get<std::int32_t>(varIn);

	else if (std::holds_alternative<std::int64_t>(varIn))
		*pdblRet = (double)std::get<std::int64_t>(varIn);

	else if (std::holds_alternative<std::uint8_t>(varIn))
		*pdblRet = (double)std::get<std::uint8_t>(varIn);

	else if (std::holds_alternative<std::uint16_t>(varIn))
		*pdblRet = (double)std::get<std::uint16_t>(varIn);

	else if (std::holds_alternative<std::uint32_t>(varIn))
		*pdblRet = (double)std::get<std::uint32_t>(varIn);

	else if (std::holds_alternative<std::uint64_t>(varIn))
		*pdblRet = (double)std::get<std::uint64_t>(varIn);

	else {
		weak_assert(false);
		*pdblRet = 0.0;
		return false;
	}

	return true;
}

