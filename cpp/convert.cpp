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

#include "intel_check.h"
#include <cstring>
#include "types.h"
#include "convert.h"
#include "ConversionFactors.h"
#include "stdchar.h"


#define PI_VALUE 3.14159265358979323846264
#define ONE_OVER_PI_VALUE 0.318309886183790671537768
#define TWO_PI_VALUE 6.28318530717958647692529
#define TWO_HALF_PI_VALUE 7.85398163397448309615661

double DEGREE_TO_RADIAN(const double X) {
	return (X / 180.0) * PI_VALUE;
}

double RADIAN_TO_DEGREE(const double X) {
	return (X * 180.0) * ONE_OVER_PI_VALUE;
}

double ANGLE_CALCULATE_BASE(const double base, const double number) {	
	if (number >= 0.0) {
		if (number < base)
			return number;
		return fmod(number, base);
	}
	return fmod(number, base) + base;
}

double NORMALIZE_ANGLE_RADIAN(const double X) {
	return ANGLE_CALCULATE_BASE(TWO_PI_VALUE, X);
}

double CARTESIAN_TO_COMPASS_RADIAN(double X) {
	double XX = TWO_HALF_PI_VALUE - X;
	return NORMALIZE_ANGLE_RADIAN(XX);
}

double COMPASS_TO_CARTESIAN_RADIAN(const double X) {
	return CARTESIAN_TO_COMPASS_RADIAN(X);
}


float UnitConvert::convertUnit(float value, STORAGE_UNIT to_format, STORAGE_UNIT from_format)
{
	return (float)convertUnit((double)value, to_format, from_format);
}

