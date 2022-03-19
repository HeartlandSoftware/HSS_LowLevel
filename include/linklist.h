//    LINKLIST.H
//
//    Started:       May 8, 1992
//    Last Modified: November 2, 2015
//    Version 2.0
//    By Robert W. Bryce

//	October 7, 2002: RWB: added __INLINE Wrap functions for Node, PtrNode
//	March 10, 2005:	RWB: added Swap()
//	March 24, 2006: RWB: added PtrList
//	January 11, 2007: RWB: added the template RefNode and RefList classes

#pragma once

#include "types.h"
#include "tstring.h"
#include "hssconfig/config.h"

#ifdef __CUDACC__
namespace HSSLowlevel_CUDA {
#else /* __CUDACC__ */
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4514 6011)			// disable warning saying that an unreferenced inline function has been removed
#endif /* _MSC_VER */					// to make this class as small as possible

#ifdef DEBUG
#include <cassert>
#endif

#endif /* __CUDACC__ */

#ifdef _MSC_VER

#if (!defined(__INTEL_COMPILER)) && (!defined(__INTEL_LLVM_COMPILER))
#pragma managed(push, off)
#endif
#endif //_MSC_VER

#ifdef HSS_SHOULD_PRAGMA_PACK
#pragma pack(push, 4)					// force to go to 4-byte packing rules,
#endif /* _MSC_VER */					// to make this class as small as possible

// ***** Standard List and Node Structures ***********************************


class MinNode {
    friend class MinList;
    private:
	MinNode *ln_Succ;
	MinNode *ln_Pred;

    public:
    DEVICE __INLINE MinNode *LN_Succ() const				{ return ln_Succ; };
    DEVICE __INLINE MinNode *LN_Pred() const				{ return ln_Pred; };
	DEVICE __INLINE MinNode *LN_SuccWrap() const;
	DEVICE __INLINE MinNode *LN_PredWrap() const;

	DEVICE __INLINE void Clear()							{ ln_Succ = ln_Pred = nullptr; };
};					// used to turn this into a circular list


#ifdef HSS_SHOULD_PRAGMA_PACK
#pragma pack(pop)
#pragma pack(push, 2)			// force to go to 1-byte packing rules,
#endif					// to make these classes nice and small


#ifndef __CUDACC__
class Node : public MinNode {
    public:
    TCHAR *ln_Name;
        
    DEVICE __INLINE Node *LN_Succ() const					{ return (Node *)MinNode::LN_Succ(); };
    DEVICE __INLINE Node *LN_Pred() const					{ return (Node *)MinNode::LN_Pred(); };
	DEVICE __INLINE Node *LN_SuccWrap() const				{ return (Node *)MinNode::LN_SuccWrap(); };
	DEVICE __INLINE Node *LN_PredWrap() const				{ return (Node *)MinNode::LN_PredWrap(); };
};


class SNode : public MinNode {
    public:
	tstring ln_Name;

	DEVICE __INLINE SNode *LN_Succ() const					{ return (SNode *)MinNode::LN_Succ(); };
	DEVICE __INLINE SNode *LN_Pred() const					{ return (SNode *)MinNode::LN_Pred(); };
	DEVICE __INLINE SNode *LN_SuccWrap() const				{ return (SNode *)MinNode::LN_SuccWrap(); };
	DEVICE __INLINE SNode *LN_PredWrap() const				{ return (SNode *)MinNode::LN_PredWrap(); };
};
#endif


class PtrNode : public MinNode {				// use RefNode instead
    public:
	APTR ln_Ptr;

	DEVICE __INLINE PtrNode *LN_Succ() const				{ return (PtrNode *)MinNode::LN_Succ(); };
	DEVICE __INLINE PtrNode *LN_Pred() const				{ return (PtrNode *)MinNode::LN_Pred(); };
	DEVICE __INLINE PtrNode *LN_SuccWrap() const			{ return (PtrNode *)MinNode::LN_SuccWrap(); };
	DEVICE __INLINE PtrNode *LN_PredWrap() const			{ return (PtrNode *)MinNode::LN_PredWrap(); };
};


template <class cls> class RefNode : public PtrNode {
    public:
	DEVICE __INLINE cls *LN_Ptr() const						{ return (cls *)ln_Ptr; };
	DEVICE __INLINE cls *LN_Ptr(const cls *object)			{ ln_Ptr = (APTR)object; return (cls *)ln_Ptr; };

	DEVICE __INLINE RefNode<cls> *LN_Succ() const			{ return (RefNode<cls> *)PtrNode::LN_Succ(); };
	DEVICE __INLINE RefNode<cls> *LN_Pred() const			{ return (RefNode<cls> *)PtrNode::LN_Pred(); };
	DEVICE __INLINE RefNode<cls> *LN_SuccWrap() const		{ return (RefNode<cls> *)PtrNode::LN_SuccWrap(); };
	DEVICE __INLINE RefNode<cls> *LN_PredWrap() const		{ return (RefNode<cls> *)PtrNode::LN_PredWrap(); };
};


