/**
 * validation_ids.h
 *
 * Copyright 2020-2023 Heartland Software Solutions Inc.
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

#pragma once

#include <string>

namespace validation
{
	namespace id
	{
		/// <summary>
		/// The version of the protobuf object is not known by the current version of Prometheus.
		/// </summary>
		constexpr std::string_view version_mismatch = "Deserialize.Version:Invalid";
		/// <summary>
		/// The version of the protobuf object is not current with this version of PSaaS.
		/// </summary>
		constexpr std::string_view version_not_current = "Deserialize.Version:NotCurrent";
		/// <summary>
		/// A protobuf message passed to a deserialize method was an incorrect type to be processed by
		/// that instance.
		/// </summary>
		constexpr std::string_view object_invalid = "Deserialize.Object:Invalid";
		/// <summary>
		/// A protobuf message passed to a deserialize method is missing some necessary members.
		/// </summary>
		constexpr std::string_view object_incomplete = "Deserialize.Object:Incomplete";
		/// <summary>
		/// An object could not be parsed.
		/// </summary>
		constexpr std::string_view parse_failed = "Deserialize.Parse:Failed";
		/// <summary>
		/// An object could be parsed but contains an invalid value.
		/// </summary>
		constexpr std::string_view parse_invalid = "Deserialize.Parse:Invalid";
		/// <summary>
		/// An index value was parsed but contains an invalid value.
		/// </summary>
		constexpr std::string_view index_invalid = "Deserialize.Index:Invalid";
		/// <summary>
		/// An enumerated value was parsed but contains an invalid value.
		/// </summary>
		constexpr std::string_view enum_invalid = "Deserialize.Enum:Invalid";
		/// <summary>
		/// A 'oneof' set was parsed but contains an invalid/unknown/missing value.
		/// </summary>
		constexpr std::string_view oneof_invalid = "Deserialize.Oneof:Invalid";
		/// <summary>
		/// A value was parsed but contains an invalid value.
		/// </summary>
		constexpr std::string_view value_invalid = "Deserialize.Value:Invalid";
		/// <summary>
		/// A required projection is missing for a georeferenced input file.
		/// </summary>
		constexpr std::string_view projection_missing = "File.Projection:Missing";
		/// <summary>
		/// Internal error: The geospatial reference for an object could not be loaded.
		/// </summary>
		constexpr std::string_view missing_georeference = "Initialization.SpatialReference:NotPresent";
		/// <summary>
		/// Internal error: The timezone for an object could not be loaded.
		/// </summary>
		constexpr std::string_view missing_timezone = "Initialization.TimeZone:NotPresent";
		/// <summary>
		/// A required internal property cannot be read, initialization must have failed previously.
		/// </summary>
		constexpr std::string_view initialization_incomplete = "Deserialize.Initialize:Incomplete";
		/// <summary>
		/// A user specified file cannot be found on the filesystem.
		/// </summary>
		constexpr std::string_view missing_file = "Input.File:Missing";

		/// <summary>
		/// A compressed archive in the FGM file could not be decompressed, it may be corrupt.
		/// </summary>
		constexpr std::string_view archive_decompress = "Archive.Decompress:Invalid";

		/// <summary>
		/// The file specified as a wind direction grid is not a valid file that can be opened by GDAL.
		/// </summary>
		constexpr std::string_view wind_grid_invalid = "Import.WindGrid:Invalid";
		/// <summary>
		/// The projection of the grid does not match the projection of the base grids.
		/// </summary>
		constexpr std::string_view grid_projection_mismatch = "Import.Grid:Projection";
		/// <summary>
		/// The resolution of the grid does not match the projection of the base grids.
		/// </summary>
		constexpr std::string_view grid_resolution_mismatch = "Import.Grid:Projection";
		/// <summary>
		/// The sector specified for a wind direction grid is not valid.
		/// </summary>
		constexpr std::string_view wind_grid_sector = "Import.WindGrid:Sector";
		/// <summary>
		/// The size of the grid doesn't match the base grids.
		/// </summary>
		constexpr std::string_view grid_size_mismatch = "Import.Grid:Size";
		/// <summary>
		/// A direction in a wind direction grid is not a valid wind direction (must be in [0, 360]).
		/// </summary>
		constexpr std::string_view wind_grid_direction = "Import.WindGrid:Direction";
		/// <summary>
		/// A speed in a wind speed grid is not a valid wind speed (must be in [0, 250]).
		/// </summary>
		constexpr std::string_view wind_grid_speed = "Import.WindGrid:Speed";
		/// <summary>
		/// A wind direction grid doesn't have a wind direction specified.
		/// </summary>
		constexpr std::string_view wind_direction_missing = "Wind.Direction:Missing";
		/// <summary>
		/// The incorrect grid type was specified (direction instead of speed or vice versa).
		/// </summary>
		constexpr std::string_view grid_type_invalid = "Grid.Type:Invalid";

		/// <summary>
		/// A user specified file doesn't contain any fuel information.
		/// </summary>
		constexpr std::string_view missing_fuels = "Input.CWFGM.GridProto.CwfgmFuelMap:Missing";
		/// <summary>
		/// A user specified file doesn't contain any fuel grid information.
		/// </summary>
		constexpr std::string_view missing_fuel_grid = "Input.CWFGM.GridProto.CwfgmGrid:Missing";
		/// <summary>
		/// A user specified fuel grid file references an unknown fuel.
		/// </summary>
		constexpr std::string_view unknown_fuel = "Input.CWFGM.GridProto.CwfgmGrid:UnknownFuel";
		/// <summary>
		/// A user specified file doesn't contain any weather station information.
		/// </summary>
		constexpr std::string_view missing_weather = "Input.CWFGM.ProjectProto.StationCollection:Missing";
		/// <summary>
		/// A user specified file doesn't contain any ignition information.
		/// </summary>
		constexpr std::string_view missing_ignitions = "Input.CWFGM.ProjectProto.FireCollection:Missing";
		/// <summary>
		/// A user specified file doesn't contain any scenario information.
		/// </summary>
		constexpr std::string_view missing_scenarios = "Input.CWFGM.ProjectProto.ScenarioCollection:Missing";
		/// <summary>
		/// A user specified file contains at least one invalid scenario / grid export combination.
		/// </summary>
		constexpr std::string_view invalid_scenario_options = "Input.CWFGM.ProjectProto.ScenarioCollection.Scenario:InvalidOptions";
		/// <summary>
		/// The object doesn't have the required name, used for identification.
		/// </summary>
		constexpr std::string_view missing_name = "Import.Object.Name:Missing";
		/// <summary>
		/// The object doesn't have the required export filename.
		/// </summary>
		constexpr std::string_view missing_filename = "Import.Object.Filename:Missing";

		/// <summary>
		/// The object's state is invalid for the operation.
		/// </summary>
		constexpr std::string_view state_invalid = "State:InValid";
		/// <summary>
		/// A scenario can't reset without any ignitions to model.
		/// </summary>
		constexpr std::string_view scenario_missing_ignitions = "Ignitions:Missing";

		/// <summary>
		/// The time is invalid.
		/// </summary>
		constexpr std::string_view time_invalid = "Time:InValid";
		/// <summary>
		/// A timespan is not within the required range.
		/// </summary>
		constexpr std::string_view time_range_invalid = "Time.Range:InValid";

		/// <summary>
		/// A specified FFMC value is not valid (must be in [0, 101]).
		/// </summary>
		constexpr std::string_view ffmc_invalid = "FWI.FFMC:Invalid";
		/// <summary>
		/// A specified DMC value is not valid (must be in [0, 500]).
		/// </summary>
		constexpr std::string_view dmc_invalid = "FWI.DMC:Invalid";
		/// <summary>
		/// A specified DC value is not valid (must be in [0, 1500]).
		/// </summary>
		constexpr std::string_view dc_invalid = "FWI.DC:Invalid";
		/// <summary>
		/// A specified BUI value is not valid (must be greater than 0).
		/// </summary>
		constexpr std::string_view bui_invalid = "FWI.BUI:Invalid";

		/// <summary>
		/// No weather values were found in the specified weather stream.
		/// </summary>
		constexpr std::string_view missing_weather_data = "WX.Hourly:Missing";
		/// <summary>
		/// No daily weather values were found in the specified weather stream.
		/// </summary>
		constexpr std::string_view missing_daily_weather_data = "WX.Daily:Missing";
		/// <summary>
		/// No weather values were found in the specified weather stream.
		/// </summary>
		constexpr std::string_view incorrect_amt_weather_data = "WX.Hourly:IncorrectCount";

		/// <summary>
		/// An imported hourly weather value is not within the valid range for its type.
		/// </summary>
		constexpr std::string_view invalid_weather = "WX.Hourly.Invalid";

		/// <summary>
		/// An imported hourly weather value started after noon.
		/// </summary>
		constexpr std::string_view invalid_weather_start = "WX.Hourly.InvalidStartHour";

		/// <summary>
		/// The scenario cannot add the specified element.
		/// </summary>
		constexpr std::string_view add_element_failed = "Object.AddElement:Failed";
		/// <summary>
		/// The scenario cannot set the specified weather stream as the primary stream.
		/// </summary>
		constexpr std::string_view primary_stream_failed = "ProjectScenario.PrimaryStream:Failed";
		/// <summary>
		/// The scenario cannot remove the specified element.
		/// </summary>
		constexpr std::string_view remove_element_failed = "Object.RemoveElement:Failed";
		/// <summary>
		/// The edit cannot proceed with the simulation executing.
		/// </summary>
		constexpr std::string_view simulation_running = "Object.Edit:Failed.SimulationRunning";

		/// <summary>
		/// The system is out of memory.
		/// </summary>
		constexpr std::string_view out_of_memory = "CWFGM.Internal:OutOfMemory";
		/// <summary>
		/// The system cannot create an object (likely related to COM registration issues).
		/// </summary>
		constexpr std::string_view cannot_allocate = "CWFGM.Internal:CannotAllocate";
		/// <summary>
		/// The system cannot obtain a pointer to another interface for an object (likely related to COM registration issues).
		/// </summary>
		constexpr std::string_view cannot_obtain_interface = "CWFGM.Internal:InterfaceUnknown";
		/// <summary>
		/// The system cannot convert a type of an object to what we're expected.
		/// </summary>
		constexpr std::string_view cannot_convert_type = "CWFGM.Internal:ObjectTypeUnknown";
		/// <summary>
		/// Internal error, reporting E_POINTER
		/// </summary>
		constexpr std::string_view e_pointer = "CWFGM.Internal:EPointer";
		/// <summary>
		/// Internal error, reporting E_INVALIDARG
		/// </summary>
		constexpr std::string_view e_invalidarg = "CWFGM.Internal:EInvalidArg";

		/// <summary>
		/// File not found
		/// </summary>
		constexpr std::string_view file_not_found = "System:FileNotFound";
		/// <summary>
		/// Too many open files
		/// </summary>
		constexpr std::string_view too_many_open_files = "System:TooManyOpenFiles";
		/// <summary>
		/// Access denied
		/// </summary>
		constexpr std::string_view access_denied = "System:AccessDenied";
		/// <summary>
		/// Invalid handle
		/// </summary>
		constexpr std::string_view invalid_handle = "System:InvalidHandle";
		/// <summary>
		/// Disk full
		/// </summary>
		constexpr std::string_view disk_full = "System:DiskFull";
		/// <summary>
		/// File exists
		/// </summary>
		constexpr std::string_view file_exists = "System:FileExists";
		/// <summary>
		/// Unknown/unspecified error, likely associated with the COM HRESULT of ERROR_SEVERITY_WARNING
		/// </summary>
		constexpr std::string_view unspecified = "CWFGM:Unspecified";

	}
}
