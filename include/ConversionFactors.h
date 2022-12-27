#ifndef _CONVERSION_FACTORS_H__
#define _CONVERSION_FACTORS_H__

#ifdef _MSC_VER
#if (!defined(__INTEL_COMPILER)) && (!defined(__INTEL_LLVM_COMPILER))
#pragma managed(push, off)
#endif
#endif

#if __has_include(<mathimf.h>)
#include <mathimf.h>
#else
#include <cmath>
#endif


// These conversions were found at:
// http://www.soton.ac.uk/~scp93ch/units/index.html?http://www.soton.ac.uk/~scp93ch/units/convfact.html

/*
		   millimetre (mm) to metre (m): 1E-3
                   centimetre (cm) to m: 1E-2
                   kilometre (km) to m: 1E3

                   inch (in) to m: 2.54E-2
                   foot (ft) to m: 3.048E-1 (12 in)
                   yard (yd) to m: 0.9144 exactly (3 ft)
                   fathom to m: 1.8288 (2 yd)
                   chain to m: 2.01168E1 (22 yd)
                   furlong to m: 2.01168E2 (10 chains, 220 yd)
                   cable - not precisely defined, 608ft (1/10 UK nautical mile) or 720 ft (120 fathoms)
                   mile to m: 1.609344E3 (1760 yd)
                   nautical mile (UK) to m: 1.853184E3 (6080 ft)

                   nautical mile (international) (n mile) to m: 1.852E3
*/

/*
// RWB: 2011-06-24:	toying with moving all conversions to a standard format for storage as below (except for circular), need to think about
//			because although it's cleaner to look at, it may not necessarily be faster for all actual conversions (even if it is
//			faster to do look-ups for name->unit, etc.), so will consider it for a while before proceeding.

struct converters {
	std::uint32_t unit;
	double add, mult;
	const char *short_format;
	const char *long_format[5];
};

static struct converters convert_table[] = {
	{ STORAGE_FORMAT_MM, 0.0, 1e-3, "mm", "millimetre", "millimeter", NULL, NULL, NULL },
	{ STORAGE_FORMAT_CM, 0.0, 1e-2, "cm", "centimetre", "centimeter", NULL, NULL, NULL },
	{ STORAGE_FORMAT_M, 0.0, 1.0, "m", "metre", "meter", NULL, NULL, NULL },
	{ STORAGE_FORMAT_KM, 0.0, 1e3, "km", "kilometre", "kilometer", NULL, NULL, NULL },
	{ STORAGE_FORMAT_INCH, 0.0, 2.54e-2, "in", "inch", NULL, NULL, NULL, NULL },
	{ STORAGE_FORMAT_FOOT, 0.0, 3.048e-1, "ft", "foot", NULL, NULL, NULL, NULL },
	{ STORAGE_FORMAT_YARD, 0.0, 0.9144, "yd", "yard", NULL, NULL, NULL, NULL },
	{ STORAGE_FORMAT_CHAIN, 0.0, 2.01168e1, "chain", "chain", NULL, NULL, NULL, NULL },
	{ STORAGE_FORMAT_MILE, 0.0, 1.609344e3, "mi", "mile", NULL, NULL, NULL, NULL },
	{ STORAGE_FORMAT_NAUTICAL_MILE, 0.0, 1.854184e3, "n.mi", "nautical mile", NULL, NULL, NULL, NULL },
	{ STORAGE_FORMAT_MM2, 0.0, 1e-6, "mm²",
	{ STORAGE_FORMAT_CM2, 0.0, 1e-4, "cm²"
	{ STORAGE_FORMAT_M2, 0.0, 1.0, "m²" 
	{ STORAGE_FORMAT_HECTARE, 0.0, 1e4, "ha"
	{ STORAGE_FORMAT_KM2, 0.0, 1e6, "km²"
	{ STORAGE_FORMAT_IN2, 0.0, 6.4516e-4, "in²"
	{ STORAGE_FORMAT_FT2, 0.0, 9.290304e-2, "ft²"
	{ STORAGE_FORMAT_YD2, 0.0, 8.3612736e-1, "yd²"
	{ STORAGE_FORMAT_ACRE, 0.0, 4.0468564224e3, "acre"
	{ STORAGE_FORMAT_MILE2, 0.0, 2.58998811e6, "mi²"
	{ 0, 0.0, 0.0, NULL, NULL, NULL, NULL, NULL, NULL }
};
*/

static double distance_translation[] = { 1e-3, 1e-2, 1.0, 1e3, 2.54e-2, 3.048e-1, 0.9144, 2.01168e1, 1.609344e3, 1.852e3, 1.853184e3 };

/*
		   mm² to m²: 1E-6
                   cm² to m²: 1E-4
                   hectare to m²: 1E4
                   km² to m²: 1E6

                   in² to m²: 6.4516E-4
                   ft² to m²: 9.290304E-2 (144 in²)
                   yd² to m²: 8.3612736E-1 (9 ft²)
                   rood to m²: 1.01171E3 (1210 yd²)
                   acre to m²: 4.0468564224E3 (4840 yd²)
                   mile² to m²: 2.58998811E6 (640 acres)
*/

static double area_translation[] = {
	1e-6,
	1e-4,
	1.0,
	1e4,
	1e6,
	6.4516e-4,
	9.290304e-2,
	8.3612736e-1,
	4.0468564224e3,
	2.58998811e6
};

/*
		   mm³ to m³: 1E-9
                   cm³ / ml to m³: 1E-6
                   dm³ / l to m³: 1E-3
                   km³ to m³: 1E9

                   in³ to m³: 1.6387064E-5
                   ft³ to m³: 2.8316846E-2 (1728 in³)
                   yd³ to m³: 7.64554858E-1 (27 ft³)
                   mile³ to m³: 4.168181825E9 (5451776000 yd³)

                   UK
                   minim (UKmin) to m³: 5.91939E-8
                   fluid drachm (UK fl dr) to m³: 3.55163E-6 (60 UKmin)
                   fluid ounce (UK fl oz) to m³: 2.84131E-5 (8 UK fl dr)
                   gill to m³: 1.42065E-4 (5 UK fl oz)
                   pint (UKpt) to m³: 5.68261E-4 (4 gill)
                   quart (UKqt) to m³: 1.13652E-3 (2 UKpt)
                   gallon (UKgal) to m³: 4.54609E-3 (4 UKqt)
                   peck to m³: 9.09218E-3 (2 UKgal)
                   bushel to m³: 3.63687E-2 (4 peck)

                   US (liquid)
                   minim (USmin) to m³: 6.16115E-8
                   fluid dram (US fl dr) to m³: 3.69669E-6 (60 USmin)
                   liquid ounce (US liq oz) to m³: 2.95735E-5 (8 US fl dr)
                   gill (gi) to m³: 1.18294E-4 (4 US liq oz)
                   liquid pint (US liq pt) to m³: 4.73176E-4 (4 gi)
                   liquid quart (US liq qt) to m³: 9.46353E-4 (2 US liq pt)
                   gallon (USgal) to m³: 3.785411784E-3 (4 US liq qt)
                   barrel to m³: 1.58987E-1 (42 USgal)

                   US (dry)
                   dry pint (dry pt) to m³: 5.50610E-4
                   dry quart (dry qt) to m³: 1.10122E-3 (2 dry pt)
                   peck (pk) to m³: 8.80977E-3 (8 dry qt)
                   bushel (bu) to m³: 3.52391E-2 (4 pk)
                   dry barrel (bbl) to m³: 1.10122E-3 (210 dry pt?)
*/

static double volume_translation[] = {
	0.000000001,
	0.000001,
	0.001,
	1.0,
	1000000000.0,
	1.6387064e-5,
	2.8316846e-2,
	7.64554858e-1,
	4.168181825e9,
	2.84131e-5,
	5.68261e-4,
	1.13652e-3,
	4.54609e-3,
	3.63687e-2,
	3.69669e-6,
	2.95735e-5,
	4.73176e-4,
	9.46353e-4,
	3.785411784e-3,
	1.58987e-1,
	5.50610e-4,
	1.10122e-3,
	1.10122e-3
};