typedef void(__cdecl *reverser_callback)(APTR parameter, const MinNode *node);

template<class type>
class MinListIterator
{
private:
	typedef type* type_pointer;
	type_pointer value;

public:
	MinListIterator(type_pointer _value) : value(_value) { }

	MinListIterator& operator++() { value = value->LN_Succ(); return *this; }
	MinListIterator operator++(int) { MinListIterator retval = *this; ++(*this); return retval; }

	bool operator==(MinListIterator other) const { return value == other.value; }
	bool operator!=(MinListIterator other) const { return !(*this == other); }

	type* operator*() { return value; }

	using difference_type = type_pointer;
	using value_type = type_pointer;
	using pointer = const type_pointer*;
	using reference = const type_pointer&;
	using iterator_category = std::forward_iterator_tag;
};

class MinList {
    protected:
        MinNode *lh_Head;
        MinNode *lh_Tail;
        MinNode *lh_TailPred;
        std::uint32_t lh_cnt; 
	                    
    public:
    DEVICE MinList();
	DEVICE MinList(MinList &&list);
    	 
    DEVICE __INLINE bool IsEmpty() const								{ return (lh_TailPred == (MinNode *)this); };
	DEVICE __INLINE std::uint32_t GetCount() const						{ return lh_cnt; };

	DEVICE __INLINE MinNode *LH_Head() const							{ return lh_Head; };
	DEVICE __INLINE MinNode *LH_Tail() const							{ return lh_TailPred; };

	DEVICE bool __FASTCALL NodeHasIndex(const MinNode* Node) const;				// returns whether a node is on the list or not
	DEVICE std::uint32_t __FASTCALL NodeIndex(const MinNode *Node) const;		// returns index of node on the list, or (std::uint32_t)-1
	DEVICE MinNode * __FASTCALL IndexNode(std::uint32_t index) const;			// return node at 'index'

	DEVICE __INLINE void AddHead(MinNode *Node)							{ Insert(Node, (MinNode *)this); };
	DEVICE __INLINE void AddTail(MinNode *Node)							{ Insert(Node, lh_TailPred); };
    DEVICE  void __FASTCALL Insert(MinNode *New, MinNode *Where);
	DEVICE __INLINE void InsertIndex(MinNode *New, std::uint32_t index)	{ if (index >= GetCount()) AddTail(New); else { MinNode *mn = IndexNode(index); Insert(New, mn->LN_Pred()); } };

	DEVICE MinNode * __FASTCALL RemHead();
	DEVICE MinNode * __FASTCALL RemTail();
	DEVICE void __FASTCALL Remove(MinNode *Node);
	DEVICE void __FASTCALL Replace(MinNode *Remove, MinNode *Insert);
	DEVICE void __FASTCALL Swap(MinNode *node1, MinNode *node2);


	DEVICE void __FASTCALL AppendList(MinList *list);				// lots quicker than just removing and adding individually
	DEVICE void __FASTCALL AppendFromList(MinList *list, MinNode *left, MinNode *right, bool could_wrap);	// left and right are inclusive
	DEVICE void __FASTCALL InsertList(MinList *list, MinNode *Where);		// same as append, but not inserting at the end of the list
	DEVICE void __FASTCALL InsertFromList(MinList *list, MinNode *left, MinNode *right, bool could_wrap, MinNode *Where);	// points are inserted after 'Where'

	DEVICE void __FASTCALL MoveHead(MinNode *NewHead);				// takes nodes from the start of the list up to "NewHead" and puts them, in order, onto the end of the list

	DEVICE void __FASTCALL ReverseOrder(reverser_callback fcn, APTR parm);
	DEVICE void __FASTCALL ReverseOrder(MinNode *start, MinNode *end, bool could_wrap, reverser_callback fcn, APTR parm);
											// fcn is called for each node (start and end inclusive) being reversed in direction, don't look forward or
											// back on the list at the time, though.  could_wrap is just for a (little) bit of efficiency if you know that
											// 'end' is after 'start' on the list

	/// <summary>
	/// Get an iterator populated with the head of the list.
	/// </summary>
	MinListIterator<MinNode> begin() { return MinListIterator<MinNode>(LH_Head()); }
	/// <summary>
	/// Get the end of the iterator.
	/// </summary>
	MinListIterator<MinNode> end() { return MinListIterator<MinNode>(LH_Tail()->LN_Succ()); }

//    #ifdef _DEBUG
	DEVICE bool __FASTCALL VerifyListCount() const;
//    #endif

