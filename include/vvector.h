//    VVECTOR.H
//
//    Started:       November 12, 2008
//    Last Modified: November 12, 2008
//    Version 1.0
//    By Robert W. Bryce

#pragma once

#include "linklist.h"
#include "hssconfig/config.h"

#ifdef _MSC_VER

#if (!defined(__INTEL_COMPILER)) && (!defined(__INTEL_LLVM_COMPILER))
#pragma managed(push, off)
#endif
#endif //_MSC_VER

#ifdef HSS_SHOULD_PRAGMA_PACK
#pragma pack(push, 8)					// force to go to 4-byte packing rules,
#endif /* _MSC_VER */					// to make this class as small as possible

typedef void (__cdecl *copy_callback)(APTR from, APTR to);
typedef void* (__cdecl *init_callback)(APTR init);
typedef void (__cdecl *deinit_callback)(APTR deinit);
typedef bool (__cdecl *iterate_callback)(APTR param, APTR obj);

class alignas(8) vvector_base {
	// Vector element = node in LinkList
	class vvector_element : public MinNode {
	    public:
		__INLINE vvector_element *LN_Succ() const				{ return (vvector_element *)MinNode::LN_Succ(); };
		__INLINE vvector_element *LN_Pred() const				{ return (vvector_element *)MinNode::LN_Pred(); };
		std::uint8_t	*m_memory;		// pointer to a chunk of memory
		size_t			m_memUsed;		// amount of memory used in this chunk
		size_t			m_memSize;		// allocated size of this memory
	};

	MinListTempl<vvector_element> m_chunks;
	size_t	m_elementSize,		// Memory size of element
			m_step,				// Memory increment size for a chunk
			m_max,				// Maximum elements per chunk
			m_size,				// Total occupied size of structure
			m_capacity;			// Total combined size of structure

	void Clear();
	void Grow(vvector_element *e, size_t amount = (size_t)-1);
	static bool size_iterator(APTR param, APTR obj);

    public:
	vvector_base(size_t element_size);
	vvector_base(vvector_base &&toMove);
	~vvector_base();

	size_t GetSize() const;
	size_t SetSize(size_t size);
	size_t GetCapacity() const;
	size_t SetCapacity(size_t capacity);
	__INLINE size_t StepSize() const							{ return m_step; };
	__INLINE size_t StepSize(size_t stepSize)					{ if (stepSize) m_step = stepSize; return m_step; };
	__INLINE size_t MaxSize() const								{ return m_max; };
	__INLINE size_t MaxSize(size_t stepSize)					{ if (stepSize) m_max = stepSize; return m_max; };

	APTR GetElementAt(size_t index) const;
	APTR Insert(size_t index, APTR element);
	APTR Add(APTR element);
	void Remove(size_t index);

	void Iterate(bool reverse, iterate_callback iterator, APTR param);

	copy_callback m_copyIt;
	init_callback m_initIt;
	deinit_callback m_deinitIt;
};


template<class cls> void __cdecl copy_call(APTR from, APTR to)	{ *((cls *)to) = *((cls *)from); };
template<class cls> void* __cdecl init_call(APTR init)			{ return (APTR) new ((cls *)init) cls(); };
template<class cls> void __cdecl deinit_call(APTR deinit)		{ ((cls *)deinit)-> ~cls(); };

template<class cls> class vvector {
	vvector_base memory;

	friend void copy_call(APTR from, APTR to);
	friend void* init_call(APTR init);
	friend void deinit_call(APTR deinit);
public:
	vvector() : memory(sizeof(cls)) {
		memory.m_copyIt = /*(copy_callback *)*/copy_call<cls>;
		memory.m_initIt = /*(init_callback *)*/init_call<cls>;
		memory.m_deinitIt = /*(deinit_callback *)*/deinit_call<cls>;
	};
	vvector(vvector &&toMove) : memory((vvector_base &&)toMove.memory) { };
	~vvector() = default;

	void reserve(size_t _Count)									{ memory.SetCapacity(_Count); };
	size_t capacity() const										{ return (size_t)memory.GetCapacity(); };
	size_t size() const											{ return (size_t)memory.GetSize(); };
	cls &at(size_t _Pos) const									{ return *(cls *)memory.GetElementAt(_Pos); };
	cls &at(size_t _Pos)										{ return *(cls *)memory.GetElementAt(_Pos); };
	const cls &operator[](size_t _Pos) const					{ return *(cls *)memory.GetElementAt(_Pos); };
	cls &operator[](size_t _Pos)								{ return *(cls *)memory.GetElementAt(_Pos); };
	void push_back(const cls& _Val)								{ memory.Add((APTR)&_Val); };
	void clear()												{ memory.SetSize(0); };
	size_t begin()												{ return 0; };
	void erase(size_t index)									{ memory.Remove(index); };
	cls *insert(size_t index, cls &_Val)						{ return (cls *)memory.Insert(index, &_Val); };
};

#ifdef _MSC_VER

#if (!defined(__INTEL_COMPILER)) && (!defined(__INTEL_LLVM_COMPILER))
#pragma managed(pop)
#endif
#endif //_MSC_VER

#ifdef HSS_SHOULD_PRAGMA_PACK
#pragma pack(pop)
#endif
