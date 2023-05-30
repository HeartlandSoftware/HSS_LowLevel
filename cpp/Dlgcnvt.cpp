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

#include "types.h"
#include "Dlgcnvt.h"
#include <float.h>              // floating point precision
#include <boost/algorithm/string.hpp>

#ifdef __GNUC__
#include <stdexcept>
void AfxThrowNotSupportedException() { throw std::logic_error("AFX Exception"); }
#endif


UnitConversion::UnitConversion(const TCHAR *group_name)
{
	m_small_measure_display = STORAGE_FORMAT_MM;
	m_small_distance_display = STORAGE_FORMAT_M;
	m_distance_display = STORAGE_FORMAT_KM;
	m_alt_distance_display = STORAGE_FORMAT_M;
	m_coordinate_display = STORAGE_COORDINATE_DEGREE;
	m_area_display = STORAGE_FORMAT_HECTARE;
	m_volume_display = STORAGE_FORMAT_M3;
	m_temp_display = STORAGE_FORMAT_CELSIUS;
	m_mass_display = STORAGE_FORMAT_KG;
	m_energy_display = STORAGE_FORMAT_JOULE;
	m_angle_display = STORAGE_FORMAT_DEGREE;
	m_velocity_display = STORAGE_FORMAT_KM | STORAGE_FORMAT_HOUR;
	m_alt_velocity_display = STORAGE_FORMAT_M | STORAGE_FORMAT_MINUTE;
	m_mass_area_display = ((UnitConvert::STORAGE_UNIT)STORAGE_FORMAT_KG << 0x20) | STORAGE_FORMAT_M2;
	m_intensity_display = ((UnitConvert::STORAGE_UNIT)STORAGE_FORMAT_KILOWATT_SECOND << 0x20) | STORAGE_FORMAT_M;
	m_power_display = STORAGE_FORMAT_KILOWATT_;
	m_language_display = "en-ca";
}


void UnitConversion::ResetToDefaults() {
	m_coordinate_display = STORAGE_COORDINATE_DEGREE;
	m_distance_display = STORAGE_FORMAT_KM;
	m_small_distance_display = STORAGE_FORMAT_M;
	m_temp_display = STORAGE_FORMAT_CELSIUS;
	m_small_measure_display = STORAGE_FORMAT_MM;
	m_velocity_display = STORAGE_FORMAT_KM | STORAGE_FORMAT_HOUR;
	m_alt_distance_display = STORAGE_FORMAT_M;
	m_area_display = STORAGE_FORMAT_HECTARE;
	m_alt_velocity_display = STORAGE_FORMAT_M | STORAGE_FORMAT_MINUTE;
	m_mass_area_display = ((UnitConvert::STORAGE_UNIT)STORAGE_FORMAT_KG << 0x20) | STORAGE_FORMAT_M2;
	m_intensity_display = ((UnitConvert::STORAGE_UNIT)STORAGE_FORMAT_KILOWATT_SECOND << 0x20) | STORAGE_FORMAT_M;
	m_power_display = STORAGE_FORMAT_KILOWATT_;

	m_volume_display = STORAGE_FORMAT_M3;
	m_mass_display = STORAGE_FORMAT_KG;
	m_energy_display = STORAGE_FORMAT_JOULE;
	m_angle_display = STORAGE_COORDINATE_DEGREE;
}


void UnitConversion::SaveToIniFile(const TCHAR *group_name) const
{
}


UnitConvert::STORAGE_UNIT UnitConversion::SmallMeasureDisplay(UnitConvert::STORAGE_UNIT mode) {
	if ((mode >= STORAGE_DISTANCE_START) && (mode <= STORAGE_DISTANCE_END))
		m_small_measure_display = mode;
	else	AfxThrowNotSupportedException();	// asked for a conversion not supported
	return m_small_measure_display;
}


