#pragma once

#include "validation_ids.h"
#include "hssconfig/config.h"

#include <string>
#include <list>
#include <stdexcept>
#include <typeinfo>
#include <variant>
#include <optional>
#include <memory>

#ifdef __GNUC__
#include <iostream>
#include <iomanip>
#include <cxxabi.h>
#endif

#ifdef _MSC_VER

#if (!defined(__INTEL_COMPILER)) && (!defined(__INTEL_LLVM_COMPILER))
#pragma managed(push, off)
#endif
#endif //_MSC_VER

#ifdef HSS_SHOULD_PRAGMA_PACK
#pragma pack(push, 4)					// force to go to 4-byte packing rules,
#endif /* _MSC_VER */					// to make this class as small as possible


#define GET_TEMPLATE_NAME(_type, str) do { template_helper<_type> _discard(str); } while(0)


template<typename T>
struct template_helper
{
	template_helper(std::string* str)
	{
		T x;
#ifdef _MSC_VER
		*str = typeid(x).name();
#else
		int status;
		char* name = abi::__cxa_demangle(typeid(x).name(), 0, 0, &status);
		if (name != 0)
			*str = std::string(name);
		else
			*str = typeid(x).name();
		free(name);
#endif
	}
};


namespace validation
{
	enum class error_level
	{
		NONE,
		INFORMATION,
		WARNING,
		SEVERE
	};

	/// <summary>
	/// The types that can be used when indicating a range of acceptable values for an invalid value.
	/// </summary>
	using range_value = std::variant<std::int32_t, double, std::string>;

	/// <summary>
	/// Information about a range of values that are allowed for a value.
	/// </summary>
	struct range_type
	{
		/// <summary>
		/// Is this end of the range inclusive or exclusive.
		/// </summary>
		bool is_inclusive;
		/// <summary>
		/// The value at this end of the range.
		/// </summary>
		range_value value;
	};

	/// <summary>
	/// Results of a validation check on an object.
	/// </summary>
	class validation_object
	{
	private:
		static error_level recursive_max_error_level(const std::list<std::shared_ptr<validation::validation_object>>& list) noexcept;

	public:
		/// <summary>
		/// The maximum error level of this node and its children.
		/// </summary>
		/// <returns>The maximum error level of this branch.</returns>
		error_level max_error_level() const noexcept;

	public:
		validation_object(const std::string& protobufObject, const std::string& name);
		validation_object(const std::string& protobufObject, std::initializer_list<std::string>&& names);
		validation_object(const validation_object&) = delete;
		validation_object(validation_object&&) = default;
		virtual ~validation_object();
		validation_object() = delete;
		validation_object& operator=(const validation_object&) = delete;
		validation_object& operator=(validation_object&&) = default;

