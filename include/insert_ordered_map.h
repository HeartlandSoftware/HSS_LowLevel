/**
 * insert_ordered_map.h
 *
 * Copyright 2019-2023 Heartland Software Solutions Inc.
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

#include <vector>
#include <map>


namespace hss
{
	/// <summary>
	/// A horrible map implementation that allows you to retrieve items in their insertion order.
	/// </summary>
	template<class key_t, class value_t>
	class insert_ordered_map
	{
	private:
		std::map<key_t, value_t> map;
		std::vector<key_t> order;

	public:
		value_t& operator[](key_t&& _Keyval)
		{	// find element matching _Keyval or insert with default mapped
			return map[_Keyval];
		}

		value_t& operator[](const key_t& _Keyval)
		{	// find element matching _Keyval or insert with default mapped
			return map[_Keyval];
		}

		void insert(std::pair<key_t, value_t> pair)
		{
			auto[it, valid] = map.insert(pair);
			if (valid)
				order.push_back(pair.first);
		}

		std::vector<std::pair<key_t, value_t>> insertOrdered()
		{
			std::vector<std::pair<key_t, value_t>> retval;

			for (auto o : order)
				retval.emplace_back(std::make_pair(o, map[o]));

			return retval;
		}

		decltype(auto) size()
		{
			return map.size();
		}
	};
}