UnitConvert::STORAGE_UNIT UnitConversion::SmallDistanceDisplay(UnitConvert::STORAGE_UNIT mode) {
	if ((mode >= STORAGE_DISTANCE_START) && (mode <= STORAGE_DISTANCE_END))
		m_small_distance_display = mode;
	else	AfxThrowNotSupportedException();	// asked for a conversion not supported
	return m_small_distance_display;
}


UnitConvert::STORAGE_UNIT UnitConversion::DistanceDisplay(UnitConvert::STORAGE_UNIT mode) {
	if ((mode >= STORAGE_DISTANCE_START) && (mode <= STORAGE_DISTANCE_END))
		m_distance_display = mode;
	else	AfxThrowNotSupportedException();	// asked for a conversion not supported
	return m_distance_display;
}


UnitConvert::STORAGE_UNIT UnitConversion::AltDistanceDisplay(UnitConvert::STORAGE_UNIT mode) {
	if ((mode >= STORAGE_DISTANCE_START) && (mode <= STORAGE_DISTANCE_END))
		m_alt_distance_display = mode;
	else	AfxThrowNotSupportedException();	// asked for a conversion not supported
	return m_alt_distance_display;
}


UnitConvert::STORAGE_UNIT UnitConversion::CoordinateDisplay(UnitConvert::STORAGE_UNIT mode) {
	if ((mode >= STORAGE_COORDINATE_START) && (mode <= STORAGE_COORDINATE_END))
		m_coordinate_display = mode;
	else	AfxThrowNotSupportedException();	// asked for a conversion not supported
	return m_coordinate_display;
}


UnitConvert::STORAGE_UNIT UnitConversion::VelocityDisplay(UnitConvert::STORAGE_UNIT mode) {
	if (((mode & 0x00ff) >= STORAGE_DISTANCE_START) && ((mode & 0x00ff) <= STORAGE_DISTANCE_END))
		m_velocity_display = mode;
	else	AfxThrowNotSupportedException();	// asked for a conversion not supported
	return m_velocity_display;
}


UnitConvert::STORAGE_UNIT UnitConversion::AltVelocityDisplay(UnitConvert::STORAGE_UNIT mode) {
	if (((mode & 0x00ff) >= STORAGE_DISTANCE_START) && ((mode & 0x00ff) <= STORAGE_DISTANCE_END))
		m_alt_velocity_display = mode;
	else	AfxThrowNotSupportedException();	// asked for a conversion not supported
	return m_alt_velocity_display;
}


UnitConvert::STORAGE_UNIT UnitConversion::AreaDisplay(UnitConvert::STORAGE_UNIT mode) {
	if ((mode >= STORAGE_AREA_START) && (mode <= STORAGE_AREA_END))
		m_area_display = mode;
	else	AfxThrowNotSupportedException();	// asked for a conversion not supported
	return m_area_display;
}


UnitConvert::STORAGE_UNIT UnitConversion::VolumeDisplay(UnitConvert::STORAGE_UNIT mode) {
	if ((mode >= STORAGE_VOLUME_START) && (mode <= STORAGE_VOLUME_END))
		m_volume_display = mode;
	else	AfxThrowNotSupportedException();	// asked for a conversion not supported
	return m_volume_display;
}


UnitConvert::STORAGE_UNIT UnitConversion::TempDisplay(UnitConvert::STORAGE_UNIT mode) {
	if ((mode >= STORAGE_TEMP_START) && (mode <= STORAGE_TEMP_END))
		m_temp_display = mode;
	else	AfxThrowNotSupportedException();	// asked for a conversion not supported
	return m_temp_display;
}


UnitConvert::STORAGE_UNIT UnitConversion::MassDisplay(UnitConvert::STORAGE_UNIT mode) {
	if ((mode >= STORAGE_MASS_START) && (mode <= STORAGE_MASS_END))
		m_mass_display = mode;
	else	AfxThrowNotSupportedException();	// asked for a conversion not supported
	return m_mass_display;
}


