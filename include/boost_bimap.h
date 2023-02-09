/**
 * boost_bimap.h
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

#define BOOST_BIMAP_DISABLE_SERIALIZATION
#include <boost/bimap.hpp>


template<typename L, typename R>
constexpr boost::bimap<L, R> makeBimap(std::initializer_list<typename boost::bimap<L, R>::value_type> list) noexcept
{
	return boost::bimap<L, R>(list.begin(), list.end());
}


template<typename L, typename R>
bool bimapContainsLeft(const boost::bimap<L, R>& map, const L& item)
{
	auto it = map.left.find(item);
	return it != map.left.end();
}


template<typename L, typename R>
bool bimapContainsRight(const boost::bimap<L, R>& map, const R& item)
{
	auto it = map.right.find(item);
	return it != map.right.end();
}


template<typename L, typename R>
bool bimapContains(const boost::bimap<L, R>& map, const L& item)
{
	return bimapContainsLeft(map, item);
}
