//    VALUECACHE.H
//
//    Started:       November 29, 2007
//    Last Modified: February 23, 2019
//    Version 1.0
//    By Robert W. Bryce

#ifndef __VALUECACHE_H
#define __VALUECACHE_H

#include "types.h"
#include "hssconfig/config.h"
/*
#if defined(DEBUG) || defined(_DEBUG)
#define PROFILE_CACHE
#endif
*/
#ifdef _MSC_VER

#if (!defined(__INTEL_COMPILER)) && (!defined(__INTEL_LLVM_COMPILER))
#pragma managed(push, off)
#endif
#endif //_MSC_VER

#ifdef HSS_SHOULD_PRAGMA_PACK
#pragma pack(push, 8)								// force to go to 4-byte packing rules,
#endif												// to make this class as small as possible

class alignas(8) ValueCache {						// This class set is designed to work with structures, not with classes with constructors and destructors
    protected:										// which would inevitably slow down its performance (and thus its performance).  Naturally, it will also
													// work great with primitive types too - but no embedded pointers.
	std::uint32_t	m_numEntries;
	std::uint32_t	m_keySize, m_answerSize, m_entrySize, m_answerIndex;
	std::uint32_t	m_call;

#ifdef PROFILE_CACHE
	std::uint32_t	m_numHits, m_numMisses;
	std::uint32_t	m_numNewStores, m_numRestores;
#endif

	std::int8_t		*m_cache, *m_end;

    public:
	ValueCache(std::uint32_t numEntries, std::uint32_t keySize, std::uint32_t answerSize);
	~ValueCache();

	typedef bool (__cdecl *clear_internal)(APTR aParm);
	typedef bool (__cdecl *iterate_internal)(APTR theParm, APTR aParm);

	void	Store(APTR key, APTR answer);
	APTR	Retrieve(APTR key);
	APTR	RetrieveCustom(APTR key, iterate_internal retrieve_fcn);
	void	Clear();

	void	IClearIndividual(clear_internal clear_fcn);
	void	IIterate(iterate_internal iterate_fcn, APTR theParm);

	std::uint32_t	NumberUsedEntries() const;
};


template<class key, class result> class ValueCacheTempl : private ValueCache {
    public:
	typedef bool (__cdecl *clear)(key *aParm);
	typedef bool (__cdecl *retrieve)(key *theParm, key *aParm);
	typedef bool (__cdecl *retrieve2)(key *theParm, APTR aParm);
	typedef bool (__cdecl *iterate)(APTR theParm, key *aParm);

	ValueCacheTempl(std::uint32_t numEntries) : ValueCache(numEntries, sizeof(key), sizeof(result)) { };

	__INLINE std::uint32_t NumberEntries() const								{ return m_numEntries; };
	__INLINE std::uint32_t NumberUsedEntries() const							{ return ValueCache::NumberUsedEntries(); };

	__INLINE void Store(const key *_key, const result *_answer)					{ ValueCache::Store((APTR)_key, (APTR)_answer); };
	__INLINE result *Retrieve(const key *_key)									{ return (result *)ValueCache::Retrieve((APTR)_key); };
	__INLINE result *RetrieveCustom(const key *_key, retrieve retrieve_fcn)		{ return (result *)ValueCache::RetrieveCustom((APTR)_key, (ValueCache::iterate_internal)retrieve_fcn); };
	__INLINE result *RetrieveCustom(const APTR _key, retrieve2 retrieve_fcn)	{ return (result *)ValueCache::RetrieveCustom((APTR)_key, (ValueCache::iterate_internal)retrieve_fcn); };

	__INLINE void Clear()														{ ValueCache::Clear(); };
	__INLINE void ClearIndividual(clear clear_fcn)								{ ValueCache::IClearIndividual((ValueCache::clear_internal)clear_fcn); };
	__INLINE void Iterate(iterate iterate_fcn, APTR theParm)					{ ValueCache::IIterate((ValueCache::iterate_internal)iterate_fcn, theParm); }
};

#ifdef _MSC_VER

#if (!defined(__INTEL_COMPILER)) && (!defined(__INTEL_LLVM_COMPILER))
#pragma managed(pop)
#endif
#endif //_MSC_VER

#ifdef HSS_SHOULD_PRAGMA_PACK
#pragma pack(pop)
#endif

#endif