double UnitConvert::convertUnit(double value, STORAGE_UNIT to_format, STORAGE_UNIT from_format)
{
	if (from_format == to_format)
		return value;
	
	if ((from_format & 0xffffffff00000000) || (to_format & 0xffffffff00000000))
	{
		double val = convertUnit(value, (to_format >> 0x20) & 0x00000000ffffffff, (from_format >> 0x20) & 0x00000000ffffffff);
		val = convertUnit(val, from_format & 0x00000000ffffffff, to_format & 0x00000000ffffffff);
		return val;
	}
	from_format &= 0xffffffff;
	to_format &= 0xffffffff;

	if (from_format && to_format)
	{
/*
		switch (from_format) {
			case STORAGE_FORMAT_WATT_HOUR:		from_format = STORAGE_FORMAT_JOULE | STORAGE_FORMAT_HOUR | STORAGE_FORMAT_TIME_MULT; break;
			case STORAGE_FORMAT_KILOWATT_HOUR:	from_format = STORAGE_FORMAT_KILOJOULE | STORAGE_FORMAT_HOUR | STORAGE_FORMAT_TIME_MULT; break;
			default:				break;
		}
		switch (to_format) {
			case STORAGE_FORMAT_WATT_HOUR:		to_format = STORAGE_FORMAT_JOULE | STORAGE_FORMAT_HOUR | STORAGE_FORMAT_TIME_MULT; break;
			case STORAGE_FORMAT_KILOWATT_HOUR:	to_format = STORAGE_FORMAT_KILOJOULE | STORAGE_FORMAT_HOUR | STORAGE_FORMAT_TIME_MULT; break;
			default:				break;
		}
*/
		if (((from_format & STORAGE_FORMAT_TIME_MASK) && (from_format & (~STORAGE_FORMAT_TIME_MASK))) || ((to_format & STORAGE_FORMAT_TIME_MASK) && (to_format & (~STORAGE_FORMAT_TIME_MASK))))
		{
			if ((from_format & STORAGE_FORMAT_TIME_MASK) != (to_format & STORAGE_FORMAT_TIME_MASK))
			{
				if (from_format & STORAGE_FORMAT_TIME_MULT) {
					switch (from_format & STORAGE_FORMAT_TIME_UNIT_MASK)
					{
						case STORAGE_FORMAT_MICROSECOND:
							value /= 1000.0;
							break;
						case STORAGE_FORMAT_MILLISECOND:
							value /= 1000000.0;
							break;
						case STORAGE_FORMAT_MINUTE:
							value *= 60.0;
							break;
						case STORAGE_FORMAT_HOUR:
							value *= (60.0 * 60.0);
							break;
						case STORAGE_FORMAT_DAY:
							value *= (24.0 * 60.0 * 60.0);
							break;
						case STORAGE_FORMAT_WEEK:
							value *= 604800.0;
							break;
						case STORAGE_FORMAT_MONTH:
							value *= 2629743.83;
							break;
						case STORAGE_FORMAT_YEAR:
							value *= 31556926.0;
							break;
						case STORAGE_FORMAT_DECADE:
							value *= 315569260.0;
							break;
						case STORAGE_FORMAT_CENTURY:
							value *= 3155692600.0;
							break;
					}
				} else {
					switch (from_format & STORAGE_FORMAT_TIME_UNIT_MASK)
					{
						case STORAGE_FORMAT_MICROSECOND:
							value *= 1000.0;
							break;
						case STORAGE_FORMAT_MILLISECOND:
							value *= 1000000.0;
							break;
						case STORAGE_FORMAT_MINUTE:
							value /= 60.0;
							break;
						case STORAGE_FORMAT_HOUR:
							value /= (60.0 * 60.0);
							break;
						case STORAGE_FORMAT_DAY:
							value /= (24.0 * 60.0 * 60.0);
							break;
						case STORAGE_FORMAT_WEEK:
							value /= 604800.0;
							break;
						case STORAGE_FORMAT_MONTH:
							value /= 2629743.83;
							break;
						case STORAGE_FORMAT_YEAR:
							value /= 31556926.0;
							break;
						case STORAGE_FORMAT_DECADE:
							value /= 315569260.0;
							break;
						case STORAGE_FORMAT_CENTURY:
							value /= 3155692600.0;
							break;
					}
				}
				if (to_format & STORAGE_FORMAT_TIME_MULT) {
					switch (to_format & STORAGE_FORMAT_TIME_UNIT_MASK)
					{
						case STORAGE_FORMAT_MICROSECOND:
							value *= 1000.0;
							break;
						case STORAGE_FORMAT_MILLISECOND:
							value *= 1000000.0;
							break;
						case STORAGE_FORMAT_MINUTE:
							value /= 60.0;
							break;
						case STORAGE_FORMAT_HOUR:
							value /= (60.0 * 60.0);
							break;
						case STORAGE_FORMAT_DAY:
							value /= (24.0 * 60.0 * 60.0);
							break;
						case STORAGE_FORMAT_WEEK:
							value /= 604800.0;
							break;
						case STORAGE_FORMAT_MONTH:
							value /= 2629743.83;
							break;
						case STORAGE_FORMAT_YEAR:
							value /= 31556926.0;
							break;
						case STORAGE_FORMAT_DECADE:
							value /= 315569260.0;
							break;
						case STORAGE_FORMAT_CENTURY:
							value /= 3155692600.0;
							break;
					}
				} else {
					switch (to_format & STORAGE_FORMAT_TIME_UNIT_MASK)
					{
						case STORAGE_FORMAT_MICROSECOND:
							value /= 1000.0;
							break;
						case STORAGE_FORMAT_MILLISECOND:
							value /= 1000000.0;
							break;
						case STORAGE_FORMAT_MINUTE:
							value *= 60.0;
							break;
						case STORAGE_FORMAT_HOUR:
							value *= (60.0 * 60.0);
							break;
						case STORAGE_FORMAT_DAY:
							value *= (24.0 * 60.0 * 60.0);
							break;
						case STORAGE_FORMAT_WEEK:
							value *= 604800.0;
							break;
						case STORAGE_FORMAT_MONTH:
							value *= 2629743.83;
							break;
						case STORAGE_FORMAT_YEAR:
							value *= 31556926.0;
							break;
						case STORAGE_FORMAT_DECADE:
							value *= 315569260.0;
							break;
						case STORAGE_FORMAT_CENTURY:
							value *= 3155692600.0;
							break;
					}
				}
			}
			from_format &= (~STORAGE_FORMAT_TIME_MASK);			// done dealing with velocity changes, so do the rest as per usual
			to_format &= (~STORAGE_FORMAT_TIME_MASK);
		}
		if ((from_format >= STORAGE_DISTANCE_START) && (from_format <= STORAGE_DISTANCE_END) &&
		    (to_format >= STORAGE_DISTANCE_START) && (to_format <= STORAGE_DISTANCE_END))
			// first, convert it to m, then to what we want;
			return value * distance_translation[from_format - STORAGE_DISTANCE_START] /
			    distance_translation[to_format - STORAGE_DISTANCE_START];

		else if ((from_format >= STORAGE_TEMP_START) && (from_format <= STORAGE_TEMP_END) &&
		    (to_format >= STORAGE_TEMP_START) && (to_format <= STORAGE_TEMP_END))
		{
			// first, convert to kelvin, then to what we want
			double kelvin = (value + temp_translate1[from_format - STORAGE_TEMP_START]) / temp_translate2[from_format - STORAGE_TEMP_START];
			return kelvin * temp_translate2[to_format - STORAGE_TEMP_START] - temp_translate1[to_format - STORAGE_TEMP_START];
		}

		else if ((from_format >= STORAGE_VOLUME_START) && (from_format <= STORAGE_VOLUME_END) &&
		    (to_format >= STORAGE_VOLUME_START) && (to_format <= STORAGE_VOLUME_END))
			// first, convert it to m3, then convert it to what we want
			return value * volume_translation[from_format - STORAGE_VOLUME_START] /
			    volume_translation[to_format - STORAGE_VOLUME_START];

		else if ((from_format >= STORAGE_AREA_START) && (from_format <= STORAGE_AREA_END) &&
		    (to_format >= STORAGE_AREA_START) && (to_format <= STORAGE_AREA_END))
			// first, convert it to m3, then convert it to what we want
			return value * area_translation[from_format - STORAGE_AREA_START] /
			    area_translation[to_format - STORAGE_AREA_START];

		else if ((from_format >= STORAGE_PERCENT_START) && (from_format <= STORAGE_PERCENT_END) &&
			(to_format >= STORAGE_PERCENT_START) && (to_format <= STORAGE_PERCENT_END))
		{
			if (from_format != to_format)
			{
				switch (from_format)
				{
					case STORAGE_FORMAT_PERCENT:
						value *= 0.01;
						break;
					case STORAGE_FORMAT_PERCENT_INVERT:
						value = (100.0 - value) * 0.01;
						break;
					case STORAGE_FORMAT_DECIMAL_INVERT:
						value = 1.0 - value;
						break;
				}
				switch (to_format)
				{
					case STORAGE_FORMAT_PERCENT:
						value *= 100.0;
						break;
					case STORAGE_FORMAT_PERCENT_INVERT:
						value = (1.0 - value) * 100.0;
						break;
					case STORAGE_FORMAT_DECIMAL_INVERT:
						value = 1.0 - value;
						break;
				}
			}
			return value;
		}

		else if ((from_format >= STORAGE_MASS_START) && (from_format <= STORAGE_MASS_END) &&
		    (to_format >= STORAGE_MASS_START) && (to_format <= STORAGE_MASS_END))
			// first, convert it to kg, then convert it to what we want
			return value * mass_translation[from_format - STORAGE_MASS_START] /
			    mass_translation[to_format - STORAGE_MASS_START];

		else if ((from_format >= STORAGE_ENERGY_START) && (from_format <= STORAGE_ENERGY_END) &&
		    (to_format >= STORAGE_ENERGY_START) && (to_format <= STORAGE_ENERGY_END))
			return value * energy_translation[from_format - STORAGE_ENERGY_START] /
			    energy_translation[to_format - STORAGE_ENERGY_START];

		else if ((from_format >= STORAGE_PRESSURE_START) && (from_format <= STORAGE_PRESSURE_END) &&
			(to_format >= STORAGE_PRESSURE_START) && (to_format <= STORAGE_PRESSURE_END))
		{
			return value * pressure_translation[from_format - STORAGE_PRESSURE_START] /
				pressure_translation[to_format - STORAGE_PRESSURE_START];
		}

		else if (((from_format & (~STORAGE_FORMAT_ANGLE_MASK)) == STORAGE_FORMAT_ANGLE) && ((to_format & (~STORAGE_FORMAT_ANGLE_MASK)) == STORAGE_FORMAT_ANGLE))
		{
			if ((from_format & STORAGE_FORMAT_ANGLE_MASK) != (to_format & STORAGE_FORMAT_ANGLE_MASK))
			{
				if ((from_format & STORAGE_FORMAT_ANGLE_UNIT_MASK) == STORAGE_FORMAT_DEGREE)
					value = DEGREE_TO_RADIAN(value);//deg2rad(value);
				else if ((from_format & STORAGE_FORMAT_ANGLE_UNIT_MASK) == STORAGE_FORMAT_ARCSECOND)
					value = DEGREE_TO_RADIAN(value / 3600.0);//deg2rad(value / 3600.0);
				if ((from_format & STORAGE_FORMAT_ANGLE_ROTATION_MASK) == STORAGE_FORMAT_COMPASS)
					value = COMPASS_TO_CARTESIAN_RADIAN(value);//comp2cartDeg(value);
				if ((to_format & STORAGE_FORMAT_ANGLE_ROTATION_MASK) == STORAGE_FORMAT_COMPASS)
					value = CARTESIAN_TO_COMPASS_RADIAN(value);//cart2compDeg(value);
				if ((to_format & STORAGE_FORMAT_ANGLE_UNIT_MASK) == STORAGE_FORMAT_DEGREE)
					value = RADIAN_TO_DEGREE(value);//rad2deg(value);
				else if ((to_format & STORAGE_FORMAT_ANGLE_UNIT_MASK) == STORAGE_FORMAT_ARCSECOND)
					value = RADIAN_TO_DEGREE(value) * 3600.0;//rad2deg(value) * 3600.0;
			}
			return value;
		}
		else if ((from_format >= STORAGE_TIME_START) && (from_format <= STORAGE_TIME_END) && (to_format >= STORAGE_TIME_START) && (to_format <= STORAGE_TIME_END))
		{
			if (from_format & STORAGE_FORMAT_TIME_MULT) {
				switch (from_format & STORAGE_FORMAT_TIME_UNIT_MASK)
				{
					case STORAGE_FORMAT_MICROSECOND:
						value /= 1000.0;
						break;
					case STORAGE_FORMAT_MILLISECOND:
						value /= 1000000.0;
						break;
					case STORAGE_FORMAT_MINUTE:
						value *= 60.0;
						break;
					case STORAGE_FORMAT_HOUR:
						value *= (60.0 * 60.0);
						break;
					case STORAGE_FORMAT_DAY:
						value *= (24.0 * 60.0 * 60.0);
						break;
					case STORAGE_FORMAT_WEEK:
						value *= 604800.0;
						break;
					case STORAGE_FORMAT_MONTH:
						value *= 2629743.83;
						break;
					case STORAGE_FORMAT_YEAR:
						value *= 31556926.0;
						break;
					case STORAGE_FORMAT_DECADE:
						value *= 315569260.0;
						break;
					case STORAGE_FORMAT_CENTURY:
						value *= 3155692600.0;
						break;
				}
			} else {
				switch (from_format & STORAGE_FORMAT_TIME_UNIT_MASK)
				{
					case STORAGE_FORMAT_MICROSECOND:
						value /= 1000.0;
						break;
					case STORAGE_FORMAT_MILLISECOND:
						value /= 1000000.0;
						break;
					case STORAGE_FORMAT_MINUTE:
						value *= 60.0;
						break;
					case STORAGE_FORMAT_HOUR:
						value *= (60.0 * 60.0);
						break;
					case STORAGE_FORMAT_DAY:
						value *= (24.0 * 60.0 * 60.0);
						break;
					case STORAGE_FORMAT_WEEK:
						value *= 604800.0;
						break;
					case STORAGE_FORMAT_MONTH:
						value *= 2629743.83;
						break;
					case STORAGE_FORMAT_YEAR:
						value *= 31556926.0;
						break;
					case STORAGE_FORMAT_DECADE:
						value *= 315569260.0;
						break;
					case STORAGE_FORMAT_CENTURY:
						value *= 3155692600.0;
						break;
				}
			}
			if (to_format & STORAGE_FORMAT_TIME_MULT) {
				switch (to_format & STORAGE_FORMAT_TIME_UNIT_MASK)
				{
					case STORAGE_FORMAT_MICROSECOND:
						value *= 1000.0;
						break;
					case STORAGE_FORMAT_MILLISECOND:
						value *= 1000000.0;
						break;
					case STORAGE_FORMAT_MINUTE:
						value /= 60.0;
						break;
					case STORAGE_FORMAT_HOUR:
						value /= (60.0 * 60.0);
						break;
					case STORAGE_FORMAT_DAY:
						value /= (24.0 * 60.0 * 60.0);
						break;
					case STORAGE_FORMAT_WEEK:
						value /= 604800.0;
						break;
					case STORAGE_FORMAT_MONTH:
						value /= 2629743.83;
						break;
					case STORAGE_FORMAT_YEAR:
						value /= 31556926.0;
						break;
					case STORAGE_FORMAT_DECADE:
						value /= 315569260.0;
						break;
					case STORAGE_FORMAT_CENTURY:
						value /= 3155692600.0;
						break;
				}
			} else {
				switch (to_format & STORAGE_FORMAT_TIME_UNIT_MASK)
				{
					case STORAGE_FORMAT_MICROSECOND:
						value *= 1000.0;
						break;
					case STORAGE_FORMAT_MILLISECOND:
						value *= 1000000.0;
						break;
					case STORAGE_FORMAT_MINUTE:
						value /= 60.0;
						break;
					case STORAGE_FORMAT_HOUR:
						value /= (60.0 * 60.0);
						break;
					case STORAGE_FORMAT_DAY:
						value /= (24.0 * 60.0 * 60.0);
						break;
					case STORAGE_FORMAT_WEEK:
						value /= 604800.0;
						break;
					case STORAGE_FORMAT_MONTH:
						value /= 2629743.83;
						break;
					case STORAGE_FORMAT_YEAR:
						value /= 31556926.0;
						break;
					case STORAGE_FORMAT_DECADE:
						value /= 315569260.0;
						break;
					case STORAGE_FORMAT_CENTURY:
						value /= 3155692600.0;
						break;
				}
			}
		}
	}
	return value;					// this didn't make sense being 0, there's no way to tell if that means error or if that was the correct value
}

