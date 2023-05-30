/**
 * Dlgcnvt.h
 *
 * Copyright 2004-2023 Heartland Software Solutions Inc.
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

#ifndef __HSS_DLGCNVT_H
#define __HSS_DLGCNVT_H

#if _DLL
#include <afxwin.h>
#endif
#include "types.h"
#include "convert.h"
#include "hssconfig/config.h"

#ifdef _MSC_VER

#if !defined(__INTEL_COMPILER) && !defined(__INTEL_LLVM_COMPILER)
#pragma managed(push, off)
#endif
#endif //_MSC_VER

#ifdef HSS_SHOULD_PRAGMA_PACK
#pragma pack(push, 2)			// force to go to 1-byte packing rules,
#endif					// to make this class as small as possible

class UnitConversion {
protected:
	UnitConvert::STORAGE_UNIT	m_small_measure_display,
								m_small_distance_display,
								m_distance_display,
								m_alt_distance_display,
								m_area_display,
								m_volume_display,
								m_temp_display,
								m_mass_display,
								m_energy_display,
								m_angle_display,
								m_velocity_display,			// the high order byte stores something in the STORAGE_TIME_START-END range, the low order byte is distance
								m_alt_velocity_display,		// the high order byte stores something in the STORAGE_TIME_START-END range, the low order byte is distance
								m_coordinate_display,
								m_intensity_display,		// use the high order word to store the energy, and the low order word to store the distance & time
								m_mass_area_display,		// use the high order word to store the mass, and the low order word to store the area
								m_power_display;
	std::string_view			m_language_display;

public:
	UnitConversion(const TCHAR *group_name);
	void SaveToIniFile(const TCHAR *group_name) const;

	void ResetToDefaults();

	UnitConvert::STORAGE_UNIT SmallMeasureDisplay(UnitConvert::STORAGE_UNIT mode);			// Precipitation
	UnitConvert::STORAGE_UNIT SmallDistanceDisplay(UnitConvert::STORAGE_UNIT mode);			// ***** see notes
	UnitConvert::STORAGE_UNIT DistanceDisplay(UnitConvert::STORAGE_UNIT mode);				// Distances
	UnitConvert::STORAGE_UNIT AltDistanceDisplay(UnitConvert::STORAGE_UNIT mode);			// Perimeter
	UnitConvert::STORAGE_UNIT AreaDisplay(UnitConvert::STORAGE_UNIT mode);					// Area
	UnitConvert::STORAGE_UNIT VolumeDisplay(UnitConvert::STORAGE_UNIT mode);
	UnitConvert::STORAGE_UNIT TempDisplay(UnitConvert::STORAGE_UNIT mode);					// Temperature
	UnitConvert::STORAGE_UNIT MassDisplay(UnitConvert::STORAGE_UNIT mode);
	UnitConvert::STORAGE_UNIT MassAreaDisplay(UnitConvert::STORAGE_UNIT mode);				// fuel loads and consumptions
	UnitConvert::STORAGE_UNIT EnergyDisplay(UnitConvert::STORAGE_UNIT mode);
	UnitConvert::STORAGE_UNIT AngleDisplay(UnitConvert::STORAGE_UNIT mode);
	UnitConvert::STORAGE_UNIT VelocityDisplay(UnitConvert::STORAGE_UNIT mode);				// Wind Speed
	UnitConvert::STORAGE_UNIT AltVelocityDisplay(UnitConvert::STORAGE_UNIT mode);			// ROS, PGR
	UnitConvert::STORAGE_UNIT CoordinateDisplay(UnitConvert::STORAGE_UNIT mode);			// Geometric units
	UnitConvert::STORAGE_UNIT IntensityDisplay(UnitConvert::STORAGE_UNIT mode);				// fire intensities
	UnitConvert::STORAGE_UNIT PowerDisplay(UnitConvert::STORAGE_UNIT mode);					// Power (a subset of the energy units)

	UnitConvert::STORAGE_UNIT SmallMeasureDisplay() const		{ return m_small_measure_display; };
	UnitConvert::STORAGE_UNIT SmallDistanceDisplay() const		{ return m_small_distance_display; };
	UnitConvert::STORAGE_UNIT DistanceDisplay() const			{ return m_distance_display; };
	UnitConvert::STORAGE_UNIT AltDistanceDisplay() const		{ return m_alt_distance_display; };
	UnitConvert::STORAGE_UNIT AreaDisplay() const				{ return m_area_display; };
	UnitConvert::STORAGE_UNIT VolumeDisplay() const			{ return m_volume_display; };
	UnitConvert::STORAGE_UNIT TempDisplay() const				{ return m_temp_display; };
	UnitConvert::STORAGE_UNIT MassDisplay() const				{ return m_mass_display; };
	UnitConvert::STORAGE_UNIT MassAreaDisplay() const			{ return m_mass_area_display; };
	UnitConvert::STORAGE_UNIT EnergyDisplay() const			{ return m_energy_display; };
	UnitConvert::STORAGE_UNIT AngleDisplay() const				{ return m_angle_display; };
	UnitConvert::STORAGE_UNIT VelocityDisplay() const			{ return m_velocity_display; };
	UnitConvert::STORAGE_UNIT AltVelocityDisplay() const		{ return m_alt_velocity_display; };
	UnitConvert::STORAGE_UNIT CoordinateDisplay() const		{ return m_coordinate_display; };
	UnitConvert::STORAGE_UNIT IntensityDisplay() const			{ return m_intensity_display; };
	UnitConvert::STORAGE_UNIT PowerDisplay() const				{ return m_power_display; }

	static float ConvertUnit(float value, UnitConvert::STORAGE_UNIT from_format, UnitConvert::STORAGE_UNIT to_format);	// **** note reversal of parmeter order
	static double ConvertUnit(double value, UnitConvert::STORAGE_UNIT from_format, UnitConvert::STORAGE_UNIT to_format);
	static std::string UnitName(UnitConvert::STORAGE_UNIT format, bool short_format);

#if _DLL
	static CString CUnitName(UnitConvert::STORAGE_UNIT format, bool short_format)											{ return CString(UnitConvert::UnitName(format, short_format).c_str()); }
#endif

	std::string_view DisplayLanguage() const;
	std::string_view DisplayPrimaryLanguage() const;
	bool SetDisplayLanguage(const char* language);
};

#ifdef HSS_SHOULD_PRAGMA_PACK
#pragma pack(pop)			// go back to whatever the project
#endif

#ifdef _MSC_VER
#if !defined(__INTEL_COMPILER) && !defined(__INTEL_LLVM_COMPILER)
#pragma managed(pop)
#endif

#endif					// settings are...

#endif
