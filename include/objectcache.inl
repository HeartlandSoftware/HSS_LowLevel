//    OBJECTCACHE.INL
//
//    Started:       December 24, 2006
//    Last Modified: April 18, 2012
//    Version 1.2
//    By Robert W. Bryce

#ifndef __OBJECTCACHE_INL
#define __OBJECTCACHE_INL

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4706)		// disable warning complaining about an assignment in an if statement
#endif

template <class cls>
ObjectCache<cls>::ObjectCache(size_t block_size, bool force_zeroed, bool auto_adjust, size_t mem_align)
    : ObjectCacheBase(sizeof(cls), block_size, force_zeroed, auto_adjust, mem_align) {
}


template <class cls>
cls *ObjectCache<cls>::New(size_t size) {
	return (cls *)_New(size);
}


template <class cls>
void ObjectCache<cls>::Delete(cls *object) {
	_Delete(object);
}


template <class cls>
std::uint32_t ObjectCache<cls>::NewSet(RefList<cls> &set, std::uint32_t num) {
	return _NewSet(set, num);
}


template <class cls>
void ObjectCache<cls>::DeleteSet(RefList<cls> &set) {
	return _DeleteSet(set);
}


template <class cls>
cls *ObjectCacheMN<cls>::New(size_t size) {
	return (cls *)m_objectCache.New(size);
}


template <class cls>
void ObjectCacheMN<cls>::Delete(cls *object) {
	MNcls *obj = (MNcls *)object;
	m_objectCache.Delete(obj);
}


template <class cls>
std::uint32_t ObjectCacheMN<cls>::NewSet(RefList<cls> &set, std::uint32_t num) {
	return m_objectCache.NewSet(set, num);
}


template <class cls>
void ObjectCacheMN<cls>::DeleteSet(RefList<cls> &set) {
	m_objectCache.DeleteSet(set);
}



#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif
