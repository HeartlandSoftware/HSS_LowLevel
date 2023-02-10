/**
 * validation_object.h
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

#include "intel_check.h"
#include "validation_object.h"
#include "types.h"
#include <algorithm>
#include <memory>


validation::error_level validation::validation_object::recursive_max_error_level(const std::list<std::shared_ptr<validation::validation_object>>& list) noexcept
{
	auto level = error_level::NONE;
	for (auto& child : list)
	{
		const auto l = child->max_error_level();
		if (l > level)
		{
			level = l;
			if (level >= error_level::SEVERE)
				break;
		}
	}
	return level;
}


validation::error_level validation::validation_object::max_error_level() const noexcept
{
	if (m_childValidation.size() == 0)
		return m_errorLevel;
	const auto level = recursive_max_error_level(m_childValidation);
	if (level > m_errorLevel)		// getting rid of std::max to avoid non-standard MSVC crap
		return level;
	return m_errorLevel;
}


validation::validation_object::validation_object(const std::string& protobufObject, const std::string& name)
	: m_protobufObject(protobufObject)
{
	if (name.length())
		m_objectName.push_back(name);
	if (!m_protobufObject.size()) {
		weak_assert(false);
		throw std::invalid_argument("Validation nodes must have a protobuf type name");
	}
	//check that an object name was passed and that none of the object names are empty
	if (!m_objectName.size() || std::any_of(m_objectName.begin(), m_objectName.end(), [](const std::string& name) { return name.size() == 0; })) {
		weak_assert(false);
		throw std::invalid_argument("Validation nodes must have an object name");
	}
}


validation::validation_object::validation_object(const std::string& protobufObject, std::initializer_list<std::string>&& names)
	: m_protobufObject(protobufObject),
	  m_objectName(std::forward<decltype(names)>(names))
{
	if (!m_protobufObject.size()) {
		weak_assert(false);
		throw std::invalid_argument("Validation nodes must have a protobuf type name");
	}
	//check that an object name was passed and that none of the object names are empty
	if (!m_objectName.size() || std::any_of(m_objectName.begin(), m_objectName.end(), [](const std::string& name) { return name.size() == 0; })) {
		weak_assert(false);
		throw std::invalid_argument("Validation nodes must have an object name");
	}
}


validation::validation_object::~validation_object()
{
	m_childValidation.clear();
}


void validation::validation_object::make_error(error_level level, std::string_view errorIdentifier, const std::string& value)
{
	m_errorLevel = level;
	m_errorIdentifier = errorIdentifier;
	if (!m_errorIdentifier.size())
		throw std::invalid_argument("Validation nodes must have an error identifier");
	m_errorValue.push_back(value);
}


void validation::validation_object::make_error(error_level level, std::string_view errorIdentifier, std::initializer_list<std::string>&& value)
{
	m_errorLevel = level;
	m_errorIdentifier = errorIdentifier;
	if (!m_errorIdentifier.size())
		throw std::invalid_argument("Validation nodes must have an error identifier");
	m_errorValue.insert(m_errorValue.end(), std::forward<decltype(value)>(value));
}


std::weak_ptr<validation::validation_object> validation::validation_object::add_child_validation(validation_object&& child)
{
	m_childValidation.push_back(std::make_shared<validation::validation_object>(std::forward<decltype(child)>(child)));
	return m_childValidation.back();
}


std::weak_ptr<validation::validation_object> validation::validation_object::add_child_validation(const std::string& protobufObject, const std::string& name,
	error_level level, std::string_view errorIdentifier, const std::string& value)
{
	validation::validation_object v(protobufObject, name);
	v.make_error(level, errorIdentifier, value);
	return add_child_validation(std::move(v));
}


std::weak_ptr<validation::validation_object> validation::validation_object::add_child_validation(const std::string& protobufObject, std::initializer_list<std::string>&& names,
	error_level level, std::string_view errorIdentifier, std::initializer_list<std::string>&& values)
{
	validation::validation_object v(protobufObject, std::forward<decltype(names)>(names));
	v.make_error(level, errorIdentifier, std::forward<decltype(values)>(values));
	if (v.m_objectName.size() != v.m_errorValue.size())
		throw std::invalid_argument("The number of values did not match the number of object names.");
	return add_child_validation(std::move(v));
}


std::weak_ptr<validation::validation_object> validation::validation_object::add_child_validation(const std::string& protobufObject, const std::string& name,
	error_level level, std::string_view errorIdentifier, const std::string& value, const std::string& errorMessage)
{
	validation::validation_object v(protobufObject, name);
	v.make_error(level, errorIdentifier, value);
	v.m_errorMessage = std::make_optional(errorMessage);
	return add_child_validation(std::move(v));
}


std::weak_ptr<validation::validation_object> validation::validation_object::add_child_validation(const std::string& protobufObject, std::initializer_list<std::string>&& names,
	error_level level, std::string_view errorIdentifier, std::initializer_list<std::string>&& values, const std::string& errorMessage)
{
	validation::validation_object v(protobufObject, std::forward<decltype(names)>(names));
	v.make_error(level, errorIdentifier, std::forward<decltype(values)>(values));
	v.m_errorMessage = std::make_optional(errorMessage);
	if (v.m_objectName.size() != v.m_errorValue.size())
		throw std::invalid_argument("The number of values did not match the number of object names.");
	return add_child_validation(std::move(v));
}


std::weak_ptr<validation::validation_object> validation::validation_object::add_child_validation(const std::string& protobufObject, const std::string& name,
	validation::error_level level, std::string_view errorIdentifier, const std::string& value,
	validation::range_type minimum, validation::range_type maximum, const std::string& units, const std::string& errorMessage)
{
	validation::validation_object v(protobufObject, name);
	v.make_error(level, errorIdentifier, value);
	v.m_min = std::make_optional(minimum);
	v.m_max = std::make_optional(maximum);
	v.m_units = std::make_optional(units);
	v.m_errorMessage = std::make_optional(errorMessage);
	return add_child_validation(std::move(v));
}


std::weak_ptr<validation::validation_object> validation::validation_object::add_child_validation(const std::string& protobufObject, const std::string& name)
{
	validation::validation_object v(protobufObject, name);
	return add_child_validation(std::move(v));
}
