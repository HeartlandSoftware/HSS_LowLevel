//	dlgcnvt.h
//
//	By Robert W. Bryce
//	Started:	April 3, 2000
//	Last Modified:	January 13, 2009

#ifndef __DLGCNVT_H
#define __DLGCNVT_H

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

	__INLINE UnitConvert::STORAGE_UNIT SmallMeasureDisplay() const		{ return m_small_measure_display; };
	__INLINE UnitConvert::STORAGE_UNIT SmallDistanceDisplay() const		{ return m_small_distance_display; };
	__INLINE UnitConvert::STORAGE_UNIT DistanceDisplay() const			{ return m_distance_display; };
	__INLINE UnitConvert::STORAGE_UNIT AltDistanceDisplay() const		{ return m_alt_distance_display; };
	__INLINE UnitConvert::STORAGE_UNIT AreaDisplay() const				{ return m_area_display; };
	__INLINE UnitConvert::STORAGE_UNIT VolumeDisplay() const			{ return m_volume_display; };
	__INLINE UnitConvert::STORAGE_UNIT TempDisplay() const				{ return m_temp_display; };
	__INLINE UnitConvert::STORAGE_UNIT MassDisplay() const				{ return m_mass_display; };
	__INLINE UnitConvert::STORAGE_UNIT MassAreaDisplay() const			{ return m_mass_area_display; };
	__INLINE UnitConvert::STORAGE_UNIT EnergyDisplay() const			{ return m_energy_display; };
	__INLINE UnitConvert::STORAGE_UNIT AngleDisplay() const				{ return m_angle_display; };
	__INLINE UnitConvert::STORAGE_UNIT VelocityDisplay() const			{ return m_velocity_display; };
	__INLINE UnitConvert::STORAGE_UNIT AltVelocityDisplay() const		{ return m_alt_velocity_display; };
	__INLINE UnitConvert::STORAGE_UNIT CoordinateDisplay() const		{ return m_coordinate_display; };
	__INLINE UnitConvert::STORAGE_UNIT IntensityDisplay() const			{ return m_intensity_display; };
	__INLINE UnitConvert::STORAGE_UNIT PowerDisplay() const				{ return m_power_display; }

	static __INLINE float ConvertUnit(float value, UnitConvert::STORAGE_UNIT from_format, UnitConvert::STORAGE_UNIT to_format)		{ return UnitConvert::convertUnit(value, to_format, from_format); };	// **** note reversal of parmeter order
	static __INLINE double ConvertUnit(double value, UnitConvert::STORAGE_UNIT from_format, UnitConvert::STORAGE_UNIT to_format)	{ return UnitConvert::convertUnit(value, to_format, from_format); };
	static __INLINE std::string UnitName(UnitConvert::STORAGE_UNIT format, bool short_format)										{ return UnitConvert::UnitName(format, short_format); }

#if _DLL
	static __INLINE CString CUnitName(UnitConvert::STORAGE_UNIT format, bool short_format)											{ return CString(UnitConvert::UnitName(format, short_format).c_str()); }
#endif

};


#if _DLL
void AFXAPI DDX_TextConvert(CDataExchange* pDX, int nIDC, float& value, UnitConvert::STORAGE_UNIT storage_format, UnitConvert::STORAGE_UNIT display_format);
void AFXAPI DDX_TextConvert(CDataExchange* pDX, int nIDC, double& value, UnitConvert::STORAGE_UNIT storage_format, UnitConvert::STORAGE_UNIT display_format);
void AFXAPI DDX_TextConvert(CDataExchange* pDX, int nIDC, float& value, int precision, UnitConvert::STORAGE_UNIT storage_format, UnitConvert::STORAGE_UNIT display_format, bool pad = false);
void AFXAPI DDX_TextConvert(CDataExchange* pDX, int nIDC, double& value, int precision, UnitConvert::STORAGE_UNIT storage_format, UnitConvert::STORAGE_UNIT display_format, bool pad = false);
void AFXAPI DDX_TextConvert(CDataExchange* pDX, int nIDC, float& value, float& def_value, int precision, UnitConvert::STORAGE_UNIT storage_format, UnitConvert::STORAGE_UNIT display_format, bool pad = false);
void AFXAPI DDX_TextConvert(CDataExchange* pDX, int nIDC, double& value, double& def_value, int precision, UnitConvert::STORAGE_UNIT storage_format, UnitConvert::STORAGE_UNIT display_format, bool pad = false);
void AFXAPI DDX_TextConvert(CDataExchange *pDX, int nIDC, float& value, const char *format, UnitConvert::STORAGE_UNIT storage_format, UnitConvert::STORAGE_UNIT display_format);
void AFXAPI DDX_TextConvert(CDataExchange *pDX, int nIDC, double& value, const char *format, UnitConvert::STORAGE_UNIT storage_format, UnitConvert::STORAGE_UNIT display_format);
void AFXAPI DDX_TextConvert(CDataExchange *pDX, int nIDC, float& value, float& def_value, const char *format, UnitConvert::STORAGE_UNIT storage_format, UnitConvert::STORAGE_UNIT display_format);
void AFXAPI DDX_TextConvert(CDataExchange *pDX, int nIDC, double& value, double& def_value, const char *format, UnitConvert::STORAGE_UNIT storage_format, UnitConvert::STORAGE_UNIT display_format);
#endif

#ifdef HSS_SHOULD_PRAGMA_PACK
#pragma pack(pop)			// go back to whatever the project
#endif

#ifdef _MSC_VER
#if !defined(__INTEL_COMPILER) && !defined(__INTEL_LLVM_COMPILER)
#pragma managed(pop)
#endif

#endif					// settings are...

#endif

