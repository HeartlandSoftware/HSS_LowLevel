/**
 * boost_compression.h
 *
 * Copyright 2012-2023 Heartland Software Solutions Inc.
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

#define BOOST_ZLIB_BINARY zlib //precompiled zlib library
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/filter/zlib.hpp>
#include <boost/iostreams/copy.hpp>


namespace Compress
{
	enum class CompressionType
	{
		NONE,
		GZIP,
		ZIP
	};

	inline std::string compress(const char* data, size_t size, CompressionType type = Compress::CompressionType::GZIP)
	{
		boost::iostreams::filtering_istreambuf out;
		if (type == CompressionType::GZIP)
			out.push(boost::iostreams::gzip_compressor());
		else if (type == CompressionType::ZIP)
			out.push(boost::iostreams::zlib_compressor());
		out.push(boost::iostreams::array_source(data, size));
		std::stringstream ss;
		boost::iostreams::copy(out, ss);
		return ss.str();
	}

	inline std::string decompress(const std::string& data, CompressionType type = Compress::CompressionType::GZIP)
	{
		std::stringstream array(data);
		boost::iostreams::filtering_istreambuf in;
		if (type == CompressionType::GZIP)
			in.push(boost::iostreams::gzip_decompressor());
		else if (type == CompressionType::ZIP)
			in.push(boost::iostreams::zlib_decompressor());
		in.push(array);
		std::stringstream ss;
		boost::iostreams::copy(in, ss);
		return ss.str();
	}
}