UnitConvert::STORAGE_UNIT UnitConversion::MassAreaDisplay(UnitConvert::STORAGE_UNIT mode) {
	if (((mode >> 0x20) >= STORAGE_MASS_START) && ((mode >> 0x20) <= STORAGE_MASS_END))
		m_mass_area_display = mode;
	else	AfxThrowNotSupportedException();	// asked for a conversion not supported
	return m_mass_area_display;
}


UnitConvert::STORAGE_UNIT UnitConversion::IntensityDisplay(UnitConvert::STORAGE_UNIT mode) {
	if ((((mode >> 0x20) & (~STORAGE_TIME_END)) >= STORAGE_ENERGY_START) && (((mode >> 0x20) & (~STORAGE_TIME_END)) <= STORAGE_ENERGY_END))
		m_intensity_display = mode;
	else	AfxThrowNotSupportedException();	// asked for a conversion not supported
	return m_intensity_display;
}


UnitConvert::STORAGE_UNIT UnitConversion::PowerDisplay(UnitConvert::STORAGE_UNIT mode) {
	if (mode == UnitConvert::STORAGE_FORMAT_ERG_PER_SECOND || mode == UnitConvert::STORAGE_FORMAT_BTU_PER_SECOND ||
		mode == UnitConvert::STORAGE_FORMAT_BTU_PER_HOUR || mode == STORAGE_FORMAT_WATT_ ||
		mode == STORAGE_FORMAT_KILOWATT_ || mode == UnitConvert::STORAGE_FORMAT_MEGAWATT)
		m_power_display = mode;
	else    AfxThrowNotSupportedException();	//asked for a conversion not supported
	return m_power_display;
}


UnitConvert::STORAGE_UNIT UnitConversion::EnergyDisplay(UnitConvert::STORAGE_UNIT mode) {
	if ((mode >= STORAGE_ENERGY_START) && (mode <= STORAGE_ENERGY_END))
		m_energy_display = mode;
	else	AfxThrowNotSupportedException();	// asked for a conversion not supported
	return m_energy_display;
}


UnitConvert::STORAGE_UNIT UnitConversion::AngleDisplay(UnitConvert::STORAGE_UNIT mode) {
	if (((mode & 0x0000ffff) == STORAGE_FORMAT_ANGLE) /*&& ((mode & 0x00ff) <= STORAGE_ANGLE_END)*/)
		m_angle_display = mode;
	else	AfxThrowNotSupportedException();	// asked for a conversion not supported
	return m_angle_display;
}


float UnitConversion::ConvertUnit(float value, UnitConvert::STORAGE_UNIT from_format, UnitConvert::STORAGE_UNIT to_format) {
	return UnitConvert::convertUnit(value, to_format, from_format); 
}	// **** note reversal of parmeter order


double UnitConversion::ConvertUnit(double value, UnitConvert::STORAGE_UNIT from_format, UnitConvert::STORAGE_UNIT to_format) { 
	return UnitConvert::convertUnit(value, to_format, from_format);
}


std::string UnitConversion::UnitName(UnitConvert::STORAGE_UNIT format, bool short_format) {
	return UnitConvert::UnitName(format, short_format); 
}


std::string_view UnitConversion::DisplayLanguage() const {
	return m_language_display;
}


bool UnitConversion::SetDisplayLanguage(const char* language) {
	m_language_display = "";
	if (boost::iequals(language, "en-us"))
		m_language_display = "en-us";
	if (boost::iequals(language, "en-ca"))
		m_language_display = "en-ca";
	if (boost::iequals(language, "fr-ca"))
		m_language_display = "fr-ca";
	return strlen(m_language_display.data()) > 0;
}


std::string_view UnitConversion::DisplayPrimaryLanguage() const {
	if ((m_language_display[0] == 'e') &&
		(m_language_display[1] == 'n'))
		return "en";
	if ((m_language_display[0] == 'f') &&
		(m_language_display[1] == 'r'))
		return "fr";
	return "";
}
