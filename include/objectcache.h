//    OBJECTCACHE.H
//
//    Started:       February 15, 2006
//    Last Modified: April 18, 2012
//    Version 1.2
//    By Robert W. Bryce

//	March 28, '07:	RWB: specified packing rules
//	Oct. 10, '07:	RWB: adjust adjust stuff
//	Nov. 21, '07:	RWB: added the CPP file, made the template classes a whole lot smaller to clean things up
//	April 18, '12:	RWB: now taking a parameter for the size of the object to be instantiated, so we can deal with inherited classes that have changed size,
//			but have not created their own caches

#ifndef __OBJECTCACHE_H
#define __OBJECTCACHE_H

#ifdef __CUDACC__
#include "linklist.cuh"
#else
#include "linklist.h"
#endif

#include "hssconfig/config.h"

#ifdef _MSC_VER

#if (!defined(__INTEL_COMPILER)) && (!defined(__INTEL_LLVM_COMPILER))
#pragma managed(push, off)
#endif
#endif //_MSC_VER

#ifdef HSS_SHOULD_PRAGMA_PACK
#pragma pack(push, 4)			// force to go to 4-byte packing rules,
#endif							// to make this class as small as possible


	// Use this class if the object factory/cache is handling a class which inherits from MinNode.
	// If not, then use the ObjectCacheMN template class (below) because it takes care of this for us - it constructs a usable class for our link list approach

	// NOTE! These caches use the new [] operator, so if the class overrides that operator (different from just 'new'), then ... well ... who knows what will happen?
	// As such, these caches will just 'respond' to individual new's.

	// I recommend using either one or the other pair of macros (immediately below) to make life easier / automate
	// overriding the new operator, declaring the cache, etc.

