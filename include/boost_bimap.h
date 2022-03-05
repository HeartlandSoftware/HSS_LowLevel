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
