/**
 * convert.h
 *
 * Copyright 2010-2023 Heartland Software Solutions Inc.
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

#ifndef __HSS_CONVERT_H__
#define __HSS_CONVERT_H__

#ifdef _MSC_VER
#if (!defined(__INTEL_COMPILER)) && (!defined(__INTEL_LLVM_COMPILER))
#pragma managed(push, off)
#endif
#endif

#include "types.h"
#include "tstring.h"

namespace UnitConvert {

typedef std::uint32_t STORAGE_UNITCONVERT;
typedef std::uint64_t STORAGE_UNIT;

#define STORAGE_TIME_START			((UnitConvert::STORAGE_UNITCONVERT)0x00010000)		
	#define STORAGE_FORMAT_MICROSECOND	((UnitConvert::STORAGE_UNITCONVERT)0x00080000)
	#define STORAGE_FORMAT_MILLISECOND	((UnitConvert::STORAGE_UNITCONVERT)0x00090000)
	#define STORAGE_FORMAT_SECOND		((UnitConvert::STORAGE_UNITCONVERT)0x00010000)
	#define STORAGE_FORMAT_MINUTE		((UnitConvert::STORAGE_UNITCONVERT)0x00020000)
	#define STORAGE_FORMAT_HOUR		((UnitConvert::STORAGE_UNITCONVERT)0x00030000)
	#define STORAGE_FORMAT_DAY		((UnitConvert::STORAGE_UNITCONVERT)0x00040000)
	#define STORAGE_FORMAT_WEEK		((UnitConvert::STORAGE_UNITCONVERT)0x00050000)
	#define STORAGE_FORMAT_MONTH		((UnitConvert::STORAGE_UNITCONVERT)0x00060000)
	#define STORAGE_FORMAT_YEAR		((UnitConvert::STORAGE_UNITCONVERT)0x00070000)
	#define STORAGE_FORMAT_DECADE		((UnitConvert::STORAGE_UNITCONVERT)0x000a0000)
	#define STORAGE_FORMAT_CENTURY		((UnitConvert::STORAGE_UNITCONVERT)0x000b0000)
	#define STORAGE_FORMAT_TIME_DIV		((UnitConvert::STORAGE_UNITCONVERT)0x00000000)	// if you need 'per' as in 'kph' (this is the default)
	#define STORAGE_FORMAT_TIME_MULT	((UnitConvert::STORAGE_UNITCONVERT)0x00100000)	// if you need 'in' instead of 'per'
#define STORAGE_FORMAT_TIME_UNIT_MASK		((UnitConvert::STORAGE_UNITCONVERT)0x000f0000)
#define STORAGE_FORMAT_TIME_MASK		((UnitConvert::STORAGE_UNITCONVERT)0x001f0000)
#define STORAGE_TIME_END			((UnitConvert::STORAGE_UNITCONVERT)0x001f0000)

#define STORAGE_DISTANCE_START			((UnitConvert::STORAGE_UNITCONVERT)0x00000001)
	#define STORAGE_FORMAT_MM		((UnitConvert::STORAGE_UNITCONVERT)0x00000001)
	#define STORAGE_FORMAT_CM		((UnitConvert::STORAGE_UNITCONVERT)0x00000002)
	#define STORAGE_FORMAT_M		((UnitConvert::STORAGE_UNITCONVERT)0x00000003)
	#define STORAGE_FORMAT_KM		((UnitConvert::STORAGE_UNITCONVERT)0x00000004)
	#define STORAGE_FORMAT_INCH		((UnitConvert::STORAGE_UNITCONVERT)0x00000005)
	#define STORAGE_FORMAT_FOOT		((UnitConvert::STORAGE_UNITCONVERT)0x00000006)
	#define STORAGE_FORMAT_YARD		((UnitConvert::STORAGE_UNITCONVERT)0x00000007)
	#define STORAGE_FORMAT_CHAIN		((UnitConvert::STORAGE_UNITCONVERT)0x00000008)
	#define STORAGE_FORMAT_MILE		((UnitConvert::STORAGE_UNITCONVERT)0x00000009)
	#define STORAGE_FORMAT_NAUTICAL_MILE	((UnitConvert::STORAGE_UNITCONVERT)0x0000000a)
	#define STORAGE_FORMAT_NAUTICAL_MILE_UK	((UnitConvert::STORAGE_UNITCONVERT)0x0000000b)
#define STORAGE_DISTANCE_END			((UnitConvert::STORAGE_UNITCONVERT)0x0000000b)

#define STORAGE_AREA_START			((UnitConvert::STORAGE_UNITCONVERT)0x00000100)
	#define STORAGE_FORMAT_MM2		((UnitConvert::STORAGE_UNITCONVERT)0x00000100)
	#define STORAGE_FORMAT_CM2		((UnitConvert::STORAGE_UNITCONVERT)0x00000101)
	#define STORAGE_FORMAT_M2		((UnitConvert::STORAGE_UNITCONVERT)0x00000102)
	#define STORAGE_FORMAT_HECTARE		((UnitConvert::STORAGE_UNITCONVERT)0x00000103)
	#define STORAGE_FORMAT_KM2		((UnitConvert::STORAGE_UNITCONVERT)0x00000104)
	#define STORAGE_FORMAT_IN2		((UnitConvert::STORAGE_UNITCONVERT)0x00000105)
	#define STORAGE_FORMAT_FT2		((UnitConvert::STORAGE_UNITCONVERT)0x00000106)
	#define STORAGE_FORMAT_YD2		((UnitConvert::STORAGE_UNITCONVERT)0x00000107)
	#define STORAGE_FORMAT_ACRE		((UnitConvert::STORAGE_UNITCONVERT)0x00000108)
	#define STORAGE_FORMAT_MILE2		((UnitConvert::STORAGE_UNITCONVERT)0x00000109)
#define STORAGE_AREA_END			((UnitConvert::STORAGE_UNITCONVERT)0x00000109)

#define STORAGE_VOLUME_START			((UnitConvert::STORAGE_UNITCONVERT)0x00000200)
	#define STORAGE_FORMAT_MM3		((UnitConvert::STORAGE_UNITCONVERT)0x00000200)
	#define STORAGE_FORMAT_CM3		((UnitConvert::STORAGE_UNITCONVERT)0x00000201)
	#define STORAGE_FORMAT_LITRE		((UnitConvert::STORAGE_UNITCONVERT)0x00000202)
	#define STORAGE_FORMAT_M3		((UnitConvert::STORAGE_UNITCONVERT)0x00000203)
	#define STORAGE_FORMAT_KM3		((UnitConvert::STORAGE_UNITCONVERT)0x00000204)
	#define STORAGE_FORMAT_IN3		((UnitConvert::STORAGE_UNITCONVERT)0x00000205)
	#define STORAGE_FORMAT_FT3		((UnitConvert::STORAGE_UNITCONVERT)0x00000206)
	#define STORAGE_FORMAT_YD3		((UnitConvert::STORAGE_UNITCONVERT)0x00000207)
	#define STORAGE_FORMAT_MILE3		((UnitConvert::STORAGE_UNITCONVERT)0x00000208)
	#define STORAGE_FORMAT_UK_FL_OZ		((UnitConvert::STORAGE_UNITCONVERT)0x00000209)
	#define STORAGE_FORMAT_UK_PINT		((UnitConvert::STORAGE_UNITCONVERT)0x0000020a)
	#define STORAGE_FORMAT_UK_QUART		((UnitConvert::STORAGE_UNITCONVERT)0x0000020b)
	#define STORAGE_FORMAT_UK_GALLON	((UnitConvert::STORAGE_UNITCONVERT)0x0000020c)
	#define STORAGE_FORMAT_BUSHEL		((UnitConvert::STORAGE_UNITCONVERT)0x0000020d)
	#define STORAGE_FORMAT_US_DRAM		((UnitConvert::STORAGE_UNITCONVERT)0x0000020e)
	#define STORAGE_FORMAT_US_FL_OZ		((UnitConvert::STORAGE_UNITCONVERT)0x0000020f)
	#define STORAGE_FORMAT_US_FL_PINT	((UnitConvert::STORAGE_UNITCONVERT)0x00000210)
	#define STORAGE_FORMAT_US_FL_QUART	((UnitConvert::STORAGE_UNITCONVERT)0x00000211)
	#define STORAGE_FORMAT_US_GALLON	((UnitConvert::STORAGE_UNITCONVERT)0x00000212)
	#define STORAGE_FORMAT_US_FL_BARREL	((UnitConvert::STORAGE_UNITCONVERT)0x00000213)
	#define STORAGE_FORMAT_US_DRY_PINT	((UnitConvert::STORAGE_UNITCONVERT)0x00000214)
	#define STORAGE_FORMAT_US_DRY_QUART	((UnitConvert::STORAGE_UNITCONVERT)0x00000215)
	#define STORAGE_FORMAT_US_DRY_BARREL	((UnitConvert::STORAGE_UNITCONVERT)0x00000216)
#define STORAGE_VOLUME_END			((UnitConvert::STORAGE_UNITCONVERT)0x00000216)

#define STORAGE_TEMP_START			((UnitConvert::STORAGE_UNITCONVERT)0x00000400)
	#define STORAGE_FORMAT_KELVIN		((UnitConvert::STORAGE_UNITCONVERT)0x00000400)
	#define STORAGE_FORMAT_CELSIUS		((UnitConvert::STORAGE_UNITCONVERT)0x00000401)
	#define STORAGE_FORMAT_FAHRENHEIT	((UnitConvert::STORAGE_UNITCONVERT)0x00000402)
	#define STORAGE_FORMAT_RANKINE		((UnitConvert::STORAGE_UNITCONVERT)0x00000403)
#define STORAGE_TEMP_END			((UnitConvert::STORAGE_UNITCONVERT)0x00000403)

#define STORAGE_PRESSURE_START			((UnitConvert::STORAGE_UNITCONVERT)0x00000500)
	#define STORAGE_FORMAT_KPA		((UnitConvert::STORAGE_UNITCONVERT)0x00000500)
	#define STORAGE_FORMAT_PSI		((UnitConvert::STORAGE_UNITCONVERT)0x00000501)
	#define STORAGE_FORMAT_BAR		((UnitConvert::STORAGE_UNITCONVERT)0x00000502)
	#define STORAGE_FORMAT_ATM		((UnitConvert::STORAGE_UNITCONVERT)0x00000503)
	#define STORAGE_FORMAT_TORR		((UnitConvert::STORAGE_UNITCONVERT)0x00000504)
#define STORAGE_PRESSURE_END			((UnitConvert::STORAGE_UNITCONVERT)0x00000504)

#define STORAGE_MASS_START			((UnitConvert::STORAGE_UNITCONVERT)0x00000600)
	#define STORAGE_FORMAT_MILLIGRAM	((UnitConvert::STORAGE_UNITCONVERT)0x00000600)
	#define STORAGE_FORMAT_GRAM		((UnitConvert::STORAGE_UNITCONVERT)0x00000601)
	#define STORAGE_FORMAT_KG		((UnitConvert::STORAGE_UNITCONVERT)0x00000602)
	#define STORAGE_FORMAT_TONNE		((UnitConvert::STORAGE_UNITCONVERT)0x00000603)
	#define STORAGE_FORMAT_OUNCE		((UnitConvert::STORAGE_UNITCONVERT)0x00000604)
	#define STORAGE_FORMAT_LB		((UnitConvert::STORAGE_UNITCONVERT)0x00000605)
	#define STORAGE_FORMAT_SHORT_TON	((UnitConvert::STORAGE_UNITCONVERT)0x00000606)
	#define STORAGE_FORMAT_TON		((UnitConvert::STORAGE_UNITCONVERT)0x00000607)
#define STORAGE_MASS_END			((UnitConvert::STORAGE_UNITCONVERT)0x00000607)

#define STORAGE_ENERGY_START			((UnitConvert::STORAGE_UNITCONVERT)0x00000700)
	#define STORAGE_FORMAT_JOULE		((UnitConvert::STORAGE_UNITCONVERT)0x00000700)
	#define STORAGE_FORMAT_KILOJOULE	((UnitConvert::STORAGE_UNITCONVERT)0x0000070a)
	#define STORAGE_FORMAT_ELECTRONVOLT	((UnitConvert::STORAGE_UNITCONVERT)0x00000701)
	#define STORAGE_FORMAT_ERG		((UnitConvert::STORAGE_UNITCONVERT)0x00000702)
constexpr UnitConvert::STORAGE_UNITCONVERT STORAGE_FORMAT_ERG_PER_SECOND = ((UnitConvert::STORAGE_UNITCONVERT)(STORAGE_FORMAT_ERG | STORAGE_FORMAT_SECOND | STORAGE_FORMAT_TIME_DIV));
	#define STORAGE_FORMAT_FT_LB		((UnitConvert::STORAGE_UNITCONVERT)0x00000703)
	#define STORAGE_FORMAT_CALORIE		((UnitConvert::STORAGE_UNITCONVERT)0x00000704)
	#define STORAGE_FORMAT_KG_METRE		((UnitConvert::STORAGE_UNITCONVERT)0x00000705)
	#define STORAGE_FORMAT_BTU		((UnitConvert::STORAGE_UNITCONVERT)0x00000706)
constexpr UnitConvert::STORAGE_UNITCONVERT STORAGE_FORMAT_BTU_PER_SECOND = ((UnitConvert::STORAGE_UNITCONVERT)(STORAGE_FORMAT_BTU | STORAGE_FORMAT_SECOND | STORAGE_FORMAT_TIME_DIV));
constexpr UnitConvert::STORAGE_UNITCONVERT STORAGE_FORMAT_BTU_PER_HOUR = ((UnitConvert::STORAGE_UNITCONVERT)(STORAGE_FORMAT_BTU | STORAGE_FORMAT_HOUR | STORAGE_FORMAT_TIME_DIV));
	#define STORAGE_FORMAT_WATT_		((UnitConvert::STORAGE_UNITCONVERT)0x00000707)
	#define STORAGE_FORMAT_WATT_SECOND	((UnitConvert::STORAGE_UNITCONVERT)(0x00000707 | STORAGE_FORMAT_SECOND | STORAGE_FORMAT_TIME_MULT))	// this assumes hour in the conversion
	#define STORAGE_FORMAT_WATT_HOUR	((UnitConvert::STORAGE_UNITCONVERT)(0x00000707 | STORAGE_FORMAT_HOUR | STORAGE_FORMAT_TIME_MULT))	// this assumes hour in the conversion
	#define STORAGE_FORMAT_KILOWATT_	((UnitConvert::STORAGE_UNITCONVERT)0x00000708)
	#define STORAGE_FORMAT_KILOWATT_SECOND	((UnitConvert::STORAGE_UNITCONVERT)(0x00000708 | STORAGE_FORMAT_SECOND | STORAGE_FORMAT_TIME_MULT))
	#define STORAGE_FORMAT_KILOWATT_HOUR	((UnitConvert::STORAGE_UNITCONVERT)(0x00000708 | STORAGE_FORMAT_HOUR | STORAGE_FORMAT_TIME_MULT))	// this assumes hour in the conversion
constexpr UnitConvert::STORAGE_UNITCONVERT STORAGE_FORMAT_MEGAWATT = ((UnitConvert::STORAGE_UNITCONVERT)0x0000070b);
constexpr UnitConvert::STORAGE_UNITCONVERT STORAGE_FORMAT_MEGAWATT_SECOND = ((UnitConvert::STORAGE_UNITCONVERT)(STORAGE_FORMAT_MEGAWATT | STORAGE_FORMAT_SECOND | STORAGE_FORMAT_TIME_MULT));
constexpr UnitConvert::STORAGE_UNITCONVERT STORAGE_FORMAT_MEGAWATT_HOUR = ((UnitConvert::STORAGE_UNITCONVERT)(STORAGE_FORMAT_MEGAWATT | STORAGE_FORMAT_HOUR | STORAGE_FORMAT_TIME_MULT));
	#define STORAGE_FORMAT_THERM		((UnitConvert::STORAGE_UNITCONVERT)0x00000709)
#define STORAGE_ENERGY_END			((UnitConvert::STORAGE_UNITCONVERT)0x0000070b)

#define STORAGE_PERCENT_START			((UnitConvert::STORAGE_UNITCONVERT)0x000004c0)
	#define STORAGE_FORMAT_DECIMAL		((UnitConvert::STORAGE_UNITCONVERT)0x000004c0)
	#define STORAGE_FORMAT_PERCENT		((UnitConvert::STORAGE_UNITCONVERT)0x000004c1)
	#define STORAGE_FORMAT_DECIMAL_INVERT	((UnitConvert::STORAGE_UNITCONVERT)0x000004c2)	// used if you store it as 15% but want it displayed at 85%
	#define STORAGE_FORMAT_PERCENT_INVERT	((UnitConvert::STORAGE_UNITCONVERT)0x000004c3)
#define STORAGE_PERCENT_END			((UnitConvert::STORAGE_UNITCONVERT)0x000004c3)

//#define STORAGE_ANGLE_START			((std::uint32_t)0x4b)
	#define STORAGE_FORMAT_ANGLE		((UnitConvert::STORAGE_UNITCONVERT)0x000004b0)	// need this or one of the other 4x values (only this one is presently supported)
	#define STORAGE_FORMAT_RADIAN		((UnitConvert::STORAGE_UNITCONVERT)0x00000000)// default
	#define STORAGE_FORMAT_CARTESIAN	((UnitConvert::STORAGE_UNITCONVERT)0x00000000)// default
	#define STORAGE_FORMAT_COMPASS		((UnitConvert::STORAGE_UNITCONVERT)0x01000000)// can be OR'd with the other angle values
	#define STORAGE_FORMAT_DEGREE		((UnitConvert::STORAGE_UNITCONVERT)0x02000000)// can be OR'd with the other angle values
	#define STORAGE_FORMAT_ARCSECOND	((UnitConvert::STORAGE_UNITCONVERT)0x04000000)// can be OR'd with the other angle values
	#define STORAGE_FORMAT_ANGLE_ROTATION_MASK ((UnitConvert::STORAGE_UNITCONVERT)0x01000000)
	#define STORAGE_FORMAT_ANGLE_UNIT_MASK	((UnitConvert::STORAGE_UNITCONVERT)0x0e000000)
	#define STORAGE_FORMAT_ANGLE_MASK	((UnitConvert::STORAGE_UNITCONVERT)0x0f000000)
//#define STORAGE_ANGLE_END			((std::uint32_t)0x4b)

#define STORAGE_COORDINATE_START			((UnitConvert::STORAGE_UNITCONVERT)0x00000800)
	#define STORAGE_COORDINATE_DEGREE		((UnitConvert::STORAGE_UNITCONVERT)0x00000800)
	#define STORAGE_COORDINATE_DEGREE_MINUTE	((UnitConvert::STORAGE_UNITCONVERT)0x00000801)
	#define STORAGE_COORDINATE_DEGREE_MINUTE_SECOND	((UnitConvert::STORAGE_UNITCONVERT)0x00000802)
	#define STORAGE_COORDINATE_UTM			((UnitConvert::STORAGE_UNITCONVERT)0x00000803)
	#define STORAGE_COORDINATE_RELATIVE_DISTANCE	((UnitConvert::STORAGE_UNITCONVERT)0x00000804)
#define STORAGE_COORDINATE_END				((UnitConvert::STORAGE_UNITCONVERT)0x00000804)

float convertUnit(float value, STORAGE_UNIT to_format, STORAGE_UNIT from_format);
double convertUnit(double value, STORAGE_UNIT to_format, STORAGE_UNIT from_format);
double convertUnit(tstring value, STORAGE_UNIT to_format, STORAGE_UNIT from_format, int* ierr = nullptr);
double convertUnit(tstring value, STORAGE_UNIT to_format, int* ierr = nullptr);
tstring UnitName(STORAGE_UNIT format, bool short_format);
STORAGE_UNIT UnitFormat(STORAGE_UNIT UnitType, const TCHAR *UnitName);
STORAGE_UNITCONVERT UnitFormat(STORAGE_UNITCONVERT UnitType, const TCHAR* UnitName);
STORAGE_UNITCONVERT UnitFormatSearch(STORAGE_UNITCONVERT UnitType, const TCHAR* UnitName, std::uint32_t trial = 0);

HSS_PRAGMA_WARNING_PUSH
HSS_PRAGMA_GCC(GCC diagnostic ignored "-Wunused-variable")
HSS_PRAGMA_CLANG(clang diagnostic ignored "-Wunused-variable")
#define UNIT_TYPE_COUNT 9
	static STORAGE_UNITCONVERT startEnds[UNIT_TYPE_COUNT][2] = { { STORAGE_DISTANCE_START, STORAGE_DISTANCE_END }, { STORAGE_AREA_START, STORAGE_AREA_END },
	                                             { STORAGE_VOLUME_START, STORAGE_VOLUME_END }, { STORAGE_TEMP_START, STORAGE_TEMP_END },
												 { STORAGE_PRESSURE_START, STORAGE_PRESSURE_END }, { STORAGE_MASS_START, STORAGE_MASS_END }, 
												 { STORAGE_ENERGY_START, STORAGE_ENERGY_END }, { STORAGE_PERCENT_START, STORAGE_PERCENT_END },
												 { STORAGE_TIME_START, STORAGE_TIME_END } };

}
HSS_PRAGMA_WARNING_POP

#ifdef _MSC_VER
#if (!defined(__INTEL_COMPILER)) && (!defined(__INTEL_LLVM_COMPILER))
#pragma managed(pop)
#endif
#endif

#endif //__HSS_CONVERT_H__