	static DEVICE MinList * __FASTCALL FindList(const MinNode *node);		// NOTE this is only intended for debugging purposes to find out where a node is in case it's been 'lost'.  It'll be
};											// correct, but not a good way to manage linked lists.  If the node isn't on a list, then this will most likely crash.


template <class nodecls> class MinListTempl : public MinList {				// just stronger typing than MinList()
    public:
	MinListTempl() = default;
	MinListTempl(MinListTempl<nodecls> &&list) : MinList((MinList &&)list)	{ };

	DEVICE __INLINE nodecls *LH_Head() const							{ return (nodecls *)MinList::LH_Head(); };
	DEVICE __INLINE nodecls *LH_Tail() const							{ return (nodecls *)MinList::LH_Tail(); };
	DEVICE __INLINE nodecls *IndexNode(std::uint32_t index) const		{ return (nodecls *)MinList::IndexNode(index); };
	DEVICE __INLINE nodecls *RemHead()									{ return (nodecls *)MinList::RemHead(); };
	DEVICE __INLINE nodecls *RemTail()									{ return (nodecls *)MinList::RemTail(); };

	MinListIterator<nodecls> begin() { return MinListIterator<nodecls>(LH_Head()); }
	MinListIterator<nodecls> end() { return MinListIterator<nodecls>(LH_Tail()->LN_Succ()); }
};


#ifndef __CUDACC__
class List : public MinList {
    public:
	List() = default;
	List(List &&list) : MinList((MinList &&)list)						{ };

	DEVICE void __FASTCALL InsertAlphabetical(Node *node, bool case_sensitive = true);

    	DEVICE Node * __FASTCALL FindName(const TCHAR *Name, bool case_sensitive = true) const;
	DEVICE Node * __FASTCALL FindNextName(Node *from, const TCHAR *Name, bool case_sensitive = true) const;
	DEVICE Node * __FASTCALL FindPredName(Node *from, const TCHAR *Name, bool case_sensitive = true) const;
	DEVICE Node * __FASTCALL FindLastName(const TCHAR *Name, bool case_sensitive = true) const;
    	
	DEVICE __INLINE Node *IndexNode(std::uint32_t index) const			{ return (Node *)MinList::IndexNode(index); };

	DEVICE __INLINE Node *RemHead()										{ return (Node *)MinList::RemHead(); };
    	DEVICE __INLINE Node *RemTail()									{ return (Node *)MinList::RemTail(); };
    	
    	DEVICE __INLINE Node *LH_Head() const							{ return (Node *)MinList::LH_Head(); };
    	DEVICE __INLINE Node *LH_Tail() const							{ return (Node *)MinList::LH_Tail(); };

	DEVICE std::uint32_t GetNameCount(const TCHAR *Name, bool case_sensitive = true) const;
};


class SList : public MinList {
public:
	SList()	= default;
	SList(SList &&list) : MinList((MinList &&)list)						{ };

	DEVICE void __FASTCALL InsertAlphabetical(SNode *node, bool case_sensitive = true);

	DEVICE SNode * __FASTCALL FindName(const tstring &Name, bool case_sensitive = true) const;
	DEVICE SNode * __FASTCALL FindNextName(SNode *from, const tstring &Name, bool case_sensitive = true) const;
	DEVICE SNode * __FASTCALL FindPredName(SNode *from, const tstring &Name, bool case_sensitive = true) const;
	DEVICE SNode * __FASTCALL FindLastName(const tstring &Name, bool case_sensitive = true) const;

	DEVICE __INLINE SNode *IndexNode(std::uint32_t index) const			{ return (SNode *)MinList::IndexNode(index); };

	DEVICE __INLINE SNode *RemHead()									{ return (SNode *)MinList::RemHead(); };
	DEVICE __INLINE SNode *RemTail()									{ return (SNode *)MinList::RemTail(); };

	DEVICE __INLINE SNode *LH_Head() const								{ return (SNode *)MinList::LH_Head(); };
	DEVICE __INLINE SNode *LH_Tail() const								{ return (SNode *)MinList::LH_Tail(); };

	DEVICE std::uint32_t GetNameCount(const tstring &Name, bool case_sensitive = true) const;
};
#endif


class PtrList : public MinList {				// use RefList instead
    protected:
	PtrList() = default;
	PtrList(PtrList &&list) : MinList((MinList &&)list)					{ };

	DEVICE void __FASTCALL InsertAscending(PtrNode *node);

	DEVICE PtrNode * __FASTCALL FindPtr(const APTR ptr) const;
	DEVICE PtrNode * __FASTCALL FindNextPtr(const PtrNode *continue_from, const APTR ptr) const;		// this will test continue_from
	DEVICE std::uint32_t __FASTCALL FindPtrIndex(const APTR ptr) const;
	DEVICE std::uint32_t __FASTCALL FindNextPtrIndex(const std::uint32_t continue_from, const APTR ptr) const;
    	
