//    VVECTOR.CPP
//
//    Started:       November 12, 2008
//    Last Modified: May 23, 2010
//    Version 1.0
//    By Robert W. Bryce

//	STL vectors are simple wrappers for C++ arrays that can dynamically grow and shrink as needed/told.
//	But what happens when so many re-allocations fragment memory so much that they can't?  Well, things
//	go wrong because they assume contiguous memory.  This class is a subset of vector's functionality
//	(because that's all we need) but don't require contiguous memory for the array.

#include "intel_check.h"
#include "vvector.h"
#include <cassert>
#include <algorithm>
#include <cstring>

//! Constructor
vvector_base::vvector_base(size_t element_size) {
	constexpr size_t memory_align = 8;
	if (memory_align > 1) {
		while (element_size & (memory_align - 1))
			element_size++;
	}

	//! Memory size of each element
	m_elementSize = element_size;
	//! Maximum elements possible
	m_max = (1024 * 1024 * 4) / element_size;
	//! Value for each step in memory
	m_step = m_max / 16;
	//! Memory used
	m_size = 0;
	//! Memory total
	m_capacity = 0;
	m_copyIt = nullptr;
	m_initIt = nullptr;
	m_deinitIt = nullptr;
}


vvector_base::vvector_base(vvector_base &&toMove) : m_chunks((MinListTempl<vvector_element> &&)toMove.m_chunks) {
	m_elementSize = toMove.m_elementSize;
	m_step = toMove.m_step;
	m_max = toMove.m_max;
	m_size = toMove.m_size;
	m_capacity = toMove.m_capacity;
	m_copyIt = toMove.m_copyIt;
	m_initIt = toMove.m_initIt;
	m_deinitIt = toMove.m_deinitIt;
}


//! Destructor
vvector_base::~vvector_base() {
	Clear();
}

//! Clears the entire vector and frees memory
void vvector_base::Clear() {
	vvector_element *e;
	while ((e = m_chunks.RemHead())) {											// Remove the head
		std::uint8_t	*addr = e->m_memory,
			*end = e->m_memory + e->m_memUsed;
		if (m_deinitIt)
			while (addr < end) {											// Then process all subelements
				m_deinitIt(addr);
				addr += m_elementSize;
			}
		free(e->m_memory);												// Free memory allocated
		delete e;													// Delete the pointer instance
	}
	m_size = 0;
	m_capacity = 0;
}


//! Returns the size of vector
size_t vvector_base::GetSize() const {

    #ifdef DEBUG
	size_t size = 0;
	vvector_element *e = m_chunks.LH_Head();										// Start at the Head
	while (e->LN_Succ()) {													// Count the nodes by running through successors
		size += e->m_memUsed;
		e = e->LN_Succ();
	}
	weak_assert(m_size == (size / m_elementSize));
    #endif

	return m_size;
}


//! Returns the capacity of vector
size_t vvector_base::GetCapacity() const {

    #ifdef DEBUG
	size_t size = 0;
	vvector_element *e = m_chunks.LH_Head();										// Start at the head
	while (e->LN_Succ()) {
		size += e->m_memSize;												// Add all the memsizes of all successor nodes 
		e = e->LN_Succ();
	}
	weak_assert(m_capacity == (size / m_elementSize));
    #endif

	return m_capacity;
}


//! Sets vector to size
size_t vvector_base::SetSize(size_t size) {
	if (!size)
		Clear();
	else {
		size_t cur_size = GetSize();
		if (size == cur_size) {												// Check the size against current size
			//return size;
		} else if (size < cur_size) {											// We shrink...
			size_t cnt = cur_size - size;
			vvector_element *e = m_chunks.LH_Tail();
			while (e->LN_Pred()) {											// Start moving through predecessors
				while (e->m_memUsed) {
					std::uint8_t *o = e->m_memory + e->m_memUsed - m_elementSize;					//   find next predecessor element
					if (m_deinitIt)		m_deinitIt(o);							//   chop off element
					e->m_memUsed -= m_elementSize;
					cnt--;
					if (!cnt)
						break;										//  Break if we've reached our intended size
				}
				if (!e->m_memUsed) {										//  If we nuked an entire chunk
					vvector_element *ee = e->LN_Pred();
					m_chunks.Remove(e);									//  Remove the chunk
					free(e->m_memory);									//  Free the memory and remove the instance
					delete e;
					e = ee;											//  Rejoin sequence?
				} else	e = e->LN_Pred();
				if (!cnt)
					break;											//  Break if we've reached our intended size
			}
		} else {													// We grow...
			SetCapacity(size);											//  Update our size
			size_t cnt = size - cur_size;										//  cnt is the amount we're growing
			vvector_element *e;
			if (!cur_size)												//  assuming our current size isn't zero
				e = m_chunks.LH_Head();										//  start our element iterator at the head
			else {
				e = m_chunks.LH_Tail();										//  otherwise start our iterator at the tail
				while (e->LN_Pred()) {										//   As long as we have a predecessor
					if (e->m_memUsed)									//   Check to see if mem is used
						break;										//   if it is, we've found the end of our list
					e = e->LN_Pred();
				}
			}
			while (cnt) {												//  While we still have new elements to process
				while (e->m_memSize == e->m_memUsed) {
					Grow(e);										//   Call grow for each used hunk of memory we find
					if (e->m_memSize == e->m_memUsed)
						e = e->LN_Succ();								//    Move to the successor
					weak_assert(e->LN_Succ());
				}
				std::uint8_t *o = e->m_memory + e->m_memUsed;								//   Set a new pointer for this memory address
				if (m_initIt)	m_initIt(o);									//   Init the memory
				e->m_memUsed += m_elementSize;									//   Update our memUsed size
				cnt--;												//   Update the list of nodes we have left to process
			}
		}
		m_size = size;
	
    #ifdef DEBUG
		weak_assert(size == GetSize());
    #endif

	}
	return size;
}


