/**
 * linklist.h
 *
 * Copyright 2004-2023 Heartland Software Solutions Inc.
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
    DEVICE MinNode *LN_Succ() const				{ return ln_Succ; };
    DEVICE MinNode *LN_Pred() const				{ return ln_Pred; };
	DEVICE MinNode *LN_SuccWrap() const;
	DEVICE MinNode *LN_PredWrap() const;

	DEVICE void Clear()							{ ln_Succ = ln_Pred = nullptr; };
};


#ifdef HSS_SHOULD_PRAGMA_PACK
#pragma pack(pop)
#pragma pack(push, 8)								// force to go to 8-byte packing rules,
#endif												// to make these classes nice and small


#ifndef __CUDACC__
class Node : public MinNode {
    public:
    TCHAR *ln_Name;
        
    DEVICE Node *LN_Succ() const					{ return (Node *)MinNode::LN_Succ(); };
    DEVICE Node *LN_Pred() const					{ return (Node *)MinNode::LN_Pred(); };
	DEVICE Node *LN_SuccWrap() const				{ return (Node *)MinNode::LN_SuccWrap(); };
	DEVICE Node *LN_PredWrap() const				{ return (Node *)MinNode::LN_PredWrap(); };
};


class SNode : public MinNode {
    public:
	tstring ln_Name;

	DEVICE SNode *LN_Succ() const					{ return (SNode *)MinNode::LN_Succ(); };
	DEVICE SNode *LN_Pred() const					{ return (SNode *)MinNode::LN_Pred(); };
	DEVICE SNode *LN_SuccWrap() const				{ return (SNode *)MinNode::LN_SuccWrap(); };
	DEVICE SNode *LN_PredWrap() const				{ return (SNode *)MinNode::LN_PredWrap(); };
};
#endif


class PtrNode : public MinNode {				// use RefNode instead
    public:
	APTR ln_Ptr;

	DEVICE PtrNode *LN_Succ() const				{ return (PtrNode *)MinNode::LN_Succ(); };
	DEVICE PtrNode *LN_Pred() const				{ return (PtrNode *)MinNode::LN_Pred(); };
	DEVICE PtrNode *LN_SuccWrap() const			{ return (PtrNode *)MinNode::LN_SuccWrap(); };
	DEVICE PtrNode *LN_PredWrap() const			{ return (PtrNode *)MinNode::LN_PredWrap(); };
};


template <class cls> class RefNode : public PtrNode {
    public:
	DEVICE cls *LN_Ptr() const						{ return (cls *)ln_Ptr; };
	DEVICE cls *LN_Ptr(const cls *object)			{ ln_Ptr = (APTR)object; return (cls *)ln_Ptr; };

	DEVICE RefNode<cls> *LN_Succ() const			{ return (RefNode<cls> *)PtrNode::LN_Succ(); };
	DEVICE RefNode<cls> *LN_Pred() const			{ return (RefNode<cls> *)PtrNode::LN_Pred(); };
	DEVICE RefNode<cls> *LN_SuccWrap() const		{ return (RefNode<cls> *)PtrNode::LN_SuccWrap(); };
	DEVICE RefNode<cls> *LN_PredWrap() const		{ return (RefNode<cls> *)PtrNode::LN_PredWrap(); };
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
    	 
    DEVICE bool IsEmpty() const								{ return (lh_TailPred == (MinNode *)this); };
	DEVICE std::uint32_t GetCount() const						{ return lh_cnt; };

	DEVICE MinNode *LH_Head() const							{ return lh_Head; };
	DEVICE MinNode *LH_Tail() const							{ return lh_TailPred; };

	DEVICE bool NodeHasIndex(const MinNode* Node) const;				// returns whether a node is on the list or not
	DEVICE std::uint32_t NodeIndex(const MinNode *Node) const;		// returns index of node on the list, or (std::uint32_t)-1
	DEVICE MinNode * IndexNode(std::uint32_t index) const;			// return node at 'index'

	DEVICE void AddHead(MinNode *Node)							{ Insert(Node, (MinNode *)this); };
	DEVICE void AddTail(MinNode *Node)							{ Insert(Node, lh_TailPred); };
    DEVICE  void Insert(MinNode *New, MinNode *Where);
	DEVICE void InsertIndex(MinNode *New, std::uint32_t index)	{ if (index >= GetCount()) AddTail(New); else { MinNode *mn = IndexNode(index); Insert(New, mn->LN_Pred()); } };

	DEVICE MinNode * RemHead();
	DEVICE MinNode * RemTail();
	DEVICE void Remove(MinNode *Node);
	DEVICE void Replace(MinNode *Remove, MinNode *Insert);
	DEVICE void Swap(MinNode *node1, MinNode *node2);


	DEVICE void AppendList(MinList *list);				// lots quicker than just removing and adding individually
	DEVICE void AppendFromList(MinList *list, MinNode *left, MinNode *right, bool could_wrap);	// left and right are inclusive
	DEVICE void InsertList(MinList *list, MinNode *Where);		// same as append, but not inserting at the end of the list
	DEVICE void InsertFromList(MinList *list, MinNode *left, MinNode *right, bool could_wrap, MinNode *Where);	// points are inserted after 'Where'

	DEVICE void MoveHead(MinNode *NewHead);				// takes nodes from the start of the list up to "NewHead" and puts them, in order, onto the end of the list

	DEVICE void ReverseOrder(reverser_callback fcn, APTR parm);
	DEVICE void ReverseOrder(MinNode *start, MinNode *end, bool could_wrap, reverser_callback fcn, APTR parm);
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
	DEVICE bool VerifyListCount() const;
//    #endif

	static DEVICE MinList * FindList(const MinNode *node);		// NOTE this is only intended for debugging purposes to find out where a node is in case it's been 'lost'.  It'll be
};											// correct, but not a good way to manage linked lists.  If the node isn't on a list, then this will most likely crash.


template <class nodecls> class MinListTempl : public MinList {				// just stronger typing than MinList()
    public:
	MinListTempl() = default;
	MinListTempl(MinListTempl<nodecls> &&list) : MinList((MinList &&)list)	{ };

	DEVICE nodecls *LH_Head() const							{ return (nodecls *)MinList::LH_Head(); };
	DEVICE nodecls *LH_Tail() const							{ return (nodecls *)MinList::LH_Tail(); };
	DEVICE nodecls *IndexNode(std::uint32_t index) const		{ return (nodecls *)MinList::IndexNode(index); };
	DEVICE nodecls *RemHead()									{ return (nodecls *)MinList::RemHead(); };
	DEVICE nodecls *RemTail()									{ return (nodecls *)MinList::RemTail(); };

	MinListIterator<nodecls> begin() { return MinListIterator<nodecls>(LH_Head()); }
	MinListIterator<nodecls> end() { return MinListIterator<nodecls>(LH_Tail()->LN_Succ()); }
};


#ifndef __CUDACC__
class List : public MinList {
    public:
	List() = default;
	List(List &&list) : MinList((MinList &&)list)						{ };

	DEVICE void InsertAlphabetical(Node *node, bool case_sensitive = true);

    	DEVICE Node * FindName(const TCHAR *Name, bool case_sensitive = true) const;
	DEVICE Node * FindNextName(Node *from, const TCHAR *Name, bool case_sensitive = true) const;
	DEVICE Node * FindPredName(Node *from, const TCHAR *Name, bool case_sensitive = true) const;
	DEVICE Node * FindLastName(const TCHAR *Name, bool case_sensitive = true) const;
    	
	DEVICE Node *IndexNode(std::uint32_t index) const			{ return (Node *)MinList::IndexNode(index); };

	DEVICE Node *RemHead()										{ return (Node *)MinList::RemHead(); };
    	DEVICE Node *RemTail()									{ return (Node *)MinList::RemTail(); };
    	
    	DEVICE Node *LH_Head() const							{ return (Node *)MinList::LH_Head(); };
    	DEVICE Node *LH_Tail() const							{ return (Node *)MinList::LH_Tail(); };

	DEVICE std::uint32_t GetNameCount(const TCHAR *Name, bool case_sensitive = true) const;
};


class SList : public MinList {
public:
	SList()	= default;
	SList(SList &&list) : MinList((MinList &&)list)						{ };

	DEVICE void InsertAlphabetical(SNode *node, bool case_sensitive = true);

	DEVICE SNode * FindName(const tstring &Name, bool case_sensitive = true) const;
	DEVICE SNode * FindNextName(SNode *from, const tstring &Name, bool case_sensitive = true) const;
	DEVICE SNode * FindPredName(SNode *from, const tstring &Name, bool case_sensitive = true) const;
	DEVICE SNode * FindLastName(const tstring &Name, bool case_sensitive = true) const;

	DEVICE SNode *IndexNode(std::uint32_t index) const			{ return (SNode *)MinList::IndexNode(index); };

	DEVICE SNode *RemHead()									{ return (SNode *)MinList::RemHead(); };
	DEVICE SNode *RemTail()									{ return (SNode *)MinList::RemTail(); };

	DEVICE SNode *LH_Head() const								{ return (SNode *)MinList::LH_Head(); };
	DEVICE SNode *LH_Tail() const								{ return (SNode *)MinList::LH_Tail(); };

	DEVICE std::uint32_t GetNameCount(const tstring &Name, bool case_sensitive = true) const;
};
#endif


class PtrList : public MinList {				// use RefList instead
    protected:
	PtrList() = default;
	PtrList(PtrList &&list) : MinList((MinList &&)list)					{ };

	DEVICE void InsertAscending(PtrNode *node);

	DEVICE PtrNode * FindPtr(const APTR ptr) const;
	DEVICE PtrNode * FindNextPtr(const PtrNode *continue_from, const APTR ptr) const;		// this will test continue_from
	DEVICE std::uint32_t FindPtrIndex(const APTR ptr) const;
	DEVICE std::uint32_t FindNextPtrIndex(const std::uint32_t continue_from, const APTR ptr) const;
    	
	DEVICE PtrNode *IndexNode(std::uint32_t index) const		{ return (PtrNode *)MinList::IndexNode(index); };

	DEVICE std::uint32_t GetPtrCount(const APTR ptr) const;

	DEVICE void FindPtr(const APTR ptr, PtrList &targetList);
};


template <class cls, class nodecls = RefNode<cls> > class RefList : public PtrList {
    public:
	RefList() = default;
	RefList(RefList<cls, nodecls> &&list) : PtrList(list)				{ };

	DEVICE nodecls *FindPtr(const cls *ptr) const				{ return (nodecls *)PtrList::FindPtr((APTR)ptr); };
	DEVICE nodecls *FindNextPtr(const nodecls *continue_from, const cls *ptr) const
																		{ return (nodecls *)PtrList::FindNextPtr(continue_from, ptr); };
	DEVICE std::uint32_t FindPtrIndex(const cls *ptr) const	{ return PtrList::FindPtrIndex((APTR)ptr); };
	DEVICE std::uint32_t FindNextPtrIndex(const std::uint32_t continue_from, const cls *ptr) const
																		{ return PtrList::FindNextPtrIndex(continue_from, ptr); };
	DEVICE nodecls *IndexNode(std::uint32_t index) const		{ return (nodecls *)PtrList::IndexNode(index); };
	DEVICE cls *IndexPtr(std::uint32_t index) const			{ nodecls *pn = IndexNode(index); if (pn) return pn->LN_Ptr(); return nullptr; };
		
	DEVICE std::uint32_t GetPtrCount(const cls *ptr) const				{ return PtrList::GetPtrCount((APTR)ptr); };

	DEVICE void FindPtr(const cls *ptr, RefList<cls, nodecls> &targetList)
																		{ PtrList::FindPtr((APTR)ptr, targetList); };

	DEVICE nodecls *RemHead()									{ return (nodecls *)PtrList::RemHead(); };
    DEVICE nodecls *RemTail()									{ return (nodecls *)PtrList::RemTail(); };
    	
    DEVICE nodecls *LH_Head() const							{ return (nodecls *)PtrList::LH_Head(); };
    DEVICE nodecls *LH_Tail() const							{ return (nodecls *)PtrList::LH_Tail(); };

	DEVICE void AddSetFrom(const RefList<cls, nodecls> &list);
	DEVICE void RemoveSetFrom(const RefList<cls, nodecls> &list);
};

#ifdef __CUDACC__
};

using namespace HSSLowlevel_CUDA;

#endif

#ifdef HSS_SHOULD_PRAGMA_PACK
#pragma pack(pop)			// go back to whatever the project
#endif					// settings are...

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