#if defined(DEBUG) || defined(_DEBUG)
#ifdef _MSC_VER
#define DECLARE_OBJECT_CACHE(cls, cachename) \
	static ObjectCache<cls> staticObjectClass##cachename; \
	DEVICE void *operator new(size_t s)																	{ return staticObjectClass##cachename.New(s); }; \
	DEVICE void *operator new(size_t s, const char * /*source_file*/, int /*line_no*/)					{ return staticObjectClass##cachename.New(s); }; \
	DEVICE void *operator new(size_t s, int /*block*/, const char * /*source_file*/, int /*line_no*/)	{ return staticObjectClass##cachename.New(s); }; \
	DEVICE void *operator new(size_t /*s*/, cls *mem)													{ return mem; } \
	DEVICE void *operator new[](size_t s)																{ return ::operator new[](s); }; \
	DEVICE void *operator new[](size_t s, int block, const char *source_file, int line_no)				{ return ::operator new[](s, block, source_file, line_no); }; \
	DEVICE void operator delete(void *obj)																{ return staticObjectClass##cachename.Delete((cls *)obj); }; \
	DEVICE void operator delete(void *obj, int /*block*/, const char * /*source_file*/, int /*line_no*/){ return staticObjectClass##cachename.Delete((cls *)obj); }; \
	DEVICE void operator delete[](void *ptr)															{ ::operator delete[](ptr); }; \
	DEVICE void operator delete[](void *ptr, int block, const char *source_file, int line_no)			{ ::operator delete[](ptr, block, source_file, line_no); };
#else
#define DECLARE_OBJECT_CACHE(cls, cachename) \
	static ObjectCache<cls> staticObjectClass##cachename; \
	DEVICE void *operator new(size_t s)																	{ return staticObjectClass##cachename.New(s); }; \
	DEVICE void *operator new(size_t s, const char * /*source_file*/, int /*line_no*/)					{ return staticObjectClass##cachename.New(s); }; \
	DEVICE void *operator new(size_t s, int /*block*/, const char * /*source_file*/, int /*line_no*/)	{ return staticObjectClass##cachename.New(s); }; \
	DEVICE void *operator new(size_t /*s*/, cls *mem)													{ return mem; } \
	DEVICE void *operator new[](size_t s)																{ return ::operator new[](s); }; \
	DEVICE void operator delete(void *obj)																{ return staticObjectClass##cachename.Delete((cls *)obj); }; \
	DEVICE void operator delete[](void *ptr)															{ ::operator delete[](ptr); };
#endif //_MSC_VER
#else
#define DECLARE_OBJECT_CACHE(cls, cachename) \
	static ObjectCache<cls> staticObjectClass##cachename; \
	DEVICE void *operator new(size_t s)																	{ return staticObjectClass##cachename.New(s); }; \
	DEVICE void *operator new(size_t /*s*/, cls *mem)													{ return mem; } \
	DEVICE void *operator new[](size_t s)																{ return ::operator new[](s); }; \
	DEVICE void operator delete(void *obj)																{ return staticObjectClass##cachename.Delete((cls *)obj); }; \
	DEVICE void operator delete[](void *ptr)															{ ::operator delete[](ptr); };
#endif

#define IMPLEMENT_OBJECT_CACHE(cls, cachename, block_size, force_zeroed, align) \
	ObjectCache<cls> cls::staticObjectClass##cachename(block_size, force_zeroed, true, align);

#if defined(DEBUG) || defined(_DEBUG)
#ifdef _MSC_VER
#define DECLARE_OBJECT_CACHE_MN(cls, cachename) \
	static ObjectCacheMN<cls> staticObjectClass##cachename; \
	DEVICE void *operator new(size_t s)																	{ return staticObjectClass##cachename.New(s); }; \
	DEVICE void *operator new(size_t s, const char * /*source_file*/, int /*line_no*/)					{ return staticObjectClass##cachename.New(s); }; \
	DEVICE void *operator new(size_t s, int /*block*/, const char * /*source_file*/, int /*line_no*/)	{ return staticObjectClass##cachename.New(s); }; \
	DEVICE void *operator new(size_t /*s*/, cls *mem)													{ return mem; } \
	DEVICE void *operator new[](size_t s)																{ return ::operator new[](s); }; \
	DEVICE void *operator new[](size_t s, int block, const char *source_file, int line_no)				{ return ::operator new[](s, block, source_file, line_no); }; \
	DEVICE void operator delete(void *obj)																{ return staticObjectClass##cachename.Delete((cls *)obj); }; \
	DEVICE void operator delete(void *obj, int /*block*/, const char * /*source_file*/, int /*line_no*/){ return staticObjectClass##cachename.Delete((cls *)obj); }; \
	DEVICE void operator delete[](void *ptr)															{ ::operator delete[](ptr); }; \
	DEVICE void operator delete[](void *ptr, int block, const char *source_file, int line_no)			{ ::operator delete[](ptr, block, source_file, line_no); };
#else
#define DECLARE_OBJECT_CACHE_MN(cls, cachename) \
	static ObjectCacheMN<cls> staticObjectClass##cachename; \
	DEVICE void *operator new(size_t s)																	{ return staticObjectClass##cachename.New(s); }; \
	DEVICE void *operator new(size_t s, const char * /*source_file*/, int /*line_no*/)					{ return staticObjectClass##cachename.New(s); }; \
	DEVICE void *operator new(size_t s, int /*block*/, const char * /*source_file*/, int /*line_no*/)	{ return staticObjectClass##cachename.New(s); }; \
	DEVICE void *operator new(size_t /*s*/, cls *mem)													{ return mem; } \
	DEVICE void *operator new[](size_t s)																{ return ::operator new[](s); }; \
	DEVICE void operator delete(void *obj)																{ return staticObjectClass##cachename.Delete((cls *)obj); }; \
	DEVICE void operator delete[](void *ptr)															{ ::operator delete[](ptr); };
#endif //_MSC_VER
#else
#define DECLARE_OBJECT_CACHE_MN(cls, cachename) \
	static ObjectCacheMN<cls> staticObjectClass##cachename; \
	DEVICE void *operator new(size_t s)																	{ return staticObjectClass##cachename.New(s); }; \
	DEVICE void *operator new(size_t /*s*/, cls *mem)													{ return mem; } \
	DEVICE void *operator new[](size_t s)																{ return ::operator new[](s); }; \
	DEVICE void operator delete(void *obj)																{ return staticObjectClass##cachename.Delete((cls *)obj); }; \
	DEVICE void operator delete[](void *ptr)															{ ::operator delete[](ptr); };
#endif

#define IMPLEMENT_OBJECT_CACHE_MN(cls, cachename, block_size, force_zeroed, align) \
	ObjectCacheMN<cls> cls::staticObjectClass##cachename(block_size, force_zeroed, true, align);


class ObjectCacheBase {
	class CMemoryBlock : public MinNode {
	    public:
		MinList	m_list;
		size_t	m_blockSize;
		APTR	m_start;
	};

    private:
	size_t						m_objectSize, m_defBlockSize;
	MinListTempl<CMemoryBlock>	m_memoryList;
	APTR						m_first;
	bool						m_forceZeroed, m_autoAdjust;

	DEVICE CMemoryBlock * __FASTCALL allocSet();
	DEVICE void __FASTCALL deleteSet(CMemoryBlock *block);
	DEVICE void __FASTCALL extractFromSet(CMemoryBlock *mem, std::uint32_t &num, MinList &set);

    public:
	DEVICE ObjectCacheBase(size_t object_size, size_t block_size, bool force_zeroed, bool auto_adjust = true, size_t memory_align = 4);
									// block_size is the number of elements to alloc at a time, force_zeroed is whether or not to set the object to all 0's
									// auto_adjust does some basic stuff to try to adjust the block size when lots of things are allocated
	DEVICE ~ObjectCacheBase();

	DEVICE APTR _New(size_t size);
	DEVICE void _Delete(APTR object);
	DEVICE std::uint32_t _NewSet(MinList &set, const std::uint32_t num);
	DEVICE void _DeleteSet(MinList &set);
};


template<class cls> class ObjectCache : private ObjectCacheBase {	// this object manages "blocks" of equal objects to cut down on the number of system new's and delete's

    public:
	DEVICE ObjectCache(size_t block_size, bool force_zeroed, bool auto_adjust = true, size_t mem_align = 4);
									// block_size is the number of elements to alloc at a time, force_zeroed is whether or not to set the object to all 0's
									// auto_adjust does some basic stuff to try to adjust the block size when lots of things are allocated
	DEVICE cls *New(size_t size);
	DEVICE void Delete(cls *object);
	DEVICE std::uint32_t NewSet(RefList<cls> &set, std::uint32_t num);
	DEVICE void DeleteSet(RefList<cls> &set);
};


template<class cls> class ObjectCacheMN {
	class MNcls : public MinNode, public cls {
//		union {
//			MinNode node;
//			cls	_class;
//		};
	};

    private:
	ObjectCache<MNcls>	m_objectCache;

    public:
	DEVICE ObjectCacheMN(size_t block_size, bool force_zeroed, bool auto_adjust = true, size_t mem_align = 8) : m_objectCache(block_size, force_zeroed, auto_adjust, mem_align) { };

	DEVICE cls *New(size_t size);
	DEVICE void Delete(cls *object);
	DEVICE std::uint32_t NewSet(RefList<cls> &set, std::uint32_t num);
	DEVICE void DeleteSet(RefList<cls> &set);
};

#ifdef HSS_SHOULD_PRAGMA_PACK
#pragma pack(pop)
#endif

#ifdef _MSC_VER
#if (!defined(__INTEL_COMPILER)) && (!defined(__INTEL_LLVM_COMPILER))
#pragma managed(pop)
#endif

#endif

#include "objectcache.inl"

#endif