double UnitConvert::convertUnit(tstring value, STORAGE_UNIT to_format, int* ierr)
{
	return convertUnit(value, to_format, 0ULL, ierr);
}

double UnitConvert::convertUnit(tstring value, STORAGE_UNIT to_format, STORAGE_UNIT from_format, int* ierr)
{
	tstring valueInLowerCase;
	tstring unitsInLowerCase;
	double val;
	if (ierr)
		*ierr = 0;
	bool inValue = true;
	for (size_t i = 0; i < value.length(); i++)
	{
		if (inValue && (value[i] == _T('.') || isdigit(value[i])))
			valueInLowerCase.push_back(value[i]);
		else if (inValue && value[i] != _T(' '))
		{
			inValue = false;
			unitsInLowerCase.push_back((TCHAR)tolower(value[i]));
		}
		else if (!inValue)
			unitsInLowerCase.push_back((TCHAR)tolower(value[i]));
	}
	
	if (from_format > 0)
	{
		tistringstream s(valueInLowerCase);
		if(!(s >> val))
		{
			if (ierr)
				*ierr = 1;
			return 0;
		}
		return convertUnit(val, to_format, from_format);
	}
	from_format = UnitFormat((STORAGE_UNIT)0, unitsInLowerCase.c_str());
	if (from_format > 0)
	{
		tistringstream s(valueInLowerCase);
		if(!(s >> val))
		{
			if (ierr)
				*ierr = 1;
			return 0;
		}
		return convertUnit(val, to_format, from_format);
	}

	if (ierr)
		*ierr = 1;
	return 0;
}


UnitConvert::STORAGE_UNITCONVERT UnitConvert::UnitFormat(STORAGE_UNITCONVERT UnitType, const TCHAR *_UnitName)
{
	if (!_UnitName)
		return 0;
	if (!(_UnitName[0]))
		return 0;
	std::uint32_t l = 0;
	TCHAR UnitNameCopy[40];
	TCHAR *t = nullptr, *UnitName, *Velocity;
	_tcscpy_s(UnitNameCopy, _UnitName);
	UnitName = _tcstok_s(UnitNameCopy, _T("/"), &t);
	Velocity = _tcstok_s(nullptr, _T("/"), &t);
	if (Velocity != nullptr)
	{
		l = UnitFormatSearch(UnitType & (~STORAGE_FORMAT_TIME_MASK), UnitName);
		l |= UnitFormatSearch(UnitType & STORAGE_FORMAT_TIME_MASK, Velocity);
	}
	else
	{
		l = UnitFormatSearch(UnitType, UnitName);
	}
	return l;
}


UnitConvert::STORAGE_UNIT UnitConvert::UnitFormat(STORAGE_UNIT UnitType, const TCHAR *_UnitName)
{
	if (!_UnitName)
		return 0;
	if (!(_UnitName[0]))
		return 0;
	STORAGE_UNIT l = 0;
	TCHAR UnitNameCopy[40];
	TCHAR *t = nullptr, *UnitName, *Velocity;
	_tcscpy_s(UnitNameCopy, _UnitName);
	UnitName = _tcstok_s(UnitNameCopy, _T("/"), &t);
	Velocity = _tcstok_s(nullptr, _T("/"), &t);
	if (Velocity != nullptr)
	{
		if ((UnitType & STORAGE_FORMAT_TIME_MASK) && !(UnitType & 0xffffffff00000000))
		{
			l = UnitFormatSearch((STORAGE_UNITCONVERT)(UnitType & (~STORAGE_FORMAT_TIME_MASK)), UnitName);
			l |= UnitFormatSearch((STORAGE_UNITCONVERT)(UnitType & STORAGE_FORMAT_TIME_MASK), Velocity);
		}
		else
		{
			l = ((STORAGE_UNIT)UnitFormatSearch((STORAGE_UNITCONVERT)((UnitType & 0xffffffff00000000) >> 0x20), UnitName)) << 0x20;
			l |= (STORAGE_UNIT)UnitFormatSearch((STORAGE_UNITCONVERT)(UnitType & 0x00000000ffffffff), Velocity);
		}
	}
	else
	{
		l = UnitFormatSearch((STORAGE_UNITCONVERT)(UnitType & (~STORAGE_FORMAT_TIME_MASK)), UnitName);
	}
	return l;
}


