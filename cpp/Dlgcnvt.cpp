#include "types.h"
#include "Dlgcnvt.h"
#include <float.h>              // floating point precision

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