//! Adds amount # of elements to vector at the address of *e
void vvector_base::Grow(vvector_element *e, size_t amount) {
	bool done = false;
	if (amount == (size_t)-1)
		amount = m_step;
	if ((e) && (e->m_memSize < m_max)) {											// if e exists and its size < max
		std::uint8_t *new_mem = (std::uint8_t *)malloc(e->m_memSize + (size_t)amount * m_elementSize);				// create new ptr to memory of e.size+amount
		if (new_mem) {													// if successful...
			done = true;												//	set done flag
			std::uint8_t *o1 = e->m_memory, *o2 = new_mem;									//  create two ptrs, o1 = e's mem, o2 = the newly created
			if (m_copyIt) {
				size_t cnt = e->m_memUsed / m_elementSize;							//  counter set to number of bits in e
				while (cnt) {											//  start counter
					m_copyIt(o1, o2);									//  if we can, callback to copyIt(old,new)
					/*else		memcpy(o2, o1, m_elementSize);*/					//  otherwise do a memcopy with each element
					o1 += m_elementSize;									//  increment memory hunks
					o2 += m_elementSize;
					cnt--;
				}
			} else	memcpy(o2, o1, e->m_memUsed);
			free(e->m_memory);											//  free e's current memory
			e->m_memory = new_mem;											//  update e's now-blank memory with the new chunk
			e->m_memSize += amount * m_elementSize;									//  update e's memSize
			m_capacity += amount;											//  update capacity
		}
	}
	if (!done) {														// now if we're NOT done... meaning we couldn't create new mem
		vvector_element *ee;												// declare a new vector element called ee
		if (e) {													// if e still exists
			ee = e->LN_Succ();											//  set ee to e's successor
			if (ee->LN_Succ())											//   and try to move to ee's successor
				if (ee->m_memSize > ee->m_memUsed)								//    check if ee's size > used
					return;											//      then bail
		}
		ee = new vvector_element();											// otherwise create instance for ee
		ee->m_memSize = amount * m_elementSize;										// set ee's size to what we're wanting to add to vector
		ee->m_memUsed = 0;												// set ee blank
		ee->m_memory = (std::uint8_t *)malloc(ee->m_memSize);									// allocate memory according to ee's size
		if (ee->m_memory) {												// if alloc was successful
			if (e)													//  and if e still exists
				m_chunks.Insert(ee, e);										//   start inserting chunks from ee to e
			else	m_chunks.AddHead(ee);										//  otherwise promote ee to having a head
			m_capacity += amount;											//  update capacity
		} else {
			delete ee;												// otherwise delete ee
			size_t amount2 = amount >> 1;										// bitshift amount and set as amount2
			Grow(e, amount2);											// Try to grow e by new amount
			Grow(e, amount - amount2);										// Try to grow e by the difference
		}
	}
}


//! Sets the capacity of the vector to size and returns size
size_t vvector_base::SetCapacity(size_t size) {
	if (!size)														// If we have no size, clear our cap
		Clear();
	size_t cur_size = GetCapacity();											// cur_size is our actual cap
	if (cur_size < size) {													// compare actual cap with specified cap
		if (m_chunks.IsEmpty())												// if we have no chunks floating around
			Grow(nullptr, size);											//  grow our cap to size with null bits
		else	Grow(m_chunks.LH_Tail(), max(size - cur_size, m_step));							// otherwise grow using what we have in our chunks
	}
	return size;
}