UnitConvert::STORAGE_UNITCONVERT UnitConvert::UnitFormatSearch(STORAGE_UNITCONVERT UnitType, const TCHAR* UnitName, std::uint32_t trial)
{
	STORAGE_UNITCONVERT comp = UnitType;
	STORAGE_UNITCONVERT l = 0;
	if ((comp == STORAGE_DISTANCE_START) || (comp == 0)) { 
		if ((!_tcsicmp(UnitName, _T("mm"))) || (!_tcsicmp(UnitName, _T("millimeter"))) || (!_tcsicmp(UnitName, _T("millimetre"))))
			l = STORAGE_FORMAT_MM;
		else if ((!_tcsicmp(UnitName, _T("cm"))) || (!_tcsicmp(UnitName, _T("centimeter"))) || (!_tcsicmp(UnitName, _T("centimetre"))))
			l = STORAGE_FORMAT_CM;
		else if ((!_tcsicmp(UnitName, _T("m"))) || (!_tcsicmp(UnitName, _T("meter"))) || (!_tcsicmp(UnitName, _T("metre"))))
			l = STORAGE_FORMAT_M;
		else if ((!_tcsicmp(UnitName,  _T("km"))) || (!_tcsicmp(UnitName,  _T("kilometer"))) || (!_tcsicmp(UnitName, _T("kilometre"))))
			l = STORAGE_FORMAT_KM;
		else if ((!_tcsicmp(UnitName,  _T("in"))) || (!_tcsicmp(UnitName,  _T("inch"))))
			l = STORAGE_FORMAT_INCH;
		else if ((!_tcsicmp(UnitName,  _T("ft"))) || (!_tcsicmp(UnitName,  _T("foot"))) || (!_tcsicmp(UnitName, _T("feet"))))
			l = STORAGE_FORMAT_FOOT;
		else if ((!_tcsicmp(UnitName,  _T("yd"))) || (!_tcsicmp(UnitName,  _T("yard"))))
			l = STORAGE_FORMAT_YARD;
		else if (!_tcsicmp(UnitName,  _T("chain")))
			l = STORAGE_FORMAT_CHAIN;
		else if ((!_tcsicmp(UnitName,  _T("mi"))) || (!_tcsicmp(UnitName,  _T("mile"))))
			l = STORAGE_FORMAT_MILE;
		else if ((!_tcsicmp(UnitName,  _T("n.mi"))) || (!_tcsicmp(UnitName,  _T("nautical mile"))))
			l = STORAGE_FORMAT_NAUTICAL_MILE;
		else if ((!_tcsicmp(UnitName,  _T("knot"))))
			l = STORAGE_FORMAT_NAUTICAL_MILE | STORAGE_FORMAT_HOUR;
	}

	if (((comp == STORAGE_AREA_START) || (comp == 0)) && (!l)) {
		if ((!_tcsicmp(UnitName, _T("mm" POWER2_SYMBOL))) || (!_tcsicmp(UnitName, _T("millimeter squared"))) || (!_tcsicmp(UnitName, _T("millimetre squared"))) || (!_tcsicmp(UnitName, _T("square millimeter"))) || (!_tcsicmp(UnitName, _T("square millimetre"))))
			l = STORAGE_FORMAT_MM2;
		else if ((!_tcsicmp(UnitName, _T("cm" POWER2_SYMBOL))) || (!_tcsicmp(UnitName, _T("centimeter squared"))) || (!_tcsicmp(UnitName, _T("centimetre squared"))) || (!_tcsicmp(UnitName, _T("square centimeter"))) || (!_tcsicmp(UnitName, _T("square centimetre"))))
			l = STORAGE_FORMAT_CM2;
		else if ((!_tcsicmp(UnitName, _T("m" POWER2_SYMBOL))) || (!_tcsicmp(UnitName, _T("square meter"))) || (!_tcsicmp(UnitName, _T("square metre"))) || (!_tcsicmp(UnitName, _T("meter squared"))) || (!_tcsicmp(UnitName, _T("metre squared"))))
			l = STORAGE_FORMAT_M2;
		else if ((!_tcsicmp(UnitName, _T("ha"))) || (!_tcsicmp(UnitName, _T("hectare"))))
			l = STORAGE_FORMAT_CM2;
		else if ((!_tcsicmp(UnitName, _T("km" POWER2_SYMBOL))) || (!_tcsicmp(UnitName, _T("square kilometer"))) || (!_tcsicmp(UnitName, _T("square kilometre"))) || (!_tcsicmp(UnitName, _T("kilometer squared"))) || (!_tcsicmp(UnitName, _T("kilometre squared"))))
			l = STORAGE_FORMAT_KM2;
		else if ((!_tcsicmp(UnitName, _T("in" POWER2_SYMBOL))) || (!_tcsicmp(UnitName, _T("square inch"))) || (!_tcsicmp(UnitName, _T("inch squared"))))
			l = STORAGE_FORMAT_IN2;
		else if ((!_tcsicmp(UnitName, _T("ft" POWER2_SYMBOL))) || (!_tcsicmp(UnitName, _T("square foot"))) || (!_tcsicmp(UnitName, _T("square feet"))) || (!_tcsicmp(UnitName, _T("foot squared"))) || (!_tcsicmp(UnitName, _T("feet squared"))))
			l = STORAGE_FORMAT_FT2;
		else if ((!_tcsicmp(UnitName, _T("yd" POWER2_SYMBOL))) || (!_tcsicmp(UnitName, _T("square yard"))) || (!_tcsicmp(UnitName, _T("yard squared"))))
			l = STORAGE_FORMAT_YD2;
		else if (!_tcsicmp(UnitName, _T("acre")))
			l = STORAGE_FORMAT_ACRE;
		else if ((!_tcsicmp(UnitName, _T("mi" POWER2_SYMBOL))) || (!_tcsicmp(UnitName, _T("square mile"))) || (!_tcsicmp(UnitName, _T("mile squared"))))
			l = STORAGE_FORMAT_MILE2;
	}
	if (((comp == STORAGE_VOLUME_START) || (comp == 0)) && (!l)) {
		if ((!_tcsicmp(UnitName, _T("mm" POWER3_SYMBOL))) || (!_tcsicmp(UnitName, _T("cubic millimeter"))) || (!_tcsicmp(UnitName, _T("cubic millimetre"))) || (!_tcsicmp(UnitName, _T("millimeter cubed"))) || (!_tcsicmp(UnitName, _T("millimetre cubed"))))
			l = STORAGE_FORMAT_MM3;
		else if ((!_tcsicmp(UnitName, _T("cm" POWER3_SYMBOL))) || (!_tcsicmp(UnitName, _T("cubic centimeter"))) || (!_tcsicmp(UnitName, _T("cubic centimetre"))) || (!_tcsicmp(UnitName, _T("centimeter cubed"))) || (!_tcsicmp(UnitName, _T("centimetre cubed"))))
			l = STORAGE_FORMAT_CM3;
		else if ((!_tcsicmp(UnitName, _T("l"))) || (!_tcsicmp(UnitName, _T("litre"))))
			l = STORAGE_FORMAT_LITRE;
		else if ((!_tcsicmp(UnitName, _T("m" POWER3_SYMBOL))) || (!_tcsicmp(UnitName, _T("cubic meter"))) || (!_tcsicmp(UnitName, _T("cubic metre"))) || (!_tcsicmp(UnitName, _T("meter cubed"))) || (!_tcsicmp(UnitName, _T("metre cubed"))))
			l = STORAGE_FORMAT_M3;
		else if ((!_tcsicmp(UnitName, _T("km" POWER3_SYMBOL))) || (!_tcsicmp(UnitName, _T("cubic kilometer"))) || (!_tcsicmp(UnitName, _T("cubic kilometre"))) || (!_tcsicmp(UnitName, _T("kilometer cubed"))) || (!_tcsicmp(UnitName, _T("kilometre cubed"))))
			l = STORAGE_FORMAT_KM3;
		else if ((!_tcsicmp(UnitName, _T("in" POWER3_SYMBOL))) || (!_tcsicmp(UnitName, _T("cubic inch"))) || (!_tcsicmp(UnitName, _T("inch cubed"))))
			l = STORAGE_FORMAT_IN3;
		else if ((!_tcsicmp(UnitName, _T("ft" POWER3_SYMBOL))) || (!_tcsicmp(UnitName, _T("cubic foot"))) || (!_tcsicmp(UnitName, _T("cubic feet"))))
			l = STORAGE_FORMAT_FT3;
		else if ((!_tcsicmp(UnitName, _T("yd" POWER3_SYMBOL))) || (!_tcsicmp(UnitName, _T("cubic yard"))) || (!_tcsicmp(UnitName, _T("yard cubed"))))
			l = STORAGE_FORMAT_YD3;
		else if ((!_tcsicmp(UnitName, _T("mi" POWER3_SYMBOL))) || (!_tcsicmp(UnitName, _T("cubic mile"))) || (!_tcsicmp(UnitName, _T("mile cubed"))))
			l = STORAGE_FORMAT_MILE3;
		else if ((!_tcsicmp(UnitName, _T("fluid ounce (UK)"))))
			l = STORAGE_FORMAT_UK_FL_OZ;
		else if ((!_tcsicmp(UnitName, _T("oz"))) || (!_tcsicmp(UnitName, _T("fluid ounce (US)"))))
			l = STORAGE_FORMAT_US_FL_OZ;
		else if ((!_tcsicmp(UnitName, _T("pint (UK)"))))
			l = STORAGE_FORMAT_UK_PINT;
		else if ((!_tcsicmp(UnitName, _T("pt"))) || (!_tcsicmp(UnitName, _T("pint (US, fluid)"))))
			l = STORAGE_FORMAT_US_FL_PINT;
		else if ((!_tcsicmp(UnitName, _T("pint (US, dry)"))))
			l = STORAGE_FORMAT_US_DRY_PINT;
		else if ((!_tcsicmp(UnitName, _T("quart (UK)"))))
			l = STORAGE_FORMAT_UK_QUART;
		else if ((!_tcsicmp(UnitName, _T("qt"))) || (!_tcsicmp(UnitName, _T("quart (US, fluid)"))))
			l = STORAGE_FORMAT_US_FL_QUART;
		else if ((!_tcsicmp(UnitName, _T("quart (US, dry)"))))
			l = STORAGE_FORMAT_US_DRY_QUART;
		else if ((!_tcsicmp(UnitName, _T("gallon (UK)"))) || (!_tcsicmp(UnitName, _T("imperial gallon"))))
			l = STORAGE_FORMAT_UK_GALLON;
		else if ((!_tcsicmp(UnitName, _T("ga"))) || (!_tcsicmp(UnitName, _T("gallon (US)"))) || (!_tcsicmp(UnitName, _T("us gallon"))))
			l = STORAGE_FORMAT_US_GALLON;
		else if ((!_tcsicmp(UnitName, _T("bu"))) || (!_tcsicmp(UnitName, _T("bushel"))))
			l = STORAGE_FORMAT_BUSHEL;
		else if ((!_tcsicmp(UnitName, _T("dry bushel"))))
			l = STORAGE_FORMAT_US_DRY_BARREL;
		else if ((!_tcsicmp(UnitName, _T("dram"))))
			l = STORAGE_FORMAT_US_DRAM;
		else if ((!_tcsicmp(UnitName, _T("ba"))) || (!_tcsicmp(UnitName, _T("barrel (US, fluid)"))))
			l = STORAGE_FORMAT_US_FL_BARREL;
	}
	if (((comp == STORAGE_TEMP_START) || (comp == 0)) && (!l)) {
		if ((!_tcsicmp(UnitName, _T(DEGREE_SYMBOL"K"))) || (!_tcsicmp(UnitName, _T("K"))) || (!_tcsicmp(UnitName, _T("Kelvin"))))
			l = STORAGE_FORMAT_KELVIN;
		else if ((!_tcsicmp(UnitName, _T(DEGREE_SYMBOL"C"))) || (!_tcsicmp(UnitName, _T("C"))) || (!_tcsicmp(UnitName, _T("Celsius"))))
			l = STORAGE_FORMAT_CELSIUS;
		else if ((!_tcsicmp(UnitName, _T(DEGREE_SYMBOL"F"))) || (!_tcsicmp(UnitName, _T("F"))) || (!_tcsicmp(UnitName, _T("Fahrenheit"))))
			l = STORAGE_FORMAT_FAHRENHEIT;
		else if ((!_tcsicmp(UnitName, _T(DEGREE_SYMBOL"R"))) || (!_tcsicmp(UnitName, _T("R"))) || (!_tcsicmp(UnitName, _T("Rankine"))))
			l = STORAGE_FORMAT_RANKINE;
	}
	if (((comp == STORAGE_PRESSURE_START) || (comp == 0)) && (!l)) {
		if ((!_tcsicmp(UnitName, _T("kPa"))) || (!_tcsicmp(UnitName, _T("kilopascals"))))
			l = STORAGE_FORMAT_KPA;
		else if ((!_tcsicmp(UnitName, _T("psi"))) || (!_tcsicmp(UnitName, _T("pounds per square inch"))))
			l = STORAGE_FORMAT_PSI;
		else if (!_tcsicmp(UnitName, _T("bar")))
			l = STORAGE_FORMAT_BAR;
		else if ((!_tcsicmp(UnitName, _T("atm"))) || (!_tcsicmp(UnitName, _T("atmospheres"))))
			l = STORAGE_FORMAT_ATM;
		else if (!_tcsicmp(UnitName, _T("torr")))
			l = STORAGE_FORMAT_TORR;
	}
	if (((comp == STORAGE_MASS_START) || (comp == 0)) && (!l)) {
		if ((!_tcsicmp(UnitName, _T("mg"))) || (!_tcsicmp(UnitName, _T("milligram"))))
			l = STORAGE_FORMAT_MILLIGRAM;
		else if ((!_tcsicmp(UnitName, _T("g"))) || (!_tcsicmp(UnitName, _T("gram"))))
			l = STORAGE_FORMAT_GRAM;
		else if ((!_tcsicmp(UnitName, _T("g"))) || (!_tcsicmp(UnitName, _T("kilogram"))))
			l = STORAGE_FORMAT_KG;
		else if ((!_tcsicmp(UnitName, _T("kg"))) || (!_tcsicmp(UnitName, _T("tonne"))))
			l = STORAGE_FORMAT_TONNE;
		else if ((!_tcsicmp(UnitName, _T("t"))) || (!_tcsicmp(UnitName, _T("ounce"))))
			l = STORAGE_FORMAT_OUNCE;
		else if ((!_tcsicmp(UnitName, _T("oz"))) || (!_tcsicmp(UnitName, _T("pound"))))
			l = STORAGE_FORMAT_LB;
		else if (!_tcsicmp(UnitName, _T("ton")))
			l = STORAGE_FORMAT_TON;
		else if (!_tcsicmp(UnitName, _T("short ton")))
			l = STORAGE_FORMAT_TON;
	}
	if (((comp == STORAGE_ENERGY_START) || (comp == 0)) && (!l)) {
		if ((!_tcsicmp(UnitName, _T("J"))) || (!_tcsicmp(UnitName, _T("Joule"))))
			l = STORAGE_FORMAT_JOULE;
		else if ((!_tcsicmp(UnitName, _T("kJ"))) || (!_tcsicmp(UnitName, _T("kiloJoule"))))
			l = STORAGE_FORMAT_KILOJOULE;
		else if ((!_tcsicmp(UnitName, _T("eV"))) || (!_tcsicmp(UnitName, _T("electron-volt"))))
			l = STORAGE_FORMAT_ELECTRONVOLT;
		else if ((!_tcsicmp(UnitName, _T("erg"))) /*|| (!_tcsicmp(UnitName, _T("erg")))*/)
			l = STORAGE_FORMAT_ERG;
		else if ((!_tcsicmp(UnitName, _T("ft-lb"))) || (!_tcsicmp(UnitName, _T("foot-pound"))))
			l = STORAGE_FORMAT_FT_LB;
		else if ((!_tcsicmp(UnitName, _T("ca"))) || (!_tcsicmp(UnitName, _T("calorie"))))
			l = STORAGE_FORMAT_CALORIE;
		else if ((!_tcsicmp(UnitName, _T("kg-m"))) || (!_tcsicmp(UnitName, _T("kg-meter"))))
			l = STORAGE_FORMAT_KG_METRE;
		else if (!_tcsicmp(UnitName, _T("btu")))
			l = STORAGE_FORMAT_BTU;
		else if ((!_tcsicmp(UnitName, _T("wa"))) || (!_tcsicmp(UnitName, _T("watt"))) || (!_tcsicmp(UnitName, _T("W"))))
			l = STORAGE_FORMAT_WATT_;
		else if ((!_tcsicmp(UnitName, _T("kwa"))) || (!_tcsicmp(UnitName, _T("kilowatt"))) || (!_tcsicmp(UnitName, _T("kW"))))
			l = STORAGE_FORMAT_KILOWATT_;
		else if ((!_tcsicmp(UnitName, _T("th"))) || (!_tcsicmp(UnitName, _T("therm"))))
			l = STORAGE_FORMAT_THERM;
		else if ((!_tcsicmp(UnitName, _T("MW"))) || (!_tcsicmp(UnitName, _T("megawatt"))))
			l = STORAGE_FORMAT_MEGAWATT;
		else if ((!_tcsicmp(UnitName, _T("btu/s"))))
			l = STORAGE_FORMAT_BTU_PER_SECOND;
		else if ((!_tcsicmp(UnitName, _T("btu/hr"))))
			l = STORAGE_FORMAT_BTU_PER_HOUR;
	}
	if (((comp == STORAGE_PERCENT_START) || (comp == 0)) && (!l)) {
		if (!_tcsicmp(UnitName,  _T("%%")))
			l = STORAGE_FORMAT_PERCENT;
		else if (!(UnitName[0]))
			l = STORAGE_FORMAT_DECIMAL;
	}
	if (((comp == STORAGE_TIME_START) || (comp == 0)) && (!l)) {
		if ((!_tcsicmp(UnitName,  _T("us"))) || (!_tcsicmp(UnitName,  _T("usec"))) || (!_tcsicmp(UnitName,  _T("microsecond"))))
			l = STORAGE_FORMAT_MICROSECOND;
		if ((!_tcsicmp(UnitName,  _T("ms"))) || (!_tcsicmp(UnitName,  _T("msec"))) || (!_tcsicmp(UnitName,  _T("millisecond"))))
			l = STORAGE_FORMAT_MILLISECOND;
		if ((!_tcsicmp(UnitName,  _T("s"))) || (!_tcsicmp(UnitName,  _T("sec"))) || (!_tcsicmp(UnitName,  _T("second"))))
			l = STORAGE_FORMAT_SECOND;
		else if ((!_tcsicmp(UnitName,  _T("m"))) || (!_tcsicmp(UnitName,  _T("min"))) || (!_tcsicmp(UnitName,  _T("minute"))))
			l = STORAGE_FORMAT_MINUTE;
		else if ((!_tcsicmp(UnitName,  _T("h"))) || (!_tcsicmp(UnitName,  _T("hr"))) || (!_tcsicmp(UnitName,  _T("hour"))))
			l = STORAGE_FORMAT_HOUR;
		else if ((!_tcsicmp(UnitName,  _T("d"))) || (!_tcsicmp(UnitName,  _T("day"))))
			l = STORAGE_FORMAT_DAY;
		else if ((!_tcsicmp(UnitName, _T("wk"))) || (!_tcsicmp(UnitName, _T("week"))))
			l = STORAGE_FORMAT_WEEK;
		else if ((!_tcsicmp(UnitName, _T("mth"))) || (!_tcsicmp(UnitName, _T("month"))))
			l = STORAGE_FORMAT_MONTH;
		else if ((!_tcsicmp(UnitName, _T("yr"))) || (!_tcsicmp(UnitName, _T("year"))))
			l = STORAGE_FORMAT_YEAR;
		else if ((!_tcsicmp(UnitName, _T("decade"))))
			l = STORAGE_FORMAT_DECADE;
		else if ((!_tcsicmp(UnitName, _T("century"))))
			l = STORAGE_FORMAT_CENTURY;
	}
	if ((((comp & (~STORAGE_FORMAT_ANGLE_MASK)) == STORAGE_FORMAT_ANGLE) || (comp == 0)) && (!l)) {
		if ((!_tcsicmp(UnitName, _T("rad"))) || (!_tcsicmp(UnitName, _T("radian"))))
			l = STORAGE_FORMAT_ANGLE | STORAGE_FORMAT_RADIAN;
		else if ((!_tcsicmp(UnitName, _T(DEGREE_SYMBOL))) || (!_tcsicmp(UnitName, _T("degree"))) || (!_tcsicmp(UnitName, _T("deg"))))
			l = STORAGE_FORMAT_ANGLE | STORAGE_FORMAT_DEGREE;
		else if ((!_tcsicmp(UnitName, _T("arcsec"))) || (!_tcsicmp(UnitName, _T("arcsecond"))))
			l = STORAGE_FORMAT_ANGLE | STORAGE_FORMAT_ARCSECOND;
	}
	if (!l && trial < 2)
	{
		tstring name = UnitName;
		size_t b1 = name.find_first_of(_T('('));
		size_t b2;
		tstring brackets = _T("");
		if (b1 != tstring::npos)
		{
			b2 = name.find_last_of(_T(')'));
			if (b2 != tstring::npos)
			{
				brackets = _T(" ") + name.substr(b1, b2 - b1 + 1);
				name = name.substr(0, b1);
				while (name[name.length() - 1] == ' ')
					name = name.substr(0, name.length() - 1);
			}
		}
		if (name.length() > 1)
		{
			if (trial == 0)
			{
				if (!name.compare(name.length() - 1, 1, _T("s")))
				{
					name = name.substr(0, name.length() - 1) + brackets;
					l = UnitFormatSearch(comp, name.c_str(), 1);
				}
				if (!l && name.length() > 1)
				{
					size_t location;
					if ((location = name.find_first_of(_T(' '))) != tstring::npos)
					{
						tstring name2 = name.substr(0, location);
						tstring part2 = name.substr(location + 1);
//						UINT count = 0;
//						if (count < 2)
//						{
							if (!name2.compare(name2.length() - 1, 1, _T("s")) && name2.length() > 1)
							{
								name2 = name2.substr(0, name2.length() - 1);
								tstring name3 = name2 + _T(" ") + part2 + brackets;
								l = UnitFormatSearch(comp, name3.c_str(), 3);
								if (!l && name2.length() > 1)
								{
									if (!name2.compare(name2.length() - 1, 1, _T("e")))
									{
										name2 = name2.substr(0, name2.length() - 1);
										name3 = name2 + _T(" ") + part2 + brackets;
										l = UnitFormatSearch(comp, name3.c_str(), 4);
									}
								}
//							}
						}
					}
				}
			}
			else /* if (trial == 1) */
			{
				if (!name.compare(name.length() - 1, 1, _T("e")))
				{
					name = name.substr(0, name.length() - 1) + brackets;
					l = UnitFormatSearch(comp, name.c_str(), 1);
				}
			}
		}
	}
	return l;
}


