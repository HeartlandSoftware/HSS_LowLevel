//	dlgcnvt.cpp
//
//	By Robert W. Bryce
//	Version 1.0
//	Started:	April 3, 2000
//	Last Modified:	December 15, 2008

//	May 23:		added m_alt_distance_display, etc.
//	Jan 29, 2001:	added conversion between percent and decimal
//	Feb. 15:	added inversion routines for percent and decimal - in case you store a value as 15% but want to display it as 85%
//	Feb. 19:	added compass/cartesian conversions
//	April 26:	changed short name for hectare from 'hectare' to 'ha'
//	July 9, 2002:	fixed bug in ConvertUnit() where decimal<->percent was broken 'cause it threw an exception when it shouldn't - why
//			wasn't this caught earlier?
//	August 20, 2005:added SmallMeasure, Measure display settings to handle distance (like cell size) vs. just measurements (like precip)
//	November 15, 2008:updated to include velocities too


#include "types.h"
#include "Dlgcnvt.h"
#include <float.h>              // floating point precision

#if defined(_DEBUG) && defined(_MSC_VER)
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#ifdef __GNUC__
#include <stdexcept>
void AfxThrowNotSupportedException() { throw std::logic_error("AFX Exception"); }
#endif


UnitConversion::UnitConversion(const TCHAR *group_name)
{
#ifdef _MSC_VER
	if (group_name)
  {
		m_small_measure_display = (UnitConvert::STORAGE_UNIT)((CWinAppExt *)AfxGetApp())->GetProfileULongLong(group_name, _T("Small Measure Display 4"), STORAGE_FORMAT_MM - STORAGE_DISTANCE_START);
		m_small_distance_display = (UnitConvert::STORAGE_UNIT)((CWinAppExt *)AfxGetApp())->GetProfileULongLong(group_name, _T("Small Distance Display 4"), STORAGE_FORMAT_M - STORAGE_DISTANCE_START);
		m_distance_display = (UnitConvert::STORAGE_UNIT)((CWinAppExt *)AfxGetApp())->GetProfileULongLong(group_name, _T("Distance Display 4"), STORAGE_FORMAT_KM - STORAGE_DISTANCE_START);
		m_alt_distance_display = (UnitConvert::STORAGE_UNIT)((CWinAppExt *)AfxGetApp())->GetProfileULongLong(group_name, _T("Alternate Distance Display 4"), STORAGE_FORMAT_M - STORAGE_DISTANCE_START);
		m_coordinate_display = (UnitConvert::STORAGE_UNIT)((CWinAppExt *)AfxGetApp())->GetProfileULongLong(group_name, _T("Geometric Display 4"), STORAGE_COORDINATE_DEGREE - STORAGE_COORDINATE_START);
		m_area_display = (UnitConvert::STORAGE_UNIT)((CWinAppExt *)AfxGetApp())->GetProfileULongLong(group_name, _T("Area Display 4"), STORAGE_FORMAT_HECTARE - STORAGE_AREA_START);
		m_volume_display = (UnitConvert::STORAGE_UNIT)((CWinAppExt *)AfxGetApp())->GetProfileULongLong(group_name, _T("Volume Display 4"), STORAGE_FORMAT_M3 - STORAGE_VOLUME_START);
		m_temp_display = (UnitConvert::STORAGE_UNIT)((CWinAppExt *)AfxGetApp())->GetProfileULongLong(group_name, _T("Temperature Display 4"), STORAGE_FORMAT_CELSIUS - STORAGE_TEMP_START);
		m_mass_display = (UnitConvert::STORAGE_UNIT)((CWinAppExt *)AfxGetApp())->GetProfileULongLong(group_name, _T("Mass Display 4"), STORAGE_FORMAT_KG - STORAGE_MASS_START);
		m_energy_display = (UnitConvert::STORAGE_UNIT)((CWinAppExt *)AfxGetApp())->GetProfileULongLong(group_name, _T("Energy Display 4"), STORAGE_FORMAT_JOULE - STORAGE_ENERGY_START);
		m_angle_display = (UnitConvert::STORAGE_UNIT)((CWinAppExt *)AfxGetApp())->GetProfileULongLong(group_name, _T("Angle Display 4"), STORAGE_FORMAT_DEGREE - STORAGE_FORMAT_ANGLE);
		m_velocity_display = (UnitConvert::STORAGE_UNIT)((CWinAppExt *)AfxGetApp())->GetProfileULongLong(group_name, _T("Velocity Display 4"), (STORAGE_FORMAT_KM | STORAGE_FORMAT_HOUR) - STORAGE_DISTANCE_START);
		m_alt_velocity_display = (UnitConvert::STORAGE_UNIT)((CWinAppExt *)AfxGetApp())->GetProfileULongLong(group_name, _T("Alternate Velocity Display 4"), (STORAGE_FORMAT_M | STORAGE_FORMAT_MINUTE) - STORAGE_DISTANCE_START);
		m_mass_area_display = (UnitConvert::STORAGE_UNIT)((CWinAppExt *)AfxGetApp())->GetProfileULongLong(group_name, _T("Mass/Area Display 4"), ((UnitConvert::STORAGE_UNIT)(STORAGE_FORMAT_KG - STORAGE_MASS_START) << 0x20) | (STORAGE_FORMAT_M2 - STORAGE_AREA_START));
		m_intensity_display = (UnitConvert::STORAGE_UNIT)((CWinAppExt *)AfxGetApp())->GetProfileULongLong(group_name, _T("Intensity Display 4"), ((UnitConvert::STORAGE_UNIT)(STORAGE_FORMAT_KILOWATT_SECOND - STORAGE_ENERGY_START) << 0x20) | (STORAGE_FORMAT_M - STORAGE_DISTANCE_START));
		m_power_display = (UnitConvert::STORAGE_UNIT)((CWinAppExt*)AfxGetApp())->GetProfileULongLong(group_name, _T("Power Display 5"), STORAGE_FORMAT_KILOWATT_ - STORAGE_ENERGY_START);

		m_small_measure_display += STORAGE_DISTANCE_START;
		m_small_distance_display += STORAGE_DISTANCE_START;
		m_distance_display += STORAGE_DISTANCE_START;
		m_alt_distance_display += STORAGE_DISTANCE_START;
		m_coordinate_display += STORAGE_COORDINATE_START;
		m_area_display += STORAGE_AREA_START;
		m_volume_display += STORAGE_VOLUME_START;
		m_temp_display += STORAGE_TEMP_START;
		m_mass_display += STORAGE_MASS_START;
		m_energy_display += STORAGE_ENERGY_START;
		m_angle_display += STORAGE_FORMAT_ANGLE;
		m_velocity_display += STORAGE_DISTANCE_START;
		m_alt_velocity_display += STORAGE_DISTANCE_START;
		m_power_display += STORAGE_ENERGY_START;
	
		m_mass_area_display += ((UnitConvert::STORAGE_UNIT)STORAGE_MASS_START << 0x20);
		m_mass_area_display += STORAGE_AREA_START;

		m_intensity_display += ((UnitConvert::STORAGE_UNIT)STORAGE_ENERGY_START << 0x20);
		m_intensity_display += STORAGE_DISTANCE_START;
	}
  else
#endif
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
#ifdef _MSC_VER
	((CWinAppExt *)AfxGetApp())->WriteProfileULongLong(group_name, _T("Small Measure Display 4"), m_small_measure_display - STORAGE_DISTANCE_START);
	((CWinAppExt *)AfxGetApp())->WriteProfileULongLong(group_name, _T("Small Distance Display 4"), m_small_distance_display - STORAGE_DISTANCE_START);
	((CWinAppExt *)AfxGetApp())->WriteProfileULongLong(group_name, _T("Distance Display 4"), m_distance_display - STORAGE_DISTANCE_START);
	((CWinAppExt *)AfxGetApp())->WriteProfileULongLong(group_name, _T("Alternate Distance Display 4"), m_alt_distance_display - STORAGE_DISTANCE_START);
	((CWinAppExt *)AfxGetApp())->WriteProfileULongLong(group_name, _T("Geometric Display 4"), m_coordinate_display - STORAGE_COORDINATE_START);
	((CWinAppExt *)AfxGetApp())->WriteProfileULongLong(group_name, _T("Area Display 4"), m_area_display - STORAGE_AREA_START);
	((CWinAppExt *)AfxGetApp())->WriteProfileULongLong(group_name, _T("Volume Display 4"), m_volume_display - STORAGE_VOLUME_START);
	((CWinAppExt *)AfxGetApp())->WriteProfileULongLong(group_name, _T("Temperature Display 4"), m_temp_display - STORAGE_TEMP_START);
	((CWinAppExt *)AfxGetApp())->WriteProfileULongLong(group_name, _T("Mass Display 4"), m_mass_display - STORAGE_MASS_START);
	((CWinAppExt *)AfxGetApp())->WriteProfileULongLong(group_name, _T("Energy Display 4"), m_energy_display - STORAGE_ENERGY_START);
	((CWinAppExt *)AfxGetApp())->WriteProfileULongLong(group_name, _T("Angle Display 4"), m_angle_display - STORAGE_FORMAT_ANGLE);
	((CWinAppExt *)AfxGetApp())->WriteProfileULongLong(group_name, _T("Velocity Display 4"), m_velocity_display - STORAGE_DISTANCE_START);
	((CWinAppExt *)AfxGetApp())->WriteProfileULongLong(group_name, _T("Alternate Velocity Display 4"), m_alt_velocity_display - STORAGE_DISTANCE_START);
	((CWinAppExt *)AfxGetApp())->WriteProfileULongLong(group_name, _T("Mass/Area Display 4"), m_mass_area_display - ((UnitConvert::STORAGE_UNIT)STORAGE_MASS_START << 0x20) - STORAGE_AREA_START);
	((CWinAppExt *)AfxGetApp())->WriteProfileULongLong(group_name, _T("Intensity Display 4"), m_intensity_display - ((UnitConvert::STORAGE_UNIT)STORAGE_ENERGY_START << 0x20) - STORAGE_DISTANCE_START);
	((CWinAppExt *)AfxGetApp())->WriteProfileULongLong(group_name, _T("Power Display 5"), m_power_display - STORAGE_ENERGY_START);
#endif
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
