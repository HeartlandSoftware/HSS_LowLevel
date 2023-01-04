//    OBJECTCACHE.CPP
//
//    Started:       November 21, 2007
//    Last Modified: April 18, 2012
//    Version 1.2
//    By Robert W. Bryce

#include "intel_check.h"

#ifdef _MSC_VER
#include <windows.h>
#endif

#include "types.h"
#include "objectcache.h"
#include <cassert>
#include <cstring>

#ifdef _WIN64
#define MAX_SINGLE_ALLOC (size_t)((size_t)1024*(size_t)1024*(size_t)1024-(size_t)500)
#else
#define MAX_SINGLE_ALLOC (size_t)(1024*1024*128-500)
#endif


ObjectCacheBase::ObjectCacheBase(size_t object_size, size_t block_size, bool force_zeroed, bool auto_adjust, size_t memory_align) {
	m_objectSize = object_size;
	if (memory_align > 1) {
		while (m_objectSize & (memory_align - 1))
			m_objectSize++;
	}
	m_defBlockSize = block_size;
	m_forceZeroed = force_zeroed;
	m_autoAdjust = auto_adjust;
	m_first = nullptr;
}


ObjectCacheBase::~ObjectCacheBase() {
	if (m_first)
		_Delete(m_first);

	CMemoryBlock *mem;
	while ((mem = m_memoryList.RemHead())) {

    #ifdef DEBUG
		weak_assert(false);						// this means not all the elements were "free"'d
    #endif

//		delete [] mem->m_start;
		free(mem->m_start);
		delete mem;
	}
}


APTR ObjectCacheBase::_New(size_t size) {
	if (size > m_objectSize)					// This check is necessary because inherited classes will use the same new/delete
		return ::operator new(size);				// operators as their base classes, so if an inherited class grows in size and
									// doesn't define its own caching, then this routine will ultimately be called
									// with an object size that is greater than what the cache is anticipating.  For
									// now, we'll just default to the global new operator.  In the future, we may
									// want to consider increasing the functionality here by then trying to cache
									// larger object sizes.
	APTR obj;
	CMemoryBlock *mem = m_memoryList.LH_Head();
	while (mem->LN_Succ()) {
		if ((obj = mem->m_list.RemHead())) {
			if (m_forceZeroed)
				memset(obj, 0, (size_t)m_objectSize);
			return obj;
		}
		mem = (CMemoryBlock *)mem->LN_Succ();
	}
	if (!(mem = allocSet()))
		return nullptr;
	if (!m_first)
		m_first = mem->m_list.RemHead();
	obj = mem->m_list.RemHead();
	if ((obj) && (m_forceZeroed))
		memset(obj, 0, (size_t)m_objectSize);
	return obj;
}


void ObjectCacheBase::extractFromSet(CMemoryBlock *mem, std::uint32_t &num, MinList &set) {
	while ((num) && (mem->m_list.GetCount())) {
		APTR obj = mem->m_list.RemHead();
		if (m_forceZeroed)
			memset(obj, 0, (size_t)m_objectSize);		// we could have been smarter if we didn't have to clear the memory (conditionally)
		set.AddTail((MinNode *)obj);
		num--;
		if (!num)
			return;
	}
}


std::uint32_t ObjectCacheBase::_NewSet(MinList &set, const std::uint32_t num) {
	std::uint32_t need = num;
	CMemoryBlock *mem = m_memoryList.LH_Head();
	while (mem->LN_Succ()) {
		extractFromSet(mem, need, set);
		if (!need)
			return num;
		mem = (CMemoryBlock *)mem->LN_Succ();
	}
	while (need) {
		if ((mem = allocSet())) {
			if (!m_first)
				m_first = mem->m_list.RemHead();
			extractFromSet(mem, need, set);
			if (!need)
				return num;
		} else
			return (num - need);
	}
	return (num - need);
}


void ObjectCacheBase::_Delete(APTR object) {
	CMemoryBlock *mem = m_memoryList.LH_Head();
	while (mem->LN_Succ()) {
		if (object >= mem->m_start) {
			std::uint64_t idx = (((std::uint8_t *)(object)) - ((std::uint8_t *)(mem->m_start))) / m_objectSize;
			if (idx < mem->m_blockSize) {
				mem->m_list.AddHead((MinNode *)object);
				deleteSet(mem);
				return;
			}
		}
		mem = (CMemoryBlock *)mem->LN_Succ();
	}
/*
    #ifdef DEBUG
	weak_assert(false);							// this means that we were asked to delete an object that we didn't create
    #endif
*/	::operator delete(object);
}


void ObjectCacheBase::_DeleteSet(MinList &set) {
	MinNode *node;
	while ((node = set.RemHead()))
		_Delete(node);
}


ObjectCacheBase::CMemoryBlock *ObjectCacheBase::allocSet() {
	size_t i;
	CMemoryBlock* mem = nullptr;
	try {
		mem = new CMemoryBlock();
	}
	catch (...) {
		return nullptr;
	}
	if (!mem)
		return nullptr;
	if ((!m_autoAdjust) || (m_memoryList.GetCount() < 2))	mem->m_blockSize = m_defBlockSize;
	else if (m_memoryList.GetCount() < 4)
		mem->m_blockSize = m_defBlockSize << 1;
	else if (m_memoryList.GetCount() < 8)
		mem->m_blockSize = m_defBlockSize << 2;
	else if (m_memoryList.GetCount() < 16)
		mem->m_blockSize = m_defBlockSize << 3;
	else							
		mem->m_blockSize = m_defBlockSize << 4;
	if (mem->m_blockSize > MAX_SINGLE_ALLOC)
		mem->m_blockSize = MAX_SINGLE_ALLOC;

#ifdef _WINDOWS
	{
		MEMORYSTATUSEX memory;
		memory.dwLength = sizeof(memory);
		::GlobalMemoryStatusEx(&memory);
		std::uint64_t max_alloc = memory.ullAvailPageFile / 5;
		if (mem->m_blockSize > max_alloc)
			mem->m_blockSize = max_alloc;
	}
#endif

	for (i = 0; i < 64; i++) {
//		if (mem->m_start = new cls[(size_t)mem->m_blockSize])
		if ((mem->m_start = malloc(mem->m_blockSize * m_objectSize)))
			break;
		mem->m_blockSize = mem->m_blockSize >> 1;
		if (mem->m_blockSize < (size_t)16) {
			delete mem;
			return nullptr;
		}
	}
	if (!mem->m_start) {
		delete mem;
		return nullptr;
	}
	APTR obj = mem->m_start;
	for (i = 0; i < mem->m_blockSize; i++, obj = (((std::uint8_t *)obj) + m_objectSize))
		mem->m_list.AddHead((MinNode *)obj);
	m_memoryList.AddHead(mem);
	return mem;
}


void ObjectCacheBase::deleteSet(CMemoryBlock *block) {
	if (block->m_blockSize == (size_t)block->m_list.GetCount()) {
//		delete [] block->m_start;
		free(block->m_start);
		m_memoryList.Remove(block);
		delete block;
	}
}

