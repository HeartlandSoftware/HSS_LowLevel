//	propsysreplacement.cpp
//
//	By Robert W. Bryce
//	Version 1.2
//	Started:	January 17, 2008
//	Last Modified:	November 4, 2018

#include <cassert>
#include "types.h"
#include "propsysreplacement.h"


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