tstring UnitConvert::UnitName(STORAGE_UNIT format, bool short_format)
{
	if (format & 0xffffffff00000000)
	{
		if ((format >> 0x20) & (~STORAGE_FORMAT_TIME_MASK)) {		// more than just time in the upper operand (which we deal with below)
			tstring formatstr = UnitName((format >> 0x20) & (~STORAGE_FORMAT_TIME_MASK), short_format);
			formatstr += _T("/");
			formatstr += UnitName(format & 0x00000000ffffffff, short_format);
			return formatstr;
		}
	}

	if ((format == (STORAGE_FORMAT_NAUTICAL_MILE_UK | STORAGE_FORMAT_HOUR)) || (format == (STORAGE_FORMAT_NAUTICAL_MILE | STORAGE_FORMAT_HOUR)))
		return _T("knot");
//	else if (format == (STORAGE_FORMAT_KILOWATT | STORAGE_FORMAT_HOUR))
//		return "kW";

	if (short_format)
	{
		if (!(format & (STORAGE_FORMAT_TIME_MASK | STORAGE_FORMAT_ANGLE_MASK)))
		{
			switch (format)
			{
				case STORAGE_FORMAT_MM:			return _T("mm");
				case STORAGE_FORMAT_CM:			return _T("cm");
				case STORAGE_FORMAT_M:			return _T("m");
				case STORAGE_FORMAT_KM:			return _T("km");
				case STORAGE_FORMAT_INCH:		return _T("in");
				case STORAGE_FORMAT_FOOT:		return _T("ft");
				case STORAGE_FORMAT_YARD:		return _T("yd");
				case STORAGE_FORMAT_CHAIN:		return _T("chain");
				case STORAGE_FORMAT_MILE:		return _T("mi");
				case STORAGE_FORMAT_NAUTICAL_MILE_UK:
				case STORAGE_FORMAT_NAUTICAL_MILE:	return _T("n.mi");

				case STORAGE_FORMAT_MM2:		return _T("mm" POWER2_SYMBOL);
				case STORAGE_FORMAT_CM2:		return _T("cm" POWER2_SYMBOL);
				case STORAGE_FORMAT_M2:			return _T("m" POWER2_SYMBOL);
				case STORAGE_FORMAT_HECTARE:		return _T("ha");
				case STORAGE_FORMAT_KM2:		return _T("km" POWER2_SYMBOL);
				case STORAGE_FORMAT_IN2:		return _T("in" POWER2_SYMBOL);
				case STORAGE_FORMAT_FT2:		return _T("ft" POWER2_SYMBOL);
				case STORAGE_FORMAT_YD2:		return _T("yd" POWER2_SYMBOL);
				case STORAGE_FORMAT_ACRE:		return _T("acre");
				case STORAGE_FORMAT_MILE2:		return _T("mi" POWER2_SYMBOL);

				case STORAGE_FORMAT_MM3:		return _T("mm" POWER3_SYMBOL);
				case STORAGE_FORMAT_CM3:		return _T("cm" POWER3_SYMBOL);
				case STORAGE_FORMAT_LITRE:		return _T("l");
				case STORAGE_FORMAT_M3:			return _T("m" POWER3_SYMBOL);
				case STORAGE_FORMAT_KM3:		return _T("km" POWER3_SYMBOL);
				case STORAGE_FORMAT_IN3:		return _T("in" POWER3_SYMBOL);
				case STORAGE_FORMAT_FT3:		return _T("ft" POWER3_SYMBOL);
				case STORAGE_FORMAT_YD3:		return _T("yd" POWER3_SYMBOL);
				case STORAGE_FORMAT_MILE3:		return _T("mi" POWER3_SYMBOL);

				case STORAGE_FORMAT_UK_FL_OZ:
				case STORAGE_FORMAT_US_FL_OZ:		return _T("oz");
				case STORAGE_FORMAT_UK_PINT:
				case STORAGE_FORMAT_US_FL_PINT:
				case STORAGE_FORMAT_US_DRY_PINT:	return _T("pt");
				case STORAGE_FORMAT_UK_QUART:
				case STORAGE_FORMAT_US_FL_QUART:
				case STORAGE_FORMAT_US_DRY_QUART:	return _T("qt");
				case STORAGE_FORMAT_UK_GALLON:
				case STORAGE_FORMAT_US_GALLON:		return _T("ga");
				case STORAGE_FORMAT_BUSHEL:
				case STORAGE_FORMAT_US_DRY_BARREL:	return _T("bu");
				case STORAGE_FORMAT_US_DRAM:		return _T("dram");
				case STORAGE_FORMAT_US_FL_BARREL:	return _T("ba");

				case STORAGE_FORMAT_KELVIN:			return _T("K");
				case STORAGE_FORMAT_CELSIUS:		return _T(DEGREE_SYMBOL"C");
				case STORAGE_FORMAT_FAHRENHEIT:		return _T(DEGREE_SYMBOL"F");
				case STORAGE_FORMAT_RANKINE:		return _T(DEGREE_SYMBOL"R");

				case STORAGE_FORMAT_MILLIGRAM:		return _T("mg");
				case STORAGE_FORMAT_GRAM:		return _T("g");
				case STORAGE_FORMAT_KG:			return _T("kg");
				case STORAGE_FORMAT_TONNE:		return _T("t");
				case STORAGE_FORMAT_OUNCE:		return _T("oz");
				case STORAGE_FORMAT_LB:			return _T("lb");
				case STORAGE_FORMAT_SHORT_TON:
				case STORAGE_FORMAT_TON:		return _T("ton");

				case STORAGE_FORMAT_JOULE:		return _T("J");
				case STORAGE_FORMAT_KILOJOULE:		return _T("kJ");
				case STORAGE_FORMAT_ELECTRONVOLT:	return _T("eV");
				case STORAGE_FORMAT_ERG:		return _T("erg");
				case STORAGE_FORMAT_FT_LB:		return _T("ft-lb");
				case STORAGE_FORMAT_CALORIE:		return _T("ca");
				case STORAGE_FORMAT_KG_METRE:		return _T("kg-m");
				case STORAGE_FORMAT_BTU:		return _T("btu");
				case STORAGE_FORMAT_WATT_:		return _T("W");
				case STORAGE_FORMAT_KILOWATT_:		return _T("kW");
				case STORAGE_FORMAT_THERM:		return _T("th");
				case STORAGE_FORMAT_MEGAWATT:		return _T("MW");

//				case STORAGE_FORMAT_RADIAN:		return "rad";
//				case STORAGE_FORMAT_DEGREE:		return "\u00B0";
//				case STORAGE_FORMAT_ARCSECOND:		return "arcsec";

				case STORAGE_FORMAT_DECIMAL:		return _T("");
				case STORAGE_FORMAT_PERCENT:		return _T("%%");
				case STORAGE_FORMAT_DECIMAL_INVERT:	return _T("");
				case STORAGE_FORMAT_PERCENT_INVERT:	return _T("%%");

				case STORAGE_FORMAT_KPA:		return _T("kPa");
				case STORAGE_FORMAT_PSI:		return _T("psi");
				case STORAGE_FORMAT_BAR:		return _T("bar");
				case STORAGE_FORMAT_ATM:		return _T("atm");
				case STORAGE_FORMAT_TORR:		return _T("torr");

				case STORAGE_COORDINATE_DEGREE:			return _T("decimal degrees");
				case STORAGE_COORDINATE_DEGREE_MINUTE:		return _T("degrees decimal minutes");
				case STORAGE_COORDINATE_DEGREE_MINUTE_SECOND:	return _T("degrees minutes seconds");
				case STORAGE_COORDINATE_UTM:			return _T("grid projection");
				case STORAGE_COORDINATE_RELATIVE_DISTANCE:	return _T("relative distance");
			}
		}
		else if ((format & (~STORAGE_FORMAT_ANGLE_MASK)) == STORAGE_FORMAT_ANGLE)
		{
			switch (format & STORAGE_FORMAT_ANGLE_UNIT_MASK)
			{
				case STORAGE_FORMAT_RADIAN:		return _T("rad");
				case STORAGE_FORMAT_DEGREE:		return _T(DEGREE_SYMBOL);
				case STORAGE_FORMAT_ARCSECOND:	return _T("arcsec");
			}
		}
		else if (format & STORAGE_FORMAT_TIME_MASK)
		{
			if (format & (~STORAGE_FORMAT_TIME_MASK))
			{
				tstring tmpstr = UnitName(format & (~STORAGE_FORMAT_TIME_MASK), short_format);
				tstring time;
				switch (format & STORAGE_FORMAT_TIME_MASK)
				{
					case STORAGE_FORMAT_MICROSECOND | STORAGE_FORMAT_TIME_MULT:	time = _T("-us");	break;
					case STORAGE_FORMAT_MILLISECOND | STORAGE_FORMAT_TIME_MULT:	time = _T("-ms");	break;
					case STORAGE_FORMAT_SECOND | STORAGE_FORMAT_TIME_MULT:		time = _T("-s");	break;
					case STORAGE_FORMAT_MINUTE | STORAGE_FORMAT_TIME_MULT:		time = _T("-min");	break;
					case STORAGE_FORMAT_HOUR | STORAGE_FORMAT_TIME_MULT:		time = _T("-h");	break;
					case STORAGE_FORMAT_DAY | STORAGE_FORMAT_TIME_MULT:		time = _T("-day");	break;
					case STORAGE_FORMAT_WEEK | STORAGE_FORMAT_TIME_MULT:		time = _T("-wk");	break;
					case STORAGE_FORMAT_MONTH | STORAGE_FORMAT_TIME_MULT:		time = _T("-mth");	break;
					case STORAGE_FORMAT_YEAR | STORAGE_FORMAT_TIME_MULT:		time = _T("-yr");	break;
					case STORAGE_FORMAT_DECADE | STORAGE_FORMAT_TIME_MULT:		time = _T("-decade");	break;
					case STORAGE_FORMAT_CENTURY | STORAGE_FORMAT_TIME_MULT:		time = _T("-century");	break;
					case STORAGE_FORMAT_MICROSECOND:				time = _T("/us");	break;
					case STORAGE_FORMAT_MILLISECOND:				time = _T("/ms");	break;
					case STORAGE_FORMAT_SECOND:					time = _T("/s");	break;
					case STORAGE_FORMAT_MINUTE:					time = _T("/min");	break;
					case STORAGE_FORMAT_HOUR:					time = _T("/h");	break;
					case STORAGE_FORMAT_DAY:					time = _T("/day");	break;
					case STORAGE_FORMAT_WEEK:					time = _T("/wk");	break;
					case STORAGE_FORMAT_MONTH:					time = _T("/mth");	break;
					case STORAGE_FORMAT_YEAR:					time = _T("/yr");	break;
					case STORAGE_FORMAT_DECADE:					time = _T("/decade");	break;
					case STORAGE_FORMAT_CENTURY:					time = _T("/century");	break;
				}
				if ((!((format >> 0x20) & (~STORAGE_FORMAT_TIME_MASK))) && (((format >> 0x20) & STORAGE_FORMAT_TIME_MASK) == (format & STORAGE_FORMAT_TIME_MASK))) {
					time += _T(POWER2_SYMBOL);
				}
				return tmpstr + time;
			}
			else
			{
				tstring time;
				switch (format & STORAGE_FORMAT_TIME_UNIT_MASK)
				{
					case STORAGE_FORMAT_MICROSECOND:				time = _T("us");	break;
					case STORAGE_FORMAT_MILLISECOND:				time = _T("ms");	break;
					case STORAGE_FORMAT_SECOND:					time = _T("s");		break;
					case STORAGE_FORMAT_MINUTE:					time = _T("min");	break;
					case STORAGE_FORMAT_HOUR:					time = _T("h");		break;
					case STORAGE_FORMAT_DAY:					time = _T("day");	break;
					case STORAGE_FORMAT_WEEK:					time = _T("wk");	break;
					case STORAGE_FORMAT_MONTH:					time = _T("mth");	break;
					case STORAGE_FORMAT_YEAR:					time = _T("yr");	break;
					case STORAGE_FORMAT_DECADE:					time = _T("decade");	break;
					case STORAGE_FORMAT_CENTURY:					time = _T("century");	break;
				}
				if ((!((format >> 0x20) & (~STORAGE_FORMAT_TIME_MASK))) && (((format >> 0x20) & STORAGE_FORMAT_TIME_MASK) == (format & STORAGE_FORMAT_TIME_MASK))) {
					time += _T(POWER2_SYMBOL);
				}
				return time;
			}
		}
	}
	else
	{
		if (!(format & (STORAGE_FORMAT_TIME_MASK | STORAGE_FORMAT_ANGLE_MASK)))
		{
			switch (format)
			{
				case STORAGE_FORMAT_MM:			return _T("millimeter");
				case STORAGE_FORMAT_CM:			return _T("centimeter");
				case STORAGE_FORMAT_M:			return _T("meter");
				case STORAGE_FORMAT_KM:			return _T("kilometer");
				case STORAGE_FORMAT_INCH:		return _T("inch");
				case STORAGE_FORMAT_FOOT:		return _T("foot");
				case STORAGE_FORMAT_YARD:		return _T("yard");
				case STORAGE_FORMAT_CHAIN:		return _T("chain");
				case STORAGE_FORMAT_MILE:		return _T("mile");
				case STORAGE_FORMAT_NAUTICAL_MILE_UK:
				case STORAGE_FORMAT_NAUTICAL_MILE:	return _T("nautical mile");

				case STORAGE_FORMAT_MM2:		return _T("millimeter squared");
				case STORAGE_FORMAT_CM2:		return _T("centimeter squared");
				case STORAGE_FORMAT_M2:			return _T("square meter");
				case STORAGE_FORMAT_HECTARE:		return _T("hectare");
				case STORAGE_FORMAT_KM2:		return _T("square kilometer");
				case STORAGE_FORMAT_IN2:		return _T("square inch");
				case STORAGE_FORMAT_FT2:		return _T("square foot");
				case STORAGE_FORMAT_YD2:		return _T("square yard");
				case STORAGE_FORMAT_ACRE:		return _T("acre");
				case STORAGE_FORMAT_MILE2:		return _T("square mile");

				case STORAGE_FORMAT_MM3:		return _T("cubic millimeter");
				case STORAGE_FORMAT_CM3:		return _T("cubic centimeter");
				case STORAGE_FORMAT_LITRE:		return _T("litre");
				case STORAGE_FORMAT_M3:			return _T("cubic meter");
				case STORAGE_FORMAT_KM3:		return _T("cubic kilometer");
				case STORAGE_FORMAT_IN3:		return _T("cubic inch");
				case STORAGE_FORMAT_FT3:		return _T("cubic foot");
				case STORAGE_FORMAT_YD3:		return _T("cubic yard");
				case STORAGE_FORMAT_MILE3:		return _T("cubic mile");
				case STORAGE_FORMAT_UK_FL_OZ:		return _T("fluid ounce (UK)");
				case STORAGE_FORMAT_US_FL_OZ:		return _T("fluid ounce (US)");
				case STORAGE_FORMAT_UK_PINT:		return _T("pint (UK)");
				case STORAGE_FORMAT_US_FL_PINT:		return _T("pint (US, fluid)");
				case STORAGE_FORMAT_US_DRY_PINT:	return _T("pint (US, dry)");
				case STORAGE_FORMAT_UK_QUART:		return _T("quart (UK)");
				case STORAGE_FORMAT_US_FL_QUART:	return _T("quart (US, fluid)");
				case STORAGE_FORMAT_US_DRY_QUART:	return _T("quart (US, dry)");
				case STORAGE_FORMAT_UK_GALLON:		return _T("gallon (UK)");
				case STORAGE_FORMAT_US_GALLON:		return _T("gallon (US)");
				case STORAGE_FORMAT_BUSHEL:		return _T("bushel");
				case STORAGE_FORMAT_US_DRY_BARREL:	return _T("dry bushel");
				case STORAGE_FORMAT_US_DRAM:		return _T("dram");
				case STORAGE_FORMAT_US_FL_BARREL:	return _T("barrel (US, fluid)");

				case STORAGE_FORMAT_KELVIN:		return _T("Kelvin");
				case STORAGE_FORMAT_CELSIUS:		return _T("Celsius"); // this is capitalized because Natural Resources of Canada says so : http://ess.nrcan.gc.ca/pubs/scipub/guide/gramm/caps_e.php
				case STORAGE_FORMAT_FAHRENHEIT:		return _T("Fahrenheit");
				case STORAGE_FORMAT_RANKINE:		return _T("Rankine");

				case STORAGE_FORMAT_MILLIGRAM:		return _T("milligram");
				case STORAGE_FORMAT_GRAM:		return _T("gram");
				case STORAGE_FORMAT_KG:			return _T("kilogram");
				case STORAGE_FORMAT_TONNE:		return _T("tonne");
				case STORAGE_FORMAT_OUNCE:		return _T("ounce");
				case STORAGE_FORMAT_LB:			return _T("pound");
				case STORAGE_FORMAT_SHORT_TON:		return _T("short ton");
				case STORAGE_FORMAT_TON:		return _T("ton");

				case STORAGE_FORMAT_JOULE:		return _T("joule");
				case STORAGE_FORMAT_KILOJOULE:		return _T("kilojoule");
				case STORAGE_FORMAT_ELECTRONVOLT:	return _T("electron-volt");
				case STORAGE_FORMAT_ERG:		return _T("erg");
				case STORAGE_FORMAT_FT_LB:		return _T("foot-pound");
				case STORAGE_FORMAT_CALORIE:		return _T("calorie");
				case STORAGE_FORMAT_KG_METRE:		return _T("kg-meter");
				case STORAGE_FORMAT_BTU:		return _T("btu");
				case STORAGE_FORMAT_WATT_:		return _T("watt");
				case STORAGE_FORMAT_KILOWATT_:		return _T("kilowatt");
				case STORAGE_FORMAT_THERM:		return _T("therm");
				case STORAGE_FORMAT_MEGAWATT:		return _T("megawatt");

//				case STORAGE_FORMAT_RADIAN:		return "radian";
//				case STORAGE_FORMAT_DEGREE:		return "degree";
//				case STORAGE_FORMAT_ARCSECOND:		return "arcsecond";

				case STORAGE_FORMAT_DECIMAL:		return _T("");
				case STORAGE_FORMAT_PERCENT:		return _T("%%");
				case STORAGE_FORMAT_DECIMAL_INVERT:	return _T("");
				case STORAGE_FORMAT_PERCENT_INVERT:	return _T("%%");

				case STORAGE_FORMAT_KPA:		return _T("kilopascals");
				case STORAGE_FORMAT_PSI:		return _T("pounds per square inch");
				case STORAGE_FORMAT_BAR:		return _T("bar");
				case STORAGE_FORMAT_ATM:		return _T("atmospheres");
				case STORAGE_FORMAT_TORR:		return _T("torr");

				case STORAGE_COORDINATE_DEGREE:			return _T("decimal degrees");
				case STORAGE_COORDINATE_DEGREE_MINUTE:		return _T("degrees decimal minutes");
				case STORAGE_COORDINATE_DEGREE_MINUTE_SECOND:	return _T("degrees minutes seconds");
				case STORAGE_COORDINATE_UTM:			return _T("grid projection");
				case STORAGE_COORDINATE_RELATIVE_DISTANCE:	return _T("relative distance");
			}
		}
		else if ((format & (~STORAGE_FORMAT_ANGLE_MASK)) == STORAGE_FORMAT_ANGLE)
		{
			switch (format & STORAGE_FORMAT_ANGLE_MASK)
			{
				case STORAGE_FORMAT_RADIAN:		return _T("radian");
				case STORAGE_FORMAT_DEGREE:		return _T("degree");
				case STORAGE_FORMAT_ARCSECOND:		return _T("arcsecond");
			}
		}
		else if (format & STORAGE_FORMAT_TIME_MASK)
		{
			if (format & (~STORAGE_FORMAT_TIME_MASK))
			{
				tstring tmpstr = UnitName(format & (~STORAGE_FORMAT_TIME_MASK), short_format);
				tstring time;
				switch (format & STORAGE_FORMAT_TIME_MASK)
				{
					case STORAGE_FORMAT_MICROSECOND | STORAGE_FORMAT_TIME_MULT:	time = _T("-usec");	break;
					case STORAGE_FORMAT_MILLISECOND | STORAGE_FORMAT_TIME_MULT:	time = _T("-msec");	break;
					case STORAGE_FORMAT_SECOND | STORAGE_FORMAT_TIME_MULT:		time = _T("-sec");	break;
					case STORAGE_FORMAT_MINUTE | STORAGE_FORMAT_TIME_MULT:		time = _T("-min");	break;
					case STORAGE_FORMAT_HOUR | STORAGE_FORMAT_TIME_MULT:		time = _T("-hour");	break;
					case STORAGE_FORMAT_DAY | STORAGE_FORMAT_TIME_MULT:		time = _T("-day");	break;
					case STORAGE_FORMAT_WEEK | STORAGE_FORMAT_TIME_MULT:		time = _T("-week");	break;
					case STORAGE_FORMAT_MONTH | STORAGE_FORMAT_TIME_MULT:		time = _T("-month");	break;
					case STORAGE_FORMAT_YEAR | STORAGE_FORMAT_TIME_MULT:		time = _T("-year");	break;
					case STORAGE_FORMAT_DECADE | STORAGE_FORMAT_TIME_MULT:		time = _T("-decade");	break;
					case STORAGE_FORMAT_CENTURY | STORAGE_FORMAT_TIME_MULT:		time = _T("-century");	break;
					case STORAGE_FORMAT_MICROSECOND:				time = _T("/usec");	break;
					case STORAGE_FORMAT_MILLISECOND:				time = _T("/msec");	break;
					case STORAGE_FORMAT_SECOND:					time = _T("/sec");	break;
					case STORAGE_FORMAT_MINUTE:					time = _T("/min");	break;
					case STORAGE_FORMAT_HOUR:					time = _T("/hour");	break;
					case STORAGE_FORMAT_DAY:					time = _T("/day");	break;
					case STORAGE_FORMAT_WEEK:					time = _T("/week");	break;
					case STORAGE_FORMAT_MONTH:					time = _T("/month");	break;
					case STORAGE_FORMAT_YEAR:					time = _T("/year");	break;
					case STORAGE_FORMAT_DECADE:					time = _T("/decade");	break;
					case STORAGE_FORMAT_CENTURY:					time = _T("/century");	break;
				}
				if ((!((format >> 0x20) & (~STORAGE_FORMAT_TIME_MASK))) && (((format >> 0x20) & STORAGE_FORMAT_TIME_MASK) == (format & STORAGE_FORMAT_TIME_MASK))) {
					time += _T(POWER2_SYMBOL);
				}
				return tmpstr + time;
			}
			else
			{
				tstring time;
				switch (format & STORAGE_FORMAT_TIME_UNIT_MASK)
				{
					case STORAGE_FORMAT_MICROSECOND:				time = _T("usec");	break;
					case STORAGE_FORMAT_MILLISECOND:				time = _T("msec");	break;
					case STORAGE_FORMAT_SECOND:					time = _T("sec");	break;
					case STORAGE_FORMAT_MINUTE:					time = _T("min");	break;
					case STORAGE_FORMAT_HOUR:					time = _T("hour");	break;
					case STORAGE_FORMAT_DAY:					time = _T("day");	break;
					case STORAGE_FORMAT_WEEK:					time = _T("week");	break;
					case STORAGE_FORMAT_MONTH:					time = _T("month");	break;
					case STORAGE_FORMAT_YEAR:					time = _T("year");	break;
					case STORAGE_FORMAT_DECADE:					time = _T("decade");	break;
					case STORAGE_FORMAT_CENTURY:					time = _T("century");	break;
				}
				if ((!((format >> 0x20) & (~STORAGE_FORMAT_TIME_MASK))) && (((format >> 0x20) & STORAGE_FORMAT_TIME_MASK) == (format & STORAGE_FORMAT_TIME_MASK))) {
					time += _T(POWER2_SYMBOL);
				}
				return time;
			}
		}
	}
	return _T("");
}
