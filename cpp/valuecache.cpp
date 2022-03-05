//    VALUECACHE.CPP
//
//    Started:       November 29, 2007
//    Last Modified: November 18, 2010
//    Version 1.1
//    By Robert W. Bryce

#include "intel_check.h"
#include <cstdint>
#include <cstdlib>
#include "valuecache.h"
#include <cassert>
#include <cstring>

/*
#ifdef _DEBUG
#define TESTING_CACHING
#endif
*/

#define KEY_INDEX	(sizeof(std::uint32_t) * 2)				/* 8 byte align */


ValueCache::ValueCache(std::uint32_t numEntries, std::uint32_t keySize, std::uint32_t answerSize) {
	m_numEntries = numEntries;

    #ifdef PROFILE_CACHE
	m_numHits = 0;
	m_numMisses = 0;
	m_numNewStores = 0;
	m_numRestores = 0;
    #endif

	m_keySize = keySize;
	m_answerSize = answerSize;
	if (keySize & 7)
		keySize = (keySize & (~7)) + 8;				// 8 byte align
	if (answerSize & 7)
		answerSize = (answerSize & (~7)) + 8;			// 8 byte align
	m_entrySize = KEY_INDEX + keySize + answerSize;
	m_answerIndex = KEY_INDEX + keySize;
	m_call = 0;
	if (m_numEntries) {
		m_cache = (std::int8_t *)malloc((size_t)m_entrySize * (size_t)m_numEntries);
		if (m_cache)
			m_end = m_cache + m_entrySize * m_numEntries;
	} else	m_cache = nullptr;
	Clear();
}


void ValueCache::Clear() {
	if (m_numEntries)
		memset(m_cache, -1, m_entrySize * m_numEntries);
}


ValueCache::~ValueCache() {

    #ifdef PROFILE_CACHE
	if ((m_numMisses) || (m_numHits))
		m_numRestores = 0;
    #endif

	if (m_cache)
		free(m_cache);
}


void ValueCache::Store(APTR key, APTR answer) {
	if (!m_numEntries)
		return;
	m_call++;
	if (!m_call) {
		Clear();
		m_call++;
	}
	std::int8_t *cache, *oldest;
//	std::uint32_t i;
//	for (i = 0, cache = m_cache, oldest = cache; i < m_numEntries; i++, cache += m_entrySize) {
	for (cache = m_cache, oldest = cache; cache < m_end; cache += m_entrySize) {
		if ((*((std::int32_t *)cache)) == -1)				// cache isn't full
			break;
		if ((*((std::uint32_t *)cache)) < (*((std::uint32_t *)oldest)))
			oldest = cache;					// search for the oldest entry
		if (!memcmp(cache + KEY_INDEX, key, m_keySize)) {

    #ifdef TESTING_CACHING
			weak_assert(!memcmp(cache + m_answerIndex, answer, m_answerSize));
    #endif

			*((std::uint32_t *)cache) = m_call;			// cache entry was hit so update its count

    #ifdef PROFILE_CACHE
			m_numRestores++;
    #endif

			return;						// entry is already in the cache
		}
	}
//	if (i == m_numEntries)
	if (cache == m_end)
		cache = oldest;						// otherwise cache already equals a blank entry that we can fill in

    #ifdef PROFILE_CACHE
	m_numNewStores++;
    #endif

	*((std::uint32_t *)cache) = m_call;
	memcpy(cache + KEY_INDEX, key, m_keySize);
	memcpy(cache + m_answerIndex, answer, (size_t)m_answerSize);
}


APTR ValueCache::Retrieve(APTR key) {

  #ifndef TESTING_CACHING
	std::int8_t *cache;
//	std::uint32_t i;
	if (!m_numEntries)
		return nullptr;
//	for (i = 0, cache = m_cache; i < m_numEntries; i++, cache += m_entrySize) {
	for (cache = m_cache; cache < m_end; cache += m_entrySize) {
		if ((*((std::int32_t *)cache)) == -1) {

    #ifdef PROFILE_CACHE
			m_numMisses++;
    #endif

			return nullptr;					// empty cache entry
		}
		if (!memcmp(cache + KEY_INDEX, key, (size_t)m_keySize)) {

    #ifdef PROFILE_CACHE
			m_numHits++;
    #endif

			m_call++;
			if (!m_call) {
				Clear();
				return nullptr;
			}
			*((std::uint32_t *)cache) = m_call;
			return cache + m_answerIndex;
		}
	}

    #ifdef PROFILE_CACHE
	m_numMisses++;
    #endif
  #endif

	return nullptr;
}


APTR ValueCache::RetrieveCustom(APTR key, iterate_internal retrieve_fcn) {

  #ifndef TESTING_CACHING
	std::int8_t *cache;
//	std::uint32_t i;
	if (!m_numEntries)
		return nullptr;
//	for (i = 0, cache = m_cache; i < m_numEntries; i++, cache += m_entrySize) {
	for (cache = m_cache; cache < m_end; cache += m_entrySize) {
		if ((*((std::int32_t *)cache)) == -1) {

    #ifdef PROFILE_CACHE
			m_numMisses++;
    #endif

			return nullptr;					// empty cache entry
		}
		if (retrieve_fcn(cache + KEY_INDEX, key)) {

    #ifdef PROFILE_CACHE
			m_numHits++;
    #endif

			m_call++;
			if (!m_call) {
				Clear();
				return nullptr;
			}
			*((std::uint32_t *)cache) = m_call;
			return cache + m_answerIndex;
		}
	}

    #ifdef PROFILE_CACHE
	m_numMisses++;
    #endif
  #endif

	return nullptr;
}


void ValueCache::IClearIndividual(ValueCache::clear_internal clear_fcn) {
	for (std::int8_t *cache = m_cache; cache < m_end; cache += m_entrySize) {
		if ((*((std::int32_t *)cache)) != -1) {
			if (clear_fcn(cache + KEY_INDEX))
				*((std::int32_t *)cache) = -1;
		}
	}
}


void ValueCache::IIterate(iterate_internal iterate_fcn, APTR theParm) {
	for (std::int8_t *cache = m_cache; cache < m_end; cache += m_entrySize) {
		if ((*((std::int32_t *)cache)) != -1) {
			if (!iterate_fcn(theParm, cache + KEY_INDEX))
				return;
		}
	}
}


std::uint32_t ValueCache::NumberUsedEntries() const {
	std::uint32_t cnt = 0;
	for (std::int8_t *cache = m_cache; cache < m_end; cache += m_entrySize) {
		if ((*((std::int32_t *)cache)) != -1)
			cnt++;
	}
	return cnt;
}