//! Returns a pointer to an element at index
APTR vvector_base::GetElementAt(size_t index) const {

    #ifdef DEBUG
	weak_assert(index < GetSize());
    #endif
	
	size_t m_vectorSize = GetSize();
	size_t m_vectorMidPoint = m_vectorSize/2;
	if (index < m_vectorMidPoint)	// index is in lower half of vector
	{
		vvector_element *e = m_chunks.LH_Head();									// set element pointer to the Head
		while (e->LN_Succ()) {
			size_t e_num_elements = e->m_memUsed / m_elementSize;							// grab number of elements by used/size
			if (e_num_elements > index)										// if index < number of elements in this block
				return e->m_memory + (index * m_elementSize);							// clearly index is IN this block, return memory address using ptr arithmetic
			index -= e_num_elements;										// otherwise subtract the number of elements from index...
			e = e->LN_Succ();											// and grab the next block in the vector
		}
	}
	else															// index is in upper half of vector
	{
		vvector_element *e = m_chunks.LH_Tail();									// set element pointer to the Tail
		size_t m_indexFromTail = m_vectorSize - index;									// use a secondary index that is the "absolute value" of index from tail
		while (e->LN_Pred())
		{
			size_t e_num_elements = e->m_memUsed / m_elementSize;							// grab number of elements
			if (e_num_elements >= m_indexFromTail)									// if index-from-tail < number of elements in this block
			{
				return e->m_memory + ((e_num_elements-m_indexFromTail) * m_elementSize);			// calc the index as we normally would
			}
			m_indexFromTail -= e_num_elements;									// otherwise subtract the number of elements from the index-from-tail
			e = e->LN_Pred();											// move to the next predecessor
		}
	}

	// ORIGINAL ALGORITHM --
/*	vvector_element *e = m_chunks.LH_Head();
	while (e->LN_Succ()) {
		size_t e_num_elements = e->m_memUsed / m_elementSize;
		if (e_num_elements > index)
			return e->m_memory + (index * m_elementSize);
		index -= e_num_elements;
		e = e->LN_Succ();
	} */
	// ---------------------
	return nullptr;
}


//! Adds element to the vector
APTR vvector_base::Add(APTR element) {
	size_t size = GetSize();												// get our size
	SetCapacity(size + 1);													// increase our cap by 1
	vvector_element *e;
	if (!size)														// if vector is blank
		e = m_chunks.LH_Head();												//  start with the head of our chunks
	else {
		e = m_chunks.LH_Tail();												// otherwise start with the tail of our chunks
		while (e->LN_Pred()) {												//  move backwards through the chunks
			if (e->m_memUsed)											//  if we run into used memory break out
				break;			
			e = e->LN_Pred();											//  otherwise keep going back through our predecessors
		}
	}
	while (e->m_memSize == e->m_memUsed) {											// While we're at the end of contiguous memory and size
		Grow(e);													//  grow the vector using our element
		if (e->m_memSize == e->m_memUsed)										//  and move into elements successor, repeat
			e = e->LN_Succ();
		weak_assert(e->LN_Succ());
	}
	std::uint8_t *o = e->m_memory + e->m_memUsed;											// Create a memory address pointer for our new element
	if (m_copyIt)	m_copyIt(element, o);											// If we've set the copy flag, copy our new element to the contiguous address
	else		memcpy(o, element, m_elementSize);									// Otherwise do a direct memory copy
	e->m_memUsed += m_elementSize;
	m_size++;
	return o;
}