		size_t child_count() const noexcept { return m_childValidation.size(); }
		/// <summary>
		/// Add a child node that is not an error but may contain children that are errors.
		/// </summary>
		/// <param name="protobufObject">The name of the protobuf object type that the node is being created for.</param>
		/// <param name="name">The object name that the error is being created for.</param>
		/// <returns>The newly added node.</returns>
		std::weak_ptr<validation_object> add_child_validation(const std::string& protobufObject, const std::string& name);
		/// <summary>
		/// Add a child node that is an error.
		/// </summary>
		/// <param name="protobufObject">The name of the protobuf object type that the node is being created for.</param>
		/// <param name="name">The object name that the error is being created for.</param>
		/// <param name="level">The severity of the error.</param>
		/// <param name="errorIdentifier">An identifier for the type of error being raised.</param>
		/// <param name="value">The value that caused the error.</param>
		/// <returns>The newly added node.</returns>
		std::weak_ptr<validation_object> add_child_validation(const std::string& protobufObject, const std::string& name, error_level level, std::string_view errorIdentifier, const std::string& value);
		/// <summary>
		/// Add a child node that is an error.
		/// </summary>
		/// <param name="protobufObject">The name of the protobuf object type that the node is being created for.</param>
		/// <param name="names">An array of object names that the error is being created for.</param>
		/// <param name="level">The severity of the error.</param>
		/// <param name="errorIdentifier">An identifier for the type of error being raised.</param>
		/// <param name="values">The values that caused the error. The length of this list must match <paramref name="names"/>.</param>
		/// <returns>The newly added node.</returns>
		std::weak_ptr<validation_object> add_child_validation(const std::string& protobufObject, std::initializer_list<std::string>&& names, error_level level, std::string_view errorIdentifier, std::initializer_list<std::string>&& values);
		/// <summary>
		/// Add a child node that is an error.
		/// </summary>
		/// <param name="protobufObject">The name of the protobuf object type that the node is being created for.</param>
		/// <param name="name">The object name that the error is being created for.</param>
		/// <param name="level">The severity of the error.</param>
		/// <param name="errorIdentifier">An identifier for the type of error being raised.</param>
		/// <param name="value">The value that caused the error.</param>
		/// <param name="errorMessage">Additional error descriptor to help the client code resolve the issue (describes logic).</param>
		/// <returns>The newly added node.</returns>
		std::weak_ptr<validation_object> add_child_validation(const std::string& protobufObject, const std::string& name, error_level level, std::string_view errorIdentifier, const std::string& value, const std::string& errorMessage);
		/// <summary>
		/// Add a child node that is an error.
		/// </summary>
		/// <param name="protobufObject">The name of the protobuf object type that the node is being created for.</param>
		/// <param name="names">An array of object names that the error is being created for.</param>
		/// <param name="level">The severity of the error.</param>
		/// <param name="errorIdentifier">An identifier for the type of error being raised.</param>
		/// <param name="values">The values that caused the error. The length of this list must match <paramref name="names"/>.</param>
		/// <param name="errorMessage">Additional error descriptor to help the client code resolve the issue (describes logic).</param>
		/// <returns>The newly added node.</returns>
		std::weak_ptr<validation_object> add_child_validation(const std::string& protobufObject, std::initializer_list<std::string>&& names, error_level level, std::string_view errorIdentifier, std::initializer_list<std::string>&& values, const std::string& errorMessage);
		/// <summary>
		/// Add a child node that is an error.
		/// </summary>
		/// <param name="protobufObject">The name of the protobuf object type that the node is being created for.</param>
		/// <param name="name">The object name that the error is being created for.</param>
		/// <param name="level">The severity of the error.</param>
		/// <param name="errorIdentifier">An identifier for the type of error being raised.</param>
		/// <param name="value">The value that caused the error.</param>
		/// <param name="min">The minimum value in a range of acceptable values.</param>
		/// <param name="max">The maximum value in a range of acceptable values.</param>
		/// <param name="errorMessage">Additional error descriptor to help the client code resolve the issue (describes logic).</param>
		/// <returns>The newly added node.</returns>
		std::weak_ptr<validation_object> add_child_validation(const std::string& protobufObject, const std::string& name, error_level level, std::string_view errorIdentifier, const std::string& value, range_type minimum, range_type maximum, const std::string& units = "", const std::string& errorMessage = "");

		/// <summary>
		/// The error level of this node.
		/// </summary>
		error_level errorLevel() const { return m_errorLevel; }
		/// <summary>
		/// An identifier for the type of error being raised.
		/// </summary>
		const std::string& errorIdentifier() const { return m_errorIdentifier; }
		/// <summary>
		/// The name of the protobuf object type that is raising an error.
		/// </summary>
		const std::string& protobufObject() const { return m_protobufObject; }
		/// <summary>
		/// The value that caused the error. Will only be serialized if there are no children.
		/// </summary>
		const std::list<std::string>& errorValue() const { return m_errorValue; }
		/// <summary>
		/// The object identifier/name (not type) that is associated with this error
		/// </summary>
		const std::list<std::string>& objectName() const { return m_objectName; }
		/// <summary>
		/// An optional minimum value for the range of allowed values.
		/// </summary>
		const std::optional<range_type>& minimum() const { return m_min; }
		/// <summary>
		/// An optional maximum value for the range of allowed values.
		/// </summary>
		const std::optional<range_type>& maximum() const { return m_max; }
		/// <summary>
		/// An optional string describing units for m_min, m_max.
		/// </summary>
		const std::optional<std::string>& units() const { return m_units; }
		/// <summary>
		/// An optional message that is associated with this error.
		/// </summary>
		const std::optional<std::string>& errorMessage() const { return m_errorMessage; }