	DEVICE __INLINE PtrNode *IndexNode(std::uint32_t index) const		{ return (PtrNode *)MinList::IndexNode(index); };

	DEVICE std::uint32_t GetPtrCount(const APTR ptr) const;

	DEVICE void FindPtr(const APTR ptr, PtrList &targetList);
};


template <class cls, class nodecls = RefNode<cls> > class RefList : public PtrList {
    public:
	RefList() = default;
	RefList(RefList<cls, nodecls> &&list) : PtrList(list)				{ };

	DEVICE __INLINE nodecls *FindPtr(const cls *ptr) const				{ return (nodecls *)PtrList::FindPtr((APTR)ptr); };
	DEVICE __INLINE nodecls *FindNextPtr(const nodecls *continue_from, const cls *ptr) const
																		{ return (nodecls *)PtrList::FindNextPtr(continue_from, ptr); };
	DEVICE __INLINE std::uint32_t FindPtrIndex(const cls *ptr) const	{ return PtrList::FindPtrIndex((APTR)ptr); };
	DEVICE __INLINE std::uint32_t FindNextPtrIndex(const std::uint32_t continue_from, const cls *ptr) const
																		{ return PtrList::FindNextPtrIndex(continue_from, ptr); };
	DEVICE __INLINE nodecls *IndexNode(std::uint32_t index) const		{ return (nodecls *)PtrList::IndexNode(index); };
	DEVICE __INLINE cls *IndexPtr(std::uint32_t index) const			{ nodecls *pn = IndexNode(index); if (pn) return pn->LN_Ptr(); return nullptr; };
		
	DEVICE std::uint32_t GetPtrCount(const cls *ptr) const				{ return PtrList::GetPtrCount((APTR)ptr); };

	DEVICE void FindPtr(const cls *ptr, RefList<cls, nodecls> &targetList)
																		{ PtrList::FindPtr((APTR)ptr, targetList); };

	DEVICE __INLINE nodecls *RemHead()									{ return (nodecls *)PtrList::RemHead(); };
    DEVICE __INLINE nodecls *RemTail()									{ return (nodecls *)PtrList::RemTail(); };
    	
    DEVICE __INLINE nodecls *LH_Head() const							{ return (nodecls *)PtrList::LH_Head(); };
    DEVICE __INLINE nodecls *LH_Tail() const							{ return (nodecls *)PtrList::LH_Tail(); };

	DEVICE __INLINE void AddSetFrom(const RefList<cls, nodecls> &list);
	DEVICE __INLINE void RemoveSetFrom(const RefList<cls, nodecls> &list);
};

#ifdef __CUDACC__
};

using namespace HSSLowlevel_CUDA;

#endif

#ifdef HSS_SHOULD_PRAGMA_PACK
#pragma pack(pop)			// go back to whatever the project
#endif					// settings are...

MinNode *MinNode::LN_SuccWrap() const {
	MinNode *node = ln_Succ;

    #ifdef DEBUG
	weak_assert(node);
    #endif							  
	
	if (!node->ln_Succ) {
		long **l = (long **)node;
		node = (MinNode *)(--l);
		node = node->ln_Succ;
	}
	return node;
}


MinNode *MinNode::LN_PredWrap() const {
	MinNode *node = ln_Pred;

    #ifdef DEBUG
	weak_assert(node);
    #endif							  

	if (!node->ln_Pred) {
		long **l = (long **)node;
		node = (MinNode *)(++l);
		node = node->ln_Pred;
	}
	return node;
}


template <class cls, class nodecls>
void RefList<cls, nodecls>::AddSetFrom(const RefList<cls, nodecls> &list) {
	nodecls *node = list.LH_Head();
	while (node->LN_Succ()) {
		if (!FindPtr(node->LN_Ptr())) {
			nodecls *new_node = new nodecls();
			new_node->LN_Ptr(node->LN_Ptr());
			AddTail(new_node);
		}
		node = node->LN_Succ();
	}
}


template <class cls, class nodecls>
void RefList<cls, nodecls>::RemoveSetFrom(const RefList<cls, nodecls> &list) {
	nodecls *node = list.LH_Head(), *old_node;
	while (node->LN_Succ()) {
		if ((old_node = FindPtr(node->LN_Ptr()))) {
			Remove(old_node);
			delete old_node;
		}
		node = node->LN_Succ();
	}
}

#ifdef _MSC_VER
#pragma warning(pop)

#if (!defined(__INTEL_COMPILER)) && (!defined(__INTEL_LLVM_COMPILER))
#pragma managed(pop)
#endif

#endif