//! Inserts element into the vector at index
APTR vvector_base::Insert(size_t index, APTR element) {
	if (index == GetSize())													// Don't bother inserting if we're inserting at the end of the vector
		return Add(element);

    #ifdef DEBUG
	weak_assert(index < GetSize());
    #endif

	vvector_element *e = m_chunks.LH_Head();										// Start at the head
	while (e->LN_Succ()) {													// Run through successors
		size_t e_num_elements = e->m_memUsed / m_elementSize;								// Find our number of elements
		if (e_num_elements > index) {											// If our index is less than our element count
			vvector_element *ee = e;										//  ee is our pointer to current element
			while (ee->m_memSize == ee->m_memUsed) {								//  While we're at the end of the contiguous vector
				Grow(ee);											//   Grow the vector given our current element
				if (ee->m_memSize == ee->m_memUsed)
					ee = ee->LN_Succ();									//   Grab element's successor if we're still contiguous
				weak_assert(ee->LN_Succ());
			}
			if (ee != e) {												//  If we've moved ee away from e
				weak_assert(e->m_memUsed == e->m_memSize);								//  Ensure that our memused and memsize 
				weak_assert(ee->m_memUsed < ee->m_memSize);								//   keep their integrity
				weak_assert(e->m_memUsed);

				std::uint8_t	*o = ee->m_memory + ee->m_memUsed - m_elementSize,
					*end = ee->m_memory;
				if (m_copyIt) {
					while (o >= end) {									//  move from our new address pointer to the end backwards
						/*if (m_copyIt)*/	m_copyIt(o, o + m_elementSize);				//  copy selected nodes after our inserted node
						//else		memcpy(o + m_elementSize, o, m_elementSize);
						o -= m_elementSize;
					}
				} else	memmove(end + m_elementSize, end, ee->m_memUsed - m_elementSize);

    #ifdef DEBUG
				o += m_elementSize;										//  move to the next block of memory
				weak_assert(o == ee->m_memory);
    #endif

				ee->m_memUsed += m_elementSize;									//  add our new element sizes to memUsed
				if (m_copyIt)	m_copyIt(e->m_memory + e->m_memUsed - m_elementSize, ee->m_memory);
				else		memcpy(ee->m_memory, e->m_memory + e->m_memUsed - m_elementSize, m_elementSize);
				e->m_memUsed -= m_elementSize;
			}
			weak_assert(e->m_memUsed < e->m_memSize);
			std::uint8_t	*o = e->m_memory + e->m_memUsed - m_elementSize,						// Create a pointer element (O)
				*end = e->m_memory + index * m_elementSize;							//  as well as our end element
			if (m_copyIt) {
				while (o >= end) {										// While O is past our end pointer mark
					/*if (m_copyIt)*/	m_copyIt(o, o + m_elementSize);					//  copy element past O
					//else		memcpy(o + m_elementSize, o, m_elementSize);	
					o -= m_elementSize;									//  ...moving backwards from the very end
				}
			} else	memmove(end + m_elementSize, end, o - end);
		
			o += m_elementSize;
			if (m_copyIt)	m_copyIt(element, o);
			else		memcpy(o, element, m_elementSize);
			e->m_memUsed += m_elementSize;
			m_size++;
			return o;
		}
		index -= e_num_elements;
		e = e->LN_Succ();
	}
	return nullptr;
}


//! Remove element from vector at index
void vvector_base::Remove(size_t index) {

    #ifdef DEBUG
	weak_assert(index < GetSize());
    #endif

	vvector_element *e = m_chunks.LH_Head();										// Start at our Head
	while (e->LN_Succ()) {													// Start moving through successors
		size_t e_num_elements = e->m_memUsed / m_elementSize;
		if (e_num_elements > index) {											// Check that our num_elements > index
			std::uint8_t	*o = e->m_memory + index * m_elementSize;							// Create a pointer set to our index
			e->m_memUsed -= m_elementSize;
			std::uint8_t	*end = e->m_memory + e->m_memUsed - m_elementSize;						// Set an end pointer
			if (m_deinitIt)		m_deinitIt(o);									// if deinitIt, then call on the O pointer
			if (m_copyIt) {
				while (o < end) {										// until o reaches our end marker
					/*if (m_copyIt)*/	m_copyIt(o + m_elementSize, o);					// copy o's successor into o
					//else		memcpy(o, o + m_elementSize, m_elementSize);
					o += m_elementSize;									// adjust the address of o backwards
				}
			} else	memmove(o, o + m_elementSize, end - o);

			if (!e->m_memUsed) {											// if our memory is that vector is no longer used
				m_chunks.Remove(e);										// remove the memory assigned
				m_capacity -= e_num_elements;
//				weak_assert(!(m_capacity & 0x8000000000000000));				// no longer relevant - was a check for 32-bit builds because the high
				free(e->m_memory);										// order bit shouldn't be set on a Win32 machine
				delete e;
			}
			m_size--;
			return;
		}
		index -= e_num_elements;
		e = e->LN_Succ();
	}
}


//! Iterate through array in direction set by reverse
void vvector_base::Iterate(bool reverse, iterate_callback iterator, APTR param) {
	if (!reverse) {
		vvector_element *e = m_chunks.LH_Head();									// Start at the head
		while (e->LN_Succ()) {												// for each successor
			std::uint8_t *o = e->m_memory, *end = e->m_memory + e->m_memUsed;						// create our iterator pointer and end ptr
			while (o < end) {
				if (!iterator(param, o))									// if we've found the point we seek, break
					return;
				o += m_elementSize;
			}
			e = e->LN_Succ();
		}
	} else {
		vvector_element *e = m_chunks.LH_Tail();									// same as above but in reverse
		while (e->LN_Pred()) {
			std::uint8_t *o = e->m_memory + e->m_memUsed - m_elementSize;
			while (o >= e->m_memory) {
				if (!iterator(param, o))
					return;
				o -= m_elementSize;
			}
			e = e->LN_Pred();
		}
	}
}