/*
		   Kelvin (K): x K 
                        = (x-273.15) °C 
                        = (5/4)(x-273.15) °Réaumur 
                        = (9/5)x-459.67 °F 
                        = (9/5)x °Rankine 
                   Celsius (also centigrade) (°C): x °C 
                        = (x+273.15) K 
                        = (5/4)x °Réaumur 
                        = (1.8x+32) °F 
                        = (1.8x+459.67) °Rankine
                   Réaumur (°R): x °Réaumur 
                        = (4/5)x °C
                   Fahrenheit (°F): x °F 
                        = (5/9)(x+459.67) K 
                        = (5/9)(x-32) °C 
                        = (25/36)(x-32) °Réaumur 
                        = (x+459.67) °Rankine
                   Rankine (°R): x °Rankine 
                        = (5/9)x K 
                        = (5/9)(x-491.67) °C 
                        = (25/36)(x-491.67) °Réaumur 
                        = (x-459.67) °F 
*/

static double	temp_translate1[] = { 0.0, 273.15, 459.67, 0.0 },
		temp_translate2[] = { 1.0, 1.0, 9.0 / 5.0, 9.0 / 5.0 };

/*
		   milligram (mg) to kilogram (kg): 1E-6
                   gram (g) to kg: 1E-3
                   tonne to kg: 1E3 (1 megagram or Mg)

                   UK (Avoirdupois)
                   dram (dr) to kg: 1.77185E-3 (7000/256 grain)
                   ounce (oz) to kg: 2.83495E-2 (16 dr)
                   pound (lb) to kg: 0.45359237 exactly (16 oz and 7000 grain)
                   stone to kg: 6.35029 (14 lb)
                   slug (also gee pound) to kg: 1.45939E1
                   quarter (qr) to kg: 1.27006E1 (2 stone)
                   hundredweight (cwt) to kg: 5.08023E1 (4 qr and 112 lb)
                   ton (UK) (UKton) to kg: 1.016047E3 (20 cwt and 1.12 short ton)

                   US
                   dram (dr) to kg: 1.77185E-3 (as UK)
                   ounce (oz) to kg: 2.83495E-2 (as UK)
                   pound (lb) to kg: 0.45359237 (as UK)
                   short hundredweight (sh cwt) to kg: 4.53592E1 (100 lb)
                   short ton (sh tn / USton) to kg: 9.07185E2 (20 sh cwt)
*/

static double mass_translation[] = {
	1e-6, 1e-3, 1.0, 1e3, 2.83495e-2, 0.45359237, 9.07185e2, 1.016047e3
};

/*
                   Energy

                   1 joule (J) is equal to one newton metre (N.m)
                   electronvolt (eV) to J: 1.6021892E-19 (1e*1V)
                   erg to J: 1E-7
                   foot pound-force (ft.lbf) to J: 1.35582
                   calorie (cal) to J: 4.1868
                   kilogram-force metre (kgf.m) to J: 9.80665
                   British Thermal Unit (Btu) to J: 1.05506E3
                   watt hour (W.h) to J: 3.6E3
                   kilowatt hour (kW.h) to J: 3.6E6
                   therm to J: 1.05506E8 (1E5 Btu)
*/

static double energy_translation[] = {
	1.0, 1.6021892E-19, 1E-7, 1.35582, 4.1868, 9.80665, 1.05506E3, 3.6E3 / 3600.0, 3.6E6 / 3600.0, 1.05506E8, 1000.0, 3.6E9 / 3600.0
};

/*
                   Power

                   1 watt (W) is equal to one joule per second (J/s or m2kgs-3)
                   foot pound-force per second (ft.lbf/s) to W: 1.35582
                   kilogram-force metre per second (kgf.m/s) to W: 9.80665
                   horsepower (metric) to W: 7.35499E2 (75 kgf.m/s) 
                   horsepower (hp) to W: 7.45700E2 (550 ft.lbf/s)
*/

/*
                   Force

                   dyne (dyn) to newton (N): 1E-5
                   poundal (pdl) to N: 1.38255E-1
                   pound-force (lbf) to N: 4.44822
                   kilogram-force (kgf) or kilopond (kp) to N: 9.80665
                   sthène (sn) to N: 1E3
*/

/*
					Pressure

					kPa to kPa: 1.0
					psi to kPa: 6.895
					bar to kPa: 100
					atm to kPa: 101.325
					torr to kPa: 0.133322
*/

static double pressure_translation[] = {
	1.0, 6.895, 100, 101.325, 0.133322
};

#ifdef _MSC_VER
#if (!defined(__INTEL_COMPILER)) && (!defined(__INTEL_LLVM_COMPILER))
#pragma managed(pop)
#endif
#endif

#endif //_CONVERSION_FACTORS_H__