		/// <summary>
		/// Child validation results.
		/// </summary>
		const std::list<std::shared_ptr<validation::validation_object>>& children() const { return m_childValidation; }

		std::weak_ptr<validation::validation_object> operator[](std::uint32_t index)
		{
			return std::weak_ptr<validation::validation_object>(*std::next(m_childValidation.begin(), index));
		}

	protected:
		/// <summary>
		/// The error level of this node.
		/// </summary>
		error_level m_errorLevel{ error_level::NONE };
		/// <summary>
		/// An identifier for the type of error being raised.
		/// </summary>
		std::string m_errorIdentifier;
		/// <summary>
		/// The name of the protobuf object type that is raising an error.
		/// </summary>
		std::string m_protobufObject;
		/// <summary>
		/// The value that caused the error. Will only be serialized if there are no children.
		/// </summary>
		std::list<std::string> m_errorValue;
		/// <summary>
		/// The object identifier/name (not type) that is associated with this error
		/// </summary>
		std::list<std::string> m_objectName;
		/// <summary>
		/// Child validation results.
		/// </summary>
		std::list<std::shared_ptr<validation::validation_object>> m_childValidation;
		/// <summary>
		/// An optional minimum value for the range of allowed values.
		/// </summary>
		std::optional<range_type> m_min;
		/// <summary>
		/// An optional maximum value for the range of allowed values.
		/// </summary>
		std::optional<range_type> m_max;
		/// <summary>
		/// An optional string describing units for m_min, m_max.
		/// </summary>
		std::optional<std::string> m_units;
		/// <summary>
		/// An optional error message describing the logic that caused the issue.
		/// </summary>
		std::optional<std::string> m_errorMessage;

		void make_error(error_level level, std::string_view errorIdentifier, const std::string& value);
		void make_error(error_level level, std::string_view errorIdentifier, std::initializer_list<std::string>&& value);
		/// <summary>
		/// Add a child node that has already been created.
		/// </summary>
		/// <param name="child">The child node to add.</param>
		/// <returns>A handle to the added node</returns>
		std::weak_ptr<validation::validation_object> add_child_validation(validation_object&& child);
	};

	/// <summary>
	/// Create a new validation object only if the parent exists.
	/// </summary>
	/// <param name="parent">The parent of the new validation object. If null, null will be returned.</param>
	/// <param name="protobufType">The name of the protobuf type that has caused an error.</param>
	/// <param name="name">The name of the errouneous object.</param>
	/// <returns>Null if parent is null, otherwise a smart pointer to a new validation object.</returns>
	inline std::weak_ptr<validation::validation_object> conditional_make_object(validation::validation_object* parent, const std::string& protobufType, const std::string& name)
	{
		if (parent)
			return parent->add_child_validation(protobufType, name);
		else
			return {};
	}

	/// <summary>
	/// Create a new validation object only if the parent exists.
	/// </summary>
	/// <param name="parent">The parent of the new validation object. If null, null will be returned.</param>
	/// <param name="protobufType">The name of the protobuf type that has caused an error.</param>
	/// <param name="name">The name of the errouneous object.</param>
	/// <returns>Null if parent is null, otherwise a smart pointer to a new validation object.</returns>
	inline std::weak_ptr<validation::validation_object> conditional_make_object(const std::shared_ptr<validation::validation_object>& parent, const std::string& protobufType, const std::string& name)
	{
		if (parent)
			return parent->add_child_validation(protobufType, name);
		else
			return {};
	}
}

#ifdef HSS_SHOULD_PRAGMA_PACK
#pragma pack(pop)
#endif

#ifdef _MSC_VER
#if (!defined(__INTEL_COMPILER)) && (!defined(__INTEL_LLVM_COMPILER))
#pragma managed(pop)
#endif
#endif
